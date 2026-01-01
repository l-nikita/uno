#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

class Game
{
public:
	void Init();

	void Run();
	void Stop();

	void Update();
	void OnEvent(SDL_Event& event);

	void Render();

private:
	bool m_running = true;
	SDL_Window* m_window = nullptr;
	SDL_Renderer* m_renderer = nullptr;
};