#include "game.hpp"

Game* g_Game = nullptr;

int main()
{
    g_Game = new Game();
    g_Game->Init();
    g_Game->Run();

    delete g_Game;

	return 0;
}