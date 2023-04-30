#include "Server.hpp"

void	Server::threadFunc(int fd)
{
	if (requestHandler(fd) == false || clients.size() < 2)
		return ;
	responseHandler();
}


int	main(int argc, char const *argv[])
{
	size_t num_threads = std::thread::hardware_concurrency();
	Server  server(num_threads, 8080);
	int  activity;

	FD_ZERO(&server.playersFd);
	FD_SET(server.server_fd, &server.playersFd);
	server.max_fd = server.server_fd;

	while (true)
	{
		activity = 0;
		while (activity == 0) {
			server.readFd = server.playersFd;
			activity = select(server.max_fd + 1, &server.readFd, NULL, NULL, NULL);
		}
		for (int i = 3; i < server.max_fd + 1; i++)
		{
			if (FD_ISSET(i, &server.readFd) && i == server.server_fd) {
				server.acceptNewConnection();
			}
			else if (FD_ISSET(i, &server.readFd)) {
				std::thread t1(std::bind(&Server::threadFunc, &server, i));
				t1.join();
			}
		}
	}
}