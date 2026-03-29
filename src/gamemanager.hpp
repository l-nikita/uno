#pragma once

#include <span>
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

	gm::IGameMode* GetGameMode() const { return m_gameMode; }

	const Players GetPlayers() const { return m_players; }
	const Player* GetLocalPlayer() const { return m_players.at(0); }

	int GetPlayerIndex(const Player* player) const;

private:
	gm::IGameMode* m_gameMode = nullptr;
	Players m_players;
};

extern GameManager* g_GameManager;