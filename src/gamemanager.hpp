#pragma once

#include "gamemodes/igamemode.hpp"

//-----------------------------------------------------------------------------
class Game;

using Players = std::vector<Player*>;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
class GameManager final
{
public:
	GameManager();
	~GameManager();

	void Start(gm::GameModeID gmId);

	void Update();
	void Render();

	gm::IGameMode* GetGameMode() const { return m_gameMode; }

	Players GetPlayers() const { return m_players; }
	Player* GetLocalPlayer() const { return m_players[0]; }

private:
	gm::IGameMode* m_gameMode = nullptr;
	Players m_players;
};

extern GameManager* g_GameManager;