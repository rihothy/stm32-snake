#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <cstdint>
#include <string>
#include <tuple>

class Button
{
	int x;
	int y;
	int w;
	int h;
	bool touched = false;
	const uint16_t* img;
	const std::string text;
	
public:
	
	Button(int x, int y, const uint16_t* img, const std::string& text="");
	
	void show(void);
	bool clicked(const std::tuple<bool, int, int>& touch_mess);
};

#endif