#pragma once

#include <map>
#include <SDL3/SDL.h>

//-----------------------------------------------------------------------------
enum class CardColor
{
	RED,
	GREEN,
	BLUE,
	YELLOW,
	WILD,
};

static const std::map<CardColor, SDL_Color> CARD_COLORS = {
	{CardColor::RED,	SDL_Color{ 201, 51, 51, 255 }},
	{CardColor::GREEN,	SDL_Color{ 62, 201, 51, 255 }},
	{CardColor::BLUE,	SDL_Color{ 51, 170, 201, 255 }},
	{CardColor::YELLOW, SDL_Color{ 201, 181, 51, 255 }},
	{CardColor::WILD,	SDL_Color{ 27, 27, 27, 255 }},
};

//-----------------------------------------------------------------------------
enum class CardType
{
	NUMBER,
	SKIP,
	REVERSE,
	DRAW_2,
	WILD,
	WILD_DRAW_4,
};

struct Card
{
	CardType Type;
	CardColor Color;
	int Value;
};

using Cards = std::vector<Card*>;