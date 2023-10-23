#ifndef VERLET_H
#define VERLET_H

#include "vec.h"

typedef struct {
    Vec2 current_position;
    Vec2 previous_position;
    Vec2 acceleration;
} Verlet_obj;

void accelerate(Verlet_obj *obj, Vec2 acc);
void update_position(Verlet_obj *obj, float dt);

#endif
