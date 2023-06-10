#include "window.h"
#include "core/input.h"

#include <stdio.h>
#include <stdlib.h>

#include <X11/Xutil.h>

#define WINDOW_EVENT_MASK KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask
#define DEFAULT_X_WINDOW_POS 0
#define DEFAULT_Y_WINDOW_POS 0

void dummy_on_key_pressed(XKeyEvent*) {}
void dummy_on_key_released(XKeyEvent*) {}
void dummy_on_button_pressed(XButtonEvent*) {}
void dummy_on_button_released(XButtonEvent*) {}
void dummy_on_mouse_motion(XMotionEvent*) {}
void dummy_on_window_resize(XConfigureEvent*) {}

void set_on_key_press_callback(SimpleWindow* window, void (*callback)(XKeyEvent*))
{
    window->on_key_pressed = callback;
}

void set_on_key_released_callback(SimpleWindow* window, void (*callback)(XKeyEvent*))
{
    window->on_key_released = callback;
}
void set_on_button_pressed_callback(SimpleWindow* window, void (*callback)(XButtonEvent*))
{
    window->on_button_pressed = callback;
}

void set_on_button_released_callback(SimpleWindow* window, void (*callback)(XButtonEvent*))
{
    window->on_button_pressed = callback;
}

void set_on_mouse_motion_callback(SimpleWindow* window, void (*callback)(XMotionEvent*))
{
    window->on_mouse_moved = callback;
}

void set_on_window_resize_callback(SimpleWindow* window, void (*callback)(XConfigureEvent*))
{
    window->on_window_resized = callback;
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
        GLX_RENDER_TYPE, GLX_RGBA_BIT,
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
        GLX_CONTEXT_MINOR_VERSION_ARB, 0,
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
    glViewport(0, 0, get_window_attributes.width, get_window_attributes.height);

    simple_window->display = display;
    simple_window->main_window = main_window;
    simple_window->colormap = colormap;
    simple_window->name = "GLAD MODERN GLX WITH X11";
    simple_window->on_key_pressed = dummy_on_key_pressed;
    simple_window->on_key_released = dummy_on_key_released;
    simple_window->on_button_pressed = dummy_on_button_pressed;
    simple_window->on_button_released = dummy_on_button_released;
    simple_window->on_mouse_moved = dummy_on_mouse_motion;
    simple_window->on_window_resized = dummy_on_window_resize;

    return simple_window;
}

void window_destroy(SimpleWindow* window)
{
    if(window == NULL)
    {
        fprintf(stderr, "ERROR: Called window_destroy() on NULL object!\n");
        exit(1);
    }

    printf("Destroying window with name: %s\n", window->name);
    glXMakeCurrent(window->display, 0, 0);
    glXDestroyContext(window->display, window->context);
    
    XDestroyWindow(window->display, window->main_window);
    XFreeColormap(window->display, window->colormap);
    XCloseDisplay(window->display);

    free(window);
}

void handle_window_resize_event(SimpleWindow* window, XConfigureEvent* event)
{
    if(window->width != event->width || window->height != event->height)
    {
        window->width = event->width;
        window->height = event->height;
        window->on_window_resized(event);
    }
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
                window->on_key_pressed(&lastEvent.xkey);
                break;
            case KeyRelease:
                window->on_key_released(&lastEvent.xkey);
                break;
            case ButtonPress:
                window->on_button_pressed(&lastEvent.xbutton);
                break;
            case ButtonRelease:
                window->on_button_released(&lastEvent.xbutton);
                break;
            case MotionNotify:
                window->on_mouse_moved(&lastEvent.xmotion);
                break;
            case ConfigureNotify:
                handle_window_resize_event(window, &lastEvent.xconfigure);
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

    glClearColor(0.5, 0.3, 0.7, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glXSwapBuffers(window->display, window->main_window);

    return true;
}

void update_window_title(SimpleWindow* window, const char* title)
{
    if(window->display != NULL)
    {
        XStoreName(window->display, window->main_window, title);
        window->name = title;
    }
}

void set_window_title_to_current_fps(SimpleWindow* window, double fps)
{
    char fps_str[64];
    sprintf(fps_str, "%f", fps);
    update_window_title(window, fps_str);
}
