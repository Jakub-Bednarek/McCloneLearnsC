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
} SimpleTimer;

extern SimpleTimer simple_timer_create(void);
extern void simple_timer_new_mark(SimpleTimer* timer);
extern void simple_timer_update_delta(SimpleTimer* timer);
extern void simple_timer_update(SimpleTimer* timer);

#endif // TIMER_H