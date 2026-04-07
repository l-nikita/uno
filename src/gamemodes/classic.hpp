#pragma once

#include <vector>
#include "igamemode.hpp"

namespace gm
{
	//-----------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------
	class Classic : public IGameMode
	{
	public:
		~Classic();

		void Start() override;
		void Stop() override;

		void Update() override;
		void OnPlayerAction(Player* player, const PlayerAction& action) override;

		void NextTurn();

		void GenerateDeckCards();
		void DealCards();
		void AddCardToDiscardPile(Card* card);
		Card* TakeCardFromDeck();

		bool CanPlayCard(Card* card);
		bool HasNoPlayableCards();

		Card* GetTopDiscardCard() override { return m_discardPile.empty() ? nullptr : m_discardPile.back(); }
		int GetCurrentPlayerIndex() override { return m_currentPlayerIndex; }
		bool IsReverse() override { return m_reverse; }

		int GetNextPlayer();

		int GetPlayersCardsNum() { return 7; }
		int GetDeckCardsNum() override { return m_deck.size(); }

		GameModeID GetID() const override { return GameModeID::CLASSIC; }

	private:
		Cards m_deck;
		Cards m_discardPile;

		bool m_reverse = false;
		bool m_skip = false;
		bool m_drawCard = false;

		int m_currentPlayerIndex = 0;
	};
}