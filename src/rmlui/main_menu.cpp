#include <SDL3/SDL.h>
#include <RmlUi/Core/Context.h>
#include <RmlUi/Core/ElementDocument.h>
#include "main_menu.hpp"
#include "../game.hpp"

MainMenu::MainMenu(Rml::Context* context)
	: m_context(context)
{
	m_document = m_context->LoadDocument("ui/rml/main_menu.rml");
	m_document->Show();

	Rml::ElementList elements;
	m_document->GetElementsByTagName(elements, "button");

	for (auto element : elements)
		element->AddEventListener(Rml::EventId::Click, this);

	m_quitDlg = m_document->GetElementById("quit_dlg");
	m_quitDlg->SetProperty("display", "none");

	Rml::ElementList elements2;
	m_quitDlg->GetElementsByTagName(elements2, "button");

	for (auto element : elements)
		element->AddEventListener(Rml::EventId::Click, this);
}

void MainMenu::Close()
{
	if (m_document)
		m_document->Close();
}

void MainMenu::ProcessEvent(Rml::Event& event)
{
	auto& id = event.GetTargetElement()->GetId();

	if (id == "start")
	{

	}
	else if (id == "settings")
	{

	}
	else if (id == "quit")
		m_quitDlg->SetProperty("display", "flex");
	else if (id == "yes")
		g_Game->RequestExit();
	else if (id == "no")
		m_quitDlg->SetProperty("display", "none");
}