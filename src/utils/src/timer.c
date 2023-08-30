#include "utils/timer.h"

SimpleTimer simple_timer_create(void)
{
    SimpleTimer timer = { .last_timepoint = clock(), .delta_in_micro = 0, .delta_in_sec = 0, .fps = 0 };
    return timer;
}

void simple_timer_new_mark(SimpleTimer* timer)
{
    timer->last_timepoint = clock();
    timer->delta_in_micro = 0;
    timer->delta_in_sec = 0.0;
}

void simple_timer_update_delta(SimpleTimer* timer)
{
    clock_t now = clock();
    double delta = (double)(now - timer->last_timepoint) / CLOCKS_PER_SEC;
    timer->last_timepoint = now;
    timer->delta_in_micro = (int)(delta * MICROSECONDS_IN_SEC);
    timer->delta_in_sec = delta;
}

void simple_timer_update(SimpleTimer* timer)
{
    simple_timer_update_delta(timer);
    timer->fps = 1.0 / timer->delta_in_sec;
}