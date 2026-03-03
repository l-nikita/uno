#pragma once

#include <RmlUi/Core.h>

//-----------------------------------------------------------------------------
enum class SceneID
{
	NONE,
	MAIN_MENU,
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

    virtual SceneID GetID() const = 0;

	virtual void Destroy() = 0;

protected:
	Rml::Context* m_context = nullptr;
};