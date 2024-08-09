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

$(info $(shell echo -e ${GREEN}TARGET_EXEC [$(TARGET_EXEC)]${NC}))

UT_CORE_DIR :=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
export PATH := $(shell pwd)/toolchain:$(PATH)

# Moveable Directories based on the caller makefile
TOP_DIR ?= $(UT_CORE_DIR)
BUILD_DIR ?= $(TOP_DIR)/obj
BIN_DIR ?= $(TOP_DIR)/bin
LIB_DIR ?= $(TOP_DIR)/lib

# Non-Moveable Directories
FRAMEWORK_DIR = $(UT_CORE_DIR)/framework
UT_CONTROL = $(FRAMEWORK_DIR)/ut-control

XCFLAGS := $(KCFLAGS)

# Enable CUnit Requirements
XCFLAGS += -DUT_CUNIT
CUNIT_DIR +=  $(FRAMEWORK_DIR)/CUnit-2.1-3/CUnit
CUNIT_SRC_DIRS += $(CUNIT_DIR)/Sources
INC_DIRS += $(CUNIT_DIR)/Headers
SRC_DIRS += $(CUNIT_SRC_DIRS)/Framework

INC_DIRS += $(UT_CORE_DIR)/include
INC_DIRS += $(UT_CONTROL)/include
INC_DIRS += $(UT_CORE_DIR)/src

SRC_DIRS += $(UT_CORE_DIR)/src

XLDFLAGS += -L $(UT_CONTROL)/lib-$(TARGET) -lut_control

MKDIR_P ?= @mkdir -p

# defaults for target arm
ifeq ($(TARGET),arm)
CUNIT_VARIANT=arm-rdk-linux-gnueabi
#CC := arm-rdk-linux-gnueabi-gcc -mthumb -mfpu=vfp -mcpu=cortex-a9 -mfloat-abi=soft -mabi=aapcs-linux -mno-thumb-interwork -ffixed-r8 -fomit-frame-pointer 
# CFLAGS will be overriden by Caller as required
INC_DIRS += $(UT_CORE_DIR)/sysroot/usr/include
TARGET = arm
else
TARGET = linux
endif
OBJ_DIR = $(BUILD_DIR)/$(TARGET)

$(info TARGET [$(TARGET)])

# Defaults for target linux
ifeq ($(TARGET),linux)
CUNIT_VARIANT=i686-pc-linux-gnu
CC := gcc -ggdb -o0 -Wall
endif

XLDFLAGS += -Wl,-rpath, $(YLDFLAGS) $(LDFLAGS) -pthread  -lpthread

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)

OBJS := $(subst $(TOP_DIR),$(OBJ_DIR),$(SRCS:.c=.o))

# Remove duplicates using sort
OBJS := $(sort $(OBJS))

INC_DIRS += $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

VERSION=$(shell git describe --tags | head -n1)

$(info VERSION [$(VERSION)])

# Final conversions
DEPS += $(OBJS:.o=.d)

XCFLAGS += $(CFLAGS) $(INC_FLAGS) -D UT_VERSION=\"$(VERSION)\"

# Library Path
VPATH += $(UT_CORE_DIR)
VPATH += $(TOP_DIR)

.PHONY: clean list arm linux framework test createdirs all

all: framework $(OBJS)

COMPLETION_MARKER := .download_and_build_done

# Ensure the framework is built
framework: createdirs download_and_build
	@echo "Framework target complete"
	@make test

download_and_build: $(COMPLETION_MARKER)

$(COMPLETION_MARKER):
	@echo -e ${GREEN}"Ensure ut-core frameworks are present"${NC}
	@${UT_CORE_DIR}/build.sh TARGET=$(TARGET)
	@echo -e ${GREEN}Completed${NC}
	@echo -e ${GREEN}"Entering ut-control [TARGET=${TARGET}]"${NC}
	@${MAKE} -C $(UT_CONTROL) TARGET=${TARGET}
	@touch $(COMPLETION_MARKER)

# Make the final test binary
test: $(OBJS) createdirs
	@echo -e ${GREEN}Linking $@ $(BUILD_DIR)/$(TARGET_EXEC)${NC}
	@$(CC) $(OBJS) -o $(BUILD_DIR)/$(TARGET_EXEC) $(XLDFLAGS) $(KCFLAGS) $(XCFLAGS)
	@cp $(BUILD_DIR)/$(TARGET_EXEC) $(BIN_DIR)/
	@cp $(UT_CONTROL)/lib-${TARGET}/libut_control.* ${LIB_DIR}
	@cp $(UT_CONTROL)/lib-${TARGET}/libut_control.* ${BIN_DIR}
	@echo -e ${GREEN}ut-control LIB Copied to [${BIN_DIR}]${NC}
ifneq ("$(wildcard $(HAL_LIB_DIR)/*.so)","")
	cp $(HAL_LIB_DIR)/*.so* $(BIN_DIR)
endif

createdirs:
	@$(MKDIR_P) ${BIN_DIR}
	@$(MKDIR_P) ${LIB_DIR}

# Make any c source
$(OBJ_DIR)/%.o: %.c
	@echo -e ${GREEN}Building [${YELLOW}$<${GREEN}]${NC}
	@$(MKDIR_P) $(dir $@)
	@$(CC) $(XCFLAGS) -c $< -o $@


arm:
	make TARGET=arm

linux: framework
	make TARGET=linux

clean:
	@echo -e ${GREEN}Performing Clean for $(TARGET) ${NC}
	@$(RM) -f $(COMPLETION_MARKER)
	@$(RM) -rf $(BUILD_DIR)/$(TARGET)
	@echo -e ${GREEN}Clean Completed${NC}

cleanall: clean 
	@echo -e ${GREEN}Performing Clean on frameworks [$(UT_CORE_DIR)/framework]${NC}
	@$(RM) -rf $(UT_CORE_DIR)/framework
	@$(RM) -rf $(BIN_DIR)/lib*.so*
	@$(RM) -rf $(BIN_DIR)/lib*.a
	@$(RM) -rf $(BIN_DIR)/$(TARGET_EXEC)

list:
	@echo --------- ut_core ----------------
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
	@echo UT_CORE_DIR:$(UT_CORE_DIR)
	@echo 
	@echo TOP_DIR:$(TOP_DIR)
	@echo 
	@echo BUILD_DIR:$(BUILD_DIR)
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
	@echo --------- ut_control ----------------
	@${MAKE} -C $(UT_CONTROL) list

-include $(DEPS)
