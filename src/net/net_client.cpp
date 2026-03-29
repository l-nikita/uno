#include <SDL3/SDL.h>
#include "net_client.hpp"
#include "net_manager.hpp"
#include "net_packet_handler.hpp"

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
    if (m_isRunning)
        return;

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

    switch ((NetConnectState)info.m_eState)
    {
        case NetConnectState::CONNECTING:
        {
            SDL_Log("[Client] Connection in progress...");
            break;
        }
        case NetConnectState::CONNECTED:
        {
            SDL_Log("[Client] Connected!");
            break;
        }
        case NetConnectState::CLOSED_BY_PEER:
            break;
        case NetConnectState::PROBLEM_DETECTED_LOCALLY:
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
    if (!m_isRunning)
        return;

    NetMessage* messages[10];
    int numMsgs = m_interface->ReceiveMessagesOnConnection(m_connection, messages, 10);
    for (size_t i = 0; i < numMsgs; ++i)
    {
        g_PacketHandler->ProcessMessage(messages[i]);
        messages[i]->Release();
    }
    
    m_interface->RunCallbacks();
}

void NetClient::Shutdown()
{
    if (!m_isRunning)
        return;

    SDL_Log("[Client] Shutting down...");

    m_isRunning = false;

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