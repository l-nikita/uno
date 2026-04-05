#pragma once

#include <span>
#include "game.hpp"
#include "state_types.hpp"
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

	gm::IGameMode* GetGameMode() { return m_gameMode; }
	const Players GetPlayers() { return m_players; }

	Player* GetPlayerAt(int index) { return m_players.at(index); }
	Player* GetPlayerByConnection(NetConnection conn);

	int GetPlayerIndex(const Player* player);

	void OnClientConnected(NetConnection conn);
	void OnClientDisconnected(NetConnection conn);
	void OnClientIdentified(const ClientInfo& info);

	GameStage GetStage() const { return m_stage; }

	void BroadcastGameState();

	void OnPlayerAction(NetConnection conn, const PlayerAction& action);

private:
	gm::IGameMode* m_gameMode = nullptr;
	
	Players m_players;
	GameStage m_stage = GameStage::Lobby;
};

extern GameManager* g_GameManager;