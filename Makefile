SERVER_SRC = mainServer.cpp Server.cpp Client.cpp
CLIENT_SRC = mainClient.cpp Game.cpp
CC = c++ -g -std=c++11
MLXDIR = mlx-linux

all: MINILIBX server client

server:
	$(CC) $(SERVER_SRC) -o server

ss: re
	./server

client:
	$(CC) $(CLIENT_SRC) -o client $(MLXDIR)/libmlx.a -Lmlx_linux -L/usr/lib -Imlx_linux -lXext -lX11 -lm -lz

MINILIBX:
	make -C $(MLXDIR) --silent
	@echo "MINILIBX compiled !"

clean :
	rm -rf server
	rm -rf client

rec: clean client

res: clean server

re: clean all
