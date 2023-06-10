PWD = $(shell pwd)
FLAGS = -L${PWD}/vendor/glew/lib/libGLEW -lGLU -lGL -lX11 
INCLUDE = -I ./vendor/glad/include
OUTPUT_DIR = "build/"
EXE_NAME = "mcclone.out"
SRCS = $(wildcard *.c) $(wildcard graphics/*.c) $(wildcard vendor/glad/src/*.c)

all: compile run

compile:
	@echo "Build begins..."
	@echo ${FLAGS}
	@gcc ${SRCS} ${FLAGS} ${INCLUDE} -o ${OUTPUT_DIR}/${EXE_NAME}
	@echo "Build finished"

run:
	@echo "Running ${EXE_NAME}"
	./${OUTPUT_DIR}/${EXE_NAME}