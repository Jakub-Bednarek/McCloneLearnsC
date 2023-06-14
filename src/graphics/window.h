#ifndef WINDOW_H
#define WINDOW_H

#define DISPLAY_FAILURE 1
#define DEFAULT_VISUAL_INFO_FAILURE 2
#define GLX_LOAD_FAILURE 3
#define FB_CONFIG_FAILURE 4
#define GLX_CONTEXT_FAILURE 5
#define GL_LOAD_FAILURE 6

#include "core/error.h"
#include "core/event.h"

#include <X11/Xlib.h>

#include <glad/gl.h>
#include <glad/glx.h>

#include <stdbool.h>

#define WINDOW_ADAPT_CALLBACK_STRUCT(func_name, event_type) \
typedef struct { \
    void (*callback)(const event_type*, void*);\
    void* user_data_ptr; \
} event_type ## Callback; \

WINDOW_ADAPT_CALLBACK_STRUCT(on_key_press_callback, KeyPressEvent)
WINDOW_ADAPT_CALLBACK_STRUCT(on_key_release_callback, KeyReleaseEvent)
WINDOW_ADAPT_CALLBACK_STRUCT(on_button_press_callback, ButtonPressEvent)
WINDOW_ADAPT_CALLBACK_STRUCT(on_button_release_callback, ButtonReleaseEvent)
WINDOW_ADAPT_CALLBACK_STRUCT(on_mouse_move_callback, MouseMoveEvent)
WINDOW_ADAPT_CALLBACK_STRUCT(on_window_resize_callback, WindowResizeEvent)

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
    KeyPressEventCallback key_press_callback;
    KeyReleaseEventCallback key_release_callback;
    ButtonPressEventCallback button_press_callback;
    ButtonReleaseEventCallback button_release_callback;
    MouseMoveEventCallback mouse_move_callback;
    WindowResizeEventCallback window_resize_callback;
} SimpleWindow;

extern void set_on_key_press_callback(SimpleWindow*, void (*)(const KeyPressEvent*, void*), void*);
extern void set_on_key_release_callback(SimpleWindow*, void (*)(const KeyReleaseEvent*, void*), void*);
extern void set_on_button_press_callback(SimpleWindow*, void (*)(const ButtonPressEvent*, void*), void*);
extern void set_on_button_release_callback(SimpleWindow*, void (*)(const ButtonReleaseEvent*, void*), void*);
extern void set_on_mouse_motion_callback(SimpleWindow*, void (*)(const MouseMoveEvent*, void*), void*);
extern void set_on_window_resize_callback(SimpleWindow*, void (*)(const WindowResizeEvent*, void*), void*);

extern bool window_is_key_pressed(SimpleWindow*, KeySym);

extern SimpleWindow* window_create(unsigned int width, unsigned int height);
extern void window_destroy(SimpleWindow*);
extern void window_swap_buffers(SimpleWindow*);
extern bool window_dispatch_events(SimpleWindow*);
extern void window_update_title(SimpleWindow*, const char* title);
extern void window_set_title_to_current_fps(SimpleWindow*, double fps);

#endif // WINDOW_H