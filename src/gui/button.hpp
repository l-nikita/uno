#pragma once

#include "panel.hpp"

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
class Button : public Panel
{
public:
	Button(const SDL_FRect& rect)
		: Panel(rect)
	{ }

	void Update() override
	{
		Panel::Update();

		if (g_Input->IsButtonPressed(SDL_BUTTON_LEFT))
			if (IsHovered())
				OnClick();
	}

	virtual void OnClick()
	{ 
		if (m_OnClick)
			m_OnClick();
	}

	std::function<void()> m_OnClick;
};