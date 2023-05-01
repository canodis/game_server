#include "Player.hpp"

Player::Player(int fd, int posx, int posy) {
	this->fd = fd;
	this->position.x = posx;
	this->position.y = posy;
	this->username = "canodis";
}

Player::Player() { }