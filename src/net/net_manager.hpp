#pragma once

#include <string>

#include "net/net_client.hpp"
#include "net/net_common.hpp"
#include "net/net_server.hpp"

namespace shared::net
{
    //-----------------------------------------------------------------------------
    //
    //-----------------------------------------------------------------------------
    class NetworkManager final
    {
    public:
        NetworkManager();
        ~NetworkManager();

        void Init();
        void Update();
        void Shutdown();

        void StartHost( uint16_t port );
        void StopHost();

        void StartClient();
        void Connect( const std::string& ip, uint16_t port );
        void Disconnect();

        bool IsHost() const { return m_server != nullptr; }
        Server* GetServer() const { return m_server; }
        Client* GetClient() const { return m_client; }

    private:
        Server* m_server = nullptr;
        Client* m_client = nullptr;

        Interface* m_interface = nullptr;

        friend class Server;
        friend class Client;
    };

    static void SteamNetDebugOutput( ESteamNetworkingSocketsDebugOutputType type, const char* msg )
    {
        SDL_Log( "%s", msg );
    }
}

extern shared::net::NetworkManager* g_NetManager;
