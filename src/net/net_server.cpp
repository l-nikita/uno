#include <iostream>
#include <format>
#include <SDL3/SDL.h>
#include "../gamemanager.hpp"
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
    if (m_isRunning)
        return;

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

    m_isRunning = true;

    SDL_Log("[Host] Start listening on port %d.", hostAddr.m_port);
}

void NetServer::OnConnectionStatusChanged(NetConnectionStatusCallback* callback)
{
    NetConnection connection = callback->m_hConn;
    auto& info = callback->m_info;

    switch ((NetConnectState)info.m_eState)
    {
        case NetConnectState::CONNECTING:
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
                SDL_Log("[Host] Couldn't set poll group.");
                break;
            }

            SDL_Log("[Host] Connection in progress...");
            break;
        }
        case NetConnectState::CONNECTED:
        {
            SDL_Log("[Host] Client connected!");
            m_clients.emplace(connection, info.m_identityRemote);
            g_GameManager->OnClientConnected(connection);
            
            break;
        }
        case NetConnectState::CLOSED_BY_PEER:
        {
            SDL_Log("[Host] Client disconnected (Reason: %d).", info.m_eEndReason);

            g_GameManager->OnClientDisconnected(connection);

            m_clients.erase(connection);
            m_interface->CloseConnection(connection, 0, nullptr, false);

            break;
        }
        case NetConnectState::PROBLEM_DETECTED_LOCALLY:
        {
            SDL_Log("[Host] Client disconnected (Reason: %d).", info.m_eEndReason);

            g_GameManager->OnClientDisconnected(connection);

            m_clients.erase(connection);
            m_interface->CloseConnection(connection, 0, nullptr, false);

            break;
        }
        default:
            break;
    }
}

void NetServer::SendToClient(NetConnection conn, const proto::NetMessage& msg)
{
    std::string data;
    msg.SerializeToString(&data);
    
    m_interface->SendMessageToConnection(
        conn, 
        data.data(), 
        (uint32_t)data.size(),
        k_nSteamNetworkingSend_Reliable,
        nullptr
    );
}
void NetServer::SendToAllClients(const proto::NetMessage& msg)
{
    for (auto& [conn, client] : m_clients)
        SendToClient(conn, msg);
}

void NetServer::PollMessages()
{
    if (!m_isRunning)
        return;

    NetMessage* messages[10];
    int numMsgs = m_interface->ReceiveMessagesOnPollGroup(m_pollGroup, messages, 10);
    for (size_t i = 0; i < numMsgs; ++i)
    {
        g_PacketHandler->ProcessMessage(messages[i]);
        messages[i]->Release();
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