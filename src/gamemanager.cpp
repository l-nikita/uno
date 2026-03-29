#include "game.hpp"
#include "gamemanager.hpp"
#include "gamemodes/classic.hpp"
#include "filesystem.hpp"
#include "net_message.pb.h"

GameManager* g_GameManager = nullptr;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GameManager::GameManager()
{
	g_GameManager = this;
}

GameManager::~GameManager()
{
	delete m_gameMode;

	for (auto& player : m_players)
		delete player;

	m_players.clear();
}

//-----------------------------------------------------------------------------
void GameManager::Start(gm::GameModeID gmId)
{
	if (m_gameMode)
	{
		Rml::Log::Message(Rml::Log::LT_WARNING, "The game has already started");
		return;
	}

	m_players.push_back(new Player("Nikita"));
	m_players.push_back(new Player("Genrih"));	
	m_players.push_back(new Player("Nikita2"));
	m_players.push_back(new Player("Genrih2"));

	if (gmId == gm::GameModeID::CLASSIC)
		m_gameMode = new gm::Classic();
	else
		Rml::Log::Message(Rml::Log::LT_ERROR, "Unknown game mode");

	if (m_gameMode)
		m_gameMode->Start();

	g_Game->SetScene(SceneId::GAME_SCREEN);
}

void GameManager::Update()
{

}

//-----------------------------------------------------------------------------
void GameManager::OnClientConnected(NetConnection conn)
{
    int newIndex = (int)m_connectionToPlayer.size();
    m_connectionToPlayer[conn] = newIndex;

	BroadcastGameState();
}

void GameManager::BroadcastGameState()
{
    for (auto& [conn, index] : m_connectionToPlayer)
    {
        proto::NetMessage netMsg;
        proto::ServerGameState* state = netMsg.mutable_game_state();
        state->set_your_index(index);
        state->set_current_turn_index(0);

        Player* me = m_players.at(index);
        for (auto* card : me->GetCards())
        {
            proto::Card* c = state->add_my_hand();
            c->set_type((int)card->Type);
            c->set_color((int)card->Color);
            c->set_value(card->Value);
        }

        for (int i = 0; i < m_players.size(); ++i)
        {
            if (i == index) 
                continue;

            proto::PlayerInfo* info = state->add_opponents();
            info->set_name(m_players[i]->GetName());
            info->set_card_count((int)m_players[i]->GetCards().size());
        }

        g_NetManager->GetServer()->SendToClient(conn, netMsg);
    }
}

int GameManager::GetPlayerIndex(const Player* player) const
{
	int id = -1;
	for (int i = 0; i < m_players.size(); ++i) 
	{
		if (m_players.at(i) == player) 
		{
			id = i;
			break;
		}
	}

	return id;
}