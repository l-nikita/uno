#include <SDL3/SDL.h>
#include "net_client.hpp"
#include "net_manager.hpp"

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
NetClient::NetClient(NetInterface* interface)
    : m_interface(interface)
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void NetClient::Start(const std::string& ip, uint16_t port)
{
    NetAddress hostAddr;
    hostAddr.Clear();
    hostAddr.ParseString(ip.c_str());
    hostAddr.m_port = port;

    SDL_Log("[Client] Connecting to host at %s:%d.", ip.c_str(), port);

    NetConfig config;
    config.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*)_OnConnectionStatusChanged);

    m_connection = m_interface->ConnectByIPAddress(hostAddr, 1, &config);
    if (m_connection == k_HSteamNetConnection_Invalid)
    {
        SDL_Log("[Client] Failed to connect to %s:%d.", ip.c_str(), port);
        return;
    }

    m_isRunning = true;
}

void NetClient::OnConnectionStatusChanged(NetConnectionStatusCallback* callback)
{
    NetConnection connection = callback->m_hConn;
    auto& info = callback->m_info;

    switch (info.m_eState)
    {
        case k_ESteamNetworkingConnectionState_Connecting:
        {
            SDL_Log("[Client] Connection in progress...");
            break;
        }
        case k_ESteamNetworkingConnectionState_Connected:
        {
            SDL_Log("[Client] Connected!");
            break;
        }
        case k_ESteamNetworkingConnectionState_ClosedByPeer:
            break;
        case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
        {
            SDL_Log("[Client] Connection closed.");
            m_isRunning = false;
            break;
        }
        default:
            break;
    }
}

void NetClient::PollMessages()
{
    NetMessage* pIncomingMsg[10];
    int numMsgs = m_interface->ReceiveMessagesOnConnection(m_connection, pIncomingMsg, 10);
    for (int i = 0; i < numMsgs; ++i)
    {
        std::string msg((char*)pIncomingMsg[i]->m_pData, pIncomingMsg[i]->m_cbSize);
        SDL_Log("Received: %s", msg);
        pIncomingMsg[i]->Release();
    }
    
    m_interface->RunCallbacks();
}

void NetClient::Shutdown()
{
    SDL_Log("[Client] Shutting down...");
    if (m_connection != k_HSteamNetConnection_Invalid)
    {
        m_interface->CloseConnection(m_connection, 0, "Client Shutdown", true);
        m_connection = k_HSteamNetConnection_Invalid;
    }

    SDL_Log("[Client] Shutdown complete.");
}

void NetClient::_OnConnectionStatusChanged(NetConnectionStatusCallback* callback)
{
    g_NetManager->m_client->OnConnectionStatusChanged(callback);
}