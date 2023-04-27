#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdio.h>
#include <stdlib.h>
#include <unordered_map>
#include <algorithm>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <thread>
#include <ctime>

#define HEIGTH 800
#define WIDTH 600

#define PORT 8080

struct vector2 {
	int x, y;
};

class   Player {
public:
	vector2 position;
	unsigned int color;
};

class Game {
public:
	// X11 lib
	Display	*dis;
	Window	win;
	GC		gc;
	XEvent	event;
	KeySym	key;
	int		screen;
	void	close();
	void	draw();
	void	eventHandler();
	bool	isKeyPressed(int keycode, const char* keyMap);

	// Game
	Game(const char *ip_adress, int port = 8080);
	Game(int port = 8080);
	int		pIndex;
	Player	player1;
	Player	player2;
	Player	*me;
	Player	*other;

	// Socket
	char	info[20];
	char	buffer[64];
	int		sock;
	void	requestHandle(char *str);
	void	selectPlayer();
	void	SocketInit(const char *ip_adress, int port);
	void	response();
	void	requestThread();
};

