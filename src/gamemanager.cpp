#include "game.hpp"
#include "gamemanager.hpp"
#include "gamemodes/classic.hpp"
#include "filesystem.hpp"

GameManager* g_GameManager = nullptr;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GameManager::GameManager()
{
	g_GameManager = this;

	g_Game->GetTextEngine()->CreateFont("Arial_50", GetAssetsPath() + "/fonts/arial.ttf", 50.0f);
	g_Game->GetTextEngine()->CreateFont("Arial_35", GetAssetsPath() + "/fonts/arial.ttf", 35.0f);

	m_players.push_back(new Player("Nikita"));
	m_players.push_back(new Player("Genrih"));

	m_gameMode = new gm::Classic();
	m_gameMode->Start();
}

GameManager::~GameManager()
{
	delete m_gameMode;

	for (auto& player : m_players)
		delete player;
}

//-----------------------------------------------------------------------------
void GameManager::Update()
{

}

void GameManager::Render()
{
	auto renderer = g_Game->GetRenderer();
	auto te = g_Game->GetTextEngine();

	float x = 50;
	for (auto& player : m_players)
	{
		SDL_FRect rect = { x, 0, 500, 100 };

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderFillRect(renderer, &rect);

		SDL_Color color = { 255, 255, 255, 255 };
		te->DrawText(player->GetName(), "Arial_50", x + 25, 25, color);

		x += 600;
	}

	auto localPlayer = GetLocalPlayer();
	auto cards = localPlayer->GetCards();

	x = 25;

	int wW, wH;
	g_Game->GetWindowSize(&wW, &wH);

	for (auto& card : cards)
	{
		float y = (float)wH - 300;
		SDL_FRect rect = { x, y, 150, 250 };

		SDL_SetRenderDrawColor(renderer, card->Color.r, card->Color.g, card->Color.b, card->Color.a);
		SDL_RenderFillRect(renderer, &rect);

		SDL_Color color = { 255, 255, 255, 255 };
		std::string text = "";

		if (card->Type == CardType::NUMBER)
			text = std::to_string(card->Value);
		else if (card->Type == CardType::SKIP)
			text = "Skip";
		else if (card->Type == CardType::REVERSE)
			text = "Reverse";
		else if (card->Type == CardType::DRAW_2)
			text = "+2";
		else if (card->Type == CardType::WILD)
			text = "Wild";
		else if (card->Type == CardType::WILD_DRAW_4)
			text = "+4";

		te->DrawText(text, "Arial_35", x + 15, y + 15, color);

		x += 200;
	}
}