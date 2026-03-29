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

	SceneId GetId() const override { return SceneId::GAME_SCREEN; }

	void Update() override;
	void Destroy() override;

private:
	void CreatePlayersCards();
	void CreateCard(const Card& card, Rml::Element* container);
	void CreateOpponentCards(int count, Rml::Element* container);

	Rml::ElementDocument* m_document = nullptr;

	struct PendingDeletion 
	{
		Rml::Element* Element;
		double Time;
	};

	std::vector<PendingDeletion> m_deletionQueue;
};