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

	m_players.clear();
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
	m_players.push_back(new Player("Nikita2"));
	m_players.push_back(new Player("Genrih2"));

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

int GameManager::GetPlayerIndex(const Player* player) const
{
	int id = -1;
	for (int i = 0; i < m_players.size(); ++i) 
	{
		if (m_players.at(i) == player) 
		{
			id = i;
			break;
		}
	}

	return id;
}