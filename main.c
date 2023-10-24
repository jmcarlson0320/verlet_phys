#include <tiny-fw.h>
#include "simulation.h"

#define WIDTH 256
#define HEIGHT 256
#define SCALE 3
#define EXIT_KEY KEY_Q
#define BACKGROUND_COLOR 0x7f7f7f

enum simulation_state {
    START_SCREEN,
    RUNNING
};

App app;
int app_is_running = 0;
int state = START_SCREEN;

void init()
{
    app = app_create(WIDTH, HEIGHT, SCALE);
    app_start(&app);
    app_is_running = 1;
}

void update()
{
    app_update(&app);
    if (app.keyboard.pressed[EXIT_KEY]) {
        app_is_running = 0;
    }
    switch (state) {
        case START_SCREEN:
            if (app.keyboard.pressed[KEY_SPACE]) {
                sim_init();
                state = RUNNING;
            }
            break;
        case RUNNING:
            if (app.keyboard.pressed[KEY_SPACE]) {
                sim_add_obj(230.0f, 120.0f);
            }
            if (app.keyboard.pressed[KEY_C]) {
                sim_create_connection((Vec2){100, 100}, (Vec2){130, 130});
            }
            if (app.keyboard.pressed[KEY_R]) {
                state = START_SCREEN;
            }
            sim_update(app.time.dt_sec);
            break;
        default:
            break;
    }
}

void render()
{
    switch (state) {
        case START_SCREEN:
            draw_fill_rect(0, 0, WIDTH, HEIGHT, 0x000000);
            draw_text("press SPACE to start simulation", 0, 0, 0xffffff);
            break;
        case RUNNING:
            draw_fill_rect(0, 0, WIDTH, HEIGHT, BACKGROUND_COLOR);
            sim_render();
            break;
        default:
            break;
    }
    app_draw_graphics(&app);
}

void cleanup()
{
    app_quit(&app);
}

int main()
{
    init();
    while (app_is_running) {
        update();
        render();
    }
    cleanup();

    return 0;
}
