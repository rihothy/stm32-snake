#include "button.h"
#include "graphics.h"

#include <fsmc_tft.h>

extern FSMC_TFT tft;

Button::Button(int x, int y, const uint16_t* img, const std::string& text) :
	x(x), y(y), img(img), text(text)
{
	h = img[0];
	w = img[1];
}

void Button::show(void)
{
	imshow(x, y, img);
	
	int tx = x + (w - 8 * text.length()) / 2;
	int ty = y + (h - 16) / 2;
	
	tft.print(tx, ty, 16, text.c_str());
}

bool Button::clicked(const std::tuple<bool, int, int>& touch_mess)
{
	bool touched;
	int x, y;
	
	std::tie(touched, x, y) = touch_mess;
	
	if (Button::touched)
	{
		if (not touched)
		{
			Button::touched = false;
			return true;
		}
		else if (!(x > Button::x && x < Button::x + w &&
			y > Button::y && y < Button::y + h))
		{
			Button::touched = false;
		}
	}
	else
	{
		if (touched && x > Button::x && x < Button::x + w &&
			y > Button::y && y < Button::y + h)
		{
			Button::touched = true;
		}
	}
	
	return false;
}