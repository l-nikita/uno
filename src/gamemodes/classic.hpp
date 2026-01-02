#pragma once

#include "igamemode.hpp"

namespace gm
{
	//-----------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------
	class Classic : public IGameMode
	{
	public:
		void Init();
		void Start();
		void Stop();

		void Update();

		void GiveCards();
	};
}