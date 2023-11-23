#include <stdlib.h>

#include <SDL3/SDL.h>

#include "Macros.h"

constexpr int screenWidth = 1024;
constexpr int screenHeight = 768;
const char* title = "SoftwareRenderer";
static SDL_Window* window;
static SDL_Renderer* renderer;
static bool quit = false;

static void Deinit(void)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

static void Init(void)
{
	SDL_CHECK(SDL_Init(SDL_INIT_VIDEO));
	SDL_PTR_CHECK(window = SDL_CreateWindow(title, screenWidth, screenHeight, 0U));

	// Despite enabling GPU acceleration, it's still a software
	// renderer, because we create the entire pipeline using
	// CPU instructions.
	SDL_PTR_CHECK(renderer = SDL_CreateRenderer(window, nullptr, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
}

static void Setup(void)
{
}

static inline void Quit()
{
	quit = true;
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
	SDL_CHECK(SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0xFF, 0xFF));
	SDL_CHECK(SDL_RenderClear(renderer));
	SDL_CHECK(SDL_RenderPresent(renderer));
}

int main(int argc, char** argv)
{
	atexit(Deinit);
	Init();


	while (!quit)
	{
		HandleInput();
		Update();
		Draw();
	}

	return 0;
}
