#ifndef EVENT_H
#define EVENT_H

typedef struct {
    int key_sym;
} KeyPressEvent;

typedef struct {
    int key_sym;
} KeyReleaseEvent;

typedef struct {
    int button_code;
} ButtonPressEvent;

typedef struct {
    int button_code;
} ButtonReleaseEvent;

typedef struct {
    float x;
    float y;
    float delta_x;
    float delta_y;
} MouseMoveEvent;

typedef struct {
    int width;
    int height;
} WindowResizeEvent;

#endif // EVENT_H