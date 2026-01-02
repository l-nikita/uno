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
		void Init();
		void Start();
		void Stop();

		void Update();

		void GenerateDeckCards();
		void DealCards();

		size_t GetPlayersCardsNum() { return 7; }
		size_t GetDeckCardsNum() { return m_deck.size(); }

	private:
		std::vector<Card*> m_deck;
	};
}