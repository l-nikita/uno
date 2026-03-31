#include <SDL3/SDL.h>
#include "net_packet_handler.hpp"
#include "net_message.pb.h"
#include "../clientmanager.hpp"

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
        case proto::NetMessage::kChat:
        {
            const auto& chat = message.chat();
            SDL_Log("username %s", chat.username());
            SDL_Log("message %s", chat.message());
            break;
        }        
        case proto::NetMessage::kGameState:
        {
            const auto& gs = message.game_state();

            ClientGameState state;
            state.MyIndex = gs.your_index();
            state.CurrentTurnIndex = gs.current_turn_index();

            for (auto& c : gs.my_hand())
                state.MyHand.push_back({ (CardType)c.type(), (CardColor)c.color(), c.value() });
                
            for (auto& o : gs.opponents())
                state.Opponents.push_back({ o.name(), o.card_count() });

            g_ClientManager->ApplyServerState(state);
            break;
        }        
        case proto::NetMessage::kLobbyData:
        {
            
            break;
        }
        default:
        {
            SDL_Log("Unknown message type");
            break;
        }
	}
}