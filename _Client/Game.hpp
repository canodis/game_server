#pragma once 

#include <iostream>
#include "../mlx-linux/mlx.h"
#include "ChatBar.hpp"
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
#include <vector>
#include "Player.hpp"

#define WIDTH 600
#define HEIGTH 480

#define PORT 8080

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

struct Request {
	std::string username;
    int client_fd;
    int x_pos;
    int y_pos;
};

class Game {
public:
	// mlx
	void		*mlx;
	void		*win;
	t_data		image;
	t_data		pimage1;
	t_data		pimage2;
	const char	*uname;
	void		draw();
	static int	key_press(int keycode, void *game);
	static int	key_release(int keycode, void *game);
	static int	update(void	 *g);
	void		move();

	// Game
	Game(const char *ip_adress, const char *username = "canodis", int port = 8080);
	Game(int port = 8080);
	int						pIndex;
	Player	player;
	std::vector<Player *>	allPlayers;
	std::unordered_map<int, Player*> playerMap;
	t_key					key;
	ChatBar					chatbar;

	// Socket
	struct sockaddr_in	serv_addr;

	char	res[64];
	char	request[64];
	int		sock;
	void	SocketInit(const char *ip_adress, int port);
	void	response();
	void	requestThread();
	void	deletePlayer(int fd);
	void	parse_requests(const std::string& input);
	void	PositionRequest(std::istringstream &ss);
	void	LoginRequest(std::istringstream &ss);
};

