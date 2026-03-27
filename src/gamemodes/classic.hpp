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
		void Init() override;
		void Start() override;
		void Stop() override;

		void Update() override;

		void GenerateDeckCards();
		void DealCards();

		size_t GetPlayersCardsNum() { return 7; }
		size_t GetDeckCardsNum() override { return m_deck.size(); }

		GameModeID GetID() const override { return GameModeID::CLASSIC; }

	private:
		Cards m_deck;
	};
}