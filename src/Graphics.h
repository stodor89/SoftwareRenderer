#pragma once

// Interface for platform-specific graphics stuff

#include <stdint.h>

enum class Color : uint32_t
{
	black = 0xFF000000,
	blue = 0xFF0000FF,
	green = 0xFF00FF00,
	cyan = blue | green,
	red = 0xFFFF0000,
	magenta = red | blue,
	yellow = red | green,
	white = 0xFFFFFFFF,
};

struct WindowData
{
	int width;
	int height;
	const char* title;
};

void InitGraphics(const char* windowTitle);
void DeinitGraphics(void);
const WindowData& GetWindowData(void);
void DrawPixel(int x, int y, Color color);
void DrawRect(int x, int y, int width, int height, Color color);
void DrawGrid(int x, int y, int width, int height, Color color, int spacing);
void BeginDraw(void);
void EndDraw(void);

void HandleInput(void);