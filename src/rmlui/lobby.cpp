#include "lobby.hpp"

Lobby::Lobby(Rml::Context* context)
	: Scene(context)
{
	m_document = m_context->LoadDocument("ui/rml/lobby.rml");
	m_document->Show();
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

}