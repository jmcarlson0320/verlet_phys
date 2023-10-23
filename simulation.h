#ifndef SIMULATION_H
#define SIMULATION_H

#define SUB_STEPS 8

void sim_init();
void sim_update(float dt);
void sim_render();
void sim_add_obj(float x, float y);

#endif
