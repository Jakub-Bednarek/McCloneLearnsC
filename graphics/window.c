#include "window.h"

#include <stdio.h>
#include <stdlib.h>
#include <X11/Xutil.h>

#define WINDOW_EVENT_MASK KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask
#define DEFAULT_X_POS 0
#define DEFAULT_Y_POS 0
#define DEFAULT_WIDTH 1280
#define DEFAULT_HEIGHT 720

struct simple_window* g_window = NULL;

void dummy_on_key_pressed(XKeyEvent*) {}
void dummy_on_key_released(XKeyEvent*) {}
void dummy_on_button_pressed(XButtonEvent*) {}
void dummy_on_button_released(XButtonEvent*) {}
void dummy_on_mouse_motion(XMotionEvent*) {}
void dummy_on_window_resize(XConfigureEvent*) {}

void set_on_key_press_callback(void (*callback)(XKeyEvent*))
{
    g_window->on_key_pressed = callback;
}

void set_on_key_released_callback(void (*callback)(XKeyEvent*))
{
    g_window->on_key_released = callback;
}
void set_on_button_pressed_callback(void (*callback)(XButtonEvent*))
{
    g_window->on_button_pressed = callback;
}

void set_on_button_released_callback(void (*callback)(XButtonEvent*))
{
    g_window->on_button_pressed = callback;
}

void set_on_mouse_motion_callback(void (*callback)(XMotionEvent*))
{
    g_window->on_mouse_moved = callback;
}

void set_on_window_resize_callback(void (*callback)(XConfigureEvent*))
{
    g_window->on_window_resized = callback;
}

void create_window()
{
    if(g_window != NULL)
    {
        fprintf(stderr, "ERROR: Window created more than once!\n");
        exit(1);
    }

    g_window = (struct simple_window*) malloc(sizeof(struct simple_window));
    Display* display = XOpenDisplay(NULL);
    if(display == NULL)
    {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }

    int defaultScreen = DefaultScreen(display);
    Window window = XCreateSimpleWindow(display, RootWindow(display, defaultScreen), DEFAULT_X_POS, DEFAULT_Y_POS, DEFAULT_WIDTH, DEFAULT_HEIGHT, 1, BlackPixel(display, defaultScreen), WhitePixel(display, defaultScreen));
    XSelectInput(display, window, WINDOW_EVENT_MASK);
    XMapWindow(display, window);

    g_window->display = display;
    g_window->window = window;
    g_window->on_key_pressed = dummy_on_key_pressed;
    g_window->on_key_released = dummy_on_key_released;
    g_window->on_button_pressed = dummy_on_button_pressed;
    g_window->on_button_released = dummy_on_button_released;
    g_window->on_mouse_moved = dummy_on_mouse_motion;
    g_window->on_window_resized = dummy_on_window_resize;
}

void destroy_window()
{
    if(g_window == NULL)
    {
        fprintf(stderr, "ERROR: Called destroy_window() before create_window()!\n");
        exit(1);
    }

    printf("Destroying window with name: %s\n", WINDOW_NAME);
    free(g_window);
}

void handle_window_resize_event(XConfigureEvent* event)
{
    if(g_window->width != event->width || g_window->height != event->height)
    {
        g_window->width = event->width;
        g_window->height = event->height;
        g_window->on_window_resized(event);
    }
}

void dispatch_events()
{    
    XFlush(g_window->display);
    int events_pending = XEventsQueued(g_window->display, QueuedAlready);
    XEvent lastEvent;
    while(events_pending > 0)
    {
       XNextEvent(g_window->display, &lastEvent);
       switch(lastEvent.type)
       {
            case KeyPress:
                g_window->on_key_pressed(&lastEvent.xkey);
                break;
            case KeyRelease:
                g_window->on_key_released(&lastEvent.xkey);
                break;
            case ButtonPress:
                g_window->on_button_pressed(&lastEvent.xbutton);
                break;
            case ButtonRelease:
                g_window->on_button_released(&lastEvent.xbutton);
                break;
            case MotionNotify:
                g_window->on_mouse_moved(&lastEvent.xmotion);
                break;
            case ConfigureNotify:
                handle_window_resize_event(&lastEvent.xconfigure);
                break;
            default:
                break;
       }
       --events_pending;
    }
}

void run_loop()
{
    while(1)
    {
        dispatch_events();
    }
}