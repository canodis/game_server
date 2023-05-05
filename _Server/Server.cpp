#include "Server.hpp"

#include <string>
#include <sstream>

struct Request {
	std::string username;
	int client_fd;
	int x_pos;
	int y_pos;
};

void Server::parse_requests(const std::string& input) {
	char	res[64];
	std::size_t start = 0, end = 0;

	while ((start = input.find('/', start)) != std::string::npos) {
		end = input.find('*', start);
		if (end == std::string::npos) {
			break;
		}
		std::string substr = input.substr(start + 1, end - start - 1);
		if (substr.find("Pos") == 0) {
			std::istringstream ss(substr.substr(3));
			Request req;
			ss >> req.client_fd >> req.username >> req.x_pos >> req.y_pos;
			sprintf(res, "/Pos%d %s %d %d*", req.client_fd, req.username.c_str(),req.x_pos, req.y_pos);
			for (int i = 0; i < clients.size(); i++)
			{
				if (clients[i]->fd != req.client_fd) {
					send(clients[i]->fd, res, strlen(res), 0);
				}
			}
		}
		start = end + 1;
	}
}

Server::Server(int port)
{
	this->server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->server_fd == 0) {
		std::cerr << "Socket create error !\n";
		exit(1); 
	}
	int opt = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEADDR, &opt, sizeof(opt))) {
		std::cerr << "setsockopt error" << std::endl;
		exit(1);
	}
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
	addrlen = sizeof(address);
	if (bind(this->server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		std::cerr << "Bind failed" << std::endl;
		exit(1);
	}
	if (listen(this->server_fd, 20) < 0) {
		std::cerr << "Listen failed" << std::endl;
		exit(1);
	}
	std::cout << Color(Color::GREEN) << "Server Up..." << Color(Color::WHITE) << std::endl;
	signal(SIGINT, signalHandler);
	instance = this;
}

Server::~Server() { }

char	sendAll[10];

void	Server::sendLoginInfo(int fd)
{
	std::string loginInfo;

	loginInfo += "/Login" + std::to_string(clients.size()) + " ";
	loginInfo += std::to_string(fd) + " ";
	for (int i = 0; i < clients.size(); i++) {
		if (clients[i]->fd != fd)
			loginInfo += std::to_string(clients[i]->fd) + " ";
	}
	loginInfo += "*";
	send(fd, loginInfo.c_str(), strlen(loginInfo.c_str()), 0);
}

void	Server::acceptNewConnection()
{
	int new_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
	FD_SET(new_fd, &playersFd);
	max_fd = new_fd > max_fd ? new_fd : max_fd;
	clients.push_back(new Client(new_fd));
	sprintf(sendAll, "/New %d*", new_fd);
	for (int i = 0; i < clients.size(); i++) {
		if (clients[i]->fd != new_fd)
			send(clients[i]->fd, sendAll, strlen(sendAll), 0);
		else
			sendLoginInfo(clients[i]->fd);
	}
	tMap.insert(std::pair<int, std::thread *>(new_fd, new std::thread(std::bind(&Server::handleThread, this, new_fd))));
	std::cout << Color(Color::PURPLE) << "New Player connected : " << new_fd << Color(Color::WHITE) << std::endl; 
}

void	Server::removeClient(int client_fd) {
	auto it = std::find_if(clients.begin(), clients.end(), [&client_fd](const Client* client) {
		return client->fd == client_fd;
	});
	if (it != clients.end()) {
		delete *it;
		clients.erase(it);
	}
}

void	Server::playerLeft(int fd)
{
	std::cout << Color(Color::RED) << "Client disconnected: " << fd << Color(Color::WHITE) << std::endl;
	char	res[64];
	FD_CLR(fd, &playersFd);
	close(fd);
	removeClient(fd);
	findMaxFd();
	sprintf(res, "/Left %d*", fd);
	for (int i = 0; i < clients.size(); i++)
		send(clients[i]->fd, res, strlen(res), 0);
}

void	Server::findMaxFd()
{
	int max = 0;
	for (int i = 0; i < clients.size(); i++)
		if (clients[i]->fd > max)
			max = clients[i]->fd;
	this->max_fd = max == 0 ? server_fd : max;
}