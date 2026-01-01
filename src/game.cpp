#include "game.hpp"

void Game::Init()
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	SDL_CreateWindowAndRenderer("Uno", 1920, 1080, NULL, &m_window, &m_renderer);
	m_running = true;
}

void Game::Run()
{
	while (m_running)
	{
		Update();
		Render();
	}

	Stop();
}

void Game::Stop()
{
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void Game::Update()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
		OnEvent(event);
}

void Game::Render()
{
	SDL_SetRenderDrawColor(m_renderer, 20, 20, 20, 0);
	SDL_RenderClear(m_renderer);

	SDL_FRect rect = { 0, 0, 100, 100 };

	SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(m_renderer, &rect);

	SDL_RenderPresent(m_renderer);
}

void Game::OnEvent(SDL_Event& event)
{
	if (event.type == SDL_EVENT_QUIT)
		m_running = false;
}