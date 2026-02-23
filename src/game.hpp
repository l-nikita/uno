#pragma once

#include <chrono>
#include <SDL3/SDL.h>
#include "gamemanager.hpp"
#include "rmlui/rmlui_renderer_gl3_sdl.hpp"
#include "rmlui/rmlui_file_interface.hpp"

using KeyDownCallback = bool (*)(Rml::Context* context, Rml::Input::KeyIdentifier key, int key_modifier, float native_dp_ratio, bool priority);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
class Panel;

class Game final
{
public:
	void Init();

	void Run();
	void RequestExit();
	void Shutdown();

	void GetWindowSize(int* w, int* h);

	SDL_Window* GetWindow() const { return m_window; }
	Rml::SystemInterface* GetSystemInterface() const { return m_systemInterface; }
	Rml::RenderInterface* GetRenderInterface() const { return m_renderInterface; }

	// Delta time (ms)
	float DeltaTime() { return m_deltaTime.count(); }

	// Elapsed time since engine loop ran (ms)
	float ElapsedTime() { return m_elapsedTime.count(); }

private:
	bool InitSDL(std::string windowName, uint32_t width, uint32_t height, bool allowResize);
	bool InitRml();

	void Update();

	void Render();
	void BeginFrame();
	void PresentFrame();

	void ProcessEvents(Rml::Context* context, KeyDownCallback key_down_callback, bool power_save);
	static bool ProcessKeyDownShortcuts(Rml::Context* context, Rml::Input::KeyIdentifier key, int key_modifier, float native_dp_ratio, bool priority);

	void OnWindowResize();

private:
	bool m_isRunning = false;

	std::chrono::duration<float> m_deltaTime;
	std::chrono::duration<float> m_elapsedTime;

	SDL_Window* m_window = nullptr;
	SDL_GLContext m_glContext = nullptr;

	SystemInterface_SDL* m_systemInterface = nullptr;
	RenderInterface_GL3_SDL* m_renderInterface = nullptr;
	FileInterface* m_fileInterface = nullptr;

	Rml::Context* m_rmlContext = nullptr;
};

extern Game* g_Game;