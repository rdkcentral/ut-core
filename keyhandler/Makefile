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
TARGET_EXEC = ut_core_key_input

# Variables
CC = gcc -ggdb -o0 -Wall
SRC = keyhandler.c ../src/ut_kvp_profile.c
INC_DIRS = ../include ../framework/CUnit-2.1-3/CUnit/Headers ../framework/ut-control/include

# Final flags
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
XCFLAGS += $(CFLAGS) $(INC_FLAGS) -DUT_CUNIT
XLDFLAGS += -L../framework/ut-control/build/linux/lib -lut_control -Wl,-rpath, -pthread -lpthread

# Default target
all: $(TARGET_EXEC)

# Build target
$(TARGET_EXEC): $(SRC)
	@$(CC) $(SRC) $(XCFLAGS) $(XLDFLAGS) -o $(TARGET_EXEC)

# Clean target
clean:
	@rm -f $(TARGET_EXEC)

# Phony targets
.PHONY: all clean
