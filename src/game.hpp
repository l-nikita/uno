#pragma once

#include <chrono>
#include <string>
#include <RmlUi/Core.h>
#include <SDL3/SDL.h>

#include "rmlui/debug_panel.hpp"
#include "rmlui/rmlui_file_interface.hpp"
#include "rmlui/rmlui_renderer_gl3_sdl.hpp"

namespace shared
{
    //-----------------------------------------------------------------------------
    //
    //-----------------------------------------------------------------------------

    using KeyDownCallback = bool ( * )( Rml::Context* context, Rml::Input::KeyIdentifier key, int key_modifier,
                                        float native_dp_ratio, bool priority );

    using WindowSize = std::tuple<std::uint32_t, std::uint32_t>;

    //-----------------------------------------------------------------------------
    struct GameSettings
    {
        bool IsFullScreen = false;
        std::string Name = "Player";
    };

    enum class GameStage : int
    {
        LOBBY,
        ROUND_IN_PROGRESS,
        ROUND_END,
    };

    enum class LaunchMode
    {
        NONE,
        HOST,
        CONNECT,
    };

    struct LaunchArgs
    {
        LaunchMode Mode = LaunchMode::NONE;
        std::string Ip = "127.0.0.1";
        uint16_t Port = 27015;
        std::string Name;
    };

    //-----------------------------------------------------------------------------
    //
    //-----------------------------------------------------------------------------
    class Game final
    {
    public:
        void Init( const LaunchArgs& args = {} );

        void Run();
        void RequestExit();
        void Shutdown();

        void SetFullscreen( bool fullscreen );
        WindowSize GetWindowSize();

        SDL_Window* GetWindow() const { return m_window; }
        Rml::SystemInterface* GetSystemInterface() const { return m_systemInterface; }
        Rml::RenderInterface* GetRenderInterface() const { return m_renderInterface; }

        double GetDeltaTime() { return m_deltaTime.count(); }
        double GetElapsedTime() { return m_systemInterface->GetElapsedTime(); }

        void StartGame();

        void StartHost();
        void StopHost();

        bool IsHost();

        void Connect( const std::string& ip, uint16_t port );
        void Disconnect();

        void SaveSettings( const std::string& filepath );
        void LoadSettings( const std::string& filepath );

    public:
        GameSettings m_GameSettings;

    private:
        bool InitSDL( std::string windowName, uint32_t width, uint32_t height, bool allowResize );
        bool InitRml();

        void Update();

        void Render();
        void BeginFrame();
        void PresentFrame();

        void ProcessEvents( Rml::Context* context, KeyDownCallback key_down_callback, bool power_save );
        static bool ProcessKeyDownShortcuts( Rml::Context* context, Rml::Input::KeyIdentifier key, int key_modifier,
                                             float native_dp_ratio, bool priority );

        void OnWindowResize();

    private:
        bool m_isRunning = false;

        std::chrono::duration<double> m_deltaTime = {};

        SDL_Window* m_window = nullptr;
        SDL_GLContext m_glContext = nullptr;

        client::ui::SystemInterface_SDL* m_systemInterface = nullptr;
        client::ui::RenderInterface_GL3_SDL* m_renderInterface = nullptr;
        client::ui::FileInterface* m_fileInterface = nullptr;

        Rml::Context* m_rmlContext = nullptr;

#ifdef DEBUG
        std::unique_ptr<client::ui::DebugPanel> m_debugPanel = nullptr;
#endif
    };
}

extern shared::Game* g_Game;
