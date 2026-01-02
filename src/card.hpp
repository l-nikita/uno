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
	{CardColor::RED,	SDL_Color{ 255, 0, 0, 255 }},
	{CardColor::GREEN,	SDL_Color{ 0, 255, 0, 255 }},
	{CardColor::BLUE,	SDL_Color{ 0, 0, 255, 255 }},
	{CardColor::YELLOW, SDL_Color{ 255, 255, 0, 255 }},
	{CardColor::WILD,	SDL_Color{ 0, 0, 0, 255 }},
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