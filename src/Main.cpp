#include <stdlib.h>

#include "Common.h"
#include "Graphics.h"
#include "Input.h"
#include "Utils.h"

static bool quit = false;

bool ShouldQuit(void)
{
	return quit;
}

void Quit(void)
{
	quit = true;
}

static void Update(void)
{
}

static void Draw(void)
{
	const WindowData& window = GetWindowData();

	BeginDraw();
	DrawRect(0, 0, window.width, window.height, Color::black);
	DrawGrid(0, 0, window.width, window.height, Color::white, 10);
	DrawRect(100, 100, 100, 100, Color::red);
	DrawPixel(150, 150, Color::cyan);
	DrawPixel(151, 151, Color::cyan);
	DrawPixel(152, 152, Color::cyan);
	EndDraw();
}

static void Init(void)
{
	InitGraphics("SoftwareRenderer");
}

static void Deinit(void)
{
	DeinitGraphics();
}

int main(void)
{
	// atexit won't work properly if the renderer goes into a DLL.
	atexit(Deinit);

	Init();

	while (!ShouldQuit())
	{
		HandleInput();
		Update();
		Draw();
	}

	return 0;
}
