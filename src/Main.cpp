#include <stdlib.h>

#include <SDL3/SDL.h>

#include "Main.h"
#include "Macros.h"

// TODO: Organize these when they become stable!

const char* title = "SoftwareRenderer";

enum class Color : uint32_t
{
	black = 0xFF000000,
	white = 0xFFFFFFFF
};

constexpr int pixelFormatSize = 4; // bytes

static SDL_Window* window;
static SDL_Renderer* renderer;

static Color* colorBuffer = nullptr;
static SDL_Texture* background = nullptr;

static Color* gridBuffer = nullptr;
static SDL_Texture* grid = nullptr;

int screenWidth = 0;
int screenHeight = 0;
int screenPixelsCount = 0;
int screenWidthBytes = 0;
int screenHeightBytes = 0;

// Not calling this function upon exit may leave the system in invalid state!
static void Deinit(void)
{
	SDL_DestroyTexture(grid);
	SDL_DestroyTexture(background);
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

	// Despite enabling GPU acceleration, it's still a software
	// renderer, because we create the entire pipeline using
	// CPU instructions.
	CHECK_SDL_PTR(renderer = SDL_CreateRenderer(window, nullptr, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
}

//static void ClearColorBuffer(Color* buffer, size_t size, Color color)
//{
//	CHECK_SDL_PTR(SDL_memset4(buffer, (uint32_t)color, size));
//}

static void PaintColorBufferPixelByPixel(Color* buffer, int w, int h, auto paintFunc)
{
	for (int y = 0; y < h; y++)
	{
		int rowStart = y * w;
		for (int x = 0; x < w; x++)
		{
			buffer[rowStart + x] = paintFunc(x, y);
		}
	}
}

// TODO: Tests for "custom" stuff!

// Don't call this at runtime! Gotta figure out something faster :)
static SDL_Texture* SetupColoredTexture(int width, int height, auto paintFunc)
{
	Color* buffer = ALLOC<Color>(width * height);
	PaintColorBufferPixelByPixel(buffer, width, height, paintFunc);
	SDL_Texture* result;
	CHECK_SDL_PTR(result = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		width,
		height
	));
	CHECK_SDL(SDL_UpdateTexture(result, nullptr, buffer, width * pixelFormatSize));
	FREE(buffer);
	return result;
}

static void Setup(void)
{
	background = SetupColoredTexture(screenWidth, screenHeight,
		[](int x [[maybe_unused]], int y [[maybe_unused]] ) { return Color::black; });
	grid = SetupColoredTexture(screenWidth, screenHeight,
		[](int x, int y) { return (x % 10 == 0 || y % 10 == 0) ? Color::white : Color::black; });
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

static void Draw(void)
{
	CHECK_SDL(SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF));
	CHECK_SDL(SDL_RenderClear(renderer));

	CHECK_SDL(SDL_RenderTexture(renderer, background, nullptr, nullptr));
	CHECK_SDL(SDL_RenderTexture(renderer, grid, nullptr, nullptr));

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
