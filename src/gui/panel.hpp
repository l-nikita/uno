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
	Panel(float w, float h);

	virtual void Update();
	virtual void Render();

	void SetHovered(bool hovered) { m_isHovered = hovered; }
	bool IsHovered() const { return m_isHovered; }

	void SetParent(Panel* panel) { m_parent = panel; }
	Panel* GetParent() { return m_parent; }

	void AddChild(Panel* child);
	const std::vector<Panel*>& GetChildren() const { return m_children; }

	void Remove();

	void SetPos(float x, float y);
	const SDL_FRect& GetRect() { return m_rect; }

	void OnMouseDown(Uint8 button) const;
	void OnMousePressed(Uint8 button) const;
	void OnMouseReleased(Uint8 button) const;

	void OnWindowResize();
	void UpdateActualCoords();

public:
	friend class Gui;

public:
	std::function<void()> m_OnUpdate;
	std::function<void()> m_OnRender;

	std::function<void()> m_OnMouseDown;
	std::function<void()> m_OnMousePressed;
	std::function<void()> m_OnMouseReleased;

	int m_zIndex = 0;

private:
	SDL_FRect m_baseRect;
	SDL_FRect m_rect;
	bool m_isHovered = false;

	bool m_shouldRemove = false;
	Panel* m_parent = nullptr;
	std::vector<Panel*> m_children;
};