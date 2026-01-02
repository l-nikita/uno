#pragma once

#include <SDL3/SDL.h>
#include "textengine.hpp"
#include "gamemanager.hpp"

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
class Game final
{
public:
	void Init();

	void Run();
	void Stop();

	void Update();
	void OnEvent(const SDL_Event& event);

	void Render();

	void GetWindowSize(int* w, int* h);

	SDL_Window* GetWindow() const { return m_window; }
	SDL_Renderer* GetRenderer() const { return m_renderer; }
	TextEngine* GetTextEngine() const { return m_textEngine; }

private:
	bool m_running = false;

	SDL_Window* m_window = nullptr;
	SDL_Renderer* m_renderer = nullptr;
	TextEngine* m_textEngine = nullptr;

	GameManager* m_gameManager = nullptr;
};

extern Game* g_Game;