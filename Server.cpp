#include "Server.hpp"

Server::Server(int num_threads, int port) : thread_pool(num_threads)
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
	std::cout << "Waiting for players to connect..." << std::endl;
}

Server::~Server() { }

bool	Server::requestHandler(int fd)
{
	memset(this->request, 0, sizeof(this->request));
	int valread = recv(fd, this->request, 64, 0);
	if (valread <= 0) { 
		this->playerLeft(fd);
		return (false);
	}
	this->request[valread] = 0;
	return true;
}

void	Server::responseHandler()
{
	int x, y = 2;
	int fd = atoi(this->request);

	x = atoi(&this->request[2]);
	while (this->request[y] != 32 && this->request[y])
		y++;
	y = atoi(&this->request[y]);
	memset(this->response, 0, sizeof(this->response));
	sprintf(this->response, "%d %d %d ", fd, x, y);
	for (int i = 0; i < clients.size(); i++)
	{
		if (clients[i]->fd != fd)
			send(clients[i]->fd, this->response, strlen(this->response), 0);
	}
}

char	sendAll[10];

// [player_count] [your_fd] [all_players_fd(split with whitespaces)]
void	Server::sendLoginInfo(int fd)
{
	std::string loginInfo;

	loginInfo += std::to_string(clients.size()) + " ";
	loginInfo += std::to_string(fd) + " ";
	for (int i = 0; i < clients.size(); i++) {
		if (clients[i]->fd != fd)
			loginInfo += std::to_string(clients[i]->fd) + " ";
	}
	send(fd, loginInfo.c_str(), strlen(loginInfo.c_str()), 0);
}

void	Server::acceptNewConnection()
{
	int new_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
	FD_SET(new_fd, &playersFd);
	max_fd = new_fd > max_fd ? new_fd : max_fd;
	clients.push_back(new Client(new_fd));
	memset(sendAll, 0, sizeof(sendAll));
	sprintf(sendAll, "new %d ", new_fd);
	for (int i = 0; i < clients.size(); i++) {
		if (clients[i]->fd != new_fd)
			send(clients[i]->fd, sendAll, strlen(sendAll), 0);
		else
			sendLoginInfo(clients[i]->fd);
	}
	printf("New Player connected !\n");
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
	std::cout << "Client disconnected: " << fd << std::endl;
	FD_CLR(fd, &playersFd);
	close(fd);
	removeClient(fd);
	findMaxFd();
	memset(response, 0, sizeof(response));
	sprintf(response, "left %d ", fd);
	for (int i = 0; i < clients.size(); i++)
		send(clients[i]->fd, response, strlen(response), 0);
}

void	Server::findMaxFd()
{
	int max = 0;
	for (int i = 0; i < clients.size(); i++)
		if (clients[i]->fd > max)
			max = clients[i]->fd;
	this->max_fd = max;
}