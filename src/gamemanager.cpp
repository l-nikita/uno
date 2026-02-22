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
}