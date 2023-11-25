#pragma once

static bool quit = false;

static inline bool ShouldQuit()
{
	return quit;
}

static inline void Quit()
{
	quit = true;
}