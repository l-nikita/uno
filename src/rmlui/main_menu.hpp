#pragma once

#include <RmlUi/Core.h>
#include "scene.hpp"

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
class MainMenu final : public Scene
{
public:
	MainMenu(Rml::Context* context);

	void Hide();
	void ProcessEvent(Rml::Event& event) override;

	SceneId GetId() const override { return SceneId::MAIN_MENU; }

	void Update() override;
	void Destroy() override;

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

	Rml::ElementDocument* m_mainMenu = nullptr;
	Rml::ElementDocument* m_settings = nullptr;

	Rml::Element* m_quitDlg = nullptr;
};