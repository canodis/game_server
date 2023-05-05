#include "Game.hpp"

int	Game::key_press(int keycode, void *game)
{
	Game *g = (Game *)game;

	if (keycode == 53) { // Escape key
		mlx_destroy_window(g->mlx, g->win);
		exit(1);
	}
	if (keycode == 0) { // A key
		g->key.a = true;
	}
	if (keycode == 1) { // S key
		g->key.s = true;
	}
	if (keycode == 2) { // D key
		g->key.d = true;
	}
	if (keycode == 13) { // W key
		g->key.w = true;
	}
	return (0);
}

void	Game::move()
{
	if (key.a) {
		player.position.x -= 2; 
		// response();
	}
	if (key.d) {
		player.position.x += 2; 
		// response();
	}
	if (key.s) {
		player.position.y += 2; 
		// response();
	}
	if (key.w) {
		player.position.y -= 2; 
		// response();
	}
}

int	Game::key_release(int keycode, void *game)
{
	Game *g = (Game *)game;

	if (keycode == 0) { // A key
		g->key.a = false;
	}
	if (keycode == 1) { // S key
		g->key.s = false;
	}
	if (keycode == 2) { // D key
		g->key.d = false;
	}
	if (keycode == 13) { // W key
		g->key.w = false;
	}
	return (0);
}