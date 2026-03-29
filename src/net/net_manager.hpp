#pragma once

#include <map>
#include <string>
#include "net_common.hpp"
#include "net_server.hpp"
#include "net_client.hpp"

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
class NetworkManager final
{
public:
    NetworkManager();
    ~NetworkManager();

    void Init();
    void Update();
    void Shutdown();

    void StartHost(uint16_t port);
    void StartClient(const std::string& ip, uint16_t port);

private:
    NetServer* m_server = nullptr;
    NetClient* m_client = nullptr;

    NetInterface* m_interface = nullptr;

    friend class NetServer;
    friend class NetClient;
};

extern NetworkManager* g_NetManager;

static void SteamNetDebugOutput(ESteamNetworkingSocketsDebugOutputType type, const char* msg)
{
    SDL_Log(msg);
}