#include "clientmanager.hpp"

ClientManager* g_ClientManager = nullptr;

ClientManager::ClientManager()
{
    g_ClientManager = this;
}

void ClientManager::ApplyServerState(ClientGameState& state)
{
    m_state = state;
}