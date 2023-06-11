#ifndef RENDERER_H
#define RENDERER_H

#include "world/world.h"

#include <GL/gl.h>

#include <string.h>

typedef unsigned int Shader;

typedef struct {
    mat4 view;
    vec3 front;
    vec3 pos;
} Camera;

Camera camera_create()
{
    static vec3 camera_pos = {0.0f, 1.0f, -15.0f};
    static vec3 camera_front = {0.0f, 0.0f, -1.0f};
    static vec3 camera_eye = {0.0f, 0.0f, -1.0f};
    static vec3 camera_up = {0.0f, 1.0f, 0.0f};
    glm_vec3_add(camera_pos, camera_eye, camera_eye);

    mat4 view_mat;
    glm_lookat_rh(camera_eye, camera_pos, camera_up, view_mat);

    Camera camera;
    memcpy(camera.view, view_mat, sizeof(view_mat));
    memcpy(camera.front, camera_front, sizeof(camera_front));
    memcpy(camera.pos, camera_pos, sizeof(camera_pos));

    return camera;
}

void camera_move(Camera* camera, vec3)
{
    vec3 new_pos = {0.0f, 0.0f, -0.1f};
    glm_vec3_add(camera->pos, new_pos, camera->pos);
    vec3 eye = {0.0f, 0.0f, -1.0f};
    glm_vec3_add(camera->pos, eye, eye);
    glm_lookat_rh(eye, camera->pos, (vec3) {0.0f, 1.0f, 0.0f}, camera->view);
    printf("New pos: [%f, %f, %f]\n", camera->pos[0], camera->pos[1], camera->pos[2]);
}

void buffers_create(unsigned int* vertex_buffer_id, unsigned int* color_buffer_id, unsigned int* vertex_array_id, unsigned int* element_buffer_id)
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

void upload_data_to_shader(Shader shader, mat4 model, mat4 view, mat4 proj)
{
    glUseProgram(shader);

    unsigned int uniform_location = glGetUniformLocation(shader, "model");
    glUniformMatrix4fv(uniform_location, 1, GL_FALSE, model[0]);

    uniform_location = glGetUniformLocation(shader, "view");
    glUniformMatrix4fv(uniform_location, 1, GL_FALSE, view[0]);

    uniform_location = glGetUniformLocation(shader, "projection");
    glUniformMatrix4fv(uniform_location, 1, GL_FALSE, proj[0]);
}

void mat4_print(mat4 mat, const char* name)
{
    printf("**** %s MAT PRINT ****\n", name);
    for(size_t i = 0; i < 4; i++) {
        for(size_t j = 0; j < 4; j++) {
            printf("%f    ", mat[i][j]);
        }
        printf("\n");
    }
    printf("**** %s MAT PRINT ****\n", name);
}

void render(Camera* camera, SimpleTimer* timer, Shader shader, unsigned int vertex_array_id, unsigned int element_buffer_id)
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.3, 0.7, 1.0);

    mat4 model;
    glm_mat4_identity(model);
    mat4 proj;
    glm_mat4_identity(proj);
    glm_rotate(model, glm_rad(45), (vec3) {1.0f, 0.0f, 0.0f});
    glm_translate(model, (vec3) {-4.0f, 5.0f, 0.0f});
    glm_perspective_rh_no(glm_rad(45), 1920.0f / 1080.0f, 0.1f, 100.0f, proj);


    glBindVertexArray(vertex_array_id);
    for(size_t i = 0; i < 10; i++)
    {
        for(size_t j = 0; j < 10; j++)
        {
            glm_translate(model, (vec3){1.0f, 0.0f, 0.0f});
            upload_data_to_shader(shader, model, camera->view, proj);
            glDrawArrays(GL_TRIANGLES, 0, 12*3);
        }
        glm_translate(model, (vec3) {-10.0f, -1.0f, 0.0f});
    }
}

#endif // RENDERER_H