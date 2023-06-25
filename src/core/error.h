#ifndef ERROR_H
#define ERROR_H

#ifndef __gl_h_
#include <glad/gl.h>
#endif

#include <stdio.h>

#define MC_ERROR_NONE 0

typedef unsigned McError;

extern GLenum g_gl_error_code;

#define GL_CALL(call) \
    call; \
    g_gl_error_code = glGetError(); \
    while (g_gl_error_code != GL_NO_ERROR){ \
        printf("OpenGL error: %d\n", g_gl_error_code); \
        g_gl_error_code = glGetError(); \
    } \

#endif // ERROR_H