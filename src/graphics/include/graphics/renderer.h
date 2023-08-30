#ifndef RENDERER_H
#define RENDERER_H

#include "world/world.h"
#include "graphics/window.h"
#include "graphics/texture.h"
#include "utils/input.h"
#include "texture_atlas.h"

#include <GL/gl.h>
#include <GL/glx.h>

#include <string.h>

typedef unsigned int Shader;

extern float mouse_last_x;
extern float mouse_last_y;

typedef struct {
    mat4 view;
    vec3 front;
    vec3 pos;
    vec3 up;
    float current_x_direction;
    float current_y_direction;
    float pitch;
    float yaw;
} Camera;

extern Camera camera_create();
void camera_update_input(Camera*, SimpleWindow*);
extern void camera_move(Camera*, float);
extern void camera_recalculate_mat(Camera*);

extern void buffers_create(unsigned int*, unsigned int*, unsigned int*, unsigned int*);

extern void gl_clean_up(Shader, unsigned int*, unsigned int*, unsigned int*, unsigned int*);
extern void gl_set_draw_mode_callback(const KeyPressEvent*, void*);
extern void gl_update_mouse_delta(const MouseMoveEvent*, void*);

extern Shader shader_create();
extern unsigned int shader_compile(unsigned int, const char*);
extern unsigned int shader_program_create(unsigned int, unsigned int);
extern void shader_upload_data(Shader, mat4, mat4);

void render(SimpleWindow*, Camera*, SimpleTimer*, Shader, TextureAtlas*, unsigned int, unsigned int);

#endif // RENDERER_H