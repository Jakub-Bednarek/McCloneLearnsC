#ifndef CUBE_H
#define CUBE_H

#include "glad/glad.h"

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

void render()
{
    unsigned VBO;
    // glGenBuffers(1, &VBO);
}

#endif // CUBE_H