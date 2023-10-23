#ifndef VEC_H
#define VEC_H

typedef struct {
    float x;
    float y;
} Vec2;

Vec2 add(Vec2 a, Vec2 b);
Vec2 sub(Vec2 a, Vec2 b);
Vec2 mult(Vec2 v, float s);
Vec2 div(Vec2 v, float s);
float length(Vec2 v);

#endif
