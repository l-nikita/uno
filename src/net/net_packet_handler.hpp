#pragma once

#include "net_manager.hpp"
#include "net_server.hpp"

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
class PacketHandler
{
public:
    PacketHandler();
    ~PacketHandler();

private:
    void ProcessMessage(NetMessage* msg);

    friend class NetServer;
    friend class NetClient;
};

extern PacketHandler* g_PacketHandler;