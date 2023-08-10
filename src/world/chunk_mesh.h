#ifndef CHUNK_MESH_H
#define CHUNK_MESH_H

#include <cglm/cglm.h>

typedef void* chunk_mesh_t;

extern float* chunk_mesh_create(int width, int height, int depth);
extern void chunk_mesh_free(float* chunk_mesh);

#endif // CHUNK_MESH_H