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

private:
	Rml::Context* m_context = nullptr;
	Rml::ElementDocument* m_document = nullptr;

	Rml::Element* m_quitDlg = nullptr;
};