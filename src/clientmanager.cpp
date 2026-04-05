#include "clientmanager.hpp"
#include "state_types.hpp"
#include "game.hpp"
#include "rmlui/main_menu.hpp"
#include "rmlui/lobby.hpp"
#include "rmlui/game_screen.hpp"
#include "net/net_manager.hpp"

ClientManager* g_ClientManager = nullptr;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ClientManager::ClientManager(Rml::Context* context)
	: m_rmlContext(context)
{
    g_ClientManager = this;
	SetScene(SceneId::MAIN_MENU);
}

ClientManager::~ClientManager()
{
	if (m_scene)
		delete m_scene, m_scene = nullptr;
}

//-----------------------------------------------------------------------------
void ClientManager::ApplyUpdate(const StateUpdate& update)
{
	if (auto* gameState = std::get_if<GameState>(&update))
	{
		m_gameState = *gameState;

		if (m_gameState.Stage == GameStage::Lobby)
			SetScene(SceneId::LOBBY);
		else if (m_gameState.Stage == GameStage::RoundInProgress)
			SetScene(SceneId::GAME_SCREEN);
	}

	for (auto& listener : m_listeners)
		listener->OnStateUpdate(update);
}

const PlayerInfo& ClientManager::GetLocalPlayerInfo()
{
	for (const auto& info : GetGameState().Players)
	{
		if (info.IsLocal)
			return info;
	}

	static const PlayerInfo emptyPlayer{}; 
    return emptyPlayer;
}

//-----------------------------------------------------------------------------
void ClientManager::Subscribe(IStateListener* listener) 
{ 
	m_listeners.push_back(listener); 
}

void ClientManager::Unsubscribe(IStateListener* listener) 
{ 
	auto it = std::find(m_listeners.begin(), m_listeners.end(), listener);
	if (it != m_listeners.end())
		m_listeners.erase(it); 
}

//-----------------------------------------------------------------------------
void ClientManager::OnConnected()
{
	proto::NetMessage netMsg;
	proto::ClientInfo* info = netMsg.mutable_client_info();
	info->set_name(g_Game->m_GameSettings.Name);

	g_NetManager->GetClient()->SendToServer(netMsg);
}

void ClientManager::OnDisconnected()
{
	SetScene(SceneId::MAIN_MENU);
}

void ClientManager::DoPlayerAction(const PlayerAction& action)
{
	proto::NetMessage netMsg;
	proto::PlayerAction* act = netMsg.mutable_player_action();

	act->set_action((proto::PlayerAction_ActionType)action.Type);
	act->set_card_id((int)action.CardId);
	act->set_chosen_color((int)action.ChosenColor);

	g_NetManager->GetClient()->SendToServer(netMsg);
}

//-----------------------------------------------------------------------------
void ClientManager::SetScene(SceneId id)
{
	m_sceneId = id;
}

void ClientManager::DestroyScene(Scene* scene)
{
	if (scene)
		scene->Destroy(), m_dirtyScenes.push_back(scene);
}

Scene* ClientManager::CreateNewScene(SceneId id)
{
	Scene* scene = nullptr;
	switch (id)
	{
	case SceneId::MAIN_MENU:
		scene = new MainMenu(m_rmlContext);
		break;
	case SceneId::LOBBY:
		scene = new Lobby(m_rmlContext);
		break;	
	case SceneId::GAME_SCREEN:
		scene = new GameScreen(m_rmlContext);
		break;
	default:
		throw std::runtime_error("Unknown scene!");
		break;
	}

	return scene;
}

//-----------------------------------------------------------------------------
void ClientManager::Update()
{
	if (m_sceneId != SceneId::NONE)
	{
		bool needsNewScene = false;
		if (!m_scene || m_scene->GetId() != m_sceneId)
			needsNewScene = true;

		if (needsNewScene)
		{
			Scene* oldScene = m_scene;
			m_scene = CreateNewScene(m_sceneId);

			if (oldScene)
				DestroyScene(oldScene);
		}
		else
			m_scene->Update();
	}
}

void ClientManager::DeleteScenes()
{
	for (Scene* scene : m_dirtyScenes)
		delete scene;

	m_dirtyScenes.clear();
}