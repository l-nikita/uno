#pragma once

#include <SDL3/SDL.h>

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
	SDL_Color Color;
};