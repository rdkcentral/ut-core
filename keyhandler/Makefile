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

# Set target based on TARGET variable
TARGET := $(if $(TARGET),$(TARGET),linux)
#export TARGET

BUILD_DIR := $(ROOT_DIR)/../build
UT_CORE_DIR := ../$(ROOT_DIR)
BIN_DIR := $(BUILD_DIR)/bin
#TOP_DIR := $(ROOT_DIR)
ECHOE = /bin/echo -e
TARGET_EXEC = ut_core_key_input

# Variables
ifeq ($(TARGET),arm)
COMPILER := $(if $(filter CPP,$(VARIANT)),$(CXX),$(CC))
#CC := arm-rdk-linux-gnueabi-gcc -mthumb -mfpu=vfp -mcpu=cortex-a9 -mfloat-abi=soft -mabi=aapcs-linux -mno-thumb-interwork -ffixed-r8 -fomit-frame-pointer
# CFLAGS will be overriden by Caller as required
INC_DIRS += $(UT_CORE_DIR)/sysroot/usr/include
else
CC = gcc -ggdb -o0 -Wall
endif

SRC = keyhandler.c
INC_DIRS = $(BUILD_DIR)/include

# Final flags
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
XCFLAGS += $(CFLAGS) $(INC_FLAGS)
XLDFLAGS += -L$(BUILD_DIR)/$(TARGET)/lib -lut_control

# Default target
all: $(TARGET_EXEC)

# Build target
$(TARGET_EXEC): $(SRC)
	@$(CC) $(SRC) $(XCFLAGS) $(XLDFLAGS) -o $(BIN_DIR)/$(TARGET_EXEC)
	@cp *.yaml *.sh $(BIN_DIR)

# Clean target
clean:
	@rm -f $(BIN_DIR)/$(TARGET_EXEC)
	@rm -f $(BIN_DIR)/*.yaml $(BIN_DIR)/*.sh $(BIN_DIR)/*.so

# Phony targets
.PHONY: all clean
