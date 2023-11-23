#pragma once

#include <stdio.h>
#include <assert.h>

#define LOG_MESSAGE(format, ...) \
    do { \
        char message[1024] = {}; \
        snprintf(message, 1023, (format), ##__VA_ARGS__); \
        fprintf(stderr, "MESSAGE(%s:%d) => %s\n", __FILE__, __LINE__, message); \
    } while (false)

#define LOG_ERROR(format, ...) \
    do { \
        char message[1024] = {}; \
        snprintf(message, 1023, (format), ##__VA_ARGS__); \
        fprintf(stderr, "ERROR(%s:%d) => %s\n", __FILE__, __LINE__, message); \
    } while (false)

#define SDL_CHECK(value) \
	if (!quit && (value) != 0) [[unlikely]] \
	{ \
        assert(false); \
        LOG_ERROR("SDL: %s", SDL_GetError()); \
        quit = true; \
    }

#define SDL_PTR_CHECK(value) \
	if (!quit && (value) == nullptr) [[unlikely]] \
	{ \
        assert(false); \
        LOG_ERROR("SDL: %s", SDL_GetError()); \
        quit = true; \
    }