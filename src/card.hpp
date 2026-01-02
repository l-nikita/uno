#pragma once

#include <SDL3/SDL.h>

//-----------------------------------------------------------------------------
constexpr auto RED_COLOR_CARD = SDL_Color{ 255, 0, 0, 255 };
constexpr auto GREEN_COLOR_CARD = SDL_Color{ 0, 255, 0, 255 };
constexpr auto BLUE_COLOR_CARD = SDL_Color{ 0, 0, 255, 255 };
constexpr auto YELLOW_COLOR_CARD = SDL_Color{ 255, 255, 0, 255 };
constexpr auto WILD_COLOR_CARD = SDL_Color{ 0, 0, 0, 255 };

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
	SDL_Color Color;
	int Value;
};