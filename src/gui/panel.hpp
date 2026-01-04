#pragma once

#include <vector>
#include <functional>
#include <SDL3/SDL.h>
#include "../game.hpp"
#include "../input.hpp"

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
class Panel
{
public:
	Panel(const SDL_FRect& rect) 
		: m_rect(rect) 
	{ }

	virtual void Update()
	{ 
		SDL_FPoint mousePoint = g_Input->GetMousePoint();
		m_isHovered = SDL_PointInRectFloat(&mousePoint, &m_rect);

		if(m_OnUpdate)
			m_OnUpdate();

		for (auto& child : GetChildren())
			child->Update();
	}

	virtual void Render()
	{
		if (m_OnRender)
			m_OnRender();

		for (auto& child : GetChildren())
			child->Render();
	}

	bool IsHovered() const { return m_isHovered; }

	std::vector<Panel*> GetChildren() const { return m_children; }

	std::function<void()> m_OnUpdate;
	std::function<void()> m_OnRender;

	const SDL_FRect& GetRect() { return m_rect; }

protected:
	SDL_FRect m_rect;
	bool m_isHovered = false;

	std::vector<Panel*> m_children;
};