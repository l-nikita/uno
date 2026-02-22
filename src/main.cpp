#include "game.hpp"

Game* g_Game = nullptr;

int main()
{
    Game game;
    g_Game = &game;

    game.Init();
    game.Run();
    game.Shutdown();

	return 0;
}