#pragma once

#include <string>
#include <variant>
#include <vector>

#include "card.hpp"
#include "game.hpp"
#include "net/net_common.hpp"

namespace shared
{
    //-----------------------------------------------------------------------------
    //
    //-----------------------------------------------------------------------------
    struct PlayerInfo
    {
        int Index;
        std::string Name;
        bool IsHost;
        bool IsLocal;
        std::vector<Card> Hand;
        int LastCardId;
    };

    struct GameState
    {
        GameStage Stage = GameStage::LOBBY;
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
        net::Connection Connection;
    };

    enum class ActionType : int
    {
        DRAW_CARD,
        PLAY_CARD,
    };

    struct PlayerAction
    {
        ActionType Type = ActionType::DRAW_CARD;
        int CardId = -1;
        CardColor ChosenColor = CardColor::RED;
    };

    //-----------------------------------------------------------------------------

    using StateUpdate = std::variant<ClientInfo, GameState, ChatMessage>;

    //-----------------------------------------------------------------------------
}
