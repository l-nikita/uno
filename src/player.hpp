#pragma once

#include <string>
#include <vector>

#include "card.hpp"
#include "state_types.hpp"
#include "net/net_common.hpp"

namespace server
{
    //-----------------------------------------------------------------------------

    using Cards = std::vector<shared::Card*>;

    //-----------------------------------------------------------------------------
    //
    //-----------------------------------------------------------------------------
    class Player
    {
    public:
        explicit Player( const shared::ClientInfo& info );

        void GiveCard( shared::Card* card );
        shared::Card* DropCard( int index );

        void SortCards();

        Cards& GetCards() { return m_cards; }
        std::size_t GetIndex() const;

        const std::string& GetName() const { return m_info.Name; }
        const shared::net::Connection& GetConnection() const { return m_info.Connection; }

        const shared::ClientInfo& GetClientInfo() { return m_info; }

        shared::Card* m_LastCard = nullptr;

    private:
        shared::ClientInfo m_info;

        Cards m_cards;
    };
}
