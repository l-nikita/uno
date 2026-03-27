#pragma once

#include <vector>
#include <string>
#include "card.hpp"

//-----------------------------------------------------------------------------
using Cars = std::vector<Card*>;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
class Player
{
public:
	Player(std::string name);

	void GiveCard(Card* card);
	void SortCards();

	std::string GetName() { return m_name; }
	Cars GetCards() { return m_cards; }

	int GetIndex() const;

private:
	std::string m_name;
	Cars m_cards;
};