#include <SDL3/SDL.h>
#include "net_packet_handler.hpp"
#include "net_message.pb.h"

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
        case proto::NetMessage::kEvent:
        {
            const auto& event = message.event();
            SDL_Log("description %s", event.description());
            SDL_Log("event_id %s", std::to_string(event.event_id()));
            break;
        }
        default:
        {
            SDL_Log("Unknown message type");
            break;
        }
	}
}