#pragma once

#include <vector>
#include "../player.hpp"

namespace gm
{
	//-----------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------
	class IGameMode
	{
	public:
		virtual ~IGameMode() = default;

		virtual void Init() = 0;
		virtual void Start() = 0;
		virtual void Stop() = 0;

		virtual void Update() = 0;

		virtual size_t GetDeckCardsNum() = 0;
	};
}