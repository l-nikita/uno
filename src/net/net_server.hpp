#pragma once

#include <map>

#include "net_message.pb.h"
#include "net/net_common.hpp"

namespace shared::net
{
    //-----------------------------------------------------------------------------
    //
    //-----------------------------------------------------------------------------
    class Server
    {
    private:
        explicit Server( Interface* interface );
        ~Server() { Shutdown(); }

        void Start( uint16_t port );

        void OnConnectionStatusChanged( ConnectionStatusCallback* callback );
        void PollMessages();

        void Shutdown();

        static void _OnConnectionStatusChanged( ConnectionStatusCallback* callback );

    public:
        void SendToClient( Connection conn, const proto::NetMessage& msg );
        void SendToAllClients( const proto::NetMessage& msg );

    private:
        struct ServerClient
        {
            Identity Identity;

            explicit ServerClient( const net::Identity& identity )
                : Identity( identity )
            {
            }
        };

    private:
        bool m_isRunning = false;

        Interface* m_interface = nullptr;
        Socket m_listenSocket = k_HSteamListenSocket_Invalid;
        PollGroup m_pollGroup = k_HSteamNetPollGroup_Invalid;

        std::map<Connection, ServerClient> m_clients;

        friend class NetworkManager;
    };
}
