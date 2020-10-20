#ifndef _SNAKE_H_
#define _SNAKE_H_

#include "button.h"
#include "images.h"

#include <string>

struct Node
{
	float x;
	float y;
};

class Snake
{
	int speed = 1;
	std::string control_mode = "rocker";
	
	Button exit_btn = Button(60, 320, button_img, "exit");
	Button begin_btn = Button(60, 200, button_img, "begin");
	Button setting_btn = Button(60, 260, button_img, "setting");
	
public:
	
	void play(void);
	void show_menu(void);
	void setting(void);
	void begin(void);
	void exit(void);
};

#endif