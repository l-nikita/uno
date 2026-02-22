#include "panel.hpp"
#include "gui.hpp"

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Panel::Panel(float w, float h)
{ 
	m_baseRect = SDL_FRect{ 0, 0, w, h };
	OnWindowResize();
}

void Panel::Update()
{
	if (IsHovered())
	{
		for (size_t i = 0; i < 6; i++)
		{
			if (g_Input->IsButtonDown(i)) OnMouseDown(i);
			if (g_Input->IsButtonPressed(i)) OnMousePressed(i);
			if (g_Input->IsButtonReleased(i)) OnMouseReleased(i);
		}
	}

	if (m_OnUpdate)
		m_OnUpdate();
}

void Panel::Render()
{
	if (m_OnRender)
		m_OnRender();
}

void Panel::AddChild(Panel* child)
{
	child->m_zIndex = m_zIndex + 1;
	child->SetParent(this);
	g_Gui->AddPanel(child);
	m_children.push_back(child);

	child->UpdateActualCoords();
}

void Panel::SetPos(float x, float y)
{
	m_baseRect.x = x;
	m_baseRect.y = y;
}

void Panel::Remove()
{
	m_shouldRemove = true;
	for (auto& child : GetChildren())
		child->Remove();
}

void Panel::OnMouseDown(Uint8 button) const
{
	if (m_OnMouseDown)
		m_OnMouseDown();
}

void Panel::OnMousePressed(Uint8 button) const
{
	if (m_OnMousePressed)
		m_OnMousePressed();
}

void Panel::OnMouseReleased(Uint8 button) const
{
	if (m_OnMouseReleased)
		m_OnMouseReleased();
}

void Panel::OnWindowResize()
{
	float scale = g_Gui->GetScale();

	m_rect.w = m_baseRect.w * scale;
	m_rect.h = m_baseRect.h * scale;

	if (m_parent)
	{
		m_rect.x = m_parent->GetRect().x + (m_baseRect.x * scale);
		m_rect.y = m_parent->GetRect().y + (m_baseRect.y * scale);
	}
	else
	{
		m_rect.x = m_baseRect.x * scale;
		m_rect.y = m_baseRect.y * scale;
	}
}

void Panel::UpdateActualCoords()
{
	OnWindowResize();
	for (auto& child : m_children)
		child->UpdateActualCoords();
}