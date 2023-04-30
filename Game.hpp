#include <iostream>
#include "mlx-linux/mlx.h"
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
	int		fd;
};

typedef struct s_data {
	int		*addr;
	void	*img;
}	t_data;

typedef struct s_key {
	bool	a = false;
	bool	w = false;
	bool	s = false;
	bool	d = false;
}	t_key;

class Game {
public:
	// mlx
	void	*mlx;
	void	*win;
	t_data	image;
	t_data	pimage1;
	t_data	pimage2;

	void		draw();
	static int	key_press(int keycode, void *game);
	static int	key_release(int keycode, void *game);
	static int exit_game(int a, void *game);
	static int	update(void	 *g);

	// Game
	Game(const char *ip_adress, int port = 8080);
	Game(int port = 8080);
	int		pIndex;
	Player	player;
	Player	AllPlayers[20];
	int		playerCount;
	t_key	key;
	void	move();

	// Socket
	struct sockaddr_in serv_addr;

	char	info[20];
	char	buffer[64];
	int		sock;
	void	requestHandle();
	void	acceptNewPlayer();
	void	SocketInit(const char *ip_adress, int port);
	void	response();
	void	requestThread();
	void	loginProccess(char *loginInfo);
};

