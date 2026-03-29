#include <iostream>
#include <format>
#include <SDL3/SDL.h>
#include "net_server.hpp"
#include "net_manager.hpp"
#include "net_packet_handler.hpp"

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
NetServer::NetServer(NetInterface* interface)
    : m_interface(interface)
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void NetServer::Start(uint16_t port)
{
    NetAddress hostAddr;
    hostAddr.Clear();
    hostAddr.m_port = port;

    NetConfig config;
    config.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*)_OnConnectionStatusChanged);

    m_listenSocket = SteamNetworkingSockets()->CreateListenSocketIP(hostAddr, 1, &config);
    if (m_listenSocket == k_HSteamListenSocket_Invalid)
        throw std::runtime_error(std::format("Failed to listen on port {}", hostAddr.m_port));

    m_pollGroup = m_interface->CreatePollGroup();
    if (m_pollGroup == k_HSteamNetPollGroup_Invalid)
        throw std::runtime_error(std::format("Failed to listen on port {}", hostAddr.m_port));

    SDL_Log("[Host] Start listening on port %d.", hostAddr.m_port);

    m_isRunning = true;
}

void NetServer::OnConnectionStatusChanged(NetConnectionStatusCallback* callback)
{
    NetConnection connection = callback->m_hConn;
    auto& info = callback->m_info;

    switch (info.m_eState)
    {
        case k_ESteamNetworkingConnectionState_Connecting:
        {
            if (m_interface->AcceptConnection(connection) != k_EResultOK)
            {
                m_interface->CloseConnection(connection, 0, "Failed to accept", false);
                SDL_Log("[Host] Couldn't accept connection.");
                break;
            }

            if (!m_interface->SetConnectionPollGroup(connection, m_pollGroup))
            {
                m_interface->CloseConnection(connection, 0, "Failed to set poll group", false);
                SDL_Log("[Host] Could not set poll group.");
                break;
            }

            SDL_Log("[Host] Connection in progress...");
            break;
        }
        case k_ESteamNetworkingConnectionState_Connected:
        {
            SDL_Log("[Host] Client connected! Handle: %u", connection);
            m_clients.emplace(connection, info.m_identityRemote);
            break;
        }
        case k_ESteamNetworkingConnectionState_ClosedByPeer:
            break;
        case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
        {
            SDL_Log("[Host] Client disconnected (Reason: %d).", info.m_eEndReason);
            m_interface->CloseConnection(connection, 0, nullptr, false);
            break;
        }
        default:
            break;
    }
}

void NetServer::PollMessages()
{
    NetMessage* msg = nullptr;

    int numMsgs = m_interface->ReceiveMessagesOnPollGroup(m_pollGroup, &msg, 1);
    if (numMsgs > 0 && msg)
    {
        g_PacketHandler->ProcessMessage(msg);
        msg->Release();
    }

    m_interface->RunCallbacks();
}

void NetServer::Shutdown()
{
    SDL_Log("[Host] Shutting down...");

    m_isRunning = false;

    SDL_Log("[Host] Closing connections...");
    for (auto& client : m_clients)
        m_interface->CloseConnection(client.first, 0, "Shutdown", true);

    m_clients.clear();

    m_interface->CloseListenSocket(m_listenSocket);
    m_listenSocket = k_HSteamListenSocket_Invalid;

    m_interface->DestroyPollGroup(m_pollGroup);
    m_pollGroup = k_HSteamNetPollGroup_Invalid;

    SDL_Log("[Host] Shutdown complete.");
}

void NetServer::_OnConnectionStatusChanged(NetConnectionStatusCallback* callback)
{
    g_NetManager->m_server->OnConnectionStatusChanged(callback);
}