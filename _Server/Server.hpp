#pragma once

// #include "ThreadPool.h"
#include "Colors.hpp"
#include "Client.hpp"
#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <vector>
#include <thread>
#include <functional>
#include <algorithm>

class Server {
public:
	Server(int port);
	~Server();
	int		server_fd;
	int		max_fd;
	fd_set	playersFd;
	fd_set	readFd;
	struct sockaddr_in address;
	socklen_t addrlen;
	std::vector<Client *> clients;
	char	request[64];
	char	response[20];

	// ThreadPool threadPool;

	void	findMaxFd();
	void	responseHandler(char *req);
	void	threadFunc(int fd);
	void	acceptNewConnection();
	void	sendLoginInfo(int fd);
	void	removeClient(int client_fd);
	void	playerLeft(int fd);
};
