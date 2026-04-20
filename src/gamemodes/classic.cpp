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
	Classic::~Classic()
	{
		for (auto card : m_deck)
			delete card;

		m_deck.clear();
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

	void Classic::OnPlayerAction(Player* player, const PlayerAction& action)
	{
		auto currentPlayer = g_GameManager->GetPlayerAt(m_currentPlayerIndex);
		if (player != currentPlayer)
		{
			SDL_Log("Player [%s] tried to act out of turn!", player->GetName().c_str());
			return;
		}

		bool turnEnded = false;

		if (action.Type == ActionType::PLAY_CARD)
		{
			auto card = player->DropCard(action.CardId);
			player->m_LastCard = card;

			if (card)
			{
				AddCardToDiscardPile(card);
				if (card->Type == CardType::WILD || card->Type == CardType::WILD_DRAW_4)
					card->Color = action.ChosenColor;

				if (card->Type == CardType::REVERSE)
					m_reverse = !m_reverse;
				else if (card->Type == CardType::SKIP)
					m_skip = true;
				else if (card->Type == CardType::DRAW_2 || card->Type == CardType::WILD_DRAW_4)
				{
					auto nextPlayer = g_GameManager->GetPlayerAt(GetNextPlayer());
					if (nextPlayer != player)
					{
						for (size_t i = 0; i < (card->Type == CardType::WILD_DRAW_4 ? 4 : 2); i++)
							nextPlayer->GiveCard(TakeCardFromDeck());	

						m_skip = true;
					}
				}
			}

			turnEnded = true;
		}
		else if (action.Type == ActionType::DRAW_CARD)
		{
			if (m_drawCard)
			{
				turnEnded = true;
			}
			else
			{
				auto card = TakeCardFromDeck();
				player->m_LastCard = card;
				player->GiveCard(card);
				player->SortCards();

				m_drawCard = true;

				if (HasNoPlayableCards())
					turnEnded = true;
			}
		}

		if (turnEnded)
			NextTurn();

		g_GameManager->BroadcastGameState();
	}

	int Classic::GetNextPlayer()
	{
		auto s = m_skip ? 2 : 1;
		if (m_reverse)
			s = -s;

		return (m_currentPlayerIndex + s) % g_GameManager->GetPlayers().size();
	}

	void Classic::NextTurn()
	{
		auto player = g_GameManager->GetPlayerAt(m_currentPlayerIndex);
		if (player)
			player->m_LastCard = nullptr;

		m_currentPlayerIndex = GetNextPlayer();
		m_skip = false;
		m_drawCard = false;
	}

	void Classic::AddCardToDiscardPile(Card* card)
	{
		if (!card)
			return;

		m_discardPile.push_back(card);
	}

	Card* Classic::TakeCardFromDeck()
	{
		if (m_deck.empty())
		{
			SDL_Log("Deck is empty.");
			return nullptr;
		}

		auto last = m_deck.back();
		m_deck.pop_back();
		return last;
	}

	bool Classic::CanPlayCard(Card* card)
	{
		auto topC = GetTopDiscardCard();

		bool sameColor = (topC->Color == card->Color);
		bool sameType = (topC->Type == card->Type);
		bool sameValue = (topC->Value == card->Value);
		bool isWild = (card->Type == CardType::WILD || card->Type == CardType::WILD_DRAW_4);

		if (topC->Color == CardColor::WILD)
			return true;

		if (isWild)
			return true;

		if (sameColor)
			return true;

		if (sameType && sameValue)
			return true;
			
		return false;
	}

	bool Classic::HasNoPlayableCards()
	{
		auto hand = g_GameManager->GetPlayerAt(GetCurrentPlayerIndex())->GetCards();
		for (size_t i = 0; i < hand.size(); i++)
		{
			if (CanPlayCard(hand.at(i)))
				return false;
		}

		return true;
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
				player->GiveCard(TakeCardFromDeck());

			player->SortCards();
		}

		// Initial discard card
		if (!m_deck.empty())
		{
			AddCardToDiscardPile(TakeCardFromDeck());
		}

		std::cout << m_deck.size() << std::endl;
	}
}