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
void GameManager::Start(gm::GameModeId gmId)
{
	if (IsGameStarted())
	{
		Rml::Log::Message(Rml::Log::LT_WARNING, "The game has been already started");
		return;
	}

	if (gmId == gm::GameModeId::CLASSIC)
		m_gameMode = new gm::Classic();
	else
		Rml::Log::Message(Rml::Log::LT_ERROR, "Unknown game mode");

	if (GetGameMode())
		GetGameMode()->Start();

	Rml::Log::Message(Rml::Log::LT_INFO, "The game has been started!");

	m_stage = GameStage::RoundInProgress;

	BroadcastGameState();
}

void GameManager::Update()
{
	if (GetGameMode())
		GetGameMode()->Update();
}

void GameManager::OnPlayerAction(NetConnection conn, const PlayerAction& action)
{
	auto player = GetPlayerByConnection(conn);
	//if (player)
	//	SDL_Log("Player [%s] action: %i, %i", player->GetName().c_str(), (int)action.Type, (int)action.CardId);

	if (GetGameMode() && player)
		GetGameMode()->OnPlayerAction(player, action);
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

		if (GetGameMode())
		{
			auto topDiscard = GetGameMode()->GetTopDiscardCard();
			if (topDiscard)
			{
				auto card = state->mutable_top_discard();
				card->set_type((int)topDiscard->Type);
				card->set_color((int)topDiscard->Color);
				card->set_value(topDiscard->Value);
			}
			
			state->set_current_player(GetGameMode()->GetCurrentPlayerIndex());
			state->set_reverse(GetGameMode()->IsReverse());
		}

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

			auto lastCard = player->m_LastCard;
			if (lastCard)
			{
				proto::Card* card = info->mutable_last_card();
				card->set_type((int)lastCard->Type);
				card->set_color((int)lastCard->Color);
				card->set_value(lastCard->Value);
			}
        }

        g_NetManager->GetServer()->SendToClient(clPlayer->GetConnection(), netMsg);
    }
}

Player* GameManager::GetPlayerByConnection(NetConnection conn)
{
	for (int i = 0; i < m_players.size(); ++i) 
	{
		auto player = m_players.at(i);
		if (player->GetConnection() == conn) 
			return player;
	}

	return nullptr;
}

int GameManager::GetPlayerIndex(const Player* player)
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