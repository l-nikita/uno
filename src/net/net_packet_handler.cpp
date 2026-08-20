#include <SDL3/SDL.h>

#include "net/net_packet_handler.hpp"
#include "clientmanager.hpp"
#include "gamemanager.hpp"
#include "net_message.pb.h"

shared::net::PacketHandler* g_PacketHandler = nullptr;

namespace shared::net
{
    //-----------------------------------------------------------------------------
    //
    //-----------------------------------------------------------------------------
    PacketHandler::PacketHandler()
    {
        g_PacketHandler = this;
    }

    //-----------------------------------------------------------------------------
    PacketHandler::~PacketHandler()
    {
    }

    //-----------------------------------------------------------------------------
    //
    //-----------------------------------------------------------------------------
    void PacketHandler::ProcessMessage( Message* msg )
    {
        proto::NetMessage message;

        if ( !message.ParseFromArray( msg->m_pData, msg->m_cbSize ) )
        {
            SDL_Log( "Error when parsing net message" );
            return;
        }

        switch ( message.payload_case() )
        {
            // From server
            case proto::NetMessage::kGameState:
            {
                const auto& gs = message.game_state();

                GameState state;
                state.Stage = static_cast<GameStage>(gs.stage());
                state.CurrentPlayer = gs.current_player();
                state.Reverse = gs.reverse();

                auto t = gs.top_discard();
                state.TopDiscard = {static_cast<CardType>(t.type()), static_cast<CardColor>(t.color()), t.value()};

                for ( auto& player: gs.players() )
                {
                    std::vector<Card> cards;
                    for ( auto& c: player.hand() )
                    {
                        cards.push_back(
                            {static_cast<CardType>(c.type()), static_cast<CardColor>(c.color()), c.value()} );
                    }

                    PlayerInfo info{
                        player.index(), player.name(), player.is_host(), player.is_local(), cards, player.last_card_id()
                    };
                    state.Players.push_back( info );
                }

                g_ClientManager->ApplyUpdate( state );
                break;
            }
            // From client
            case proto::NetMessage::kChat:
            {
                const auto& chat = message.chat();
                g_ClientManager->ApplyUpdate( ChatMessage{chat.message()} );
                break;
            }
            case proto::NetMessage::kClientInfo:
            {
                const auto& info = message.client_info();

                ClientInfo clientInfo{info.name(), msg->GetConnection()};
                g_GameManager->OnClientIdentified( clientInfo );
                break;
            }
            case proto::NetMessage::kPlayerAction:
            {
                const auto& pa = message.player_action();

                PlayerAction action{
                    .Type = static_cast<ActionType>(pa.action()),
                    .CardId = pa.card_id(),
                    .ChosenColor = static_cast<CardColor>(pa.chosen_color())
                };
                g_GameManager->OnPlayerAction( msg->GetConnection(), action );
                break;
            }
            default:
            {
                SDL_Log( "Unknown message type" );
                break;
            }
        }
    }
}
