#include "world.h"

float cube_vertices[] =  {
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f
};

float texture_coordinates[] = {
    0.0f, 0.5f,
    0.5f, 0.5f,
    0.5f, 1.0f,
    0.5f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.5f,
    0.0f, 0.5f,
    0.5f, 0.5f,
    0.5f, 1.0f,
    0.5f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.5f,
    0.5f, 0.5f,
    0.5f, 1.0f,
    0.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.5f,
    0.5f, 0.5f,
    0.5f, 0.5f,
    0.5f, 1.0f,
    0.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.5f,
    0.5f, 0.5f,
    0.0f, 1.0f,
    0.5f, 1.0f,
    0.5f, 0.5f,
    0.5f, 0.5f,
    0.0f, 0.5f,
    0.0f, 1.0f,
    0.0f, 1.0f,
    0.5f, 1.0f,
    0.5f, 0.5f,
    0.5f, 0.5f,
    0.0f, 0.5f,
    0.0f, 1.0f
};