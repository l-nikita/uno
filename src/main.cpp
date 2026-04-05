#include <cstring>
#include "game.hpp"

Game* g_Game = nullptr;

int main(int argc, char* argv[])
{
    LaunchArgs args;

    for (int i = 1; i < argc; ++i)
    {
        if (std::strcmp(argv[i], "--host") == 0)
        {
            args.Mode = LaunchMode::HOST;
        }
        else if (std::strcmp(argv[i], "--connect") == 0)
        {
            args.Mode = LaunchMode::CONNECT;
            if (i + 1 < argc && argv[i + 1][0] != '-')
                args.Ip = argv[++i];
        }
        else if (std::strcmp(argv[i], "--port") == 0)
        {
            if (i + 1 < argc)
                args.Port = static_cast<uint16_t>(std::atoi(argv[++i]));
        }
        else if (std::strcmp(argv[i], "--name") == 0)
        {
            if (i + 1 < argc)
                args.Name = argv[++i];
        }
    }

    Game game;
    g_Game = &game;

    game.Init(args);
    game.Run();
    game.Shutdown();

	return 0;
}