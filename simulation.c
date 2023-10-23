#include "simulation.h"
#include "verlet.h"
#include "vec.h"
#include <tiny-fw.h>

#define MAX_OBJECTS 100

static const Vec2 gravity = {0.0f, 100.0f};

static Verlet_obj objects[MAX_OBJECTS];
static int num_obj = 0;

static Vec2 constraint_center = {128.0f, 128.0f};
static float constraint_radius = 120.0f;

static void apply_gravity()
{
    for (int i = 0; i < num_obj; i++) {
        accelerate(&objects[i], gravity);
    }
}

static void update_positions(float dt)
{
    for (int i = 0; i < num_obj; i++) {
        update_position(&objects[i], dt);
    }
}

static void apply_constraint()
{
    for (int i = 0; i < num_obj; i++) {
        Vec2 to_obj = sub(constraint_center, objects[i].current_position);
        float dist = length(to_obj);
        if (dist > constraint_radius - 10.0f) {
            Vec2 n = div(to_obj, dist);
            n = mult(n, constraint_radius - 10.0f);
            objects[i].current_position = sub(constraint_center, n);
        }
    }
}

static void solve_collisions()
{
    for (int i = 0; i < num_obj - 1; i++) {
        Verlet_obj obj1 = objects[i];
        for (int j = i + 1; j < num_obj; j++) {
            Verlet_obj obj2 = objects[j];
            Vec2 axis = sub(obj2.current_position, obj1.current_position);
            float dist = length(axis);
            if (dist < 20.0f) {
                float overlap = 20.0f - dist;
                float correction_amt = overlap / 2;
                axis = div(axis, dist);
                axis = mult(axis, correction_amt);
                objects[i].current_position = sub(objects[i].current_position, axis);
                objects[j].current_position = add(objects[j].current_position, axis);
            }
        }
    }
}

static void draw_objects()
{
    for (int i = 0; i < num_obj; i++) {
        int x = objects[i].current_position.x;
        int y = objects[i].current_position.y;
        int radius = 10;
        int color = 0xffffff;
        draw_fill_circle(x, y, radius, color);
    }
}

static void draw_constraint()
{
    draw_fill_circle(constraint_center.x, constraint_center.y, constraint_radius, 0x000000);
}

void sim_init()
{
    num_obj = 0;
    sim_add_obj(230.0f, 120.0f);
}

void sim_update(float dt)
{
    float sub_dt = dt / SUB_STEPS;
    for (int i = 0; i < SUB_STEPS; i++) {
        apply_gravity();
        apply_constraint();
        solve_collisions();
        update_positions(sub_dt);
    }
}

void sim_render()
{
    draw_constraint();
    draw_objects();
}

void sim_add_obj(float x, float y)
{
    if (num_obj < MAX_OBJECTS) {
        objects[num_obj] = (Verlet_obj) {
            .current_position = {x, y},
            .previous_position = {x, y},
            .acceleration = {0.0f, 0.0f}
        };
        num_obj++;
    }
}
