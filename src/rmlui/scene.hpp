#pragma once

#include <RmlUi/Core.h>

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
	Scene(Rml::Context* context) 
        : m_context(context) 
    { }

    virtual SceneId GetId() const = 0;

	virtual void Update() = 0;
	virtual void Destroy() = 0;

protected:
	Rml::Context* m_context = nullptr;
};