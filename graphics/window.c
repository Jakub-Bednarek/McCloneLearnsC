#include "window.h"
#include "input.h"

#include <stdio.h>
#include <stdlib.h>
#include <X11/Xutil.h>

#define WINDOW_EVENT_MASK KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask
#define DEFAULT_X_POS 0
#define DEFAULT_Y_POS 0
#define DEFAULT_WIDTH 1280
#define DEFAULT_HEIGHT 720
#define XLIB_SYM_INDEX 1
#define DEFAULT_WINDOW_NAME "McCloneLearnsC v0.1"

SimpleWindow* g_window = NULL;

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

    g_window = (SimpleWindow*) malloc(sizeof(SimpleWindow));
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
    update_window_title(DEFAULT_WINDOW_NAME);
}

void destroy_window()
{
    if(g_window == NULL)
    {
        fprintf(stderr, "ERROR: Called destroy_window() before create_window()!\n");
        exit(1);
    }

    printf("Destroying window with name: %s\n", g_window->name);
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

void print_key_info(int key_sym, KeyInfo* info)
{
    printf("Key pressed: %d, key_held: %d, state: %d\n", key_sym, info->key_held, info->state);
}

void process_key_event(XKeyEvent* key_event, key_state new_state)
{
    // KEY_HELD doesn't work atm
    KeySym key_sym = XLookupKeysym(key_event, XLIB_SYM_INDEX);
    if(g_input.keys_info[key_sym].state == PRESSED && new_state == PRESSED )
    {
        g_input.keys_info[key_sym].key_held = true;
    }
    else
    {
        g_input.keys_info[key_sym].key_held = false;
        g_input.keys_info[key_sym].state = new_state;
    }
    print_key_info(key_sym, &(g_input.keys_info[key_sym]));
}

void dispatch_window_events()
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
                process_key_event(&lastEvent.xkey, PRESSED);
                g_window->on_key_pressed(&lastEvent.xkey);
                break;
            case KeyRelease:
                process_key_event(&lastEvent.xkey, RELEASED);
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

void update_window_title(const char* title)
{
    if(g_window->display != NULL)
    {
        XStoreName(g_window->display, g_window->window, title);
        g_window->name = title;
    }
}

void set_window_title_to_current_fps(double fps)
{
    char fps_str[64];
    sprintf(fps_str, "%f", fps);
    update_window_title(fps_str);
}
