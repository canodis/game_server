SERVER_SRC = mainServer.cpp Server.cpp Client.cpp
CLIENT_SRC = mainClient.cpp Game.cpp
CC = c++ -g

all: server client

server:
	$(CC) $(SERVER_SRC) -o server

ss: re
	./server

client:
	$(CC) $(CLIENT_SRC) -o client -lX11


clean :
	rm -rf server
	rm -rf client

rec: clean client

res: clean server

re: clean all
