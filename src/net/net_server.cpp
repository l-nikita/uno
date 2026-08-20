#include <format>
#include <SDL3/SDL.h>

#include "gamemanager.hpp"
#include "net/net_server.hpp"
#include "net/net_manager.hpp"
#include "net/net_packet_handler.hpp"

namespace shared::net
{
    //-----------------------------------------------------------------------------
    //
    //-----------------------------------------------------------------------------
    Server::Server( Interface* interface )
        : m_interface( interface )
    {
    }

    //-----------------------------------------------------------------------------
    //
    //-----------------------------------------------------------------------------
    void Server::Start( uint16_t port )
    {
        if ( m_isRunning )
            return;

        Address hostAddr;
        hostAddr.Clear();
        hostAddr.m_port = port;

        Config config;
        config.SetPtr( k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*)_OnConnectionStatusChanged );

        m_listenSocket = SteamNetworkingSockets()->CreateListenSocketIP( hostAddr, 1, &config );
        if ( m_listenSocket == k_HSteamListenSocket_Invalid )
            throw std::runtime_error( std::format( "Failed to listen on port {}", hostAddr.m_port ) );

        m_pollGroup = m_interface->CreatePollGroup();
        if ( m_pollGroup == k_HSteamNetPollGroup_Invalid )
            throw std::runtime_error( std::format( "Failed to listen on port {}", hostAddr.m_port ) );

        m_isRunning = true;

        SDL_Log( "[Host] Start listening on port %d.", hostAddr.m_port );
    }

    //-----------------------------------------------------------------------------
    //
    //-----------------------------------------------------------------------------
    void Server::OnConnectionStatusChanged( ConnectionStatusCallback* callback )
    {
        Connection connection = callback->m_hConn;
        auto& info = callback->m_info;

        switch ( static_cast<ConnectState>(info.m_eState) )
        {
            case ConnectState::CONNECTING:
            {
                if ( m_interface->AcceptConnection( connection ) != k_EResultOK )
                {
                    m_interface->CloseConnection( connection, 0, "Failed to accept", false );
                    SDL_Log( "[Host] Couldn't accept connection." );
                    break;
                }

                if ( !m_interface->SetConnectionPollGroup( connection, m_pollGroup ) )
                {
                    m_interface->CloseConnection( connection, 0, "Failed to set poll group", false );
                    SDL_Log( "[Host] Couldn't set poll group." );
                    break;
                }

                if ( g_GameManager->IsGameStarted() )
                {
                    m_interface->CloseConnection( connection, 0, "The game has been already started", false );
                    break;
                }

                SDL_Log( "[Host] Connection in progress..." );
                break;
            }
            case ConnectState::CONNECTED:
            {
                SDL_Log( "[Host] Client connected!" );
                m_clients.emplace( connection, info.m_identityRemote );
                g_GameManager->OnClientConnected( connection );

                break;
            }
            case ConnectState::CLOSED_BY_PEER:
            {
                SDL_Log( "[Host] Client disconnected (Reason: %d).", info.m_eEndReason );

                g_GameManager->OnClientDisconnected( connection );

                m_clients.erase( connection );
                m_interface->CloseConnection( connection, 0, nullptr, false );

                break;
            }
            case ConnectState::PROBLEM_DETECTED_LOCALLY:
            {
                SDL_Log( "[Host] Client disconnected (Reason: %d).", info.m_eEndReason );

                g_GameManager->OnClientDisconnected( connection );

                m_clients.erase( connection );
                m_interface->CloseConnection( connection, 0, nullptr, false );

                break;
            }
            default:
                break;
        }
    }

    //-----------------------------------------------------------------------------
    //
    //-----------------------------------------------------------------------------
    void Server::PollMessages()
    {
        if ( !m_isRunning )
            return;

        Message* messages[ 10 ];
        int numMsgs = m_interface->ReceiveMessagesOnPollGroup( m_pollGroup, messages, 10 );
        for ( size_t i = 0; i < numMsgs; ++i )
        {
            g_PacketHandler->ProcessMessage( messages[ i ] );
            messages[ i ]->Release();
        }

        m_interface->RunCallbacks();
    }

    //-----------------------------------------------------------------------------
    //
    //-----------------------------------------------------------------------------
    void Server::Shutdown()
    {
        SDL_Log( "[Host] Shutting down..." );

        m_isRunning = false;

        SDL_Log( "[Host] Closing connections..." );
        for ( auto& client: m_clients )
            m_interface->CloseConnection( client.first, 0, "Shutdown", true );

        m_clients.clear();

        m_interface->CloseListenSocket( m_listenSocket );
        m_listenSocket = k_HSteamListenSocket_Invalid;

        m_interface->DestroyPollGroup( m_pollGroup );
        m_pollGroup = k_HSteamNetPollGroup_Invalid;

        SDL_Log( "[Host] Shutdown complete." );
    }

    //-----------------------------------------------------------------------------
    void Server::_OnConnectionStatusChanged( ConnectionStatusCallback* callback )
    {
        g_NetManager->m_server->OnConnectionStatusChanged( callback );
    }

    //-----------------------------------------------------------------------------
    //
    //-----------------------------------------------------------------------------
    void Server::SendToClient( Connection conn, const proto::NetMessage& msg )
    {
        std::string data;
        msg.SerializeToString( &data );

        m_interface->SendMessageToConnection(
            conn,
            data.data(),
            (uint32_t)data.size(),
            k_nSteamNetworkingSend_Reliable,
            nullptr
        );
    }

    //-----------------------------------------------------------------------------
    void Server::SendToAllClients( const proto::NetMessage& msg )
    {
        for ( const auto& conn: m_clients | std::views::keys )
            SendToClient( conn, msg );
    }
}
