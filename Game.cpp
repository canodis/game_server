#include "Game.hpp"

Game::Game(int port) : Game("127.0.0.1", port) { }

Game::Game(const char *ip_adress, int port)
{
    dis = XOpenDisplay((char *)0);
    screen = DefaultScreen(dis);
    win = XCreateSimpleWindow(dis, DefaultRootWindow(dis), 0, 0, WIDTH, HEIGTH, 5, 0xFFFFFF, 0); 
    XSetStandardProperties(dis, win, "canodis", "hi", None, NULL, 0, NULL);
    XSelectInput(dis, win, ExposureMask | ButtonPressMask | KeyPressMask);
    gc = XCreateGC(dis, win, 0, 0);
    XSetBackground(dis, gc, 0xFFFFFF);
    XSetForeground(dis, gc, 0);
    XClearWindow(dis, win);
    XMapRaised(dis, win);
    XAutoRepeatOff(dis);

    player1.position.x = 0;
    player1.position.y = 0;
    player2.position.x = 0;
    player2.position.y = 100;
    SocketInit(ip_adress, port);
}

void    Game::close()
{
    XFreeGC(dis, gc);
    XDestroyWindow(dis, win);
    XCloseDisplay(dis);
    exit(0);
}

void    Game::draw()
{
    XClearWindow(dis, win);
    if (this->pIndex == 1)
        XSetForeground(dis, gc, 0xFF0000);
    else
        XSetForeground(dis, gc, 0x00FF00);
    XFillRectangle(dis, win, gc, me->position.x, me->position.y, 30, 30);
    if (this->pIndex == 1)
        XSetForeground(dis, gc, 0x00FF00);
    else
        XSetForeground(dis, gc, 0xFF0000);
    XFillRectangle(dis, win, gc, other->position.x, other->position.y, 30, 30);
}

bool Game::isKeyPressed(int keycode, const char* keyMap)
{
    return keyMap[keycode / 8] & (1 << (keycode % 8));
}

void    Game::eventHandler()
{
    while (XPending(dis))
    {
        XNextEvent(dis, &event);
        if (event.type == KeyPress && event.xkey.keycode == 9) // Escape key
            close();
    }
    char keyMap[32];
    XQueryKeymap(dis, keyMap);
    if (pIndex == 1)
    {
        if (isKeyPressed(38, keyMap)) // A key
            me->position.x -= 1;
        if (isKeyPressed(40, keyMap)) // D key
            me->position.x += 1;
        if (isKeyPressed(25, keyMap)) // W key
            me->position.y -= 1;
        if (isKeyPressed(39, keyMap)) // S key
            me->position.y += 1;
    }
    else if (pIndex == 2)
    {
        if (isKeyPressed(113, keyMap)) // Left arrow key
            me->position.x -= 1;
        if (isKeyPressed(114, keyMap)) // Right arrow key
            me->position.x += 1;
        if (isKeyPressed(111, keyMap)) // Up arrow key
            me->position.y -= 1;
        if (isKeyPressed(43, keyMap)) // Down arrow key
            me->position.y += 1;
    }

}

void    Game::requestHandle(char *str)
{
    int index = 0;
    other->position.x = atoi(str);
    while (str[index] != 32)
        index++;
    other->position.y = atoi(&str[index]);
}

void    Game::selectPlayer()
{
    if (pIndex == 1) {
        this->me = &player1;
        this->other = &player2;
    }
    else {
        this->me = &player2;
        this->other = &player1;
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
    char whoami[2];
    recv(sock, whoami, 1, 0);
    this->pIndex = atoi(whoami);
    this->selectPlayer();
}

void    Game::response()
{
    memset(this->info, 0, sizeof(this->info));
    sprintf(info, "%d %d %d ", pIndex, me->position.x, me->position.y);
    send(this->sock, this->info, strlen(this->info), 0);
}