#pragma once

#include <vector>
#include <string>
#include "card.hpp"
#include "net/net_common.hpp"

//-----------------------------------------------------------------------------
using Cars = std::vector<Card*>;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
class Player
{
public:
	Player(std::string name, NetConnection conn);

	void GiveCard(Card* card);
	void SortCards();

	std::string GetName() { return m_name; }
	Cars GetCards() { return m_cards; }

	int GetIndex() const;
	NetConnection GetConnection() { return m_connection; }

private:
	NetConnection m_connection;

	std::string m_name;
	Cars m_cards;
};