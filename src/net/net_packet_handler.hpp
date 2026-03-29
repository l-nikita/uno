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

    friend NetServer;
};

extern PacketHandler* g_PacketHandler;