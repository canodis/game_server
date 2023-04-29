#include "Game.hpp"

void Game::requestThread()
{
    fd_set readfds;

    FD_ZERO(&readfds);
    FD_SET(this->sock, &readfds);

    while (666)
    {
        select(this->sock + 1, &readfds, NULL, NULL, NULL);
        if (FD_ISSET(this->sock, &readfds))
        {
            memset(this->buffer, 0, sizeof(buffer));

            if (recv(this->sock, buffer, sizeof(buffer), 0) <= 0)
            {
                ::close(this->sock);
                std::cerr << "Server disconnected" << std::endl;
                exit(1);
            }
            printf("alindi : %s\n", buffer);
            this->requestHandle();
        }
    }
}

int Game::update(void *g)
{
    Game *game = (Game *)g;

    game->draw();
    game->response();
    usleep(10000);
    return (0);
}

int main(int ac, char **av)
{
    // int port = 8080;

    // if (ac != 2 && ac != 3) {
    //     std::cout << "Ip adresi girmelisin [./client 127.0.0.1]\n3. arguman olarak istege bagli olarak portta girebilirsin\n";
    //     exit(0);
    // }
    // if (ac == 3)
    //     port = atoi(av[2]);

    Game game;

    std::thread t1(&Game::requestThread, &game);
    t1.detach();

    mlx_hook(game.win, 2, 1, Game::key_press, &game);
    mlx_loop_hook(game.mlx, Game::update, &game);
    mlx_loop(game.mlx);

    return 0;
}