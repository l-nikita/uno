#include <random>
#include <algorithm>
#include <ranges>
#include <iostream>
#include "classic.hpp"
#include "../gamemanager.hpp"

namespace gm
{
	//-----------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------
	void Classic::Init()
	{

	}

	void Classic::Start()
	{
		GenerateDeckCards();
		DealCards();
	}

	void Classic::Stop()
	{

	}

	//-----------------------------------------------------------------------------
	void Classic::Update()
	{

	}

	//-----------------------------------------------------------------------------
	void Classic::GenerateDeckCards()
	{
		// color
		for (size_t c = 0; c < 4; c++)
		{
			CardColor color = static_cast<CardColor>(c);

			// duplication 
			for (size_t d = 0; d < 2; d++)
			{
				// number
				for (size_t n = 0; n < 10; n++)
				{
					if (n == 0 && d == 1) // 0 not duplicated
						continue;

					auto num = new Card();
					num->Type = CardType::NUMBER;
					num->Value = n;
					num->Color = color;

					m_deck.push_back(num);
				}

				// skip
				auto skip = new Card();
				skip->Type = CardType::SKIP;
				skip->Value = 10;
				skip->Color = color;

				m_deck.push_back(skip);

				// reverse
				auto reverse = new Card();
				reverse->Type = CardType::REVERSE;
				reverse->Value = 10;
				reverse->Color = color;

				m_deck.push_back(reverse);

				// draw 2
				auto draw2 = new Card();
				draw2->Type = CardType::DRAW_2;
				draw2->Value = 10;
				draw2->Color = color;

				m_deck.push_back(draw2);
			}
		}

		// wild
		for (size_t i = 0; i < 4; i++)
		{
			auto wild = new Card();
			wild->Type = CardType::WILD;
			wild->Value = 20;
			wild->Color = CardColor::WILD;

			m_deck.push_back(wild);
		}

		// wild +4
		for (size_t i = 0; i < 4; i++)
		{
			auto wild4 = new Card();
			wild4->Type = CardType::WILD_DRAW_4;
			wild4->Value = 20;
			wild4->Color = CardColor::WILD;

			m_deck.push_back(wild4);
		}

		std::cout << m_deck.size() << std::endl;
	}

	void Classic::DealCards()
	{
		std::random_device rd;
		std::mt19937 gen(rd());

		std::ranges::shuffle(m_deck, gen);

		for (auto& player : g_GameManager->GetPlayers())
		{
			for (size_t i = 0; i < GetPlayersCardsNum(); i++)
			{
				auto last = m_deck.back();
				player->GiveCard(last);
				m_deck.pop_back();
			}

			player->SortCards();
		}
	}
}