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
        case proto::NetMessage::kChat:
        {
            const auto& chat = message.chat();
            g_ClientManager->ApplyUpdate(ChatMessage{ chat.message() });
            break;
        }        
        case proto::NetMessage::kGameState:
        {
            const auto& gs = message.game_state();

            GameState state;
            state.Stage = (GameStage)gs.stage();         
            for (auto& player : gs.players())
            {
                std::vector<Card> cards;
                for (auto& c : player.hand())
                {
                    cards.push_back({ (CardType)c.type(), (CardColor)c.color(), c.value() });
                }

                PlayerInfo info{ player.index(), player.name(), player.is_host(), player.is_local(), cards };
                state.Players.push_back(info);
            }

            std::cout << g_Game->m_GameSettings.Name << ": " << gs.DebugString() << std::endl;

            g_ClientManager->ApplyUpdate(state);
            break;
        }
        case proto::NetMessage::kClientInfo:
        {
            const auto& info = message.client_info();

            ClientInfo clientInfo{ info.name(), msg->GetConnection() };
            g_GameManager->OnClientIdentified(clientInfo);
            break;
        }       
        default:
        {
            SDL_Log("Unknown message type");
            break;
        }
	}
}