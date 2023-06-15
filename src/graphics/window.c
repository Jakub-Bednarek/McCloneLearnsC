#include "window.h"

#include <stdio.h>
#include <stdlib.h>

#include <X11/Xutil.h>
#include <X11/XKBlib.h>
#include <X11/extensions/Xfixes.h>

#define WINDOW_EVENT_MASK KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask | LeaveWindowMask
#define DEFAULT_X_WINDOW_POS 0
#define DEFAULT_Y_WINDOW_POS 0

void dummy_on_key_pressed(const KeyPressEvent*, void*) {}
void dummy_on_key_released(const KeyReleaseEvent*, void*) {}
void dummy_on_button_pressed(const ButtonPressEvent*, void*) {}
void dummy_on_button_released(const ButtonReleaseEvent*, void*) {}
void dummy_on_mouse_motion(const MouseMoveEvent*, void*) {}
void dummy_on_window_resize(const WindowResizeEvent*, void*) {}

void set_on_key_press_callback(SimpleWindow* window, void (*callback)(const KeyPressEvent*, void*), void* user_data_ptr)
{
    window->key_press_callback.callback = callback;
    window->key_press_callback.user_data_ptr = user_data_ptr;
}

void set_on_key_release_callback(SimpleWindow* window, void (*callback)(const KeyReleaseEvent*, void*), void* user_data_ptr)
{
    window->key_release_callback.callback = callback;
    window->key_release_callback.user_data_ptr = user_data_ptr;
}

void set_on_button_press_callback(SimpleWindow* window, void (*callback)(const ButtonPressEvent*, void*), void* user_data_ptr)
{
    window->button_press_callback.callback = callback;
    window->button_press_callback.user_data_ptr = user_data_ptr;
}

void set_on_button_release_callback(SimpleWindow* window, void (*callback)(const ButtonReleaseEvent*, void*), void* user_data_ptr)
{
    window->button_release_callback.callback = callback;
    window->button_release_callback.user_data_ptr = user_data_ptr;
}

void set_on_mouse_motion_callback(SimpleWindow* window, void (*callback)(const MouseMoveEvent*, void*), void* user_data_ptr)
{
    window->mouse_move_callback.callback = callback;
    window->mouse_move_callback.user_data_ptr = user_data_ptr;
}

void set_on_window_resize_callback(SimpleWindow* window, void (*callback)(const WindowResizeEvent*, void*), void* user_data_ptr)
{
    window->window_resize_callback.callback = callback;
    window->window_resize_callback.user_data_ptr = user_data_ptr;
}

void window_handle_key_press_event(SimpleWindow* window, const XKeyEvent* key_press_event)
{
    KeyPressEvent event;
    event.key_sym = XkbKeycodeToKeysym(window->display, key_press_event->keycode, 0, key_press_event->state & ShiftMask ? 1 : 0);
    
    int x = 0;
    window->key_press_callback.callback(&event, (void*)(x));
}

void window_handle_key_release_event(SimpleWindow* window, const XKeyEvent* key_release_event)
{
    KeyReleaseEvent event;
    event.key_sym = XkbKeycodeToKeysym(window->display, key_release_event->keycode, 0, key_release_event->state & ShiftMask ? 1 : 0);

    window->key_release_callback.callback(&event, window->key_release_callback.user_data_ptr);
}

void window_handle_button_press_event(SimpleWindow* window, const XButtonEvent* button_press_event)
{
    ButtonPressEvent event;
    event.button_code = button_press_event->button;

    window->button_press_callback.callback(&event, window->button_press_callback.user_data_ptr);
}

void window_handle_button_release_event(SimpleWindow* window, const XButtonEvent* button_release_event)
{
    ButtonReleaseEvent event;
    event.button_code = button_release_event->button;

    window->button_release_callback.callback(&event, window->button_release_callback.user_data_ptr);
}

void window_handle_mouse_move_event(SimpleWindow* window, const XMotionEvent* mouse_move_event)
{
    if(mouse_move_event->x == (int)(window->mouse_x_center) && mouse_move_event->y == (int)(window->mouse_y_center)) {
        return;
    }
    MouseMoveEvent event;
    event.x = mouse_move_event->x;
    event.y = mouse_move_event->y;
    event.delta_x = event.x - window->mouse_x_center;
    event.delta_y = event.y - window->mouse_y_center;

    XWarpPointer(window->display,
        window->main_window,
        window->main_window,
        0,
        0,
        window->width,
        window->height,
        window->mouse_x_center,
        window->mouse_y_center
    );
    window->mouse_move_callback.callback(&event, window->mouse_move_callback.user_data_ptr);
}

void window_handle_resize_event(SimpleWindow* window, const XConfigureEvent* window_resize_event)
{
    glViewport(0, 0, window->width, window->height);

    if(window->width == window_resize_event->width && window->height == window_resize_event->height)
    {
        return;
    }

    WindowResizeEvent event;
    event.width = window_resize_event->width;
    event.height = window_resize_event->height;
    window->width = window_resize_event->width;
    window->height = window_resize_event->height;
    window->window_resize_callback.callback(&event, window->window_resize_callback.user_data_ptr);
}

SimpleWindow* window_create(unsigned int width, unsigned int height)
{
    printf("Creating SimpleWindow, width: %d, height: %d.\n", width, height);
    SimpleWindow* simple_window = malloc(sizeof(SimpleWindow));
    if(simple_window == NULL) {
        printf("CRITICAL ERROR: couldn't allocate memory for SimpleWindow\n");
        exit(1);
    }

    Display* display = XOpenDisplay(NULL);
    if(display == NULL) {
        simple_window->error_code = DISPLAY_FAILURE;
        return simple_window;
    }

    int screen = DefaultScreen(display);
    Window root_window = RootWindow(display, screen);
    Visual* default_visual_info = DefaultVisual(display, screen);

    if(default_visual_info == NULL) {
        simple_window->error_code = DEFAULT_VISUAL_INFO_FAILURE;
        return simple_window;
    }

    Colormap colormap = XCreateColormap(display, root_window, default_visual_info, AllocNone);
    XSetWindowAttributes set_window_attributes = { .event_mask = WINDOW_EVENT_MASK, .colormap = colormap };
    Window main_window = XCreateWindow(
        display,
        root_window,
        DEFAULT_X_WINDOW_POS,
        DEFAULT_Y_WINDOW_POS,
        width,
        height,
        0,
        DefaultDepth(display, screen),
        InputOutput,
        default_visual_info,
        CWColormap | CWEventMask,
        &set_window_attributes
    );
    
    XMapWindow(display, main_window);

    int glx_version = gladLoaderLoadGLX(display, screen);
    if (!glx_version)
    {
        simple_window->error_code = GLX_LOAD_FAILURE;
        return simple_window;
    }
    printf("GLX loaded successfuly. Version major: %d | Version minor: %d\n", GLAD_VERSION_MAJOR(glx_version), GLAD_VERSION_MINOR(glx_version));

    GLint visual_attributes[] = {
        GLX_RENDER_TYPE, GLX_RGBA_BIT, GLX_DEPTH_SIZE, 24,
        GLX_DOUBLEBUFFER, 1,
        None
    };

    int num_fb_config = 0;
    GLXFBConfig* fb_config = glXChooseFBConfig(display, screen, visual_attributes, &num_fb_config);
    if(fb_config == NULL) {
        simple_window->error_code = FB_CONFIG_FAILURE;
        return simple_window;
    }

    GLint context_attributes[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
        GLX_CONTEXT_MINOR_VERSION_ARB, 6,
        GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
        None
    };

    GLXContext glx_context = glXCreateContextAttribsARB(display, fb_config[0], NULL, 1, context_attributes);
    if(glx_context == NULL) {
        simple_window->error_code = GLX_CONTEXT_FAILURE;
        return simple_window;
    }

    glXMakeCurrent(display, main_window, glx_context);

    int gl_version = gladLoaderLoadGL();
    if(!gl_version) {
        simple_window->error_code = GL_LOAD_FAILURE;
        return simple_window;
    }

    printf("GL loaded successfuly. Version major: %d | minor: %d\n", GLAD_VERSION_MAJOR(gl_version), GLAD_VERSION_MINOR(gl_version));

    XWindowAttributes get_window_attributes;
    XGetWindowAttributes(display, main_window, &get_window_attributes);
    XFixesHideCursor(display, main_window);
    XWarpPointer(display, main_window, main_window, 0, 0, width, height, (float)(width) / 2.0f, (float)(height) / 2.0f);

    simple_window->display = display;
    simple_window->main_window = main_window;
    simple_window->colormap = colormap;
    simple_window->name = "OpenGL Window";
    simple_window->mouse_x_center = (float)(width) / 2.0f;
    simple_window->mouse_y_center = (float)(height) / 2.0f;
    simple_window->key_press_callback.callback = dummy_on_key_pressed;
    simple_window->key_release_callback.callback = dummy_on_key_released;
    simple_window->button_press_callback.callback = dummy_on_button_pressed;
    simple_window->button_release_callback.callback = dummy_on_button_released;
    simple_window->mouse_move_callback.callback = dummy_on_mouse_motion;
    simple_window->window_resize_callback.callback = dummy_on_window_resize;

    glViewport(0, 0, get_window_attributes.width, get_window_attributes.height);

    return simple_window;
}

bool window_is_key_pressed(SimpleWindow* window, KeySym key_sym)
{
    KeyCode key_code = XKeysymToKeycode(window->display, key_sym);
    int target_byte = key_code / 8;
    int target_bit = key_code % 8;
    int target_mask = 0x01 << target_bit;

    char keys_return[32] = {0};
    XQueryKeymap(window->display, keys_return);

    return (keys_return[target_byte] & target_mask) && 1;
}

void window_destroy(SimpleWindow* window)
{
    if(window == NULL)
    {
        fprintf(stderr, "ERROR: Called window_destroy() on NULL object!\n");
        exit(1);
    }

    printf("Destroying window with name: %s \n", window->name);
    glXMakeCurrent(window->display, 0, 0);
    glXDestroyContext(window->display, window->context);
    
    XDestroyWindow(window->display, window->main_window);
    XFreeColormap(window->display, window->colormap);
    XCloseDisplay(window->display);

    free(window);
}

void window_swap_buffers(SimpleWindow* window)
{
    glXSwapBuffers(window->display, window->main_window);
}

bool window_dispatch_events(SimpleWindow* window)
{    
    Atom wm_delete_window;
    wm_delete_window =
        XInternAtom (window->display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(window->display, window->main_window, &wm_delete_window, 1);

    XFlush(window->display);
    int events_pending = XEventsQueued(window->display, QueuedAlready);
    XEvent lastEvent;
    while(events_pending > 0)
    {
       XNextEvent(window->display, &lastEvent);
       switch(lastEvent.type)
       {
            case KeyPress:
                window_handle_key_press_event(window, &lastEvent.xkey);
                break;
            case KeyRelease:
                window_handle_key_release_event(window, &lastEvent.xkey);
                break;
            case ButtonPress:
                window_handle_button_press_event(window, &lastEvent.xbutton);
                break;
            case ButtonRelease:
                window_handle_button_release_event(window, &lastEvent.xbutton);
                break;
            case MotionNotify:
                window_handle_mouse_move_event(window, &lastEvent.xmotion);
                break;
            case ConfigureNotify:
                window_handle_resize_event(window, &lastEvent.xconfigure);
                break;
            case ClientMessage:
                if((Atom)lastEvent.xclient.data.l[0] == wm_delete_window) {
                    return false;
                }
            default:
                break;
       }
       --events_pending;
    }

    return true;
}

void window_update_title(SimpleWindow* window, const char* title)
{
    if(window->display != NULL)
    {
        XStoreName(window->display, window->main_window, title);
        window->name = title;
    }
}

void window_set_title_to_current_fps(SimpleWindow* window, double fps)
{
    char fps_str[64];
    sprintf(fps_str, "%f", fps);
    window_update_title(window, fps_str);
}
