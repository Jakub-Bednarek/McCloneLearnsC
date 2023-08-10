PWD=$(shell pwd)
BUILD_DIR = "build"
UT_DIR = ${BUILD_DIR}/ut
UT_BIN_DIR = ${PWD}/${UT_DIR}/bin
RELEASE_DIR = ${BUILD_DIR}/release
DEBUG_DIR = ${BUILD_DIR}/debug
SOURCE_DIR = $(pwd)
EXE_NAME = "McCloneLearnsC"

COLOR_RED    := $(shell tput -Txterm setaf 1)
COLOR_GREEN  := $(shell tput -Txterm setaf 2)
COLOR_YELLOW := $(shell tput -Txterm setaf 3)
COLOR_CLEAR  := $(shell tput -Txterm sgr0)

all: debug_config debug_build debug_run

create_debug_dir:
	@if [ ! -d ${DEBUG_DIR} ]; then echo "${COLOR_GREEN}Creating ${DEBUG_DIR}...${COLOR_CLEAR}" && mkdir -p ${DEBUG_DIR}; fi

create_release_dir:
	@if [ ! -d ${RELEASE_DIR} ]; then echo "${COLOR_GREEN}Creating ${RELEASE_DIR}...${COLOR_CLEAR}" && mkdir -p ${RELEASE_DIR}; fi

create_ut_dir:
	@if [ ! -d ${UT_DIR} ]; then echo "${COLOR_GREEN}Creating ${DEBUG_DIR}...${UT_CLEAR}" && mkdir -p ${UT_DIR}; fi

config_debug: create_debug_dir
	@echo "${COLOR_GREEN}Running debug_config...${COLOR_CLEAR}"
	@cd ${DEBUG_DIR} && cmake -DDEBUG=True ../..
	@echo "${COLOR_GREEN}--- Config done ---${COLOR_CLEAR}"

config_release: create_release_dir
	@echo "${COLOR_GREEN}Running release_config...${COLOR_CLEAR}"
	@cd ${RELEASE_DIR} && cmake -DRELEASE=True ../..
	@echo "${COLOR_GREEN}--- Config done ---${COLOR_CLEAR}"

config_ut: create_ut_dir
	@echo "${COLOR_GREEN}Running UT config...${COLOR_CLEAR}"
	@cd ${UT_DIR} && cmake -DUT_ENABLED=True -DUT_BIN_DIR=${UT_BIN_DIR} ../..
	@echo "${COLOR_GREEN}--- UT Config done ---${COLOR_CLEAR}"

release_build: config
	@echo "${COLOR_GREEN}Running build...${COLOR_CLEAR}"
	@cd ${RELEASE_DIR} && make
	@echo "${COLOR_GREEN}--- Build done ---${COLOR_CLEAR}"

clean:
	@echo "${COLOR_GREEN}Cleaning ${BUILD_DIR}...${COLOR_CLEAR}"
	@rm -rf ${BUILD_DIR}
	@echo "${COLOR_GREEN}--- Done ---${COLOR_CLEAR}"

build_debug: config_debug
	@echo "${COLOR_GREEN}Building ${EXE_NAME} release${COLOR_CLEAR}"
	@cd ${DEBUG_DIR} && make
	@echo "${COLOR_GREEN}--- Done ---${COLOR_CLEAR}"

build_release: config_release
	@echo "${COLOR_GREEN}Building ${EXE_NAME} release${COLOR_CLEAR}"
	@cd ${RELEASE_DIR} && make
	@echo "${COLOR_GREEN}--- Done ---${COLOR_CLEAR}"

build_ut: config_ut
	@echo "${COLOR_GREEN}Starting UT builder...${COLOR_CLEAR}"
	@cd ${UT_DIR} && make

run_release: build_release
	@echo "${COLOR_GREEN}Running release ${EXE_NAME}...${COLOR_CLEAR}"
	@./${RELEASE_DIR}/${EXE_NAME}

run_debug: build_debug
	@echo "${COLOR_GREEN}Running debug ${EXE_NAME}...${COLOR_CLEAR}"
	@./${DEBUG_DIR}/${EXE_NAME}

run_ut: build_ut
	@echo "${COLOR_GREEN}Starting UT runner...${COLOR_CLEAR}"
	@python3 scripts/ut_runner.py -d ${UT_BIN_DIR} -f
