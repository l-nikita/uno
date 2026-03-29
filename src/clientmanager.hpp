#pragma once

#include <vector>
#include <string>
#include <functional>
#include "card.hpp"

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
struct PlayerInfo 
{
    std::string Name;
    int CardCount;
};

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
struct ClientGameState 
{
    int CurrentTurnIndex = -1;
    int MyIndex = -1;

    Card TopDiscard;

    std::vector<Card> MyHand;
    std::vector<PlayerInfo> Opponents;

    bool IsMyTurn() const { return CurrentTurnIndex == MyIndex; }
};

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
class ClientManager final
{
public:
    ClientManager();

    using StateCallback = std::function<void(const ClientGameState&)>;

    ClientGameState& GetState() { return m_state; }
    void ApplyServerState(ClientGameState& state);

    void ListenState(StateCallback cb) { m_listeners.push_back(cb); }

    void OnDisconnected();
    void OnConnected();

private:
    ClientGameState m_state;

    std::vector<StateCallback> m_listeners;
};

extern ClientManager* g_ClientManager;