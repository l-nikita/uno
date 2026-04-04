#pragma once

#include <vector>
#include <string>
#include "card.hpp"
#include "state_types.hpp"
#include "net/net_common.hpp"

//-----------------------------------------------------------------------------
using Cars = std::vector<Card*>;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
class Player
{
public:
	Player(const ClientInfo& info);

	void GiveCard(Card* card);
	void SortCards();

	Cars GetCards() { return m_cards; }
	int GetIndex();

	std::string GetName() { return m_info.Name; }
	NetConnection GetConnection() { return m_info.Connection; }

	ClientInfo GetClientInfo() { return m_info; }

private:
	ClientInfo m_info;

	Cars m_cards;
};