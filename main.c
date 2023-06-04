#include "graphics/window.h"

int main()
{
    // Display* display;
    // Window window;
    // XEvent lastEvent;
    // const char* window_name = "Minecraft Clone 0.1";
    
    // display = XOpenDisplay(NULL);
    // if(display == NULL)
    // {
    //     fprintf(stderr, "Cannot open display\n");
    //     exit(1);
    // }

    // int defaultScreen = DefaultScreen(display);
    // window = XCreateSimpleWindow(display, RootWindow(display, defaultScreen), 10, 10, 200, 200, 1, BlackPixel(display, defaultScreen), WhitePixel(display, defaultScreen));
    // XSelectInput(display, window, ExposureMask | KeyPressMask);
    // XMapWindow(display, window);

    // do
    // {
    //     XNextEvent(display, &lastEvent);
    // } while (lastEvent.type != KeyPress);
    
    // XDestroyWindow(display, window);
    // XCloseDisplay(display);
    create_window();
    run_loop();
    destroy_window();

    return 0;
}