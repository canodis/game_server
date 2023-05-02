#pragma once
#include <iostream>

class ChatBar {
public:
	void *img;
	int width;
	int heigth;
	bool visible;
	ChatBar() {
		visible = false;
	}
};
