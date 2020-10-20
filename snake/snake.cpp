#include "snake.h"
#include "images.h"
#include "graphics.h"
#include "listener.h"

#include <XPT2046_Touchscreen.h>
#include <fsmc_tft.h>
#include <tuple>

extern FSMC_TFT tft;
extern XPT2046_Touchscreen ts;

void Snake::play(void)
{
	while (true)
	{
		show_menu();
		
		while (true)
		{
			auto touch_mess = Listener.listen(ts);
			
			if (exit_btn.clicked(touch_mess))
			{
				exit();
				break;
			}
			else if (begin_btn.clicked(touch_mess))
			{
				begin();
				break;
			}
			else if (setting_btn.clicked(touch_mess))
			{
				setting();
				break;
			}
		}
	}
}

void Snake::show_menu(void)
{
	tft_clear(logo_img[2]);
	
	imshow(0, 100, "x center", logo_img);
	
	exit_btn.show();
	begin_btn.show();
	setting_btn.show();
}

void Snake::setting(void)
{
	tft_clear(logo_img[2]);
	
	imshow(0, 100, "x center", logo_img);
	
	tft.print(112, 200, 16, "Control Mode");
	tft.print(140, 230, 16, "Rocker");
	tft.print(140, 250, 16, "Dirkey");
	
	tft.print(112, 300, 16, "Movement speed");
	tft.print(140, 330, 16, "High");
	tft.print(140, 350, 16, "Mid");
	tft.print(140, 370, 16, "Low");
	
	while (true)
	{
		if (control_mode == "rocker")
		{
			tft.setColor(0, GREEN);
			LCD_Draw_Circle(120, 238, 5);
			tft.setColor(0, logo_img[2]);
			LCD_Draw_Circle(120, 258, 5);
		}
		else if (control_mode == "dirkey")
		{
			tft.setColor(0, GREEN);
			LCD_Draw_Circle(120, 258, 5);
			tft.setColor(0, logo_img[2]);
			LCD_Draw_Circle(120, 238, 5);
		}
		
		for (int i = 0; i < 3; ++i)
		{
			if (speed == i)
			{
				tft.setColor(0, GREEN);
				LCD_Draw_Circle(120, 338 + 20 * i, 5);
			}
			else
			{
				tft.setColor(0, logo_img[2]);
				LCD_Draw_Circle(120, 338 + 20 * i, 5);
			}
		}
		
		tft.setColor(0, BLACK);
		
		bool touched;
		int x, y;
		
		std::tie(touched, x, y) = Listener.listen(ts);
		
		if (touched)
		{
			if (x > 110 && x < 188)
			{
				if (y > 230 && y < 246)
				{
					control_mode = "rocker";
				}
				else if (y > 250 && y < 266)
				{
					control_mode = "dirkey";
				}
				else if (y > 330 && y < 346)
				{
					speed = 0;
				}
				else if (y > 350 && y < 366)
				{
					speed = 1;
				}
				else if (y > 380 && y < 396)
				{
					speed = 2;
				}
			}
			
			if (x < 90 || x > 230 || y < 200 || y > 400)
			{
				return;
			}
		}
	}
}

void Snake::exit(void)
{
	LCD_DisplayOff();
	digitalWrite(17, LOW);
	while (true);
}