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
	void AskPlayOrKeep();
	void HidePlayOrKeep();

	void ChooseColor(int cardId);
	void HideColorSelection();

	void PlayCard(int cardId);

	void CreatePlayersCards();
	void CreateTopDiscardCard();

	Rml::ElementPtr CreateCard(const Card& card, bool isStatic);
	void CreateCard(const Card& card, int index, Rml::Element* container);
	void CreateOpponentCards(int count, Rml::Element* container);

	bool CanPlay();
	bool CanPlayCard(int index);
	bool CanPlayCard(const Card& card);
	bool HasNoPlayableCards();

	Rml::ElementDocument* m_document = nullptr;

	struct PendingDeletion 
	{
		Rml::Element* Element;
		double Time;
	};

	std::vector<PendingDeletion> m_deletionQueue;

	bool m_drawCard = false;
};