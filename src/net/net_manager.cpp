#include <iostream>
#include <SDL3/SDL.h>
#include <steam/steamnetworkingtypes.h>
#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>
#include "net_manager.hpp"
#include "net_packet_handler.hpp"

NetworkManager* g_NetManager = nullptr;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
NetworkManager::NetworkManager()
{
	g_NetManager = this;
    g_PacketHandler = new PacketHandler();
}

NetworkManager::~NetworkManager()
{
    if (g_PacketHandler)
        delete g_PacketHandler, g_PacketHandler = nullptr;

    Shutdown();
}

void NetworkManager::Init()
{
    NetErrorMsg errMsg;
    if (!GameNetworkingSockets_Init(nullptr, errMsg)) 
        throw std::runtime_error("Couldn't initialize GameNetworkingSockets: " + std::string(errMsg));

    SteamNetworkingUtils()->SetDebugOutputFunction(k_ESteamNetworkingSocketsDebugOutputType_Msg, SteamNetDebugOutput);

    m_interface = SteamNetworkingSockets();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void NetworkManager::StartHost(uint16_t port = 27015)
{
    if (m_server)
        return;

    m_server = new NetServer(m_interface);
    m_server->Start(port);
}

void NetworkManager::StartClient()
{
    if (m_client)
        return;

    m_client = new NetClient(m_interface);
}

void NetworkManager::Connect(const std::string& ip, uint16_t port)
{
    if (!m_client)
        return;

    m_client->Start(ip, port);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void NetworkManager::Update()
{
    if (m_server)
        m_server->PollMessages();
        
    if (m_client)
        m_client->PollMessages();
}

//-----------------------------------------------------------------------------
void NetworkManager::Shutdown()
{
    if (m_server)
        delete m_server, m_server = nullptr;    
    
    if (m_client)
        delete m_client, m_client = nullptr;

    GameNetworkingSockets_Kill();
    m_interface = nullptr;
}