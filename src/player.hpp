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
	Card* DropCard(int index);

	void SortCards();

	Cars& GetCards() { return m_cards; }
	int GetIndex();

	const std::string& GetName() { return m_info.Name; }
	const NetConnection& GetConnection() { return m_info.Connection; }

	const ClientInfo& GetClientInfo() { return m_info; }

private:
	ClientInfo m_info;

	Cars m_cards;
};