#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

#include "Common.h"
#include "Graphics.h"
#include "Utils.h"

static WindowData windowData = { 0, 0, 0 };
static int screenPixelsCount = 0;
static int screenWidthBytes = 0;
static int screenHeightBytes = 0;
constexpr int pixelFormatSize = 4; // bytes

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;

static Color* colorBuffer = nullptr;
static SDL_Texture* colorBufferTexture = nullptr;

constexpr bool CHECK_SDL(auto value)
{
	if (!ShouldQuit() && value != 0) [[unlikely]]
		{
			ERROR("SDL: %s", SDL_GetError());
			assert(false);
			return false;
		}
		return true;
}

constexpr bool CHECK_SDL_PTR(auto* ptr)
{
	if (!ShouldQuit() && ptr == nullptr) [[unlikely]]
		{
			ERROR("SDL: %s", SDL_GetError());
			assert(false);
			return false;
		}
		return true;
}

const WindowData& GetWindowData(void)
{
	return windowData;
}

void DeinitGraphics(void)
{
	FREE(colorBuffer);
	SDL_DestroyTexture(colorBufferTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void InitGraphics(const char* windowTitle)
{
	CHECK_SDL(SDL_Init(SDL_INIT_VIDEO));
	CHECK_SDL_PTR(window = SDL_CreateWindow(windowTitle, 0, 0, SDL_WINDOW_FULLSCREEN));
	CHECK_SDL(SDL_GetWindowSizeInPixels(window, &windowData.width, &windowData.height));
	screenPixelsCount = windowData.width * windowData.height;
	screenWidthBytes = windowData.width * sizeof(uint32_t);
	screenHeightBytes = windowData.height * sizeof(uint32_t);

	CHECK_SDL_PTR(renderer = SDL_CreateRenderer(window, nullptr, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));

	colorBuffer = ALLOC<Color>(screenPixelsCount);
	CHECK_SDL_PTR(colorBufferTexture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		windowData.width,
		windowData.height
	));
}

static void ClearColorBuffer(Color* buffer, size_t size, Color color)
{
	CHECK_SDL_PTR(SDL_memset4(buffer, (uint32_t)color, size));
}

void DrawRect(int x, int y, int width, int height, Color color)
{
	for (int i = 0; i < height; i++)
	{
		const int currentY = y + i;
		const int currentRow = windowData.width * currentY;
		for (int j = 0; j < width; j++)
		{
			const int currentX = x + j;
			colorBuffer[currentRow + currentX] = color;
		}
	}
}

void DrawGrid(int x, int y, int width, int height, Color color, int spacing)
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

void BeginDraw(void)
{
	CHECK_SDL(SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF));
	CHECK_SDL(SDL_RenderClear(renderer));
}

void EndDraw(void)
{
	CHECK_SDL(SDL_UpdateTexture(colorBufferTexture, nullptr, colorBuffer, screenWidthBytes));
	CHECK_SDL(SDL_RenderTexture(renderer, colorBufferTexture, nullptr, nullptr));
	CHECK_SDL(SDL_RenderPresent(renderer));
}