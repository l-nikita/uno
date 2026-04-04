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
	if (GetStage() == GameStage::RoundInProgress || GetStage() == GameStage::RoundEnd)
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

	m_stage = GameStage::RoundInProgress;

	BroadcastGameState();
}

void GameManager::Update()
{

}

//-----------------------------------------------------------------------------
void GameManager::OnClientConnected(NetConnection conn)
{
}

void GameManager::OnClientDisconnected(NetConnection conn)
{
	auto it = std::find_if(m_players.begin(), m_players.end(), [&](Player* p) {
        return p->GetConnection() == conn;
    });

	if (it != m_players.end()) 
	{
		delete *it;
        m_players.erase(it);
        BroadcastGameState();
    }
}

void GameManager::OnClientIdentified(const ClientInfo& info)
{
    auto it = std::find_if(m_players.begin(), m_players.end(), [&](Player* p) {
        return p->GetConnection() == info.Connection;
    });

    if (it == m_players.end()) 
    {
        Player* player = new Player(info);
        m_players.push_back(player);
        
        SDL_Log("Player '%s' (conn %u) identified!", player->GetName().c_str(), player->GetConnection());

        BroadcastGameState();
    }
}

//-----------------------------------------------------------------------------
void GameManager::BroadcastGameState()
{
    for (auto clPlayer : m_players)
    {
		auto clIndex = clPlayer->GetIndex();
        proto::NetMessage netMsg;
        proto::ServerGameState* state = netMsg.mutable_game_state();
		state->set_stage((int)GetStage());

        for (int i = 0; i < m_players.size(); ++i)
        {
			auto player = m_players.at(i);

            proto::PlayerInfo* info = state->add_players();
			info->set_index(i);
            info->set_name(player->GetName());
			info->set_is_host((i == 0));
			info->set_is_local((clIndex == i));

			for (auto* card : player->GetCards())
			{
				proto::Card* c = info->add_hand();
				c->set_type((int)card->Type);
				c->set_color((int)card->Color);
				c->set_value(card->Value);
			}
        }

        g_NetManager->GetServer()->SendToClient(clPlayer->GetConnection(), netMsg);
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