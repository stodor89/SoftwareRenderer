#include <stdlib.h>

#include <SDL3/SDL.h>

#include "Main.h"
#include "Macros.h"

// Not calling this function upon exit may leave the system in invalid state!
static void Deinit(void)
{
	SDL_DestroyTexture(colorBufferTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

static void Init(void)
{
	CHECK_SDL(SDL_Init(SDL_INIT_VIDEO));
	CHECK_SDL_PTR(window = SDL_CreateWindow(title, screenWidth, screenHeight, 0U));

	// Despite enabling GPU acceleration, it's still a software
	// renderer, because we create the entire pipeline using
	// CPU instructions.
	CHECK_SDL_PTR(renderer = SDL_CreateRenderer(window, nullptr, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
}

static void Setup(void)
{
	// No need to free this for now.
	colorBuffer = ALLOC<uint32_t>(screenWidth * screenHeight);

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

static void ClearColorBuffer(uint32_t color)
{
	for (int y = 0; y < screenHeight; y++)
	{
		int rowStart = y * screenWidth;
		for (int x = 0; x < screenWidth; x++)
		{
			colorBuffer[rowStart + x] = color;
		}
	}
}

static void RenderColorBuffer()
{
	CHECK_SDL(SDL_UpdateTexture(colorBufferTexture, nullptr, colorBuffer, screenWidthBytes));
	CHECK_SDL(SDL_RenderTexture(renderer, colorBufferTexture, nullptr, nullptr));
}

static void Draw(void)
{
	CHECK_SDL(SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF));
	CHECK_SDL(SDL_RenderClear(renderer));

	RenderColorBuffer();
	ClearColorBuffer(0xFFFF00FF);

	CHECK_SDL(SDL_RenderPresent(renderer));
}

int main(int argc, char** argv)
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
