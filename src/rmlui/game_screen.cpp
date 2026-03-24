#include <SDL3/SDL.h>
#include "game_screen.hpp"
#include "../game.hpp"
#include "../gamemodes/classic.hpp"

GameScreen::GameScreen(Rml::Context* context)
	: Scene(context)
{
	m_document = m_context->LoadDocument("ui/rml/game_screen.rml");
	m_document->Show();

	Rml::Element* cardContainer = m_document->GetElementById("card_container");

	Rml::ElementList cards;
	cardContainer->GetElementsByClassName(cards, "card-wrapper");

	for (auto card : cards)
	{
		card->AddEventListener(Rml::EventId::Dragend, this);
	}
}

void GameScreen::Destroy()
{

}

void GameScreen::ProcessEvent(Rml::Event& event)
{
	auto element = event.GetTargetElement();
    if (!element) 
		return;

    if (event == Rml::EventId::Dragend)
    {
        float duration = 0.25f; 
        Rml::Tween tween{Rml::Tween::Quadratic, Rml::Tween::Out};

        element->Animate("left", Rml::Property(0.0f, Rml::Unit::PX), duration, tween);
        element->Animate("top",  Rml::Property(0.0f, Rml::Unit::PX), duration, tween);
    }
}