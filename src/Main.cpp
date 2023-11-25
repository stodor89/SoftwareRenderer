#include <stdlib.h>

#include <SDL3/SDL.h>

#include "Common.h"
#include "Utils.h"

const char* title = "SoftwareRenderer";

constexpr int pixelFormatSize = 4; // bytes

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;

static int screenWidth = 0;
static int screenHeight = 0;
static int screenPixelsCount = 0;
static int screenWidthBytes = 0;
static int screenHeightBytes = 0;

static bool quit = false;

enum class ArgbColor : uint32_t
{
	black = 0xFF000000,
	white = 0xFFFFFFFF,
	red = 0xFFFF0000
};

using Color = ArgbColor;

static Color* colorBuffer = nullptr;
static SDL_Texture* colorBufferTexture = nullptr;

constexpr uint32_t operator>>(Color c, uint32_t offset)
{
	return (uint32_t)c >> offset;
}

SDL_Color ToSdlColor(Color color)
{
	constexpr uint32_t alphaOffset = 24;
	constexpr uint32_t redOffset = 16;
	constexpr uint32_t greenOffset = 8;
	constexpr uint32_t blueOffset = 0;
	return SDL_Color
	{
		.r = (Uint8)((color >> redOffset) & 0xFF),
		.g = (Uint8)((color >> greenOffset) & 0xFF),
		.b = (Uint8)((color >> blueOffset) & 0xFF),
		.a = (Uint8)((color >> alphaOffset) & 0xFF)
	};
}

static inline bool ShouldQuit()
{
	return quit;
}

static inline void Quit()
{
	quit = true;
}

// Not calling this function upon exit may leave the system in invalid state!
static void Deinit(void)
{
	FREE(colorBuffer);
	SDL_DestroyTexture(colorBufferTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

static void Init(void)
{
	CHECK_SDL(SDL_Init(SDL_INIT_VIDEO));
	CHECK_SDL_PTR(window = SDL_CreateWindow(title, 0, 0, SDL_WINDOW_FULLSCREEN));
	CHECK_SDL(SDL_GetWindowSizeInPixels(window, &screenWidth, &screenHeight));
	screenPixelsCount = screenWidth * screenHeight;
	screenWidthBytes = screenWidth * sizeof(uint32_t);
	screenHeightBytes = screenWidth * sizeof(uint32_t);

	CHECK_SDL_PTR(renderer = SDL_CreateRenderer(window, nullptr, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
}

static void ClearColorBuffer(Color* buffer, size_t size, Color color)
{
	CHECK_SDL_PTR(SDL_memset4(buffer, (uint32_t)color, size));
}

static void Setup(void)
{
	colorBuffer = ALLOC<Color>(screenPixelsCount);
	CHECK_SDL_PTR(colorBufferTexture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		screenWidth,
		screenHeight
	));
}

static void OnKeyDown(SDL_Keycode key)
{
	switch (key)
	{
		case SDLK_ESCAPE:
			Quit();
			break;
	}
}

static void HandleInput(void)
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) 
	{
		switch (event.type) 
		{
			case SDL_EVENT_KEY_DOWN:
				OnKeyDown(event.key.keysym.sym);
				break;
			case SDL_EVENT_QUIT:
				Quit();
				break;
		}
	}
}

static void Update(void)
{
}

static void DrawRect(int x, int y, int width, int height, Color color)
{
	for (int i = 0; i < height; i++)
	{
		const int currentY = y + i;
		const int currentRow = screenWidth * currentY;
		for (int j = 0; j < width; j++)
		{
			const int currentX = x + j;
			colorBuffer[currentRow + currentX] = color;
		}
	}
}

static void DrawGrid(int x, int y, int width, int height, Color color, int spacing)
{
	constexpr int thickness = 1;
	for (int i = 0; i < height; i += spacing)
	{
		DrawRect(0, y + i, width, thickness, color);
	}
	for (int i = 0; i < width; i += spacing)
	{
		DrawRect(x + i, 0, thickness, height, color);
	}
}

static void Draw(void)
{
	// Begin draw
	CHECK_SDL(SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF));
	CHECK_SDL(SDL_RenderClear(renderer));

	// Actual draw
	DrawRect(0, 0, screenWidth, screenHeight, Color::black);
	DrawGrid(0, 0, screenWidth, screenHeight, Color::white, 10);
	DrawRect(100, 100, 100, 100, Color::red);

	// End draw
	CHECK_SDL(SDL_UpdateTexture(colorBufferTexture, nullptr, colorBuffer, screenWidthBytes));
	CHECK_SDL(SDL_RenderTexture(renderer, colorBufferTexture, nullptr, nullptr));
	CHECK_SDL(SDL_RenderPresent(renderer));
}

int main(void)
{
	// atexit won't work properly if the renderer goes into a DLL.
	atexit(Deinit);

	Init();
	Setup();

	while (!ShouldQuit())
	{
		HandleInput();
		Update();
		Draw();
	}

	return 0;
}
