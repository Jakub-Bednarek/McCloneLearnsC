project(GLAD)

add_library(GLAD STATIC src/gl.c src/glx.c)
target_include_directories(GLAD PUBLIC include/)