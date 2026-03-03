#include <SDL3/SDL.h>
#include "game_screen.hpp"
#include "../game.hpp"
#include "../gamemodes/classic.hpp"

GameScreen::GameScreen(Rml::Context* context)
	: Scene(context)
{
	m_document = m_context->LoadDocument("ui/rml/main_menu.rml");
	m_document->Show();
}

void GameScreen::Destroy()
{
	
}

void GameScreen::ProcessEvent(Rml::Event& event)
{

}