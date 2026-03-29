#include "clientmanager.hpp"
#include "game.hpp"

ClientManager* g_ClientManager = nullptr;

ClientManager::ClientManager()
{
    g_ClientManager = this;
}

void ClientManager::ApplyServerState(ClientGameState& state)
{
    m_state = state;

    for (auto& cb : m_listeners)
        cb(m_state); 
}

void ClientManager::OnConnected()
{
    g_Game->SetScene(SceneId::LOBBY);
}

void ClientManager::OnDisconnected()
{
    g_Game->SetScene(SceneId::MAIN_MENU);
}