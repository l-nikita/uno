#pragma once

#include <map>
#include <RmlUi/Core.h>
#include "scene.hpp"
#include "clientmanager.hpp"

class Player;
struct Card;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
class GameScreen final : public Scene, IStateListener
{
public:
	GameScreen(Rml::Context* context);

	void ProcessEvent(Rml::Event& event) override;

	SceneId GetId() const override { return SceneId::GAME_SCREEN; }

	void Update() override;
	void Destroy() override;

	void OnStateUpdate(const StateUpdate& update) override;

private:
	void CreatePlayersCards();
	void CreateCard(const Card& card, int index, Rml::Element* container);
	void CreateOpponentCards(int count, Rml::Element* container);

	Rml::ElementDocument* m_document = nullptr;

	struct PendingDeletion 
	{
		Rml::Element* Element;
		double Time;
	};

	std::vector<PendingDeletion> m_deletionQueue;
};