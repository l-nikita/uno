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
	g_Game->SetScene(SceneID::MAIN_MENU);
}

GameManager::~GameManager()
{
	delete m_gameMode;

	for (auto& player : m_players)
		delete player;
}

//-----------------------------------------------------------------------------
void GameManager::Start(gm::GameModeID gmId)
{
	if (m_gameMode)
	{
		Rml::Log::Message(Rml::Log::LT_WARNING, "The game has already started");
		return;
	}

	m_players.push_back(new Player("Nikita"));
	m_players.push_back(new Player("Genrih"));

	if (gmId == gm::GameModeID::CLASSIC)
		m_gameMode = new gm::Classic();
	else
		Rml::Log::Message(Rml::Log::LT_ERROR, "Unknown game mode");

	if (m_gameMode)
		m_gameMode->Start();

	g_Game->SetScene(SceneID::GAME_SCREEN);
}

void GameManager::Update()
{
}

void GameManager::Render()
{
}