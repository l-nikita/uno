#include <iostream>
#include "game.hpp"

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Game::Init()
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	SDL_CreateWindowAndRenderer("Uno", 1920, 1080, NULL, &m_window, &m_renderer);

	m_textEngine = new TextEngine(m_renderer);
	m_gameManager = new GameManager();

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
	delete m_textEngine;

	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

//-----------------------------------------------------------------------------
void Game::Update()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
		OnEvent(event);

	m_gameManager->Update();
}

void Game::Render()
{
	SDL_SetRenderDrawColor(m_renderer, 20, 20, 20, 0);
	SDL_RenderClear(m_renderer);

	m_gameManager->Render();

	SDL_RenderPresent(m_renderer);
}

//-----------------------------------------------------------------------------
void Game::OnEvent(const SDL_Event& event)
{
	if (event.type == SDL_EVENT_QUIT)
		m_running = false;
}

void Game::GetWindowSize(int* w, int* h)
{
	SDL_GetWindowSize(m_window, w, h);
}