#pragma once

#include <string>
#include <vector>
#include <variant>
#include "card.hpp"
#include "game.hpp"

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
    GameStage Stage;
    Card TopDiscard;
    std::vector<PlayerInfo> Players;
};

struct ChatMessage
{
	std::string Username;
	std::string Message;
};

using StateUpdate = std::variant<GameState, ChatMessage>;