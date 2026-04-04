#include "lobby.hpp"
#include "../game.hpp"
#include "../clientmanager.hpp"

Lobby::Lobby(Rml::Context* context)
	: Scene(context)
{
	m_document = m_context->LoadDocument("ui/rml/lobby.rml");
	m_document->Show();

	Rml::Element* quit = m_document->GetElementById("quit");
	quit->AddEventListener(Rml::EventId::Click, this);	
	
	Rml::Element* startGame = m_document->GetElementById("start_game");
	if (g_Game->IsHost())
		startGame->AddEventListener(Rml::EventId::Click, this);
	else
		startGame->GetParentNode()->RemoveChild(startGame);

	RefreshPlayers();

	g_ClientManager->Subscribe(this);
}

void Lobby::Destroy()
{
	if (m_document)
		m_document->Close(), m_document = nullptr;

	g_ClientManager->Unsubscribe(this);
}

void Lobby::OnStateUpdate(const StateUpdate& update)
{
	if (std::get_if<GameState>(&update))
		RefreshPlayers();
}

void Lobby::RefreshPlayers()
{
	const auto& state = g_ClientManager->GetGameState();
	Rml::Element* container = m_document->GetElementById("players")->GetFirstChild();
	if (!container) 
		return;

	container->SetInnerRML("");

	for (const auto& player : state.Players)
	{
		Rml::ElementPtr el = Rml::Factory::InstanceElement(container, "*", "div", Rml::XMLAttributes());
		el->SetClass("player", true);
		el->SetInnerRML(player.Name);
		container->AppendChild(std::move(el));
	}
}

void Lobby::Update()
{

}

void Lobby::ProcessEvent(Rml::Event& event)
{
	auto& id = event.GetTargetElement()->GetId();

	if (id == "quit")
		g_Game->Disconnect();	
	else if (id == "start_game")
		g_Game->StartGame();
}