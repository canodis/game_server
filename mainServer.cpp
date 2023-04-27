#include "Server.hpp"

void Server::threadFunc(int fd)
{
    requestHandler(fd);
    responseHandler();
}

int main(int argc, char const *argv[])
{
    Server  server(8080);
    int  activity;

    server.sendInit();

    while (true)
    {
        FD_ZERO(&server.playersFd);
        FD_SET(server.p1sock, &server.playersFd);
        FD_SET(server.p2sock, &server.playersFd);
        server.max_fd = server.p2sock;

        activity = 0;
        while (activity == 0) {
            server.readFd = server.playersFd;
            activity = select(server.max_fd + 1, &server.readFd, NULL, NULL, NULL);
        }
        for (int i = server.p1sock; i < server.p2sock + 1; i++)
        {
            if (FD_ISSET(i, &server.readFd)) {
                std::thread t1(&Server::threadFunc, &server, i);
                t1.join();
            }
        }
    }
}