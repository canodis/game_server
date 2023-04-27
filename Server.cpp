#include "Server.hpp"

struct sockaddr_in address;
int addrlen = sizeof(address);

Server::Server(int port)
{
    this->server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->server_fd == 0) {
        std::cerr << "Socket create error !\n";
        exit(1); 
    }
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        std::cerr << "setsockopt error" << std::endl;
        exit(1);
    }
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    if (bind(this->server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        exit(1);
    }
    if (listen(this->server_fd, 2) < 0) {
        std::cerr << "Listen failed" << std::endl;
        exit(1);
    }
    std::cout << "Waiting for players to connect..." << std::endl;
    this->p1sock = accept(this->server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
    this->p2sock = accept(this->server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
    clients.push_back(p1sock);
    clients.push_back(p2sock);
    std::cout << "Players connected !\n"; 
}

void    Server::sendInit()
{
    const char    *p1 = "1";
    const char    *p2 = "2";

    send(this->clients[0].fd, p1, strlen(p1), 0);
    send(this->clients[1].fd, p2, strlen(p2), 0);
}

Server::~Server() { }

void    Server::requestHandler(int fd)
{
    memset(this->request, 0, sizeof(this->request));
    int valread = recv(fd, this->request, 64, 0);
    this->request[valread] = 0;
}

void    Server::responseHandler()
{
    int x, y = 2;

    x = atoi(&this->request[2]);
    while (this->request[y] != 32 && this->request[y])
        y++;
    y = atoi(&this->request[y]);
    memset(this->response, 0, sizeof(this->response));
    sprintf(this->response, "%d %d ", x, y);
    if (this->request[0] == '1')
        send(this->p2sock, this->response, strlen(this->response), 0);
    else if (this->request[0] == '2')
        send(this->p1sock, this->response, strlen(this->response), 0);
}