#pragma once

#include <vector>
#include <string>
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

    ClientGameState& GetState() { return m_state; }
    void ApplyServerState(ClientGameState& state);

private:
    ClientGameState m_state;
};

extern ClientManager* g_ClientManager;