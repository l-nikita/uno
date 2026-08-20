#include "gamemanager.hpp"
#include "game.hpp"
#include "net_message.pb.h"
#include "gamemodes/classic.hpp"

server::GameManager* g_GameManager = nullptr;

namespace server
{
    //-----------------------------------------------------------------------------

    using namespace shared;

    //-----------------------------------------------------------------------------
    //
    //-----------------------------------------------------------------------------
    GameManager::GameManager()
    {
        g_GameManager = this;
    }

    GameManager::~GameManager()
    {
        m_players.clear();
    }

    //-----------------------------------------------------------------------------
    void GameManager::Start( mode::GameModeId gmId )
    {
        if ( IsGameStarted() )
        {
            Rml::Log::Message( Rml::Log::LT_WARNING, "The game has been already started" );
            return;
        }

        if ( gmId == mode::GameModeId::CLASSIC )
            m_gameMode = std::make_unique<mode::Classic>();
        else
            Rml::Log::Message( Rml::Log::LT_ERROR, "Unknown game mode" );

        if ( GetGameMode() )
            GetGameMode()->Start();

        Rml::Log::Message( Rml::Log::LT_INFO, "The game has been started!" );

        m_stage = GameStage::ROUND_IN_PROGRESS;

        BroadcastGameState();
    }

    void GameManager::Update()
    {
        if ( GetGameMode() )
            GetGameMode()->Update();
    }

    Player* GameManager::GetPlayerByConnection( net::Connection conn )
    {
        auto it = std::ranges::find_if( m_players, [&]( const std::unique_ptr<Player>& p )
        {
            return p->GetConnection() == conn;
        } );

        return ( it != m_players.end() ) ? it->get() : nullptr;
    }

    std::size_t GameManager::GetPlayerIndex( const Player* player ) const
    {
        std::size_t id = -1;
        for ( std::size_t i = 0; i < m_players.size(); ++i )
        {
            if ( m_players.at( i ).get() == player )
            {
                id = i;
                break;
            }
        }

        return id;
    }

    //-----------------------------------------------------------------------------
    void GameManager::OnClientConnected( net::Connection conn )
    {
    }

    void GameManager::OnClientDisconnected( net::Connection conn )
    {
        auto it = std::ranges::find_if( m_players, [&]( const std::unique_ptr<Player>& p )
        {
            return p->GetConnection() == conn;
        } );

        if ( it != m_players.end() )
        {
            m_players.erase( it );
            BroadcastGameState();
        }
    }

    void GameManager::OnClientIdentified( const ClientInfo& info )
    {
        auto it = std::ranges::find_if( m_players, [&]( const std::unique_ptr<Player>& p )
        {
            return p->GetConnection() == info.Connection;
        } );

        if ( it == m_players.end() )
        {
            auto player = std::make_unique<Player>( info );
            SDL_Log( "Player '%s' (conn %u) identified!", player->GetName().c_str(), player->GetConnection() );

            m_players.push_back( std::move( player ) );

            BroadcastGameState();
        }
    }

    //-----------------------------------------------------------------------------
    void GameManager::BroadcastGameState()
    {
        for ( auto& clPlayer: m_players )
        {
            auto clIndex = clPlayer->GetIndex();
            proto::NetMessage netMsg;
            proto::ServerGameState* state = netMsg.mutable_game_state();
            state->set_stage( static_cast<int>(GetStage()) );

            if ( GetGameMode() )
            {
                if ( auto topDiscard = GetGameMode()->GetTopDiscardCard() )
                {
                    auto card = state->mutable_top_discard();
                    card->set_type( static_cast<int>(topDiscard->Type) );
                    card->set_color( static_cast<int>(topDiscard->Color) );
                    card->set_value( topDiscard->Value );
                }

                state->set_current_player( GetGameMode()->GetCurrentPlayerIndex() );
                state->set_reverse( GetGameMode()->IsReverse() );
            }

            for ( std::size_t i = 0; i < m_players.size(); ++i )
            {
                auto player = GetPlayerAt( i );

                proto::PlayerInfo* info = state->add_players();
                info->set_index( i );
                info->set_name( player->GetName() );
                info->set_is_host( ( i == 0 ) );
                info->set_is_local( ( clIndex == i ) );

                info->set_last_card_id( -1 );

                auto hand = player->GetCards();
                for ( std::size_t j = 0; j < hand.size(); ++j )
                {
                    auto card = hand.at( j );

                    proto::Card* c = info->add_hand();
                    c->set_type( static_cast<int>(card->Type) );
                    c->set_color( static_cast<int>(card->Color) );
                    c->set_value( card->Value );

                    if ( card == player->m_LastCard )
                        info->set_last_card_id( j );
                }
            }

            g_NetManager->GetServer()->SendToClient( clPlayer->GetConnection(), netMsg );
        }
    }

    void GameManager::OnPlayerAction( net::Connection conn, const PlayerAction& action )
    {
        auto player = GetPlayerByConnection( conn );
        //if (player)
        //	SDL_Log("Player [%s] action: %i, %i", player->GetName().c_str(), (int)action.Type, (int)action.CardId);

        if ( GetGameMode() && player )
            GetGameMode()->OnPlayerAction( player, action );
    }
}
