#ifndef SIMULATION_H
#define SIMULATION_H

#define SUB_STEPS 8

#include "vec.h"

void sim_init();
void sim_update(float dt);
void sim_render();
void sim_add_obj(float x, float y);
void sim_create_connection(Vec2 pos1, Vec2 pos2);

#endif
