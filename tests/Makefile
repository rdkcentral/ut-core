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
BIN_DIR := $(ROOT_DIR)/build/bin
#TOP_DIR := $(ROOT_DIR)
ECHOE = /bin/echo -e
TARGET_EXEC = ut-test

# Switch on Linux & disable asserts for the testing suites
CFLAGS += -DNDEBUG
ifneq ($(VARIANT),CPP)
VARIANT = C
XCFLAGS += -DUT_CUNIT
SRC_DIRS = $(ROOT_DIR)/src/c_source $(ROOT_DIR)/src
INC_DIRS := $(ROOT_DIR)/include
INC_DIRS += $(ROOT_DIR)/../include
else
VARIANT = CPP
SRC_DIRS = $(ROOT_DIR)/src/cpp_source $(ROOT_DIR)/src
INC_DIRS := $(ROOT_DIR)/include
endif

ifneq ($(TARGET),arm)
TARGET = linux
endif
BUILD_DIR := $(ROOT_DIR)/build/${TARGET}/obj

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

GREEN='\033[0;32m'
NC='\033[0m'

.PHONY: clean list build all

all: framework

build:
	@${ECHOE} ${GREEN}Build [$@] ${NC}
	@make -C ../ test TARGET=$(TARGET) VARIANT=${VARIANT}

run: build
	@${ECHOE} ${GREEN}Running [$@]${NC}
	@mkdir -p ${ROOT_DIR}/logs
	@bash -c "cd ${ROOT_DIR}/bin; ./ut-test -a -p ./assets/test_kvp.yaml -l ${ROOT_DIR}/logs/"

rebuild: clean build

framework:
	make -C ../ framework TARGET=$(TARGET) VARIANT=${VARIANT}
	@${ECHOE} ${GREEN}Copy Assets to [${BIN_DIR}/assets] ${NC}
	@mkdir -p ${BIN_DIR}/assets
	@cp -r ${ROOT_DIR}/src/assets/* ${BIN_DIR}/assets
	@cp ${ROOT_DIR}/src/*.sh ${BIN_DIR}/.
	@${ECHOE} ${GREEN}Build Complete${NC}

list:
	@${ECHOE} ${GREEN}List [$@]${NC}
	@${ECHOE} INC_DIRS: ${INC_DIRS}
	@${ECHOE} SRC_DIRS: ${SRC_DIRS}
	@${ECHOE} CFLAGS: ${CFLAGS}
	@${ECHOE} LDFLAGS: ${LDFLAGS}
	@make -C ../ list TARGET=$(TARGET) VARIANT=${VARIANT}

clean:
	@${ECHOE} ${GREEN}Clean [$@]${NC}
	@$(RM) -rf ${BIN_DIR}
	@make -C ../ clean TARGET=$(TARGET) VARIANT=${VARIANT}

cleanall:
	@${ECHOE} ${GREEN}Clean [$@]${NC}
	@make -C ../ cleanall TARGET=$(TARGET) VARIANT=${VARIANT}
