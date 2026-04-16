#include <SDL3/SDL.h>
#include "net_packet_handler.hpp"
#include "net_message.pb.h"
#include "../clientmanager.hpp"
#include "../gamemanager.hpp"

PacketHandler* g_PacketHandler = nullptr;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
PacketHandler::PacketHandler()
{
	g_PacketHandler = this;
}

PacketHandler::~PacketHandler()
{

}

void PacketHandler::ProcessMessage(NetMessage* msg)
{
	proto::NetMessage message;

    if (!message.ParseFromArray(msg->m_pData, msg->m_cbSize)) 
    {
        SDL_Log("Error when parsing net message");
        return;
    }

	switch (message.payload_case()) 
    {
        // From server    
        case proto::NetMessage::kGameState:
        {
            const auto& gs = message.game_state();

            GameState state;
            state.Stage = (GameStage)gs.stage();
            state.CurrentPlayer = gs.current_player();
            state.Reverse = gs.reverse();

            auto t = gs.top_discard();
            state.TopDiscard = { (CardType)t.type(), (CardColor)t.color(), t.value() };
            
            for (auto& player : gs.players())
            {
                std::vector<Card> cards;
                for (auto& c : player.hand())
                {
                    cards.push_back({ (CardType)c.type(), (CardColor)c.color(), c.value() });
                }

                auto lC = player.last_card();
                Card lastCard{ (CardType)lC.type(), (CardColor)lC.color(), lC.value() };

                PlayerInfo info{ player.index(), player.name(), player.is_host(), player.is_local(), cards, lastCard };
                state.Players.push_back(info);
            }

            g_ClientManager->ApplyUpdate(state);
            break;
        }
        // From client
        case proto::NetMessage::kChat:
        {
            const auto& chat = message.chat();
            g_ClientManager->ApplyUpdate(ChatMessage{ chat.message() });
            break;
        }
        case proto::NetMessage::kClientInfo:
        {
            const auto& info = message.client_info();

            ClientInfo clientInfo{ info.name(), msg->GetConnection() };
            g_GameManager->OnClientIdentified(clientInfo);
            break;
        }        
        case proto::NetMessage::kPlayerAction:
        {
            const auto& pa = message.player_action();

            PlayerAction action{ (ActionType)pa.action(), pa.card_id(), (CardColor)pa.chosen_color() };
            g_GameManager->OnPlayerAction(msg->GetConnection(), action);
            break;
        }
        default:
        {
            SDL_Log("Unknown message type");
            break;
        }
	}
}