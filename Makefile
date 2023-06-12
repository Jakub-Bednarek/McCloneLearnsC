OUTPUT_DIR = "build/"
EXE_NAME = "McCloneLearnsC"

all: config compile run

config:
	@cd build && cmake ..

compile:
	@cd build && make

clean:
	@cd build && rm -rf ./*

run:
	@echo "Running ${EXE_NAME}"
	@./${OUTPUT_DIR}/${EXE_NAME}