#include <iostream>
#include "game.hpp"
#include "input.hpp"
#include "gui/panel.hpp"
#include "gui/button.hpp"
#include "gui/mainmenu.hpp"
#include <RmlUi/Core.h>

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Game::Init()
{
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_Log("Couldn't initialize SDL: %s\n", SDL_GetError());
		throw std::runtime_error("Couldn't initialize SDL");
	}

	if (!SDL_CreateWindowAndRenderer("Uno", 1280, 720, 0, &m_window, &m_renderer)) 
	{
		SDL_Log("Couldn't create window and renderer: %s\n", SDL_GetError());
		throw std::runtime_error("Couldn't create window and renderer");
	}

	const char* videoName = SDL_GetCurrentVideoDriver();
	SDL_Log("Video driver: %s\n", videoName);	
	
	const char* rendererName = SDL_GetRendererName(m_renderer);
	SDL_Log("Renderer name: %s\n", rendererName);

	m_textEngine = new TextEngine(m_renderer);
	g_Input = new Input();
	g_GameManager = new GameManager();

	int wW, wH;
	g_Game->GetWindowSize(&wW, &wH);
	m_panels.push_back(new MainMenu(SDL_FRect{ 0, 0, (float)wW, (float)wH }));

	m_isRunning = true;
}

void Game::Run()
{
	using namespace std::chrono;
	auto lastTime = high_resolution_clock::now();

	while (m_isRunning)
	{
		auto currentTime = high_resolution_clock::now();
		m_deltaTime = duration<float>(currentTime - lastTime);
		lastTime = currentTime;

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

	for (auto& panel : GetPanels())
	{
		panel->Update();
	}

	g_GameManager->Update();

	g_Input->Reset();
}

void Game::Render()
{
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
	SDL_RenderClear(m_renderer);

	//g_GameManager->Render();

	for (auto& panel : GetPanels())
	{
		panel->Render();
	}

	SDL_RenderPresent(m_renderer);
}

//-----------------------------------------------------------------------------
void Game::OnEvent(const SDL_Event& event)
{
	auto type = event.type;
	switch (type)
	{
	case SDL_EVENT_QUIT:
		m_isRunning = false;
		break;
	case SDL_EVENT_KEY_DOWN:
		g_Input->OnKey(event.key);
		break;
	case SDL_EVENT_KEY_UP:
		g_Input->OnKey(event.key);
		break;
	case SDL_EVENT_MOUSE_MOTION:
		g_Input->OnMouseMotion(event.motion);
		break;
	case SDL_EVENT_MOUSE_BUTTON_DOWN:
		g_Input->OnMouseButton(event.button);
		break;
	case SDL_EVENT_MOUSE_BUTTON_UP:
		g_Input->OnMouseButton(event.button);
		break;
	case SDL_EVENT_MOUSE_WHEEL:
		g_Input->OnMouseWheel(event.wheel);
		break;
	}
}

void Game::GetWindowSize(int* w, int* h)
{
	SDL_GetWindowSize(m_window, w, h);
}
