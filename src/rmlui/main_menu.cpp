#include <SDL3/SDL.h>
#include "main_menu.hpp"
#include "../game.hpp"

MainMenu::MainMenu(Rml::Context* context)
	: m_context(context)
{
	if (Rml::DataModelConstructor constructor = m_context->CreateDataModel("game_settings"))
	{
		constructor.BindFunc("is_fullscreen", [](Rml::Variant& variant) { 
			variant = &g_Game->m_GameSettings.IsFullScreen; 
		}, 
		[](const Rml::Variant& variant) {
			g_Game->m_GameSettings.IsFullScreen = variant.Get<bool>();
			g_Game->SetFullscreen(g_Game->m_GameSettings.IsFullScreen);
		});
	}

	OpenMenu();
}

void MainMenu::OpenMenu()
{
	Close();

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

void MainMenu::OpenSettings()
{
	Close();

	m_document = m_context->LoadDocument("ui/rml/settings.rml");
	m_document->Show();

	Rml::Element* back = m_document->GetElementById("back_to_menu");
	back->AddEventListener(Rml::EventId::Click, this);
}

void MainMenu::ProcessEvent(Rml::Event& event)
{
	auto& id = event.GetTargetElement()->GetId();

	if (id == "start")
	{

	}
	else if (id == "settings")
		OpenSettings();
	else if (id == "back_to_menu")
		OpenMenu();
	else if (id == "quit")
		m_quitDlg->SetProperty("display", "flex");
	else if (id == "yes")
		g_Game->RequestExit();
	else if (id == "no")
		m_quitDlg->SetProperty("display", "none");
}

void MainMenu::Close()
{
	if (m_document)
		m_document->Close();
}