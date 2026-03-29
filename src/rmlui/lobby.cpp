#include "lobby.hpp"
#include "../game.hpp"

Lobby::Lobby(Rml::Context* context)
	: Scene(context)
{
	m_document = m_context->LoadDocument("ui/rml/lobby.rml");
	m_document->Show();

	Rml::Element* quit = m_document->GetElementById("quit");
	quit->AddEventListener(Rml::EventId::Click, this);
}

void Lobby::Destroy()
{
	if (m_document)
		m_document->Close(), m_document = nullptr;
}

void Lobby::Update()
{

}

void Lobby::ProcessEvent(Rml::Event& event)
{
	auto& id = event.GetTargetElement()->GetId();

	if (id == "quit")
		g_Game->Disconnect();
}