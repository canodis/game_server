SERVER_DIR	= ./_Server
CLIENT_DIR	= ./_Client
MLXDIR		= mlx-linux

SERVER_SRC	= $(wildcard $(SERVER_DIR)/*.cpp)
CLIENT_SRC	= $(wildcard $(CLIENT_DIR)/*.cpp)

CC			= c++ -g -std=gnu++14
MFLAGS		= -framework OpenGL -framework AppKit
LFLAGS		= -Lmlx_linux -L/usr/lib -Imlx_linux -lXext -lX11 -lm -lz

all: MINILIBX server client

server:
	$(CC) $(SERVER_SRC) -o server

client:
	$(CC) $(CLIENT_SRC) -o client $(MLXDIR)/libmlx.a -Lmlx_linux -L/usr/lib -Imlx_linux -lXext -lX11 -lm -lz 

MINILIBX:
	make -C $(MLXDIR) --silent
	@echo "MINILIBX compiled !"

multiclient:
	$(call while_loop)

clean :
	rm -rf server
	rm -rf client

re: clean all
