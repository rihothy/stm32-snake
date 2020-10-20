#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <cstdint>
#include <string>

void imshow(int x, int y, const uint16_t img[]);
void imshow(const std::string& mode, const uint16_t img[]);
void imshow(int x, int y, const std::string& mode, const uint16_t img[]);
void tft_clear(const uint16_t color);
void rotate(const uint8_t src[], uint8_t dst[], float th);

#endif