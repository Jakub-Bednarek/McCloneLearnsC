#ifndef WINDOW_H
#define WINDOW_H

#include <X11/Xlib.h>

struct SimpleWindow {
    Display* display;
    Window window;
    XEvent lastEvent;
    char* name; 
};

void create_window();
void destroy_window();
void pull_events();

extern struct SimpleWindow* globalWindow;

#endif // WINDOW_H