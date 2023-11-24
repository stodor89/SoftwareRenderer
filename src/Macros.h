#pragma once

#include <stdio.h>
#include <assert.h>

#define MESSAGE(format, ...) \
    do { \
        char message[1024] = {}; \
        snprintf(message, 1023, (format), ##__VA_ARGS__); \
        fprintf(stderr, "MESSAGE(%s:%d) => %s\n", __FILE__, __LINE__, message); \
    } while (false)

#define ERROR(format, ...) \
    do { \
        char message[1024] = {}; \
        snprintf(message, 1023, (format), ##__VA_ARGS__); \
        fprintf(stderr, "ERROR(%s:%d) => %s\n", __FILE__, __LINE__, message); \
        Quit(); \
    } while (false)

constexpr void CHECK_PTR(auto * ptr)
{
    if (!ShouldQuit() && ptr == nullptr) [[unlikely]]
        {
            ERROR("Null pointer!");
            assert(false);
        }
}

template <typename BufferType>
constexpr BufferType* ALLOC(int count)
{
    BufferType* result = static_cast<BufferType*>(malloc(sizeof(BufferType) * count));
    CHECK_PTR(result);
    return result;
}

constexpr void FREE(auto* ptr)
{
    CHECK_PTR(ptr);
    free(ptr);
}

constexpr void CHECK_NULLPTR(auto* ptr)
{
    if (!ShouldQuit() && ptr != nullptr) [[unlikely]]
        {
            ERROR("Value is not zero!");
            assert(false);
        }
}

constexpr void CHECK_SDL(auto value)
{
    if (!ShouldQuit() && value != 0) [[unlikely]]
    {
        ERROR("SDL: %s", SDL_GetError());
        assert(false);
    }
}

constexpr void CHECK_SDL_PTR(auto* ptr)
{
    if (!ShouldQuit() && ptr == nullptr) [[unlikely]]
    {
        ERROR("SDL: %s", SDL_GetError());
        assert(false);
    }
}
