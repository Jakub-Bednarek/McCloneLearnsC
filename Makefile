FLAGS = -lX11
OUTPUT_DIR = "build/"
EXE_NAME = "mcclone.out"
SRCS = $(wildcard *.c) $(wildcard graphics/*.c)

all: compile run

compile:
	@echo "Build begins..."
	@gcc ${FLGAS} ${SRCS} ${FLAGS} -o ${OUTPUT_DIR}/${EXE_NAME}
	@echo "Build finished"

run:
	@echo "Running ${EXE_NAME}"
	./${OUTPUT_DIR}/${EXE_NAME}