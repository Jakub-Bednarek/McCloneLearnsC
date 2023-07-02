#include "graphics/window.h"
#include "graphics/renderer.h"
#include "graphics/texture.h"
#include "graphics/texture_atlas.h"
#include "core/timer.h"
#include "utils/bmp_loader.h"
#include "core/ecs/entity.h"
#include "data_structures/hash_map.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define TICKS_THRESHOLD 200

void tick(SimpleWindow* window, SimpleTimer* timer)
{
    static int total_delta = 0;

    total_delta += timer->delta_in_micro;
    while(total_delta > TICKS_THRESHOLD)
    {
        total_delta -= TICKS_THRESHOLD;
    }
}

typedef struct {
    int x;
    int d;
} xd;

void add(struct hash_map_t* m)
{
    xd* x = calloc(sizeof(xd), 1);
    x->x = 150;
    x->d = 250;
    hash_map_add(m, "a", (void*)x);
}

int main()
{
    entity_manager_initialize();

    SimpleTimer simple_timer = simple_timer_create();
    SimpleWindow* simple_window = window_create(1920, 1080);
    Camera camera = camera_create();

    const char* textures_to_load[4] = {"res/textures/dirt.bmp", "res/textures/sand.bmp", "res/textures/stone.bmp", "res/textures/wood.bmp"};
    TextureAtlas texture_atlas = texture_atlas_create(16, 16, textures_to_load, 4);
    if(texture_atlas.error_code != MC_ERROR_NONE) {
        printf("ERROR WHILE LOADING TEXTURE ATLAS: %d\n", texture_atlas.error_code);
    }

    if(simple_window->error_code != MC_ERROR_NONE) {
        printf("SimpleWindow creation failure with errorCode: %d\n", simple_window->error_code);
        window_destroy(simple_window);
        exit(1);
    }

    struct hash_map_t* m = hash_map_create(10);
    add(m);

    xd* x = NULL;
    hash_map_get(m, "a", &x);
    if(x != NULL) {
        printf("GET: %d, %d \n", x->x, x->d);
        free(x);
    }

    set_on_key_press_callback(simple_window, gl_set_draw_mode_callback, NULL);
    set_on_mouse_motion_callback(simple_window, gl_update_mouse_delta, (void*)(&camera));

    Shader shader = shader_create();
    unsigned int vbo;
    unsigned int texture_buffer;
    unsigned int vao;
    unsigned int ebo;
    buffers_create(&vbo, &texture_buffer, &vao, &ebo);

    while(true) {
        simple_timer_update(&simple_timer);
        if(!window_dispatch_events(simple_window)) {
            break;
        }
        tick(simple_window, &simple_timer);
        render(simple_window, &camera, &simple_timer, shader, &texture_atlas, vao, ebo);
        window_swap_buffers(simple_window);
    }

    gl_clean_up(shader, &vbo, &texture_buffer, &vao, &ebo);
    window_destroy(simple_window);

    return 0;
}