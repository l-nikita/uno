#pragma once

#include <string>
#include <vector>
#include <variant>
#include "card.hpp"
#include "game.hpp"
#include "net/net_common.hpp"

struct PlayerInfo 
{
    int Index;
    std::string Name;
    bool IsHost;
    bool IsLocal;
    std::vector<Card> Hand;
};

struct GameState
{
    GameStage Stage = GameStage::Lobby;
    int CurrentPlayer = -1;
    bool Reverse = false;
    bool DrawCard = false;
    Card TopDiscard;
    std::vector<PlayerInfo> Players;
};

struct ChatMessage
{
	std::string Message;
};

struct ClientInfo
{
    std::string Name;
    NetConnection Connection;
};

enum class ActionType : int 
{
    DRAW_CARD,
    PLAY_CARD,
};

struct PlayerAction
{
    ActionType Type;
    int CardId = -1;
    CardColor ChosenColor = CardColor::RED;
};

using StateUpdate = std::variant<ClientInfo, GameState, ChatMessage>;