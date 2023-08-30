#include "world/chunk_mesh.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void print_mat4(mat4* mat)
{
    printf("\n\n%f | %f | %f | %f\n%f | %f | %f | %f\n%f | %f | %f | %f\n%f | %f | %f | %f\n", 
        *mat[0][0], *mat[0][1], *mat[0][2], *mat[0][3],
        *mat[1][0], *mat[1][1], *mat[1][2], *mat[1][3],
        *mat[2][0], *mat[2][1], *mat[2][2], *mat[2][3],
        *mat[3][0], *mat[3][1], *mat[3][2], *mat[3][3]);
}

float* chunk_mesh_create(int width, int height, int depth)
{
    float* chunk_mesh_attributes = calloc(sizeof(float) * 3, width * height * depth);
    if(!chunk_mesh_attributes)
    {
        printf("Error while creating chunk_mesh buffer\n");
        return NULL;
    }

    size_t index = 0;
    for(size_t y = 0; y < height; y++)
    {
        for(size_t x = 0; x < width; x++)
        {
            for(size_t z = 0; z < depth; z++)
            {
                index = y * (width * depth) + x * depth + z;
                // printf("Index: %d\n", index);
                *(chunk_mesh_attributes + (index * 3)) = (float)x;
                *(chunk_mesh_attributes + (index * 3) + 1) = (float)y;
                *(chunk_mesh_attributes + (index * 3) + 2) = (float)z;
            }
        }
    }

    return chunk_mesh_attributes;
}

void chunk_mesh_free(float* chunk_mesh)
{
    if(chunk_mesh != NULL)
    {
        free(chunk_mesh);
        chunk_mesh = NULL;
    }
}