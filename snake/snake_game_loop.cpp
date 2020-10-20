#include "snake.h"
#include "graphics.h"
#include "listener.h"
#include <XPT2046_Touchscreen.h>
#include <fsmc_tft.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <tuple>
#include <cmath>
extern FSMC_TFT tft;
extern XPT2046_Touchscreen ts;
const uint16_t palette[] = { 0xfef5, 0xc618, 0x6b4d, 0xef5d, 0x0000 };
auto&& get_dis = [](auto& a, auto& b)
{
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
};
auto&& generate_food = [](auto& body)
{
	srand(ts.getPoint().x);
	Node food;
	while (true)
	{
		food.x = rand() % 250 + 15;
		food.y = rand() % 350 + 15;
		
		bool legal = true;
		
		for (const auto& node : body)
		{
			if (food.x == node.x && food.y == node.y ||
				get_dis(food, node) < 20)
			{
				legal = false;
				break;
			}
		}
		if (legal)
		{
			return food;
		}
	}
};
auto&& update = [](auto& body, auto& food, auto dir)
{
	static uint8_t buff[380][280];
	static uint8_t rotate_img[25 * 25];
	memset(buff, 0, 380 * 280);
	
	auto&& draw_node = [](auto& node)
	{
		int x = node.x - 10;
		int y = node.y - 10;
		
		for (int ty = y, iy = 0; ty < y + 20; ++ty, ++iy)
		{
			for (int tx = x, ix = 0; tx < x + 20; ++tx, ++ix)
			{
				if (node_img[iy * 20 + ix + 2])
				{
					buff[ty][tx] = node_img[iy * 20 + ix + 2];
				}
			}
		}
	};
	
	auto&& draw_head = [&](auto& head)
	{
		int x = head.x - 12;
		int y = head.y - 12;
		
		rotate(head_img, rotate_img, dir);
		
		for (int ty = y, iy = 0; ty < y + 25; ++ty, ++iy)
		{
			for (int tx = x, ix = 0; tx < x + 25; ++tx, ++ix)
			{
				if (rotate_img[iy * 25 + ix + 2])
				{
					buff[ty][tx] = rotate_img[iy * 25 + ix + 2];
				}
			}
		}
	};
	
	draw_node(food);
	for (int i = body.size() - 1; i > 0; --i)
	{
		draw_node(body[i]);
	}
	draw_head(body[0]);
	LCD_Set_Window(20, 80, 280, 380);
	LCD_WriteRAM_Prepare();
	
	for (int i = 0; i < 380; ++i)
	{
		for (int j = 0; j < 280; ++j)
		{
			LCD_TYPE->LCD_RAM = palette[buff[i][j]];
		}
	}
	
	LCD_Set_Window(0, 0, 320, 480);
	LCD_WriteRAM_Prepare();
};
auto&& change_dir = [](auto& dir, auto& mode, auto& body)
{
	static float rx = 0, ry = 0;
	static bool clicked = false;
	static bool click_out = false;
	
	bool touched;
	int x, y;
	
	std::tie(touched, x, y) = Listener.listen(ts);
	
	if (not touched)
	{
		if (click_out)
		{
			click_out = false;
			return false;
		}
		
		clicked = false;
	}
	else
	{
		if (x < 20 || x > 300 || y < 80 || y > 460)
		{
			click_out = true;
		}
		else
		{
			click_out = false;
		}
		
		if (not clicked)
		{
			clicked = true;
			rx = x;
			ry = y;
		}
		else
		{
			if (mode == "dirkey")
			{
				rx = body[0].x + 20;
				ry = body[0].y + 80;
			}
			
			dir = atan2(y - ry, x - rx);
		}
	}
	
	return true;
};
auto&& out = [](auto& body)
{
	return body[0].x < 12 || body[0].x > 268 || body[0].y < 12 || body[0].y > 368;
};
auto&& move = [](auto& body, auto& dir, auto speed)
{
	static unsigned long long tic = millis();
	
	if (millis() - tic > 500)
	{
		tic = millis();
		return;
	}
	else
	{
		auto t = (millis() - tic) / 1000.0;
		
		speed = 4 - speed;
		speed *= 32 * t;
		
		body[0].x += cos(dir) * speed;
		body[0].y += sin(dir) * speed;
		
		for (int i = 1; i < body.size(); ++i)
		{
			body[i].x -= (body[i].x - body[i - 1].x) / 15 * speed;
			body[i].y -= (body[i].y - body[i - 1].y) / 15 * speed;
		}
		
		tic = millis();
	}
};
auto&& ate = [](auto& body, auto& food)
{
	return sqrt(pow(body[0].x - food.x, 2) + pow(body[0].y - food.y, 2)) < 22;
};
void Snake::begin(void)
{
	tft_clear(logo_img[2]);
	tft.rectangle(19, 79, 301, 461);
	tft.print(20, 20, 16, "Score 1");
	
	std::vector<Node> body{ {140, 190} };
	auto food = generate_food(body);
	auto dir = 0.0f;
	
	while (change_dir(dir, control_mode, body) && not out(body))
	{
		update(body, food, dir);
		
		move(body, dir, speed);
	
		if (ate(body, food))
		{
			body.push_back({ body.back().x + cos(3.1416f - dir) * 15, body.back().y + sin(3.1416f - dir) * 15 });
			food = generate_food(body);
			tft.fill(68, 20, 116, 36, logo_img[2]);
			tft.print(68, 20, 16, std::to_string(body.size()).c_str());
		}
	}
}