#ifndef RENDERER_H
#define RENDERER_H

#include <GL/gl.h>

#include "world/world.h"

typedef unsigned int Shader;

unsigned int buffers_create(unsigned int* vertex_buffer_id, unsigned int* color_buffer_id, unsigned int* vertex_array_id, unsigned int* element_buffer_id)
{
    glGenVertexArrays(1, vertex_array_id);
    glGenBuffers(1, vertex_buffer_id);

    glBindVertexArray(*vertex_array_id);
    glBindBuffer(GL_ARRAY_BUFFER, *vertex_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, color_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, *color_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colors), cube_colors, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
}

void gl_clean_up(Shader shader, unsigned int* vertex_buffer_id, unsigned int* color_buffer_id, unsigned int* vertex_array_id, unsigned int* element_buffer_id)
{
    glDeleteVertexArrays(1, vertex_array_id);
    glDeleteBuffers(1, vertex_buffer_id);
    glDeleteProgram(shader);

    printf("Shader, VAO and VBO destroyed\n");
}

void gl_set_draw_mode_callback(XKeyEvent* key_event)
{
    static unsigned int current_gl_mode = GL_FILL;
    if(XLookupKeysym(key_event, 1) == MC_SPACE) {
        if(current_gl_mode == GL_FILL) {
            current_gl_mode = GL_LINE;
        }
        else {
            current_gl_mode = GL_FILL;
        }
        glPolygonMode(GL_FRONT_AND_BACK, current_gl_mode);
    }
}

unsigned int shader_compile(unsigned int shader_type, const char* shader_source)
{
    unsigned int vertex_shader_id = glCreateShader(shader_type);
    glShaderSource(vertex_shader_id, 1, &shader_source, NULL);
    glCompileShader(vertex_shader_id);

    int success;
    glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &success);
    if(!success) {
        char info_log[512];
        glGetShaderInfoLog(vertex_shader_id, 512, NULL, info_log);
        printf("ERROR: Vertex shader compilation failure, error message: %s\n", info_log);
        return 0;
    }

    return vertex_shader_id;
}

unsigned int shader_program_create(unsigned int vertex_shader_id, unsigned int fragment_shader_id)
{
    unsigned int shader_program_id = glCreateProgram();
    glAttachShader(shader_program_id, vertex_shader_id);
    glAttachShader(shader_program_id, fragment_shader_id);
    glLinkProgram(shader_program_id);

    int success;
    glGetProgramiv(shader_program_id, GL_LINK_STATUS, &success);
    if(!success) {
        char info_log[512];
        glGetProgramInfoLog(shader_program_id, 512, NULL, info_log);
        printf("ERROR: Shader program link failure, error message: %s\n", info_log);
        return 0;
    }

    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    return shader_program_id;
}

Shader shader_create() {
    Shader shader;
    char* vertex_shader_source = shader_load_source("res/shaders/vertex.vert");
    char* fragment_shader_source = shader_load_source("res/shaders/fragment.frag");
    unsigned int vertex_shader_id = shader_compile(GL_VERTEX_SHADER, vertex_shader_source);
    unsigned int fragment_shader_id = shader_compile(GL_FRAGMENT_SHADER, fragment_shader_source);
    shader = shader_program_create(vertex_shader_id, fragment_shader_id);

    free(vertex_shader_source);
    free(fragment_shader_source);

    return shader;
}

void render(SimpleTimer* timer, Shader shader, unsigned int vertex_array_id, unsigned int element_buffer_id)
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    static double total_passed = 0.0;
    total_passed += timer->delta_in_sec * 1000.0;
    glClearColor(0.5, 0.3, 0.7, 1.0);

    mat4 transform;
    glm_mat4_identity(transform);
    glm_scale(transform, (vec3) { 0.2f, 0.2f, 0.2f});
    glm_rotate(transform, glm_rad(total_passed), (vec3) { 0.0, 1.0, 1.0} );

    glUseProgram(shader);
    unsigned int transform_loc = glGetUniformLocation(shader, "transform");
    glUniformMatrix4fv(transform_loc, 1, GL_FALSE, transform[0]);

    glBindVertexArray(vertex_array_id);
    glDrawArrays(GL_TRIANGLES, 0, 12*3);
}

#endif // RENDERER_H