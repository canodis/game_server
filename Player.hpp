#include <iostream>

struct vector2 {
	int x, y;
};

class	Player {
public:
	Player();
	Player(int fd, int posx, int posy);
	vector2 position;
	int		fd;
};
