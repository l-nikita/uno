#pragma once

#include "net/net_manager.hpp"
#include "net/net_server.hpp"

namespace shared::net
{
    //-----------------------------------------------------------------------------
    //
    //-----------------------------------------------------------------------------
    class PacketHandler
    {
    public:
        PacketHandler();
        ~PacketHandler();

    private:
        void ProcessMessage( Message* msg );

        friend class Server;
        friend class Client;
    };
}

extern shared::net::PacketHandler* g_PacketHandler;
