#pragma once

#include <vector>

#include "istatelistener.hpp"
#include "rmlui/scene.hpp"

namespace client
{
    //-----------------------------------------------------------------------------
    //
    //-----------------------------------------------------------------------------
    class ClientManager final
    {
    public:
        ClientManager( Rml::Context* context );
        ~ClientManager();

        void Update();
        void DeleteScenes();

        void ApplyUpdate( const shared::StateUpdate& update );

        void Subscribe( IStateListener* listener );
        void Unsubscribe( IStateListener* listener );

        const shared::GameState& GetGameState() { return m_gameState; }
        const shared::PlayerInfo& GetLocalPlayerInfo();

        void OnDisconnected();
        void OnConnected();
        void DoPlayerAction( const shared::PlayerAction& action );

        void SetScene( ui::SceneId id );
        void DestroyScene( ui::Scene* scene );
        ui::Scene* CreateNewScene( ui::SceneId id );

    private:
        shared::GameState m_gameState;
        std::vector<IStateListener*> m_listeners;

        ui::Scene* m_scene = nullptr;
        ui::SceneId m_sceneId = ui::SceneId::NONE;

        std::vector<ui::Scene*> m_dirtyScenes;

        Rml::Context* m_rmlContext = nullptr;
    };
}

extern client::ClientManager* g_ClientManager;
