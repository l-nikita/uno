#include <random>
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
		GiveCards();
	}

	void Classic::Stop()
	{

	}

	//-----------------------------------------------------------------------------
	void Classic::Update()
	{

	}

	//-----------------------------------------------------------------------------
	void Classic::GiveCards()
	{
		std::random_device rd;
		std::mt19937 gen(rd());

		for (auto& player : g_GameManager->GetPlayers())
		{
			for (size_t i = 0; i < 7; i++)
			{
				auto card = new Card();

				std::uniform_int_distribution<> dist(0, 5);
				auto type = static_cast<CardType>(dist(gen));
				card->Type = type;

				if (type == CardType::NUMBER)
				{
					std::uniform_int_distribution<> vDist(1, 9);
					card->Value = vDist(gen);
				}
				else if (type == CardType::SKIP
					|| type == CardType::REVERSE
					|| type == CardType::DRAW_2)
				{
					card->Value = 10;
				}
				else if (type == CardType::WILD || type == CardType::WILD_DRAW_4)
				{
					card->Value = 20;
				}

				if (type != CardType::WILD && type != CardType::WILD_DRAW_4)
				{
					std::uniform_int_distribution<> cDist(1, 4);
					int color = cDist(gen);
					if (color == 1)
						card->Color = RED_COLOR_CARD;
					else if (color == 2)
						card->Color = GREEN_COLOR_CARD;
					else if (color == 3)
						card->Color = BLUE_COLOR_CARD;
					else if (color == 4)
						card->Color = YELLOW_COLOR_CARD;
				}
				else
					card->Color = WILD_COLOR_CARD;

				player->GiveCard(card);
			}
		}
	}
}