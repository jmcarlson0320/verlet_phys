#include "simulation.h"
#include "verlet.h"
#include "vec.h"
#include <tiny-fw.h>

#define bool int
#define true 1
#define false 0

#define MAX_OBJECTS 100
#define MAX_CONNECTIONS 5

typedef struct {
    int obj1;
    int obj2;
    float length;
} Connection;

static const Vec2 gravity = {0.0f, 100.0f};

static Verlet_obj objects[MAX_OBJECTS];
static int num_obj = 0;

static Connection connections[MAX_CONNECTIONS];
static int num_connections = 0;

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

static void solve_connections()
{
    for (int i = 0; i < num_connections; i++) {
        int index1 = connections[i].obj1;
        int index2 = connections[i].obj2;
        Vec2 pos1 = objects[index1].current_position;
        Vec2 pos2 = objects[index2].current_position;
        Vec2 axis = sub(pos2, pos1);
        float dist = length(axis);
        float difference = connections[i].length - dist;
        float correction_amt = difference / 2;
        axis = div(axis, dist);
        axis = mult(axis, correction_amt);
        objects[index1].current_position = sub(objects[index1].current_position, axis);
        objects[index2].current_position = add(objects[index2].current_position, axis);
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

static void draw_connections()
{
    for (int i = 0; i < num_connections; i++) {
        int index1 = connections[i].obj1;
        int index2 = connections[i].obj2;
        Vec2 pos1 = objects[index1].current_position;
        Vec2 pos2 = objects[index2].current_position;
        draw_line(pos1.x, pos1.y, pos2.x, pos2.y, 0xff0000);
    }
}

void sim_init()
{
    num_obj = 0;
    num_connections = 0;
}

void sim_update(float dt)
{
    apply_gravity();
    for (int i = 0; i < SUB_STEPS; i++) {
        apply_constraint();
        solve_collisions();
        solve_connections();
    }
    update_positions(dt);
}

void sim_render()
{
    draw_constraint();
    draw_connections();
    draw_objects();
}

void sim_add_obj(float x, float y)
{
    if (num_obj >= MAX_OBJECTS) {
        return;
    }
    objects[num_obj] = (Verlet_obj) {
        .current_position = {x, y},
        .previous_position = {x, y},
        .acceleration = {0.0f, 0.0f}
    };
    num_obj++;
}

void sim_create_connection(Vec2 pos1, Vec2 pos2)
{
    if ((num_connections >= MAX_CONNECTIONS) || (MAX_OBJECTS - num_obj < 2)) {
        return;
    }
    int index_obj1 = num_obj;
    int index_obj2 = num_obj + 1;
    sim_add_obj(pos1.x, pos1.y);
    sim_add_obj(pos2.x, pos2.y);
    connections[num_connections].obj1 = index_obj1;
    connections[num_connections].obj2 = index_obj2;
    connections[num_connections].length = length(sub(pos2, pos1));
    num_connections++;
}
