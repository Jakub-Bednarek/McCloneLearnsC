OUTPUT_DIR = "build/"
EXE_NAME = "McCloneLearnsC"

COLOR_RED    := $(shell tput -Txterm setaf 1)
COLOR_GREEN  := $(shell tput -Txterm setaf 2)
COLOR_YELLOW := $(shell tput -Txterm setaf 3)
COLOR_CLEAR  := $(shell tput -Txterm sgr0)

all: config build run

create_build_dir:
	@if [ ! -d ${OUTPUT_DIR} ]; then echo "${COLOR_GREEN}Creating ${OUTPUT_DIR}...${COLOR_CLEAR}" && mkdir ${OUTPUT_DIR}; fi

config: create_build_dir
	@echo "${COLOR_GREEN}Running config...${COLOR_CLEAR}"
	@cd build && cmake ..
	@echo "${COLOR_GREEN}--- Config done ---${COLOR_CLEAR}"

build: config
	@echo "${COLOR_GREEN}Running build...${COLOR_CLEAR}"
	@cd build && make
	@echo "${COLOR_GREEN}--- Build done ---${COLOR_CLEAR}"

clean:
	@echo "${COLOR_GREEN}Cleaning ${OUTPUT_DIR}...${COLOR_CLEAR}"
	@rm -rf ${OUTPUT_DIR}
	@echo "${COLOR_GREEN}--- Done ---${COLOR_CLEAR}"

run:	
	@echo "${COLOR_GREEN}Running ${EXE_NAME}...${COLOR_CLEAR}"
	@./${OUTPUT_DIR}/${EXE_NAME}