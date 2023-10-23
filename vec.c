#include "vec.h"
#include "math.h"

Vec2 add(Vec2 a, Vec2 b)
{
    return (Vec2) {a.x + b.x, a.y + b.y};
}

Vec2 sub(Vec2 a, Vec2 b)
{
    return (Vec2) {a.x - b.x, a.y - b.y};
}

Vec2 mult(Vec2 v, float s)
{
    return (Vec2) {s * v.x, s * v.y};
}

Vec2 div(Vec2 v, float s)
{
    return (Vec2) {v.x / s, v.y / s};
}

float length(Vec2 v)
{
    return sqrt(v.x * v.x + v.y * v.y);
}
