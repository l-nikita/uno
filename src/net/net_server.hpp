#pragma once

#include <map>
#include "net_common.hpp"

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
class NetServer
{
private:
    NetServer(NetInterface* interface);

    void Start(uint16_t port);
    
    void OnConnectionStatusChanged(NetConnectionStatusCallback* callback);
    void PollMessages();

    void Shutdown();

    static void _OnConnectionStatusChanged(NetConnectionStatusCallback* callback);
    
private:
    struct NetClient2
    {
        NetIdentity Identity;

        NetClient2(const NetIdentity& identity) 
            : Identity(identity) 
        { }
    };

private:
    bool m_isRunning = false;

    NetInterface* m_interface = nullptr;
    NetSocket m_listenSocket;
    NetPollGroup m_pollGroup;
    
    std::map<NetConnection, NetClient2> m_clients;

    friend class NetworkManager;
};