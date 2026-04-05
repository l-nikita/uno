#pragma once

#include <string>
#include "net_common.hpp"
#include "net_message.pb.h"

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
class NetClient
{
public:
    void SendToServer(const proto::NetMessage& msg);
    bool IsConnected() { return m_isRunning; }

private:
    NetClient(NetInterface* interface);
    ~NetClient() { Shutdown(); }

    void Start(const std::string& ip, uint16_t port);
    
    void OnConnectionStatusChanged(NetConnectionStatusCallback* callback);
    void PollMessages();

    void Shutdown();

    static void _OnConnectionStatusChanged(NetConnectionStatusCallback* callback);

private:
    bool m_isRunning = false;

    NetInterface* m_interface = nullptr;
    NetConnection m_connection = k_HSteamNetConnection_Invalid;

    friend class NetworkManager;
};