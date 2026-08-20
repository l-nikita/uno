#pragma once

#include <vector>

#include "gamemodes/igamemode.hpp"

namespace server::mode
{
    //-----------------------------------------------------------------------------
    //
    //-----------------------------------------------------------------------------
    class Classic : public IGameMode
    {
    public:
        ~Classic() override;

        void Start() override;
        void Stop() override;

        void Update() override;
        void OnPlayerAction( Player* player, const shared::PlayerAction& action ) override;

        void NextTurn();

        void GenerateDeckCards();
        void DealCards();
        void AddCardToDiscardPile( shared::Card* card );
        shared::Card* TakeCardFromDeck();

        bool CanPlayCard( shared::Card* card );
        bool HasNoPlayableCards();

        shared::Card* GetTopDiscardCard() override { return m_discardPile.empty() ? nullptr : m_discardPile.back(); }
        int GetCurrentPlayerIndex() override { return m_currentPlayerIndex; }
        bool IsReverse() override { return m_reverse; }

        int GetNextPlayer();

        int GetPlayersCardsNum() { return 7; }
        int GetDeckCardsNum() override { return m_deck.size(); }

        GameModeId GetId() const override { return GameModeId::CLASSIC; }

    private:
        Cards m_deck;
        Cards m_discardPile;

        bool m_reverse = false;
        bool m_skip = false;
        bool m_drawCard = false;

        int m_currentPlayerIndex = 0;
    };
}
