#include "graphics.h"

#include <fsmc_tft.h>
#include <cstring>
#include <cmath>

extern FSMC_TFT tft;

void imshow(int x, int y, const uint16_t img[])
{
	size_t h = img[0];
	size_t w = img[1];
	
	LCD_Set_Window(x, y, w, h);
	LCD_WriteRAM_Prepare();
	
	for (size_t i = 0; i < h * w; ++i)
	{
		LCD_TYPE->LCD_RAM = img[i + 2];
	}
	
	LCD_Set_Window(0, 0, 320, 480);
	LCD_WriteRAM_Prepare();
}

void imshow(const std::string& mode, const uint16_t img[])
{
	if (mode == "center")
	{
		int y = 480 / 2 - img[0] / 2;
		imshow(0, y, "x center", img);
	}
}

void imshow(int x, int y, const std::string& mode, const uint16_t img[])
{
	if (mode == "x center")
	{
		x = 320 / 2 - img[1] / 2;
	}
	else if (mode == "y center")
	{
		y = 480 / 2 - img[0] / 2;
	}
	
	imshow(x, y, img);
}

void tft_clear(const uint16_t color)
{
	LCD_Set_Window(0, 0, 320, 480);
	LCD_WriteRAM_Prepare();
	
	for (size_t i = 0; i < 320 * 480; ++i)
	{
		LCD_TYPE->LCD_RAM = color;
	}
	
	LCD_Set_Window(0, 0, 320, 480);
	LCD_WriteRAM_Prepare();
}

//！！！为了对本项目最大优化，把一些数据写死！！！//
void rotate(const uint8_t src[], uint8_t dst[], float th)
{
	//uint8_t h = src[0];
	//uint8_t w = src[1];
	th += 3.1416;
	
	float cos_th = cos(th);
	float sin_th = sin(th);
	
	memset(dst, 0, 25 * 25 + 2);
	
	//dst[0] = h;
	//dst[1] = w;
	
	for (int i = 0; i < 25; ++i)
	{
		for (int j = 0; j < 25; ++j)
		{
			int t_h = sin_th * (i - 12.5f) + cos_th * (j - 12.5f) + 12.5f;
			int t_w = cos_th * (i - 12.5f) - sin_th * (j - 12.5f) + 12.5f;
			
			if (t_h >= 0 && t_h < 25 && t_w >= 0 && t_w < 25)
			{
				dst[i * 25 + j + 2] = src[t_h * 25 + t_w + 2];
			}
		}
	}
}