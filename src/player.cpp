#include <algorithm>
#include <map>
#include "player.hpp"
#include "gamemanager.hpp"

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Player::Player(std::string name, NetConnection conn)
	: m_name(name), m_connection(conn)
{

}

void Player::GiveCard(Card* card)
{
	m_cards.push_back(card);
}

void Player::SortCards()
{
	std::sort(m_cards.begin(), m_cards.end(), [](const Card* a, const Card* b) {
		return std::tie(b->Color, b->Type, b->Value)
			< std::tie(a->Color, a->Type, a->Value);
	});
}

int Player::GetIndex() const
{
	return g_GameManager->GetPlayerIndex(this);
}