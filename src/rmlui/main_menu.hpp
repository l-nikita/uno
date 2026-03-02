#pragma once

#include <RmlUi/Core.h>

class MainMenu final : public Rml::EventListener
{
public:
	MainMenu(Rml::Context* context);

	void Close();
	void ProcessEvent(Rml::Event& event) override;

private:
	void OpenMenu();
	void OpenSettings();

	enum class MainMenuPage
	{
		MAIN_MENU,
		SETTINGS,
	};

private:
	MainMenuPage m_currentPage = MainMenuPage::MAIN_MENU;

	Rml::Context* m_context = nullptr;
	Rml::ElementDocument* m_mainMenu = nullptr;
	Rml::ElementDocument* m_settings = nullptr;

	Rml::Element* m_quitDlg = nullptr;
};