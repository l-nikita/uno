#include <SDL3/SDL.h>
#include "game_screen.hpp"
#include "clientmanager.hpp"
#include "../game.hpp"
#include "../card.hpp"
#include "../player.hpp"

GameScreen::GameScreen(Rml::Context* context)
	: Scene(context)
{
	m_document = m_context->LoadDocument("ui/rml/game_screen.rml");
	m_document->Show();

	CreatePlayersCards();
	CreateTopDiscardCard();

	Rml::Element* window = m_document->GetElementById("play_or_keep_window");
	Rml::ElementList list;
	window->GetElementsByTagName(list, "button");

	for (auto but : list)
		but->AddEventListener(Rml::EventId::Click, this);

    g_ClientManager->Subscribe(this);
}

void GameScreen::Destroy()
{
	if (m_document)
		m_document->Close(), m_document = nullptr;

	g_ClientManager->Unsubscribe(this);
}

void GameScreen::OnStateUpdate(const StateUpdate& update)
{
	if (std::get_if<GameState>(&update))
	{
		m_deletionQueue.clear();

		if (m_drawCard)
		{
			auto& info = g_ClientManager->GetLocalPlayerInfo();
			if (!CanPlay() || info.LastCardId == -1 || !CanPlayCard(info.LastCardId))
				m_drawCard = false;
		}
		
		if (m_drawCard)
			AskPlayOrKeep();

		CreatePlayersCards();
		CreateTopDiscardCard();

		if (CanPlay())
		{
			Rml::ElementList list;
			m_document->GetElementsByClassName(list, "card-wrapper");
		}
	}
}

void GameScreen::CreateTopDiscardCard()
{
	Rml::Element* tableContainer = m_document->GetElementById("table_container");
	tableContainer->SetInnerRML("<img id='deck' sprite='back-card'/>");

	Rml::Element* deck = m_document->GetElementById("deck");
	deck->AddEventListener(Rml::EventId::Click, this);

	if (CanPlay() && !m_drawCard)
		deck->SetClass("can-take", true);
	
	Rml::ElementPtr card = CreateCard(g_ClientManager->GetGameState().TopDiscard, true);
	card->SetId("discard_pile");
	tableContainer->AppendChild(std::move(card));
}

void GameScreen::CreatePlayersCards()
{
	auto& clState = g_ClientManager->GetGameState();

	for (size_t i = 0; i < 3; i++)
	{
		auto id = "player" + Rml::ToString(i + 1) + "_card_container";
		Rml::Element* container = m_document->GetElementById(id);
		if (container)
			container->SetInnerRML("");
	}

	int s = 1;

	for (size_t i = 0; i < clState.Players.size(); i++)
	{
		const auto& info = clState.Players.at(i);

		if (info.IsLocal)
		{
			Rml::Element* container = m_document->GetElementById("card_container");
			container->SetInnerRML(""); 

			for (size_t j = 0; j < info.Hand.size(); j++)
			{
				auto card = info.Hand.at(j);
				CreateCard(card, j, container);
			}
		}
		else
		{
			auto id = "player" + Rml::ToString(s) + "_card_container";
			Rml::Element* container = m_document->GetElementById(id);
			CreateOpponentCards(info.Hand.size(), container);
			s++;
		}
	}
}

void GameScreen::CreateOpponentCards(int count, Rml::Element* container)
{
	for (size_t i = 0; i < count; i++)
	{
		Rml::ElementPtr cardImg = Rml::Factory::InstanceElement(container, "img", "img", Rml::XMLAttributes());

		cardImg->SetClass("player-card", true);
		cardImg->SetAttribute("sprite", "back-card");	
		container->AppendChild(std::move(cardImg));
	}
}

Rml::ElementPtr GameScreen::CreateCard(const Card& card, bool isStatic)
{
	auto tag = isStatic ? "div" : "handle";
	Rml::ElementPtr cardW = Rml::Factory::InstanceElement(nullptr, tag, tag, Rml::XMLAttributes());

	Rml::String colorStr;
	switch (card.Color)
	{
		case CardColor::RED:
			colorStr = "red";
			break;			
		case CardColor::GREEN:
			colorStr = "green";
			break;			
		case CardColor::BLUE:
			colorStr = "blue";
			break;			
		case CardColor::YELLOW:
			colorStr = "yellow";
			break;			
		default:
			colorStr = "red";
			break;
	}

	bool isWild = (card.Type == CardType::WILD || card.Type == CardType::WILD_DRAW_4);
	if (isWild)
	{
		if (card.Type == CardType::WILD)
			colorStr = "wild";
		else if (card.Type == CardType::WILD_DRAW_4)
			colorStr = "wild4";
	}

	Rml::String spec;
	Rml::String sprite;
	Rml::String value;
	Rml::String corners;

	switch (card.Type)
	{
	case CardType::SKIP:
		sprite = "skip-icon";
		break;		
	case CardType::REVERSE:
		sprite = "reverse-icon";
		break;		
	case CardType::DRAW_2:
		sprite = "draw2-icon";
		break;
	default:
		sprite = "skip-icon";
		break;
	}

	if (card.Type == CardType::WILD || card.Type == CardType::WILD_DRAW_4)
	{
		if (card.Type == CardType::WILD_DRAW_4)
		{
			corners = "<span class='corner-text top-left'>+4</span>"
					"<span class='corner-text bottom-right'>+4</span>";
		}
	}
	else if (card.Type != CardType::NUMBER)
	{
		spec = "<img class='card-icon' sprite='"+sprite+"' />";
		if (card.Type == CardType::DRAW_2)
		{
			corners = "<span class='corner-text top-left'>+2</span>"
					"<span class='corner-text bottom-right'>+2</span>";
		}
		else
		{
			corners = "<img class='corner top-left' sprite='"+sprite+"' />"
				"<img class='corner bottom-right' sprite='"+sprite+"' />";
		}
	}
	else
	{
		value = "<span class='card-value'>" + Rml::ToString(card.Value) + "</span>";
		Rml::String valStr = Rml::ToString(card.Value);
		corners = "<span class='corner-text top-left'>" + valStr + "</span>"
				"<span class='corner-text bottom-right'>" + valStr + "</span>";
	}

	cardW->SetInnerRML(
		"<div class='card-content'>"
			"<img class='card' sprite='" + colorStr + "-num-card' />"
			+ spec + value + corners +
		"</div>"
	);

	return cardW;
}

void GameScreen::CreateCard(const Card& card, int index, Rml::Element* container)
{
	Rml::ElementPtr cardW = CreateCard(card, false);

	int lastCardId = g_ClientManager->GetLocalPlayerInfo().LastCardId;

	bool playable = false;
	if (m_drawCard)
		playable = (CanPlay() && index == lastCardId && CanPlayCard(card));
	else
		playable = (CanPlay() && CanPlayCard(card));

	cardW->SetClass("card-wrapper", true);
	cardW->SetClass("playable", playable);
	cardW->SetAttribute("move_target", "#self");
	cardW->SetAttribute("edge_margin", "none");

	cardW->AddEventListener(Rml::EventId::Dragend, this);
	cardW->SetAttribute("card_id", index);

	if (playable)
		cardW->Animate("margin-bottom", Rml::Property(2.0f, Rml::Unit::VH), 0.15, { Rml::Tween::Quadratic, Rml::Tween::Out });

	container->AppendChild(std::move(cardW));
}

void GameScreen::Update()
{
	if (m_deletionQueue.size() > 0)
	{
		auto curTime = g_Game->GetElapsedTime();
		auto it = std::remove_if(m_deletionQueue.begin(), m_deletionQueue.end(), [&](const PendingDeletion& c) {
			if (curTime >= c.Time) 
			{
				int cardId = c.Element->GetAttribute("card_id")->Get<int>();
				PlayCard(cardId);
				return true;
			}

			return false;
		});

		m_deletionQueue.erase(it, m_deletionQueue.end());
	}
}

void GameScreen::PlayCard(int cardId)
{
	auto& card = g_ClientManager->GetLocalPlayerInfo().Hand.at(cardId);

	if (card.Color == CardColor::WILD)
		ChooseColor(cardId);
	else
		g_ClientManager->DoPlayerAction({ ActionType::PLAY_CARD, cardId });

	m_drawCard = false;
}

void GameScreen::AskPlayOrKeep()
{
	Rml::Element* container = m_document->GetElementById("play_or_keep");
	container->SetClass("hidden", false);

	Rml::Element* window = m_document->GetElementById("play_or_keep_window");
	window->SetClass("show", true);
}

void GameScreen::HidePlayOrKeep()
{
	Rml::Element* container = m_document->GetElementById("play_or_keep");
	container->SetClass("hidden", true);

	Rml::Element* window = m_document->GetElementById("play_or_keep_window");
	window->SetClass("show", false);
}

void GameScreen::ChooseColor(int cardId)
{
	Rml::Element* container = m_document->GetElementById("change_color");
	container->SetClass("hidden", false);

	Rml::Element* window = m_document->GetElementById("change_color_window");
	window->SetInnerRML("");
	window->SetClass("show", true);

	for (size_t i = 0; i < 4; i++)
	{
		Rml::ElementPtr colorBut = Rml::Factory::InstanceElement(window, "button", "button", Rml::XMLAttributes());
		colorBut->SetClass("color-button", true);
		colorBut->SetAttribute("color_id", i);
		colorBut->SetAttribute("card_id", cardId);
		colorBut->SetProperty("background-color", GetCardColorToRCSS_RGB((CardColor)i));
		colorBut->AddEventListener(Rml::EventId::Click, this);
		
		window->AppendChild(std::move(colorBut));
	}
}

void GameScreen::HideColorSelection()
{
	Rml::Element* changeColor = m_document->GetElementById("change_color");
	changeColor->SetClass("hidden", true);

	Rml::Element* window = m_document->GetElementById("change_color_window");
	window->SetClass("show", false);
}

void GameScreen::ProcessEvent(Rml::Event& event)
{
	auto elem = event.GetTargetElement();
    if (!elem) 
		return;

    if (event == Rml::EventId::Dragend)
    {
		double duration = 0.25; 
		Rml::Tween tween{Rml::Tween::Quadratic, Rml::Tween::Out};

		float mouseX = event.GetParameter("mouse_x", 0.0f);
    	float mouseY = event.GetParameter("mouse_y", 0.0f);

		int cardId = elem->GetAttribute("card_id")->Get<int>();

		Rml::Element* hoverElement = m_context->GetElementAtPoint(Rml::Vector2f(mouseX, mouseY), elem);
		if (CanPlay() && CanPlayCard(cardId) && hoverElement && hoverElement->GetId() == "table")
		{
			elem->SetClass("deleted", true);
			elem->Animate("opacity", Rml::Property(0.0f, Rml::Unit::NUMBER), duration, tween);
			
			PendingDeletion p{elem, g_Game->GetElapsedTime() + duration - 0.05};
			m_deletionQueue.push_back(p);
		}
		else
		{
			elem->Animate("left", Rml::Property(0.0f, Rml::Unit::PX), duration, tween);
			elem->Animate("top",  Rml::Property(0.0f, Rml::Unit::PX), duration, tween);
		}
    }
	else if (event == Rml::EventId::Click)
	{
		auto& id = elem->GetId();
		if (id == "play")
		{
			auto lastCard = g_ClientManager->GetLocalPlayerInfo().LastCardId;
			PlayCard(lastCard);
			HidePlayOrKeep();
		}
		else if (id == "keep")
		{
			g_ClientManager->DoPlayerAction({ ActionType::DRAW_CARD });
			HidePlayOrKeep();
			m_drawCard = false;
		}
		else if (id == "deck" && elem->IsClassSet("can-take"))
		{
			g_ClientManager->DoPlayerAction({ ActionType::DRAW_CARD });
			m_drawCard = true;
		}
		else if (elem->IsClassSet("color-button") && elem->GetParentNode()->IsClassSet("show"))
		{
			int colorId = elem->GetAttribute("color_id")->Get<int>();
			int cardId = elem->GetAttribute("card_id")->Get<int>();

			g_ClientManager->DoPlayerAction({ ActionType::PLAY_CARD, cardId, (CardColor)colorId });
			HideColorSelection();
		}
	}
}

bool GameScreen::CanPlayCard(int index)
{
	auto hand = g_ClientManager->GetLocalPlayerInfo().Hand;
	
	for (size_t i = 0; i < hand.size(); i++)
	{
		if (i == index)
			return CanPlayCard(hand.at(i));
	}

	return false;
}

bool GameScreen::CanPlayCard(const Card& card)
{
	auto& clState = g_ClientManager->GetGameState();
	auto topC = clState.TopDiscard;

	bool sameColor = (topC.Color == card.Color);
	bool sameType = (topC.Type == card.Type);
	bool sameValue = (topC.Value == card.Value);
	bool isWild = (card.Type == CardType::WILD || card.Type == CardType::WILD_DRAW_4);

	if (topC.Color == CardColor::WILD)
		return true;

	if (isWild)
		return true;

	if (sameColor)
		return true;

	if (sameType && sameValue)
		return true;
		
	return false;
}

bool GameScreen::HasNoPlayableCards()
{
	auto hand = g_ClientManager->GetLocalPlayerInfo().Hand;

	for (size_t i = 0; i < hand.size(); i++)
	{
		if (CanPlayCard(hand.at(i)))
			return false;
	}

	return true;
}

bool GameScreen::CanPlay()
{
	auto cm = g_ClientManager;
	return cm->GetGameState().CurrentPlayer == cm->GetLocalPlayerInfo().Index;
}