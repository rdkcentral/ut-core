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

TARGET_EXEC ?= hal_test

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NC='\033[0m'

$(info $(shell echo ${GREEN}TARGET_EXEC [$(TARGET_EXEC)]${NC}))

UT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
export PATH := $(shell pwd)/toolchain:$(PATH)
TOP_DIR ?= $(UT_DIR)
BUILD_DIR ?= $(TOP_DIR)/obj
BIN_DIR ?= $(TOP_DIR)/bin
XCFLAGS := $(KCFLAGS)

# Enable CUnit Requirements
XCFLAGS += -DUT_CUNIT
CUNIT_DIR +=  $(UT_DIR)/framework/CUnit-2.1-3/CUnit
#CUNIT_SRC_DIRS +=  $(UT_DIR)/src/cunit
CUNIT_SRC_DIRS += $(CUNIT_DIR)/Sources
INC_DIRS += $(CUNIT_DIR)/Headers
#SRC_DIRS += $(CUNIT_SRC_DIRS)/Automated
#SRC_DIRS += $(CUNIT_SRC_DIRS)/Basic
#SRC_DIRS += $(CUNIT_SRC_DIRS)/Console
SRC_DIRS += $(CUNIT_SRC_DIRS)/Framework
#SRC_DIRS += $(CUNIT_SRC_DIRS)/Curses
#SRC_DIRS += $(CUNIT_SRC_DIRS)/wxWidget
#SRC_DIRS += $(CUNIT_SRC_DIRS)/Win
#SRC_DIRS += $(CUNIT_SRC_DIRS)/Test

# LIBWEBSOCKETS Requirements
LIBWEBSOCKETS_DIR = $(UT_DIR)/framework/libwebsockets-4.3.3
INC_DIRS += $(LIBWEBSOCKETS_DIR)/build/include

INC_DIRS += $(UT_DIR)/include
INC_DIRS += $(UT_DIR)/src

SRC_DIRS += $(UT_DIR)/src

MKDIR_P ?= @mkdir -p

$(info TARGET [$(TARGET)])

# defaults for target arm
ifeq ($(TARGET),arm)
CUNIT_VARIANT=arm-rdk-linux-gnueabi
#CC := arm-rdk-linux-gnueabi-gcc -mthumb -mfpu=vfp -mcpu=cortex-a9 -mfloat-abi=soft -mabi=aapcs-linux -mno-thumb-interwork -ffixed-r8 -fomit-frame-pointer 
# CFLAGS will be overriden by Caller as required
INC_DIRS += $(UT_DIR)/sysroot/usr/include
endif

# Defaults for target linux
ifeq ($(TARGET),linux)
CUNIT_VARIANT=i686-pc-linux-gnu
CC := gcc -ggdb -o0 -Wall
endif

XLDFLAGS += -Wl,-rpath, $(YLDFLAGS) $(LDFLAGS)

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)

OBJS := $(subst $(TOP_DIR),$(BUILD_DIR),$(SRCS:.c=.o))

INC_DIRS += $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

VERSION=$(shell git describe --tags | head -n1)

$(info VERSION [$(VERSION)])

# Final conversions
DEPS += $(OBJS:.o=.d)
XCFLAGS += $(CFLAGS) $(INC_FLAGS) -D UT_VERSION=\"$(VERSION)\" -L$(LIBWEBSOCKETS_DIR)/build/lib -lwebsockets

# Library Path
VPATH += $(UT_DIR)
VPATH += $(TOP_DIR)

# Make the final test binary
test: $(OBJS)
	@echo -e ${GREEN}Linking $@ $(BUILD_DIR)/$(TARGET_EXEC)${NC}
	@$(CC) $(OBJS) -o $(BUILD_DIR)/$(TARGET_EXEC) $(XLDFLAGS) $(KCFLAGS) $(XCFLAGS)
	@$(MKDIR_P) $(BIN_DIR)
	@cp $(BUILD_DIR)/$(TARGET_EXEC) $(BIN_DIR)
ifneq ("$(wildcard $(HAL_LIB_DIR)/*.so)","")
	cp $(HAL_LIB_DIR)/*.so* $(BIN_DIR)
endif

# Make any c source
$(BUILD_DIR)/%.o: %.c
	@echo -e ${GREEN}Building [${YELLOW}$<${GREEN}]${NC}
	@$(MKDIR_P) $(dir $@)
	@$(CC) $(XCFLAGS) -c $< -o $@

.PHONY: clean list arm linux framework
all: framework linux

# Ensure the framework is built
framework: $(eval SHELL:=/usr/bin/env bash)
	@echo -e ${GREEN}"Ensure framework is present"${NC}
	${SHELL} -c ${UT_DIR}/build.sh
	@echo -e ${GREEN}Completed${NC}

arm:
	make TARGET=arm

linux: framework
	make TARGET=linux

clean:
	@echo -e ${GREEN}Performing Clean${NC}
	@$(RM) -rf $(BUILD_DIR)
	@echo -e ${GREEN}Clean Completed${NC}

list:
	@echo 
	@echo CC:$(CC)
	@echo 
	@echo BUILD_DIR:$(BUILD_DIR)
	@echo 
	@echo BIN_DIR:$(BIN_DIR)
	@echo 
	@echo OBJS:$(OBJS)
	@echo 
	@echo SRCS:$(SRCS)
	@echo 
	@echo UT_DIR:$(UT_DIR)
	@echo 
	@echo TOP_DIR:$(TOP_DIR)
	@echo 
	@echo BUILD_DIR:$(BUILD_DIR)
	@echo
	@echo
	@echo CFLAGS:$(CFLAGS)
	@echo
	@echo XCFLAGS:$(XCFLAGS)
	@echo
	@echo LDFLAGS:$(LDFLAGS)
	@echo 
	@echo YLDFLAGS:$(YLDFLAGS)
	@echo 
	@echo XLDFLAGS:$(XLDFLAGS)
	@echo 
	@echo SRC_DIRS:$(SRC_DIRS)
	@echo 
	@echo INC_DIRS:$(INC_DIRS)
	@echo 
	@echo INC_FLAGS:$(INC_FLAGS)
	@echo
	@echo DEPS:$(DEPS)
	@echo
	@echo CONFIGURE_FLAGS:$(CONFIGURE_FLAGS)
	@echo
	@echo PKG_CONFIG_PATH:$(PKG_CONFIG_PATH)
	@echo

-include $(DEPS)
