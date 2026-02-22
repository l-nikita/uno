#pragma once

#include "../game.hpp"
#include "../filesystem.hpp"
#include "panel.hpp"

class MainMenu : public Panel
{
public:
	MainMenu(float w, float h)
		: Panel(w, h)
	{ 
		int wW, wH;
		g_Game->GetWindowSize(&wW, &wH);

		//-----------------------------------------------------------------------------
		auto but1 = new Panel(500, 100);
		but1->SetPos(((float)wW / 2) - (500 / 2), ((float)wH / 2) - (100 / 2));

		but1->m_OnRender = [but1]() {
			if (but1->IsHovered())
				SDL_SetRenderDrawColor(g_Game->GetRenderer(), 200, 200, 200, 255);
			else
				SDL_SetRenderDrawColor(g_Game->GetRenderer(), 255, 255, 255, 255);

			SDL_RenderFillRect(g_Game->GetRenderer(), &but1->GetRect());
		};

		but1->m_OnMousePressed = [but1]() {
		};

		AddChild(but1);

		//-----------------------------------------------------------------------------
		auto but2 = new Panel(500, 100);
		but2->SetPos(((float)wW / 2) - (500 / 2), ((float)wH / 2) - (100 / 2) + 50);

		but2->m_OnRender = [but2]() {
			if (but2->IsHovered())
				SDL_SetRenderDrawColor(g_Game->GetRenderer(), 100, 200, 200, 255);
			else
				SDL_SetRenderDrawColor(g_Game->GetRenderer(), 155, 255, 255, 255);

			SDL_RenderFillRect(g_Game->GetRenderer(), &but2->GetRect());
		};

		but2->m_OnMousePressed = [but2]() {
		};

		AddChild(but2);

		//-----------------------------------------------------------------------------
		g_TextEngine->CreateFont("Logo", GetAssetsPath() + "/fonts/arial.ttf", 80.0f);

		m_OnRender = [this]() {
			SDL_SetRenderDrawColor(g_Game->GetRenderer(), 30, 30, 30, 255);
			SDL_RenderFillRect(g_Game->GetRenderer(), &GetRect());

			g_TextEngine->DrawText("UNO", "Logo", 45, 45, SDL_Color{ 255, 255, 255, 255 });
		};
	}
};