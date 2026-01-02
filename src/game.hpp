#pragma once

#include <SDL3/SDL.h>
#include "textengine.hpp"
#include "gamemodes/igamemode.hpp"

using Players = std::vector<Player*>;

class Game final
{
public:
	void Init();

	void Run();
	void Stop();

	void Update();
	void OnEvent(const SDL_Event& event);

	void Render();

	TextEngine* GetTextEngine() const { return m_textEngine; }

	gm::IGameMode* GetGameMode() const { return m_gameMode; }
	Players GetPlayers() const { return m_players; }

private:
	bool m_running = true;

	SDL_Window* m_window = nullptr;
	SDL_Renderer* m_renderer = nullptr;
	TextEngine* m_textEngine = nullptr;

	gm::IGameMode* m_gameMode = nullptr;
	Players m_players;
};