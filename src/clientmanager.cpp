#include "clientmanager.hpp"
#include "game.hpp"
#include "state_types.hpp"
#include "net/net_manager.hpp"
#include "rmlui/game_screen.hpp"
#include "rmlui/lobby.hpp"
#include "rmlui/main_menu.hpp"

client::ClientManager* g_ClientManager = nullptr;

namespace client
{
    //-----------------------------------------------------------------------------

    using namespace shared;

    //-----------------------------------------------------------------------------
    //
    //-----------------------------------------------------------------------------
    ClientManager::ClientManager( Rml::Context* context )
        : m_rmlContext( context )
    {
        g_ClientManager = this;
        SetScene( ui::SceneId::MAIN_MENU );
    }

    ClientManager::~ClientManager()
    {
        if ( m_scene )
            delete m_scene, m_scene = nullptr;
    }

    //-----------------------------------------------------------------------------
    void ClientManager::Update()
    {
        if ( m_sceneId != ui::SceneId::NONE )
        {
            bool needsNewScene = false;
            if ( !m_scene || m_scene->GetId() != m_sceneId )
                needsNewScene = true;

            if ( needsNewScene )
            {
                ui::Scene* oldScene = m_scene;
                m_scene = CreateNewScene( m_sceneId );

                if ( oldScene )
                    DestroyScene( oldScene );
            } else
                m_scene->Update();
        }
    }

    void ClientManager::DeleteScenes()
    {
        for ( ui::Scene* scene: m_dirtyScenes )
            delete scene;

        m_dirtyScenes.clear();
    }

    //-----------------------------------------------------------------------------
    void ClientManager::ApplyUpdate( const StateUpdate& update )
    {
        if ( auto* gameState = std::get_if<GameState>( &update ) )
        {
            m_gameState = *gameState;

            if ( m_gameState.Stage == GameStage::LOBBY )
                SetScene( ui::SceneId::LOBBY );
            else if ( m_gameState.Stage == GameStage::ROUND_IN_PROGRESS )
                SetScene( ui::SceneId::GAME_SCREEN );
        }

        for ( auto& listener: m_listeners )
            listener->OnStateUpdate( update );
    }

    //-----------------------------------------------------------------------------
    void ClientManager::Subscribe( IStateListener* listener )
    {
        m_listeners.push_back( listener );
    }

    void ClientManager::Unsubscribe( IStateListener* listener )
    {
        auto it = std::find( m_listeners.begin(), m_listeners.end(), listener );
        if ( it != m_listeners.end() )
            m_listeners.erase( it );
    }

    const PlayerInfo& ClientManager::GetLocalPlayerInfo()
    {
        for ( const auto& info: GetGameState().Players )
        {
            if ( info.IsLocal )
                return info;
        }

        static const PlayerInfo emptyPlayer{};
        return emptyPlayer;
    }

    void ClientManager::OnDisconnected()
    {
        SetScene( ui::SceneId::MAIN_MENU );
    }

    //-----------------------------------------------------------------------------
    void ClientManager::OnConnected()
    {
        proto::NetMessage netMsg;
        proto::ClientInfo* info = netMsg.mutable_client_info();
        info->set_name( g_Game->m_GameSettings.Name );

        g_NetManager->GetClient()->SendToServer( netMsg );
    }

    void ClientManager::DoPlayerAction( const PlayerAction& action )
    {
        proto::NetMessage netMsg;
        proto::PlayerAction* act = netMsg.mutable_player_action();

        act->set_action( static_cast<proto::PlayerAction_ActionType>(action.Type) );
        act->set_card_id( action.CardId );
        act->set_chosen_color( static_cast<int>(action.ChosenColor) );

        g_NetManager->GetClient()->SendToServer( netMsg );
    }

    //-----------------------------------------------------------------------------
    void ClientManager::SetScene( ui::SceneId id )
    {
        m_sceneId = id;
    }

    void ClientManager::DestroyScene( ui::Scene* scene )
    {
        if ( scene )
            scene->Destroy(), m_dirtyScenes.push_back( scene );
    }

    ui::Scene* ClientManager::CreateNewScene( ui::SceneId id )
    {
        ui::Scene* scene = nullptr;
        switch ( id )
        {
            case ui::SceneId::MAIN_MENU:
                scene = new ui::MainMenu( m_rmlContext );
                break;
            case ui::SceneId::LOBBY:
                scene = new ui::Lobby( m_rmlContext );
                break;
            case ui::SceneId::GAME_SCREEN:
                scene = new ui::GameScreen( m_rmlContext );
                break;
            default:
                throw std::runtime_error( "Unknown scene!" );
        }

        return scene;
    }
}
