#include "graphics/window.h"
#include "world/world.h"
#include "core/timer.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define TICKS_THRESHOLD 200

void tick(SimpleWindow* window, SimpleTimer* timer)
{
    static int total_delta = 0;

    total_delta += timer->delta_in_micro;
    while(total_delta > TICKS_THRESHOLD)
    {
        set_window_title_to_current_fps(window, timer->fps);
        total_delta -= TICKS_THRESHOLD;
    }
}

int main()
{
    SimpleTimer simple_timer = simple_timer_create();
    SimpleWindow* simple_window = window_create(1920, 1080);

    if(simple_window->error_code != MC_ERROR_NONE) {
        printf("SimpleWindow creation failure with errorCode: %d\n", simple_window->error_code);
        window_destroy(simple_window);
        exit(1);
    }

    Shader shader = shader_create();
    unsigned int vbo;
    unsigned int vao;
    buffers_create(&vbo, &vao);

    while(true) {
        simple_timer_update(&simple_timer);
        if(!window_dispatch_events(simple_window)) {
            break;
        }
        tick(simple_window, &simple_timer);
        render(shader, vao);
        window_swap_buffers(simple_window);
    }

    gl_clean_up(shader, &vbo, &vao);
    window_destroy(simple_window);

    return 0;
}