#pragma once

#include <vector>
#include <string>
#include <functional>
#include <variant>
#include "card.hpp"
#include "game.hpp"
#include "istatelistener.hpp"
#include "rmlui/scene.hpp"

//-----------------------------------------------------------------------------

struct PlayerAction;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
class ClientManager final
{
public:
    ClientManager(Rml::Context* context);
    ~ClientManager();

    void Update();
    void DeleteScenes();

    void ApplyUpdate(const StateUpdate& update);
    
    void Subscribe(IStateListener* listener);
    void Unsubscribe(IStateListener* listener);

    const GameState& GetGameState() { return m_gameState; }
    const PlayerInfo& GetLocalPlayerInfo();

    void OnDisconnected();
    void OnConnected();
    void DoPlayerAction(const PlayerAction& action);

	void SetScene(SceneId id);
	void DestroyScene(Scene* scene);
	Scene* CreateNewScene(SceneId id);

private:
    GameState m_gameState;
    std::vector<IStateListener*> m_listeners;

	Scene* m_scene = nullptr;
	SceneId m_sceneId = SceneId::NONE;

	std::vector<Scene*> m_dirtyScenes;

    Rml::Context* m_rmlContext = nullptr;
};

extern ClientManager* g_ClientManager;