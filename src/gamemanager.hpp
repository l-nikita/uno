#pragma once

#include <span>
#include "game.hpp"
#include "net/net_manager.hpp"
#include "gamemodes/igamemode.hpp"

//-----------------------------------------------------------------------------
class Game;

using Players = std::vector<Player*>;

constexpr auto MAX_PLAYERS = 4;

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

	int GetPlayerIndex(const Player* player) const;

	void OnClientConnected(NetConnection conn);
	void OnClientDisconnected(NetConnection conn);

	GameStage GetStage() const { return m_stage; }

	void BroadcastGameState();

private:
	gm::IGameMode* m_gameMode = nullptr;
	
	Players m_players;
	GameStage m_stage = GameStage::Lobby;
};

extern GameManager* g_GameManager;