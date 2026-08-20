#pragma once

#include <RmlUi/Core.h>

namespace client::ui
{
    //-----------------------------------------------------------------------------
    enum class SceneId
    {
        NONE,
        MAIN_MENU,
        LOBBY,
        GAME_SCREEN,
    };

    //-----------------------------------------------------------------------------
    //
    //-----------------------------------------------------------------------------
    class Scene : public Rml::EventListener
    {
    public:
        explicit Scene( Rml::Context* context )
            : m_context( context )
        {
        }

        virtual SceneId GetId() const = 0;

        virtual void Update() = 0;
        virtual void Destroy() = 0;

    protected:
        Rml::Context* m_context = nullptr;
    };
}
