#include <iostream>
#include <print>
#ifdef DEBUG
	#include <RmlUi/Debugger.h>
#endif

#include "game.hpp"
#include "gamemanager.hpp"
#include "clientmanager.hpp"
#include "net/net_manager.hpp"
#include "input.hpp"
#include "filesystem.hpp"
#include "rmlui/rmlui_renderer_gl3.hpp"
#include "rmlui/main_menu.hpp"
#include "rmlui/game_screen.hpp"

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Game::Init()
{
	if (!InitSDL("Uno", 1280, 720, true))
		throw std::runtime_error("Couldn't initialize SDL!");

	if (!InitRml())
		throw std::runtime_error("Couldn't initialize RmlUi!");

	g_NetManager = new NetworkManager();
	g_NetManager->Init();

	g_Game->SetScene(SceneId::MAIN_MENU);

	m_isRunning = true;

	SDL_Log("Game successfully initialized.");
}

void Game::StartHost()
{
	if (g_GameManager || g_ClientManager)
		return;

	g_NetManager->StartHost(27015);
	g_NetManager->StartClient();
	g_NetManager->Connect("127.0.0.1", 27015);

	g_ClientManager = new ClientManager();

	g_GameManager = new GameManager();
	g_GameManager->Start(gm::GameModeID::CLASSIC);
}

void Game::Connect(const std::string& ip, uint16_t port)
{
	if (g_GameManager || g_ClientManager)
		return;

	g_NetManager->StartClient();
	g_NetManager->Connect(ip, port);

	g_ClientManager = new ClientManager();
}

bool Game::InitSDL(std::string windowName, uint32_t width, uint32_t height, bool allowResize)
{
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
	{
		Rml::Log::Message(Rml::Log::LT_ERROR, "Couldn't initialize SDL: %s", SDL_GetError());
		return false;
	}

	// Submit click events when focusing the window.
	SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");

	// Touch events are handled natively, no need to generate synthetic mouse events for touch devices.
	SDL_SetHint(SDL_HINT_TOUCH_MOUSE_EVENTS, "0");

#ifdef RMLUI_BACKEND_SIMULATE_TOUCH
	// Simulate touch events from mouse events for testing touch behavior on a desktop machine.
	SDL_SetHint(SDL_HINT_MOUSE_TOUCH_EVENTS, "1");
#endif

#ifdef __ANDROID__
	// GLES 3.2 on Android
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
	// GL 3.3 Core
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#endif

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	const float window_size_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
	SDL_PropertiesID props = SDL_CreateProperties();
	SDL_SetStringProperty(props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, windowName.c_str());
	SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_X_NUMBER, SDL_WINDOWPOS_CENTERED);
	SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_Y_NUMBER, SDL_WINDOWPOS_CENTERED);
	SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, int(width * window_size_scale));
	SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, int(height * window_size_scale));
	SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_OPENGL_BOOLEAN, true);
	SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_RESIZABLE_BOOLEAN, allowResize);
	SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_HIGH_PIXEL_DENSITY_BOOLEAN, true);

	m_window = SDL_CreateWindowWithProperties(props);
	SDL_DestroyProperties(props);

	if (!m_window)
	{
		Rml::Log::Message(Rml::Log::LT_ERROR, "SDL error on create window: %s", SDL_GetError());
		return false;
	}

	m_glContext = SDL_GL_CreateContext(m_window);
	SDL_GL_MakeCurrent(m_window, m_glContext);

	SDL_GL_SetSwapInterval(1);

	if (!RmlGL3::Initialize())
	{
		Rml::Log::Message(Rml::Log::LT_ERROR, "Failed to initialize OpenGL renderer");
		return false;
	}

	m_systemInterface = new SystemInterface_SDL();
	m_systemInterface->SetWindow(m_window);

	m_renderInterface = new RenderInterface_GL3_SDL();
	m_renderInterface->SetViewport(width, height);

	const char* videoName = SDL_GetCurrentVideoDriver();
	SDL_Log("Video driver: %s\n", videoName);

	return true;
}

bool Game::InitRml()
{
	Rml::SetSystemInterface(GetSystemInterface());
	Rml::SetRenderInterface(GetRenderInterface());

	Rml::Initialise();

	int wW, wH;
	GetWindowSize(&wW, &wH);

	m_rmlContext = Rml::CreateContext("main", Rml::Vector2i(wW, wH));
	if (!m_rmlContext)
	{
		Rml::Log::Message(Rml::Log::LT_ERROR, "Failed to create RmlUi context");
		return false;
	}

#ifdef DEBUG
	Rml::Debugger::Initialise(m_rmlContext);
#endif

	m_fileInterface = new FileInterface(fs::GetAssetsPath().string() + "/");
	Rml::SetFileInterface(m_fileInterface);

	Rml::LoadFontFace("ui/fonts/PixelifySans-Bold.ttf");
	Rml::LoadFontFace("ui/fonts/PixelifySans-Regular.ttf");
	Rml::LoadFontFace("ui/fonts/NotoEmoji-Regular.ttf", true);

	return true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Game::Run()
{
	using namespace std::chrono;
	auto lastTime = high_resolution_clock::now();

	while (m_isRunning)
	{
		auto currentTime = high_resolution_clock::now();
		m_deltaTime = duration<double>(currentTime - lastTime);
		lastTime = currentTime;

		Update();
		Render();
	}
}

void Game::Shutdown()
{
	Rml::Shutdown();

	if (g_GameManager)
		delete g_GameManager, g_GameManager = nullptr;	
		
	if (g_ClientManager)
		delete g_ClientManager, g_ClientManager = nullptr;

	if (g_NetManager)
		delete g_NetManager, g_NetManager = nullptr;

	if (m_systemInterface)
		delete m_systemInterface, m_systemInterface = nullptr;

	if (m_renderInterface)
		delete m_renderInterface, m_renderInterface = nullptr;

	if (m_fileInterface)
		delete m_fileInterface, m_fileInterface = nullptr;

	RmlGL3::Shutdown();

	SDL_GL_DestroyContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_Quit();

	SDL_Log("Game shutdown complete.");
}

void Game::RequestExit()
{
	m_isRunning = false;
}

//-----------------------------------------------------------------------------
void Game::SetScene(SceneId id)
{
	m_sceneId = id;
}

void Game::DestroyScene(Scene* scene)
{
	if (scene)
		scene->Destroy(), m_dirtyScenes.push_back(scene);
}

Scene* Game::CreateNewScene(SceneId id)
{
	Scene* scene = nullptr;
	switch (id)
	{
	case SceneId::MAIN_MENU:
		scene = new MainMenu(m_rmlContext);
		break;
	case SceneId::GAME_SCREEN:
		scene = new GameScreen(m_rmlContext);
		break;
	default:
		throw std::runtime_error("Unknown scene!");
		break;
	}

	return scene;
}

//-----------------------------------------------------------------------------
void Game::Update()
{
	ProcessEvents(m_rmlContext, &ProcessKeyDownShortcuts, false);

	g_NetManager->Update();

	if (m_sceneId != SceneId::NONE)
	{
		bool needsNewScene = false;
		if (!m_scene || m_scene->GetId() != m_sceneId)
			needsNewScene = true;

		if (needsNewScene)
		{
			Scene* oldScene = m_scene;
			m_scene = CreateNewScene(m_sceneId);

			if (oldScene)
				DestroyScene(oldScene);
		}
		else
			m_scene->Update();
	}

	if (g_GameManager)
		g_GameManager->Update();
		
	m_rmlContext->Update();

	for (Scene* scene : m_dirtyScenes)
		delete scene;

	m_dirtyScenes.clear();
}

void Game::Render()
{
	BeginFrame();
	m_rmlContext->Render();
	PresentFrame();
}

void Game::BeginFrame()
{
	m_renderInterface->Clear();
	m_renderInterface->BeginFrame();
}

void Game::PresentFrame()
{
	m_renderInterface->EndFrame();
	SDL_GL_SwapWindow(m_window);

	// Optional, used to mark frames during performance profiling.
	RMLUI_FrameMark;
}

//-----------------------------------------------------------------------------
bool Game::ProcessKeyDownShortcuts(Rml::Context* context, Rml::Input::KeyIdentifier key, int key_modifier, float native_dp_ratio, bool priority)
{
	SDL_assert(context);

	// Result should return true to allow the event to propagate to the next handler.
	bool result = false;

	// This function is intended to be called twice by the backend, before and after submitting the key event to the context. This way we can
	// intercept shortcuts that should take priority over the context, and then handle any shortcuts of lower priority if the context did not
	// intercept it.
	if (priority)
	{
		// Priority shortcuts are handled before submitting the key to the context.

		// Toggle debugger and set dp-ratio using Ctrl +/-/0 keys.
		if (key == Rml::Input::KI_OEM_3)
		{
#ifdef DEBUG
			Rml::Debugger::SetVisible(!Rml::Debugger::IsVisible());
#endif
		}
		else if (key == Rml::Input::KI_ESCAPE)
		{
			g_Game->RequestExit();
		}
		else if (key == Rml::Input::KI_0 && key_modifier & Rml::Input::KM_CTRL)
		{
			context->SetDensityIndependentPixelRatio(native_dp_ratio);
		}
		else if (key == Rml::Input::KI_1 && key_modifier & Rml::Input::KM_CTRL)
		{
			context->SetDensityIndependentPixelRatio(1.f);
		}
		else if ((key == Rml::Input::KI_OEM_MINUS || key == Rml::Input::KI_SUBTRACT) && key_modifier & Rml::Input::KM_CTRL)
		{
			const float new_dp_ratio = Rml::Math::Max(context->GetDensityIndependentPixelRatio() / 1.2f, 0.5f);
			context->SetDensityIndependentPixelRatio(new_dp_ratio);
		}
		else if ((key == Rml::Input::KI_OEM_PLUS || key == Rml::Input::KI_ADD) && key_modifier & Rml::Input::KM_CTRL)
		{
			const float new_dp_ratio = Rml::Math::Min(context->GetDensityIndependentPixelRatio() * 1.2f, 2.5f);
			context->SetDensityIndependentPixelRatio(new_dp_ratio);
		}
	}
	else
	{
		// We arrive here when no priority keys are detected and the key was not consumed by the context. Check for shortcuts of lower priority.
		if (key == Rml::Input::KI_R && key_modifier & Rml::Input::KM_CTRL)
		{
			for (int i = 0; i < context->GetNumDocuments(); i++)
			{
				Rml::ElementDocument* document = context->GetDocument(i);
				const Rml::String& src = document->GetSourceURL();
				if (src.size() > 4 && src.substr(src.size() - 4) == ".rml")
				{
					document->ReloadStyleSheet();
				}
			}
		}
		else
		{
			result = true;
		}
	}

	return result;
}

void Game::ProcessEvents(Rml::Context* context, KeyDownCallback key_down_callback, bool power_save)
{
	#define RMLSDL_WINDOW_EVENTS_BEGIN
	#define RMLSDL_WINDOW_EVENTS_END

	auto GetKey = [](const SDL_Event& event) { return event.key.key; };
	auto GetDisplayScale = []() { return SDL_GetWindowDisplayScale(g_Game->m_window); };
	constexpr auto event_quit = SDL_EVENT_QUIT;
	constexpr auto event_key_down = SDL_EVENT_KEY_DOWN;
	constexpr auto event_window_size_changed = SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED;
	bool has_event = false;

	SDL_Event ev;
	if (power_save)
		has_event = SDL_WaitEventTimeout(&ev, static_cast<int>(Rml::Math::Min(context->GetNextUpdateDelay(), 10.0) * 1000));
	else
		has_event = SDL_PollEvent(&ev);

	while (has_event)
	{
		bool propagate_event = true;
		switch (ev.type)
		{
			case event_quit:
			{
				propagate_event = false;
				RequestExit();
			}
			break;

			case event_key_down:
			{
				propagate_event = false;
				const Rml::Input::KeyIdentifier key = RmlSDL::ConvertKey(GetKey(ev));
				const int key_modifier = RmlSDL::GetKeyModifierState();
				const float native_dp_ratio = GetDisplayScale();

				// See if we have any global shortcuts that take priority over the context.
				if (key_down_callback && !key_down_callback(context, key, key_modifier, native_dp_ratio, true))
					break;

				// Otherwise, hand the event over to the context by calling the input handler as normal.
				if (!RmlSDL::InputEventHandler(context, m_window, ev))
					break;

				// The key was not consumed by the context either, try keyboard shortcuts of lower priority.
				if (key_down_callback && !key_down_callback(context, key, key_modifier, native_dp_ratio, false))
					break;
			}
			break;

			RMLSDL_WINDOW_EVENTS_BEGIN

			case event_window_size_changed:
			{
				Rml::Vector2i dimensions = { ev.window.data1, ev.window.data2 };
				m_renderInterface->SetViewport(dimensions.x, dimensions.y);
			}
			break;

			RMLSDL_WINDOW_EVENTS_END

			default: 
				break;
		}

		if (propagate_event)
			RmlSDL::InputEventHandler(context, m_window, ev);

		has_event = SDL_PollEvent(&ev);
	}
}

void Game::SetFullscreen(bool fullscreen)
{
	SDL_SetWindowFullscreen(m_window, fullscreen);
}

void Game::GetWindowSize(int* w, int* h)
{
	SDL_GetWindowSize(m_window, w, h);
}

void Game::OnWindowResize()
{
}