#ifndef WINDOW_H
#define WINDOW_H

#define DISPLAY_FAILURE 1
#define DEFAULT_VISUAL_INFO_FAILURE 2
#define GLX_LOAD_FAILURE 3
#define FB_CONFIG_FAILURE 4
#define GLX_CONTEXT_FAILURE 5
#define GL_LOAD_FAILURE 6

#include "core/error.h"
#include "core/input.h"

#include <X11/Xlib.h>

#include <glad/gl.h>
#include <glad/glx.h>

#include <stdbool.h>

#define WINDOW_ADAPT_CALLBACK_STRUCT(func_name, event_type) \
typedef struct { \
    void (*callback)(event_type*, void*);\
    void* user_data_ptr; \
} event_type ## Callback; \

WINDOW_ADAPT_CALLBACK_STRUCT(on_key_pressed_callback, XKeyEvent)

typedef struct {
    Display* display;
    Window main_window;
    GLXContext context;
    Colormap colormap;
    const char* name;
    int x_pos;
    int y_pos;
    int width;
    int height;
    float mouse_x_center;
    float mouse_y_center;
    McError error_code;
    void (*on_key_pressed)(XKeyEvent*);
    void (*on_key_released)(XKeyEvent*);
    void (*on_button_pressed)(XButtonEvent*);
    void (*on_button_released)(XButtonEvent*);
    void (*on_mouse_moved)(MouseMoveEvent*);
    void (*on_window_resized)(XConfigureEvent*);
    XKeyEventCallback c;
} SimpleWindow;



void set_on_key_press_callback(SimpleWindow*, void (*callback)(XKeyEvent*));
void set_on_key_released_callback(SimpleWindow*, void (*callback)(XKeyEvent*));
void set_on_button_pressed_callback(SimpleWindow*, void (*callback)(XButtonEvent*));
void set_on_button_released_callback(SimpleWindow*, void (*callback)(XButtonEvent*));
void set_on_mouse_motion_callback(SimpleWindow*, void (*callback)(MouseMoveEvent*));
void set_on_window_resize_callback(SimpleWindow*, void (*callback)(XConfigureEvent*));

bool window_is_key_pressed(SimpleWindow*, KeySym);

SimpleWindow* window_create(unsigned int width, unsigned int height);
void window_destroy(SimpleWindow*);
void window_swap_buffers(SimpleWindow*);
bool window_dispatch_events(SimpleWindow*);
void update_window_title(SimpleWindow*, const char* title);
void set_window_title_to_current_fps(SimpleWindow*, double fps);

#endif // WINDOW_H