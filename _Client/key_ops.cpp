#include "Game.hpp"

int		Game::key_press(int keycode, void *game)
{
	Game *g = (Game *)game;

	if (keycode == 65307) { // Escape key
		mlx_destroy_window(g->mlx, g->win);
		exit(1);
	}
	if (keycode == 97) { // A key
		g->key.a = true;
	}
	if (keycode == 115) { // S key
		g->key.s = true;
	}
	if (keycode == 100) { // D key
		g->key.d = true;
	}
	if (keycode == 119) { // W key
		g->key.w = true;
	}
	return (0);
}

void	Game::move()
{
	if (key.a) {
		player.position.x -= 2; 
	}
	if (key.d) {
		player.position.x += 2; 
	}
	if (key.s) {
		player.position.y += 2; 
	}
	if (key.w) {
		player.position.y -= 2; 
	}
}

int    Game::key_release(int keycode, void *game)
{
	Game *g = (Game *)game;

	if (keycode == 97) { // A key
		g->key.a = false;
	}
	if (keycode == 115) { // S key
		g->key.s = false;
	}
	if (keycode == 100) { // D key
		g->key.d = false;
	}
	if (keycode == 119) { // W key
		g->key.w = false;
	}
	return (0);
}