#ifndef WINDOW_H
#define WINDOW_H

#include <X11/Xlib.h>
#include <GL/glx.h>

typedef int file_descriptor;

typedef enum {
    none = 0,
    displayCreationFailed,
    visualInfoCreationFailed
} WindowErrorCode;

typedef struct {
    Display* display;
    Window root;
    Window mainWindow;
    GLXContext context;
    const char* name;
    int x_pos;
    int y_pos;
    int width;
    int height;
    WindowErrorCode errorCode;
    void (*on_key_pressed)(XKeyEvent*);
    void (*on_key_released)(XKeyEvent*);
    void (*on_button_pressed)(XButtonEvent*);
    void (*on_button_released)(XButtonEvent*);
    void (*on_mouse_moved)(XMotionEvent*);
    void (*on_window_resized)(XConfigureEvent*);
} SimpleWindow;

void set_on_key_press_callback(void (*callback)(XKeyEvent*));
void set_on_key_released_callback(void (*callback)(XKeyEvent*));
void set_on_button_pressed_callback(void (*callback)(XButtonEvent*));
void set_on_button_released_callback(void (*callback)(XButtonEvent*));
void set_on_mouse_motion_callback(void (*callback)(XMotionEvent*));
void set_on_window_resize_callback(void (*callback)(XConfigureEvent*));

void create_window();
void destroy_window();
void dispatch_window_events();
void update_window_title(const char* title);
void set_window_title_to_current_fps(double fps);
void draw();

extern SimpleWindow* g_window;

#endif // WINDOW_H