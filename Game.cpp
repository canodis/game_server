#include "Game.hpp"

Game::Game(int port) : Game("127.0.0.1", port) { }

Game::Game(const char *ip_adress, int port)
{
    int a;

    mlx = mlx_init();
    win = mlx_new_window(mlx,  WIDTH, HEIGTH, "canodis");
    playerCount = 0;
    memset(AllPlayers, 0, sizeof(AllPlayers));
    player.position.x = 0;
    player.position.y = 0;
    // image.img = mlx_new_image(mlx, WIDTH, HEIGTH);
    // image.addr = (int *)mlx_get_data_addr(image.img, &a, &a, &a);
    pimage1.img = mlx_xpm_file_to_image(mlx, "sprite1.xpm", &a, &a);
    pimage2.img = mlx_xpm_file_to_image(mlx, "sprite2.xpm", &a, &a);
    // pimage1.addr = (int *)mlx_get_data_addr(pimage1.img, &a, &a, &a);
    // pimage2.addr = (int *)mlx_get_data_addr(pimage2.img, &a, &a, &a);
    SocketInit(ip_adress, port);
}

void    Game::draw()
{
    mlx_clear_window(mlx, win);

    mlx_put_image_to_window(mlx, win, pimage1.img, player.position.x, player.position.y);
    for (int i = 0; i < playerCount; i++) {
        mlx_put_image_to_window(mlx, win, pimage2.img, AllPlayers[i].position.x, AllPlayers[i].position.y);
    }
}

void    Game::requestHandle()
{
    int index = 0;
    int fd = 0;
    int i = 0;

    if (strncmp(this->buffer, "new", 3) == 0)
        acceptNewPlayer();
    else
    {
        fd = atoi(this->buffer);
        for (i = 0; i < playerCount; i++)
            if (AllPlayers[i].fd == fd)
                break;
        while (this->buffer[index] != 32)
            index++;
        AllPlayers[i].position.x = atoi(&this->buffer[index]);
            index++;
        while (this->buffer[index] != 32)
            index++;
        AllPlayers[i].position.y = atoi(&this->buffer[index]);
    }
    
}

void    Game::SocketInit(const char *ip_adress, int port)
{
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        exit(31);
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip_adress, &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        exit(1);
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        exit(1);
    }
    char loginInfo[128];
    bzero(loginInfo, sizeof(loginInfo));
    recv(sock, loginInfo, 127, 0);
    loginProccess(loginInfo);
}

void    Game::response()
{
    memset(this->info, 0, sizeof(this->info));
    sprintf(info, "%d %d %d ", pIndex, player.position.x, player.position.y);
    send(this->sock, this->info, strlen(this->info), 0);
}

int Game::key_press(int keycode, void *game)
{
    Game *g = (Game *)game;

    if (keycode == 53) { // Escape key
        mlx_destroy_window(g->mlx, g->win);
        exit(1);
    }
    if (keycode == 0) { // A key
        g->key.a = true;
    }
    if (keycode == 1) { // S key
        g->key.s = true;
    }
    if (keycode == 2) { // D key
        g->key.d = true;
    }
    if (keycode == 13) { // W key
        g->key.w = true;
    }
    return (0);
}

void    Game::acceptNewPlayer()
{
    int fd = atoi(&this->buffer[3]);
    AllPlayers[playerCount].fd = fd;
    AllPlayers[playerCount].position.x = 0;
    AllPlayers[playerCount].position.y = 0;
    playerCount++;
}

void    Game::loginProccess(char *loginInfo)
{
    int pCount = atoi(loginInfo);
    int myFd = atoi(&loginInfo[2]);
    int index = 2;

    for (int i = 0; i < pCount - 1; i++) {
        while (loginInfo[index] != 32 && loginInfo[index])
            index++;
        AllPlayers[playerCount].fd = atoi(&loginInfo[index]);
        AllPlayers[playerCount].position.x = 0;
        AllPlayers[playerCount].position.y = 0;
        playerCount++;
        index++;
    }
    pIndex = myFd;
}

void    Game::move()
{
    if (key.a) {
        player.position.x -= 1; 
    }
    if (key.d) {
        player.position.x += 1; 
    }
    if (key.s) {
        player.position.y += 1; 
    }
    if (key.w) {
        player.position.y -= 1; 
    }
}

int    Game::key_release(int keycode, void *game)
{
    Game *g = (Game *)game;

    if (keycode == 0) { // A key
        g->key.a = false;
    }
    if (keycode == 1) { // S key
        g->key.s = false;
    }
    if (keycode == 2) { // D key
        g->key.d = false;
    }
    if (keycode == 13) { // W key
        g->key.w = false;
    }
    return (0);
}

int Game::exit_game(int a, void *game) {
    Game *g = (Game *)game;

    mlx_destroy_window(g->mlx, g->win);
    exit(1);
}
