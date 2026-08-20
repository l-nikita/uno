#pragma once

#include <RmlUi/Core.h>

#include "card.hpp"
#include "clientmanager.hpp"
#include "scene.hpp"

namespace client::ui
{
    //-----------------------------------------------------------------------------
    //
    //-----------------------------------------------------------------------------
    class GameScreen final : public Scene, IStateListener
    {
    public:
        explicit GameScreen( Rml::Context* context );

        void ProcessEvent( Rml::Event& event ) override;

        SceneId GetId() const override { return SceneId::GAME_SCREEN; }

        void Update() override;
        void Destroy() override;

        void OnStateUpdate( const shared::StateUpdate& update ) override;

    private:
        void AskPlayOrKeep();
        void HidePlayOrKeep();

        void ChooseColor( int cardId );
        void HideColorSelection();

        void PlayCard( int cardId );

        void CreatePlayersCards();
        void CreateTopDiscardCard();

        Rml::ElementPtr CreateCard( const shared::Card& card, bool isStatic );
        void CreateCard( const shared::Card& card, int index, Rml::Element* container );
        void CreateOpponentCards( int count, Rml::Element* container );

        bool CanPlay();
        bool CanPlayCard( int index );
        bool CanPlayCard( const shared::Card& card );
        bool HasNoPlayableCards();

        Rml::ElementDocument* m_document = nullptr;

        struct PendingDeletion
        {
            Rml::Element* Element;
            double Time;
        };

        std::vector<PendingDeletion> m_deletionQueue;

        bool m_drawCard = false;
    };
}
