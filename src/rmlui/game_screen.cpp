#include <SDL3/SDL.h>
#include "game_screen.hpp"
#include "../game.hpp"
#include "../card.hpp"
#include "../player.hpp"
#include "../gamemodes/classic.hpp"

GameScreen::GameScreen(Rml::Context* context)
	: Scene(context)
{
	m_document = m_context->LoadDocument("ui/rml/game_screen.rml");
	m_document->Show();
	
	for (auto player : g_GameManager->GetPlayers())
	{
		CreatePlayerCards(player);
	}

	double duration = 2; 
	Rml::Tween tween{Rml::Tween::Quadratic, Rml::Tween::Out};
}

void GameScreen::CreatePlayerCards(Player* player)
{
	auto localPlayer = g_GameManager->GetLocalPlayer();
	Rml::Element* cardContainer = nullptr;

	for (auto card : player->GetCards())
	{
		if (player == localPlayer)
			cardContainer = m_document->GetElementById("card_container");
		else
			cardContainer = m_document->GetElementById("player" + Rml::ToString(player->GetIndex()) + "_card_container");

		CreateCard(card, cardContainer, (player == localPlayer));
	}
}

void GameScreen::CreateCard(Card* card, Rml::Element* container, bool isVisible)
{
	auto tag = isVisible ? "handle" : "img";
	Rml::ElementPtr cardW = Rml::Factory::InstanceElement(container, tag, tag, Rml::XMLAttributes());

	if (isVisible)
	{
		cardW->SetClass("card-wrapper", true);
		cardW->SetAttribute("move_target", "#self");
		cardW->SetAttribute("edge_margin", "none");

		Rml::String colorStr;
		switch (card->Color)
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
			case CardColor::WILD:
				if (card->Type == CardType::WILD)
					colorStr = "wild";
				else if (card->Type == CardType::WILD_DRAW_4)
					colorStr = "wild4";

				break;
			default:
				colorStr = "red";
				break;
		}

		Rml::String spec;
		Rml::String sprite;
		Rml::String value;
		Rml::String corners;

		switch (card->Type)
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

		if (card->Type == CardType::WILD || card->Type == CardType::WILD_DRAW_4)
		{
			if (card->Type == CardType::WILD_DRAW_4)
			{
				corners = "<span class='corner-text top-left'>+4</span>"
						"<span class='corner-text bottom-right'>+4</span>";
			}
		}
		else if (card->Type != CardType::NUMBER)
		{
			spec = "<img class='card-icon' sprite='"+sprite+"' />";
			if (card->Type == CardType::DRAW_2)
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
			value = "<span class='card-value'>" + Rml::ToString(card->Value) + "</span>";
			Rml::String valStr = Rml::ToString(card->Value);
			corners = "<span class='corner-text top-left'>" + valStr + "</span>"
					"<span class='corner-text bottom-right'>" + valStr + "</span>";
		}

		cardW->SetInnerRML(
			"<div class='card-content'>"
				"<img class='card' sprite='" + colorStr + "-num-card' />"
				+ spec + value + corners +
			"</div>"
		);

		cardW->AddEventListener(Rml::EventId::Dragend, this);
	}
	else
	{
		cardW->SetClass("player-card", true);
		cardW->SetAttribute("sprite", "back-card");
	}	

	container->AppendChild(std::move(cardW));
}

void GameScreen::Destroy()
{
	if (m_document)
		m_document->Close(), m_document = nullptr;
}

void GameScreen::Update()
{
	auto curTime = g_Game->GetElapsedTime();
	auto it = std::remove_if(m_deletionQueue.begin(), m_deletionQueue.end(), [&](const PendingDeletion& c) {
        if (curTime >= c.Time) 
		{
			c.Element->GetParentNode()->RemoveChild(c.Element);
            return true;
        }

        return false;
    });

	m_deletionQueue.erase(it, m_deletionQueue.end());
}

void GameScreen::ProcessEvent(Rml::Event& event)
{
	auto card = event.GetTargetElement();
    if (!card) 
		return;

    if (event == Rml::EventId::Dragend)
    {
		double duration = 0.25; 
		Rml::Tween tween{Rml::Tween::Quadratic, Rml::Tween::Out};

		float mouseX = event.GetParameter("mouse_x", 0.0f);
    	float mouseY = event.GetParameter("mouse_y", 0.0f);

		Rml::Element* hoverElement = m_context->GetElementAtPoint(Rml::Vector2f(mouseX, mouseY), card);
		if (hoverElement && hoverElement->GetId() == "table")
		{
			card->SetClass("deleted", true);
			card->Animate("opacity", Rml::Property(0.0f, Rml::Unit::NUMBER), duration, tween);
			
			PendingDeletion p{card, g_Game->GetElapsedTime() + duration};
			m_deletionQueue.push_back(p);
		}
		else
		{
			card->Animate("left", Rml::Property(0.0f, Rml::Unit::PX), duration, tween);
			card->Animate("top",  Rml::Property(0.0f, Rml::Unit::PX), duration, tween);
		}
    }
}