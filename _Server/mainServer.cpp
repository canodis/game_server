#include "Server.hpp"

Server* Server::instance = nullptr;

void Server::signalHandler(int signum) {
	std::cout << Color(Color::BOLD) << Color(Color::YELLOW) << "Server is shutting down..." << std::endl;

	for (int i = 0; i < instance->clients.size(); i++) {
		close(instance->clients[i]->fd);
		delete(instance->clients[i]);
	}
	close(instance->server_fd);
	exit(signum);
}

void Server::threadFunc(int fd)
{
	// char	requ[64];
	// int valread = recv(fd, requ, 63, 0);
	// if (valread <= 0) { 
	// 	this->playerLeft(fd);
	// 	return ;
	// }
	// requ[valread] = 0;
	// if (clients.size() < 2)
	// 	return;
	// parse_requests(requ);
}

void	Server::handleThread(int fd) 
{
	char	req[128];

	while (1)
	{
		if (FD_ISSET(fd, &this->readFd))
		{
			int valread = recv(fd, req, 127, 0);
			if (valread <= 0) { 
				this->playerLeft(fd);
				break ;
			}
			req[valread] = 0;
			if (clients.size() < 2)
				continue ;
			parse_requests(req);
		}
	}
	auto it = tMap.find(fd);
	tMap.erase(it);
}

int main(int argc, char const *argv[])
{
	Server server(8080);
	int activity;

	FD_ZERO(&server.playersFd);
	FD_SET(server.server_fd, &server.playersFd);
	server.max_fd = server.server_fd;

	while (true)
	{
		server.readFd = server.playersFd;
		activity = select(server.max_fd + 1, &server.readFd, NULL, NULL, NULL);
		if (activity == -1)
			continue;
		for (int i = 3; i < server.max_fd + 1; i++)
		{
			if (FD_ISSET(i, &server.readFd) && i == server.server_fd)
				server.acceptNewConnection();
			// else if (FD_ISSET(i, &server.readFd))
			// {
			// 	std::thread t1(std::bind(&Server::threadFunc, &server, i));
			// 	t1.join();
			// }
		}
	}
}