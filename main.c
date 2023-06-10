#include "graphics/cube.h"
#include "graphics/window.h"
#include "core/timer.h"

#include <stdio.h>
#include <unistd.h>

#define TICKS_THRESHOLD 20000

void initialize_systems(Timer* timer)
{
    printf("Initializing system...\n");

    create_window();
    initialize_timer(timer);

    printf("Systems initialized!\n");
}

void clean_up()
{
    printf("Performing cleanup...\n");
    destroy_window();
    printf("Cleanup done!\n");
}

void tick(Timer* timer)
{
    static double total_delta = 0;
    total_delta += timer->delta_in_micro;
    while(total_delta > (TICKS_THRESHOLD))
    {
        set_window_title_to_current_fps(timer->fps);
        total_delta -= TICKS_THRESHOLD;
    }
}

void dummy_loop()
{
    int sum = 0;
    for(int i = 0; i < 10000000; i++)
    {
        if (i > 50000)
        {
            sum += i;
        }
        else if (i > 1000000)
        {
            sum -= i;
        }
    }
}

void run_game_loop(Timer* timer)
{
    printf("Starting game loop!\n");
    mark_new_timestep(timer);
    while(1) {
        dispatch_window_events();
        update_timer(timer);
        tick(timer);
        dummy_loop();
        draw();
        render();
    }
}

int main()
{
    Timer main_timer;
    
    initialize_systems(&main_timer);
    run_game_loop(&main_timer);
    clean_up();

    return 0;
}