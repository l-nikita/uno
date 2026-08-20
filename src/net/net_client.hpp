#pragma once

#include <string>
#include "net_common.hpp"
#include "net_message.pb.h"

namespace shared::net
{
    //-----------------------------------------------------------------------------
    //
    //-----------------------------------------------------------------------------
    class Client
    {
    private:
        explicit Client( Interface* interface );
        ~Client() { Shutdown(); }

        void Start( const std::string& ip, uint16_t port );

        void OnConnectionStatusChanged( ConnectionStatusCallback* callback );
        void PollMessages();

        void Shutdown();

        static void _OnConnectionStatusChanged( ConnectionStatusCallback* callback );

    public:
        void SendToServer( const proto::NetMessage& msg );
        bool IsConnected() const { return m_isRunning; }

    private:
        bool m_isRunning = false;

        Interface* m_interface = nullptr;
        Connection m_connection = k_HSteamNetConnection_Invalid;

        friend class NetworkManager;
    };
}
