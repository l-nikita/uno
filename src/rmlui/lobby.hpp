#pragma once

#include <RmlUi/Core.h>

#include "clientmanager.hpp"
#include "scene.hpp"

namespace client::ui
{
    //-----------------------------------------------------------------------------
    //
    //-----------------------------------------------------------------------------
    class Lobby final : public Scene, IStateListener
    {
    public:
        explicit Lobby( Rml::Context* context );

        void ProcessEvent( Rml::Event& event ) override;

        SceneId GetId() const override { return SceneId::LOBBY; }

        void Update() override;
        void Destroy() override;

        void OnStateUpdate( const shared::StateUpdate& update ) override;

    private:
        void RefreshPlayers();

        Rml::ElementDocument* m_document = nullptr;
    };
}
