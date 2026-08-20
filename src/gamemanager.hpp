#pragma once

#include "game.hpp"
#include "state_types.hpp"
#include "gamemodes/igamemode.hpp"
#include "net/net_manager.hpp"

namespace server
{
    //-----------------------------------------------------------------------------

    using Players = std::vector<std::unique_ptr<Player>>;

    //-----------------------------------------------------------------------------

    constexpr auto MAX_PLAYERS = 4;

    //-----------------------------------------------------------------------------
    //
    //-----------------------------------------------------------------------------
    class GameManager final
    {
    public:
        GameManager();
        ~GameManager();

        void Start( mode::GameModeId gmId );
        void Update();

        mode::IGameMode* GetGameMode() const { return m_gameMode.get(); }
        const Players& GetPlayers() { return m_players; }

        Player* GetPlayerAt( std::size_t index ) const { return m_players.at( index ).get(); }
        Player* GetPlayerByConnection( shared::net::Connection conn );

        std::size_t GetPlayerIndex( const Player* player ) const;

        void OnClientConnected( shared::net::Connection conn );
        void OnClientDisconnected( shared::net::Connection conn );
        void OnClientIdentified( const shared::ClientInfo& info );

        shared::GameStage GetStage() const { return m_stage; }

        bool IsGameStarted() const
        {
            return ( GetStage() == shared::GameStage::ROUND_IN_PROGRESS || GetStage() == shared::GameStage::ROUND_END );
        }

        void BroadcastGameState();

        void OnPlayerAction( shared::net::Connection conn, const shared::PlayerAction& action );

    private:
        std::unique_ptr<mode::IGameMode> m_gameMode = nullptr;

        Players m_players;
        shared::GameStage m_stage = shared::GameStage::LOBBY;
    };
}

extern server::GameManager* g_GameManager;
