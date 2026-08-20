#include <iostream>
#include <thread>
#include <SDL3/SDL.h>
#include <steam/isteamnetworkingutils.h>
#include <steam/steamnetworkingsockets.h>
#include <steam/steamnetworkingtypes.h>

#include "net/net_manager.hpp"
#include "net/net_packet_handler.hpp"

shared::net::NetworkManager* g_NetManager = nullptr;

namespace shared::net
{
    //-----------------------------------------------------------------------------
    //
    //-----------------------------------------------------------------------------
    NetworkManager::NetworkManager()
    {
        g_NetManager = this;
        g_PacketHandler = new PacketHandler();
    }

    NetworkManager::~NetworkManager()
    {
        if ( g_PacketHandler )
            delete g_PacketHandler, g_PacketHandler = nullptr;

        Shutdown();
    }

    void NetworkManager::Init()
    {
        ErrorMsg errMsg;
        if ( !GameNetworkingSockets_Init( nullptr, errMsg ) )
            throw std::runtime_error( "Couldn't initialize GameNetworkingSockets: " + std::string( errMsg ) );

        SteamNetworkingUtils()->SetDebugOutputFunction( k_ESteamNetworkingSocketsDebugOutputType_Msg,
                                                        SteamNetDebugOutput );

        m_interface = SteamNetworkingSockets();
    }

    //-----------------------------------------------------------------------------
    //
    //-----------------------------------------------------------------------------
    void NetworkManager::Update()
    {
        if ( m_server )
            m_server->PollMessages();

        if ( m_client )
            m_client->PollMessages();
    }

    //-----------------------------------------------------------------------------
    void NetworkManager::Shutdown()
    {
        if ( m_server )
            delete m_server, m_server = nullptr;

        if ( m_client )
            delete m_client, m_client = nullptr;

        GameNetworkingSockets_Kill();
        m_interface = nullptr;
    }

    //-----------------------------------------------------------------------------
    //
    //-----------------------------------------------------------------------------
    void NetworkManager::StartHost( uint16_t port = 27015 )
    {
        if ( m_server )
            return;

        if ( m_client && m_client->IsConnected() )
            return;

        m_server = new Server( m_interface );
        m_server->Start( port );
    }

    void NetworkManager::StopHost()
    {
        if ( m_server )
            delete m_server, m_server = nullptr;
    }

    void NetworkManager::StartClient()
    {
        if ( m_client )
            return;

        m_client = new Client( m_interface );
    }

    void NetworkManager::Connect( const std::string& ip, uint16_t port )
    {
        if ( !m_client )
            return;

        m_client->Start( ip, port );
    }

    void NetworkManager::Disconnect()
    {
        if ( IsHost() )
            StopHost();

        std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );

        if ( m_client )
            delete m_client, m_client = nullptr;
    }
}
