#pragma once

#include <type_traits>

template <typename T>
struct Vec2
{
    T x, y;
};
using Vec2f = Vec2<float>;
static_assert(std::is_standard_layout_v<Vec2f> && std::is_trivially_copyable_v<Vec2f>);

template <typename T>
struct Vec3
{
    T x, y, z;
};
using Vec3f = Vec3<float>;
static_assert(std::is_standard_layout_v<Vec3f>&& std::is_trivially_copyable_v<Vec3f>);
