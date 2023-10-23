#include "verlet.h"
#include "vec.h"

void accelerate(Verlet_obj *obj, Vec2 acc)
{
    obj->acceleration = add(obj->acceleration, acc);
}

void update_position(Verlet_obj *obj, float dt)
{
    Vec2 velocity_times_dt = sub(obj->current_position, obj->previous_position);
    Vec2 acceleration_times_dt_squared = mult(obj->acceleration, dt * dt);
    Vec2 next_position = add(obj->current_position, velocity_times_dt);
    next_position = add(next_position, acceleration_times_dt_squared);
    obj->previous_position = obj->current_position;
    obj->current_position = next_position;
    obj->acceleration = (Vec2) {0.0f, 0.0f};
}
