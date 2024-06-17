# *
# * If not stated otherwise in this file or this component's LICENSE file the
# * following copyright and licenses apply:
# *
# * Copyright 2023 RDK Management
# *
# * Licensed under the Apache License, Version 2.0 (the "License");
# * you may not use this file except in compliance with the License.
# * You may obtain a copy of the License at
# *
# * http://www.apache.org/licenses/LICENSE-2.0
# *
# * Unless required by applicable law or agreed to in writing, software
# * distributed under the License is distributed on an "AS IS" BASIS,
# * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# * See the License for the specific language governing permissions and
# * limitations under the License.
# *
ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
BIN_DIR := $(ROOT_DIR)/bin
BUILD_DIR := $(ROOT_DIR)/obj
#TOP_DIR := $(ROOT_DIR)

SRC_DIRS = $(ROOT_DIR)/src
INC_DIRS := $(ROOT_DIR)/include
INC_DIRS += $(ROOT_DIR)/../include

# Switch on Linux & disable asserts for the testing suites
CFLAGS += -DNDEBUG
TARGET_EXEC = ut-test

TARGET = linux
BUILD = test

$(info TARGET [$(TARGET)])

.PHONY: clean list all

# Here is a list of exports from this makefile to the next
export BIN_DIR
export SRC_DIRS
export INC_DIRS
export BUILD_DIR
#export TOP_DIR
export TARGET
export TARGET_EXEC
export CFLAGS
export LDFLAGS
export BUILD

GREEN='\033[0;32m'
NC='\033[0m'

.PHONY: clean list build all

build: framework
	@echo -e ${GREEN}Build [$@] ${NC}
	@make -C ../ test TARGET=linux
	@echo -e ${GREEN}Copy Assets to [${BIN_DIR}/assets] ${NC}
	@mkdir -p ${BIN_DIR}/assets
	@cp ./src/assets/* ${BIN_DIR}/assets
	@cp ./src/*.sh ${BIN_DIR}/.
	@cp ./../lib/* ${BIN_DIR}/.
	@echo -e ${GREEN}Build Complete${NC}

run: build
	@echo -e ${GREEN}Running [$@]${NC}
	@mkdir -p ./tests/logs
	@bash -c "cd ./bin;./ut-test -a -p ./assets/test_kvp.yaml -l $(ROOT_DIR)/logs/"

rebuild: clean build

framework:
	make -C ../ framework TARGET=linux

list:
	@echo ${GREEN}List [$@]${NC}
	@echo INC_DIRS: ${INC_DIRS}
	@echo SRC_DIRS: ${SRC_DIRS}
	@echo CFLAGS: ${CFLAGS}
	@echo LDFLAGS: ${LDFLAGS}
	@echo BUILD: ${BUILD}
	@make -C ../ list TARGET=linux

clean:
	@echo ${GREEN}Clean [$@]${NC}
	@$(RM) -rf ${BIN_DIR}/assets
	@make -C ../ clean TARGET=linux

cleanall:
	@echo ${GREEN}Clean [$@]${NC}
	@make -C ../ cleanall TARGET=linux
