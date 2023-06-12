#include "renderer.h"

#include <stdbool.h>

#define CAMERA_SPEED 0.1f

float mouse_last_x = 0.0f;
float mouse_last_y = 0.0f;
bool mouse_first_move = true;
Camera* global_cam = NULL;

Camera camera_create()
{
    Camera camera;
    memcpy(camera.up, (vec3){0.0f, 1.0f, 0.0f}, sizeof(camera.up));
    memcpy(camera.pos, (vec3){0.0f, 0.0f, -13.0f}, sizeof(camera.pos));
    memcpy(camera.front, (vec3){0.0f, 0.0f, -1.0f}, sizeof(camera.front));
    camera.pitch = 0.0f;
    camera.yaw = -90.0f;
    mat4 view;
    glm_mat4_identity(view);

    vec3 eye;
    glm_vec3_add(camera.pos, camera.front, eye);
    glm_lookat_rh(eye, camera.pos, camera.up, view);

    memcpy(camera.view, view, sizeof(view));

    return camera;
}

void camera_move(Camera* camera, float speed)
{
    vec3 move_vec;
    move_vec[0] = camera->front[0] * speed;
    move_vec[1] = camera->front[1] * speed;
    move_vec[2] = camera->front[2] * speed; 
    glm_vec3_add(camera->pos, move_vec, camera->pos);   
}

void camera_recalculate_mat(Camera* camera)
{
    mat4 view;
    glm_mat4_identity(view);

    vec3 eye;
    glm_vec3_add(camera->pos, camera->front, eye);
    glm_lookat_rh(eye, camera->pos, camera->up, view);

    memcpy(camera->view, view, sizeof(view));
}

void camera_update_input(Camera* camera, SimpleWindow* window)
{
    vec3 move = {0.0f, 0.0f, 0.0f};
    if(window_is_key_pressed(window, MC_W)) {
        move[0] += camera->front[0] * -CAMERA_SPEED;
        move[1] += camera->front[1] * -CAMERA_SPEED;
        move[2] += camera->front[2] * -CAMERA_SPEED;
    }
    if(window_is_key_pressed(window, MC_S)) {
        move[0] += camera->front[0] * CAMERA_SPEED;
        move[1] += camera->front[1] * CAMERA_SPEED;
        move[2] += camera->front[2] * CAMERA_SPEED;
    }
    if(window_is_key_pressed(window, MC_A)) {
        glm_cross(camera->front, camera->up, move);
        glm_normalize(move);
        move[0] *= CAMERA_SPEED;
        move[1] *= CAMERA_SPEED;
        move[2] *= CAMERA_SPEED;
    }
    if(window_is_key_pressed(window, MC_D)) {
        glm_cross(camera->front, camera->up, move);
        glm_normalize(move);
        move[0] *= -CAMERA_SPEED;
        move[1] *= -CAMERA_SPEED;
        move[2] *= -CAMERA_SPEED;
    }
    glm_vec3_add(camera->pos, move, camera->pos);
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

void gl_update_mouse_delta(XMotionEvent* event)
{
    if(mouse_first_move) {
        mouse_last_x = event->x;
        mouse_last_y = event->y;
        mouse_first_move = false;
        return;
    }
    float x_delta = event->x - mouse_last_x;
    float y_delta = mouse_last_y - event->y;
    mouse_last_x = event->x;
    mouse_last_y = event->y;

    const static float sensitivity = 0.15f;
    global_cam->yaw += x_delta * sensitivity;
    global_cam->pitch +=  y_delta * sensitivity;

    if(global_cam->pitch > 89.0f) {
        global_cam->pitch = 89.0f;
    }
    if(global_cam->pitch < -89.0f) {
        global_cam->pitch = -89.0f;
    }

    vec3 direction;
    direction[0] = cos(glm_rad(global_cam->yaw)) * cos(glm_rad(global_cam->pitch));
    direction[1] = sin(glm_rad(global_cam->pitch));
    direction[2] = sin(glm_rad(global_cam->yaw)) * cos(glm_rad(global_cam->pitch));
    memcpy(global_cam->front, direction, sizeof(direction));

    glm_normalize(global_cam->front);
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

void shader_upload_data(Shader shader, mat4 model, mat4 view, mat4 proj)
{
    glUseProgram(shader);

    unsigned int uniform_location = glGetUniformLocation(shader, "model");
    glUniformMatrix4fv(uniform_location, 1, GL_FALSE, model[0]);

    uniform_location = glGetUniformLocation(shader, "view");
    glUniformMatrix4fv(uniform_location, 1, GL_FALSE, view[0]);

    uniform_location = glGetUniformLocation(shader, "projection");
    glUniformMatrix4fv(uniform_location, 1, GL_FALSE, proj[0]);
}

void render(SimpleWindow* window, Camera* camera, SimpleTimer* timer, Shader shader, unsigned int vertex_array_id, unsigned int element_buffer_id)
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
    camera_update_input(camera, window);
    camera_recalculate_mat(camera);

    glBindVertexArray(vertex_array_id);
    for(size_t i = 0; i < 10; i++)
    {
        for(size_t j = 0; j < 10; j++)
        {
            glm_translate(model, (vec3){1.0f, 0.0f, 0.0f});
            shader_upload_data(shader, model, camera->view, proj);
            glDrawArrays(GL_TRIANGLES, 0, 12*3);
        }
        glm_translate(model, (vec3) {-10.0f, -1.0f, 0.0f});
    }
}
