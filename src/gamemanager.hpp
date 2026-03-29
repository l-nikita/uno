#pragma once

#include <span>
#include "net/net_manager.hpp"
#include "gamemodes/igamemode.hpp"

//-----------------------------------------------------------------------------
class Game;

using Players = std::vector<Player*>;
using NetPlayers = std::map<NetConnection, int>;

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

	void OnClientConnected(NetConnection conn);
	void BroadcastGameState();

private:
	gm::IGameMode* m_gameMode = nullptr;
	
	Players m_players;
	NetPlayers m_connectionToPlayer;
};

extern GameManager* g_GameManager;