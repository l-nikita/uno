#include "player.hpp"

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Player::Player(std::string name)
{
	m_name = name;
}

void Player::GiveCard(Card* card)
{
	m_cards.push_back(card);
}