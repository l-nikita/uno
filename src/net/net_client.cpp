#include <SDL3/SDL.h>

#include "clientmanager.hpp"
#include "net/net_client.hpp"
#include "net/net_manager.hpp"
#include "net/net_packet_handler.hpp"

namespace shared::net
{
    //-----------------------------------------------------------------------------
    //
    //-----------------------------------------------------------------------------
    Client::Client( Interface* interface )
        : m_interface( interface )
    {
    }

    //-----------------------------------------------------------------------------
    //
    //-----------------------------------------------------------------------------
    void Client::Start( const std::string& ip, uint16_t port )
    {
        if ( m_isRunning )
            return;

        Address hostAddr;
        hostAddr.Clear();
        hostAddr.ParseString( ip.c_str() );
        hostAddr.m_port = port;

        SDL_Log( "[Client] Connecting to host at %s:%d.", ip.c_str(), port );

        Config config;
        config.SetPtr( k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*)_OnConnectionStatusChanged );

        m_connection = m_interface->ConnectByIPAddress( hostAddr, 1, &config );
        if ( m_connection == k_HSteamNetConnection_Invalid )
        {
            SDL_Log( "[Client] Failed to connect to %s:%d.", ip.c_str(), port );
            return;
        }

        m_isRunning = true;
    }

    //-----------------------------------------------------------------------------
    void Client::OnConnectionStatusChanged( ConnectionStatusCallback* callback )
    {
        Connection connection = callback->m_hConn;
        auto& info = callback->m_info;

        switch ( static_cast<ConnectState>(info.m_eState) )
        {
            case ConnectState::CONNECTING:
            {
                SDL_Log( "[Client] Connection in progress..." );
                break;
            }
            case ConnectState::CONNECTED:
            {
                SDL_Log( "[Client] Connected!" );
                g_ClientManager->OnConnected();

                m_isRunning = true;
                break;
            }
            case ConnectState::CLOSED_BY_PEER:
            {
                SDL_Log( "[Client] Connection closed." );
                g_ClientManager->OnDisconnected();

                m_isRunning = false;
                break;
            }
            case ConnectState::PROBLEM_DETECTED_LOCALLY:
            {
                SDL_Log( "[Client] Connection closed." );
                g_ClientManager->OnDisconnected();

                m_isRunning = false;
                break;
            }
            default:
                break;
        }
    }

    void Client::PollMessages()
    {
        if ( !m_isRunning )
            return;

        Message* messages[ 10 ];
        int numMsgs = m_interface->ReceiveMessagesOnConnection( m_connection, messages, 10 );
        for ( size_t i = 0; i < numMsgs; ++i )
        {
            g_PacketHandler->ProcessMessage( messages[ i ] );
            messages[ i ]->Release();
        }

        m_interface->RunCallbacks();
    }

    //-----------------------------------------------------------------------------
    void Client::Shutdown()
    {
        SDL_Log( "[Client] Shutting down..." );

        m_isRunning = false;

        if ( m_connection != k_HSteamNetConnection_Invalid )
        {
            m_interface->CloseConnection( m_connection, 0, "Client Shutdown", true );
            m_connection = k_HSteamNetConnection_Invalid;
        }

        if ( g_ClientManager )
            g_ClientManager->OnDisconnected();

        SDL_Log( "[Client] Shutdown complete." );
    }

    void Client::_OnConnectionStatusChanged( ConnectionStatusCallback* callback )
    {
        g_NetManager->m_client->OnConnectionStatusChanged( callback );
    }

    //-----------------------------------------------------------------------------
    void Client::SendToServer( const proto::NetMessage& msg )
    {
        std::string buffer;
        if ( !msg.SerializeToString( &buffer ) )
            return;

        m_interface->SendMessageToConnection( m_connection, buffer.data(), buffer.size(),
                                              k_nSteamNetworkingSend_Reliable, nullptr );
    }
}
