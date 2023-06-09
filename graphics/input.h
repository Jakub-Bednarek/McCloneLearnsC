#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include <X11/keysymdef.h>

#define MAX_KEY 256

#ifdef __linux__
#define MC_BACKSPACE XK_BackSpace
#define MC_TAB XK_Tab // DOESN'T WORK?
#define MC_RETURN XK_Return
#define MC_PAUSE XK_Pause // DOESN'T WORK
#define MC_SCROLL_LOCK XK_Scroll_Lock
#define MC_ESC XK_Escape
#define MC_DEL XK_Delete
#define MC_SPACE XK_space

/* NUM */
#define MC_NUMPAD_0 XK_KP_0
#define MC_0 XK_0
#define MC_NUMPAD_1 XK_KP_1
#define MC_1 XK_1
#define MC_NUMPAD_2 XK_KP_2
#define MC_2 XK_2
#define MC_NUMPAD_3 XK_KP_3
#define MC_3 XK_3
#define MC_NUMPAD_4 XK_KP_4
#define MC_4 XK_4
#define MC_NUMPAD_5 XK_KP_5
#define MC_5 XK_5
#define MC_NUMPAD_6 XK_KP_6
#define MC_6 XK_6
#define MC_NUMPAD_7 XK_KP_7
#define MC_7 XK_7
#define MC_NUMPAD_8 XK_KP_8
#define MC_8 XK_8
#define MC_NUMPAD_9 XK_KP_9
#define MC_9 XK_9

/* ARROW KEYS */
#define MC_LEFT XK_Left
#define MC_RIGHT XK_Right
#define MC_UP XK_Up
#define MC_DOWN XK_Down

/* CHAR KEYS */
#define MC_A XK_A
#define MC_B XK_B
#define MC_C XK_C
#define MC_D XK_D
#define MC_E XK_E
#define MC_F XK_F
#define MC_G XK_G
#define MC_H XK_H
#define MC_I XK_I
#define MC_J XK_J
#define MC_K XK_K
#define MC_L XK_L
#define MC_M XK_M
#define MC_N XK_N
#define MC_O XK_O
#define MC_P XK_P
#define MC_Q XK_Q
#define MC_R XK_R
#define MC_S XK_S
#define MC_T XK_T
#define MC_U XK_U
#define MC_V XK_V
#define MC_W XK_W
#define MC_X XK_X
#define MC_Y XK_Y
#define MC_Z XK_Z
#elif __WIN32
#endif

typedef enum 
{
    released = 0,
    pressed
} KeyState;

typedef struct
{
    bool key_held;
    KeyState state;
} KeyInfo;

typedef struct
{
    KeyInfo keys_info[MAX_KEY];
    int x_mouse_pos;
    int y_mouse_pos;
    bool is_mouse_left_pressed;
    bool is_mouse_right_pressed;
} Input;

Input g_input;

void initialize_input(const int x_mouse_pos, const int y_mouse_pos)
{
    KeyInfo default_key_info = {.key_held = false, .state = released};
    for (int i = 0; i < MAX_KEY; i++)
    {
        g_input.keys_info[i] = default_key_info;
    }
    g_input.x_mouse_pos = x_mouse_pos;
    g_input.y_mouse_pos = y_mouse_pos;
    g_input.is_mouse_left_pressed = false;
    g_input.is_mouse_right_pressed = false;
}

#endif // INPUT_H
