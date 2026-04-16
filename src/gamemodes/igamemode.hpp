#pragma once

#include <vector>
#include "../player.hpp"

namespace gm
{
	enum class GameModeId
	{
		UNKNOWN,
		CLASSIC,
	};

	//-----------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------
	class IGameMode
	{
	public:
		virtual ~IGameMode() = default;

		virtual void Start() = 0;
		virtual void Stop() = 0;

		virtual void Update() = 0;
		virtual void OnPlayerAction(Player* player, const PlayerAction& action) = 0;

		virtual Card* GetTopDiscardCard() { return nullptr; }
		virtual int GetCurrentPlayerIndex() { return -1; }
		virtual bool IsReverse() { return false; }
		virtual bool IsDrawCard() { return false; }

		virtual int GetDeckCardsNum() = 0;

		virtual GameModeId GetId() const = 0;
	};
}