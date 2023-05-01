#include "Game.hpp"
#include <string>
#include <sstream>

Game::Game(int port) : Game("127.0.0.1", port) { }

Game::Game(const char *ip_adress, int port)
{
	int a;

	mlx = mlx_init();
	win = mlx_new_window(mlx,  WIDTH, HEIGTH, "canodis");
	pIndex = 666;
	player.position.x = 0;
	player.position.y = 0;
	pimage1.img = mlx_xpm_file_to_image(mlx, "./Sprites/sprite1.xpm", &a, &a);
	pimage2.img = mlx_xpm_file_to_image(mlx, "./Sprites/sprite2.xpm", &a, &a);
	SocketInit(ip_adress, port);
}

void	Game::draw()
{
	mlx_clear_window(mlx, win);

	mlx_put_image_to_window(mlx, win, pimage1.img, player.position.x, player.position.y);
	for (int i = 0; i < allPlayers.size(); i++) {
		mlx_put_image_to_window(mlx, win, pimage2.img, allPlayers[i]->position.x, allPlayers[i]->position.y);
	}
}

void	Game::response()
{
	if (pIndex == 666)
		return ;
	sprintf(res, "/%d %d %d*", pIndex, player.position.x, player.position.y);
	send(this->sock, this->res, strlen(this->res), 0);
}

void	Game::deletePlayer(int fd) {
	auto it = std::find_if(allPlayers.begin(), allPlayers.end(), [&fd](const Player* player) {
		return player->fd == fd;
	});
	if (it != allPlayers.end()) {
		delete *it;
		allPlayers.erase(it);
	}
}

void Game::parse_requests(const std::string& input)
{
    std::size_t start = 0, end = 0;

    while ((start = input.find('/', start)) != std::string::npos) {
        end = input.find('*', start);
        if (end == std::string::npos) {
            break;
        }
		std::string substr = input.substr(start + 1, end - start - 1);
		if (substr.find("Pos") == 0) {
			std::istringstream ss(substr.substr(3));
        	this->PositionRequest(ss);
		} else if (substr.find("New") == 0) {
			std::istringstream ss(substr.substr(3));
			int new_fd;
			ss >> new_fd;
			allPlayers.push_back(new Player(new_fd, 0, 0));
		} else if (substr.find("Left") == 0) {
			std::istringstream ss(substr.substr(4));
			int fd;
			ss >> fd;
			deletePlayer(fd);
		} else if (substr.find("Login") == 0) {
			std::istringstream ss(substr.substr(5));
			LoginRequest(ss);
		}
        start = end + 1;
    }
}

void	Game::PositionRequest(std::istringstream &ss)
{
	Request req;

	ss >> req.client_fd >> req.x_pos >> req.y_pos;

	for (auto player : allPlayers)
    	playerMap[player->fd] = player;

	auto player = playerMap[req.client_fd];

	if (player) {
		player->position.x = req.x_pos;
		player->position.y = req.y_pos;
	}
}

void	Game::LoginRequest(std::istringstream &ss)
{
	int playerCount;
	int	playerFd;

	ss >> playerCount;
	ss >> pIndex;
	for (int i = 0; i < playerCount - 1; i++) {
		ss >> playerFd;
		allPlayers.push_back(new Player(playerFd, 0, 0));
	}
}