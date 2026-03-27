#pragma once

#include <RmlUi/Core.h>
#include "scene.hpp"

class Player;
struct Card;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
class GameScreen final : public Scene
{
public:
	GameScreen(Rml::Context* context);

	void ProcessEvent(Rml::Event& event) override;

	SceneID GetID() const override { return SceneID::GAME_SCREEN; }

	void Update() override;
	void Destroy() override;

private:
	void CreatePlayerCards(Player* player);
	void CreateCard(Card* card, Rml::Element* container, bool isVisible);

	Rml::ElementDocument* m_document = nullptr;

	struct PendingDeletion 
	{
		Rml::Element* Element;
		double Time;
	};

	std::vector<PendingDeletion> m_deletionQueue;
};