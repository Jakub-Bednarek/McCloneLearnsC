#ifndef WINDOW_H
#define WINDOW_H

#include <X11/Xlib.h>

#define WINDOW_NAME "McCloneLearnsC v0.1"

struct SimpleWindow {
    Display* display;
    Window window;
    XEvent lastEvent;
};

void create_window();
void destroy_window();
void pull_events();

extern struct SimpleWindow* globalWindow;

#endif // WINDOW_H