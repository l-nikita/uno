#pragma once

#include <chrono>
#include <SDL3/SDL.h>
#include "textengine.hpp"
#include "gamemanager.hpp"

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
class Panel;

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

	// Delta time (ms)
	float DeltaTime() { return m_deltaTime.count(); }

	// Elapsed time since engine loop ran (ms)
	float ElapsedTime() { return m_elapsedTime.count(); }

	std::vector<Panel*> GetPanels() const { return m_panels; }

private:
	bool m_isRunning = false;

	std::chrono::duration<float> m_deltaTime;
	std::chrono::duration<float> m_elapsedTime;

	SDL_Window* m_window = nullptr;
	SDL_Renderer* m_renderer = nullptr;
	TextEngine* m_textEngine = nullptr;

	std::vector<Panel*> m_panels;
};

extern Game* g_Game;