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
	if (IsGameInProgress())
	{
		Rml::Log::Message(Rml::Log::LT_WARNING, "The game has already started");
		return;
	}

	if (gmId == gm::GameModeID::CLASSIC)
		m_gameMode = new gm::Classic();
	else
		Rml::Log::Message(Rml::Log::LT_ERROR, "Unknown game mode");

	if (m_gameMode)
		m_gameMode->Start();

	Rml::Log::Message(Rml::Log::LT_INFO, "The game has been started!");
}

void GameManager::Update()
{

}

//-----------------------------------------------------------------------------
void GameManager::OnClientConnected(NetConnection conn)
{
    m_players.push_back(new Player("Player" + m_players.size() + 1, conn));

	//if (!IsGameInProgress())
	//	g_GameManager->Start(gm::GameModeID::CLASSIC);

	//BroadcastGameState();
}

void GameManager::OnClientDisconnected(NetConnection conn)
{
	auto it = std::find_if(m_players.begin(), m_players.end(), [&](Player* p) {
        return p->GetConnection() == conn;
    });

	if (it != m_players.end()) 
	{
        m_players.erase(it);
        BroadcastGameState();
    }
}

void GameManager::BroadcastGameState()
{
    for (auto player : m_players)
    {
		auto index = player->GetIndex();
        proto::NetMessage netMsg;
        proto::ServerGameState* state = netMsg.mutable_game_state();
        state->set_your_index(index);
        state->set_current_turn_index(0);

        for (auto* card : player->GetCards())
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

        g_NetManager->GetServer()->SendToClient(player->GetConnection(), netMsg);
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