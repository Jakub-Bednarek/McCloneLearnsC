#include "window.h"

#include <stdio.h>
#include <stdlib.h>

struct SimpleWindow* globalWindow = NULL;

void create_window()
{
    if(globalWindow != NULL)
    {
        fprintf(stderr, "ERROR: Window created more than once!\n");
        exit(1);
    }

    globalWindow = (struct SimpleWindow*) malloc(sizeof(struct SimpleWindow));
    globalWindow->name = "MineClone v0.1";
    Display* display = XOpenDisplay(NULL);
    if(display == NULL)
    {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }

    int defaultScreen = DefaultScreen(display);
    Window window = XCreateSimpleWindow(display, RootWindow(display, defaultScreen), 10, 10, 200, 200, 1, BlackPixel(display, defaultScreen), WhitePixel(display, defaultScreen));
    XSelectInput(display, window, ExposureMask | KeyPressMask);
    XMapWindow(display, window);

    globalWindow->display = display;
    globalWindow->window = window;
}

void destroy_window()
{
    if(globalWindow == NULL)
    {
        fprintf(stderr, "ERROR: Window not created before destroying!\n");
        exit(1);
    }

    printf("Destroying window with name: %s\n", globalWindow->name);
    free(globalWindow);
}

void pull_events()
{    
    do
    {
        XNextEvent(globalWindow->display, &(globalWindow->lastEvent));
        printf("New event\n");
    } while (globalWindow->lastEvent.type != KeyPress);
}