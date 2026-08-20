#include <algorithm>
#include <map>

#include "player.hpp"
#include "gamemanager.hpp"

namespace server
{
    //-----------------------------------------------------------------------------

    using namespace shared;

    //-----------------------------------------------------------------------------
    //
    //-----------------------------------------------------------------------------
    Player::Player( const ClientInfo& info )
        : m_info( info )
    {
    }

    void Player::GiveCard( Card* card )
    {
        if ( !card )
            return;

        m_cards.push_back( card );
    }

    Card* Player::DropCard( int index )
    {
        if ( index < 0 || index >= m_cards.size() )
            return nullptr;

        auto card = m_cards.at( index );
        m_cards.erase( m_cards.begin() + index );

        SortCards();

        return card;
    }

    void Player::SortCards()
    {
        std::ranges::stable_sort( m_cards, []( const Card* a, const Card* b )
        {
            return std::tie( b->Color, b->Type, b->Value )
                   < std::tie( a->Color, a->Type, a->Value );
        } );
    }

    std::size_t Player::GetIndex() const
    {
        return g_GameManager->GetPlayerIndex( this );
    }
}
