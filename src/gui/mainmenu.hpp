#pragma once

#include "../game.hpp"
#include "../filesystem.hpp"
#include "panel.hpp"
#include "button.hpp"

class MainMenu : public Panel
{
public:
	MainMenu(const SDL_FRect& rect)
		: Panel(rect)
	{ 
		int wW, wH;
		g_Game->GetWindowSize(&wW, &wH);

		auto rect1 = SDL_FRect{ 
			((float)wW / 2) - (500 / 2), 
			((float)wH / 2) - (100 / 2), 
			500, 100 
		};

		auto but1 = new Button(rect1);
		but1->m_OnRender = [but1]()
		{
			SDL_SetRenderDrawColor(g_Game->GetRenderer(), 255, 255, 255, 255);
			SDL_RenderFillRect(g_Game->GetRenderer(), &but1->GetRect());
		};

		but1->m_OnClick = [but1]()
		{
			SDL_Log("TEST");
		};

		m_children.push_back(but1);

		//-----------------------------------------------------------------------------
		auto te = g_Game->GetTextEngine();
		te->CreateFont("Logo", GetAssetsPath() + "/fonts/arial.ttf", 80.0f);

		m_OnRender = [this]() 
		{
			SDL_SetRenderDrawColor(g_Game->GetRenderer(), 30, 30, 30, 255);
			SDL_RenderFillRect(g_Game->GetRenderer(), &m_rect);

			g_Game->GetTextEngine()->DrawText("UNO", "Logo", 45, 45, SDL_Color{ 255, 255, 255, 255 });
		};
	}
};