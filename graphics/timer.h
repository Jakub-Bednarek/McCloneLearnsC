#ifndef TIMER_H
#define TIMER_H

#include <time.h>
#include <stdio.h>

#define MILISECONDS_IN_SEC 1000
#define MICROSECONDS_IN_SEC 1000000
#define TICS_PER_SECOND 30
#define FPS_STRING_PREFIX "FPS: "
#define FPS_DIGITS_WIDTH 3

typedef struct {
    clock_t last_timepoint;
    int delta_in_micro;
    double delta_in_sec;
    double fps;
} timer;

void initialize_timer(timer* timer)
{
    timer->last_timepoint = clock();
    timer->delta_in_micro = 0;
    timer->delta_in_sec = 0;
    timer->fps = 0;
}

void mark_new_timestep(timer* timer)
{
    timer->last_timepoint = clock();
    timer->delta_in_micro = 0;
    timer->delta_in_sec = 0.0;
}

void update_delta_time(timer* timer)
{
    clock_t now = clock();
    double delta = (double)(now - timer->last_timepoint) / CLOCKS_PER_SEC;
    timer->last_timepoint = now;
    timer->delta_in_micro = (int)(delta * MICROSECONDS_IN_SEC);
    timer->delta_in_sec = delta;
}

void update_timer(timer* timer)
{
    update_delta_time(timer);
    timer->fps = 1.0 / timer->delta_in_sec;
    printf("delta: %f, fps: %f\n", timer->delta_in_sec, timer->fps);
}

#endif // TIMER_H