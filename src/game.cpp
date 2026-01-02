#include <iostream>
#include "game.hpp"
#include "gamemodes/classic.hpp"
#include "filesystem.hpp"

void Game::Init()
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	SDL_CreateWindowAndRenderer("Uno", 1920, 1080, NULL, &m_window, &m_renderer);

	m_textEngine = new TextEngine(m_renderer);
	GetTextEngine()->CreateFont("Arial_25", GetAssetsPath() + "/fonts/arial.ttf", 60.0f);

	m_gameMode = new gm::Classic();

	m_players.push_back(new Player("Nikita"));
	m_players.push_back(new Player("Genrih"));

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
	delete m_gameMode;
	for (auto& player : m_players)
		delete player;

	delete m_textEngine;

	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void Game::Update()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
		OnEvent(event);

	m_gameMode->Update();
}

void Game::Render()
{
	SDL_SetRenderDrawColor(m_renderer, 20, 20, 20, 0);
	SDL_RenderClear(m_renderer);

	float x = 0;
	for (auto& player : m_players)
	{
		SDL_FRect rect = { x, 0, 100, 100 };

		SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 255);
		SDL_RenderFillRect(m_renderer, &rect);

		SDL_Color color = { 255, 255, 255, 255 };
		GetTextEngine()->DrawText(player->GetName(), "Arial_25", x, 25, color);

		x += 200;
	}

	SDL_RenderPresent(m_renderer);
}

void Game::OnEvent(const SDL_Event& event)
{
	if (event.type == SDL_EVENT_QUIT)
		m_running = false;
}