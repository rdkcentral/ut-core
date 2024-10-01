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
ECHOE = /bin/echo -e

$(info $(shell ${ECHOE} ${GREEN}TARGET_EXEC [$(TARGET_EXEC)]${NC}))

UT_CORE_DIR :=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
export PATH := $(shell pwd)/toolchain:$(PATH)

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
export TARGET

$(info TARGET [$(TARGET)])

# Moveable Directories based on the caller makefile
TOP_DIR ?= $(UT_CORE_DIR)
BIN_DIR ?= $(TOP_DIR)/build/bin
LIB_DIR ?= $(TOP_DIR)/build/${TARGET}/lib
BUILD_DIR ?= $(TOP_DIR)/build/$(TARGET)/obj

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

XLDFLAGS += -L $(UT_CONTROL)/build/$(TARGET)/lib -lut_control

MKDIR_P ?= @mkdir -p

# Defaults for target linux
ifeq ($(TARGET),linux)
CUNIT_VARIANT=i686-pc-linux-gnu
CC := gcc -ggdb -o0 -Wall
endif

XLDFLAGS += -Wl,-rpath, $(YLDFLAGS) $(LDFLAGS) -pthread  -lpthread

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)

OBJS := $(subst $(TOP_DIR),$(BUILD_DIR),$(SRCS:.c=.o))

# Remove duplicates using Makefile sort
# Duplicate obj are created as make is getting called recursively in framework
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

# Ensure the framework is built
# Recursive make is needed as src files are not available during the first iteration
framework: createdirs download_and_build
	@${ECHOE} ${GREEN}Framework downloaded and built${NC}
	@make test
	@cp $(UT_CONTROL)/build/$(TARGET)/lib/libut_control.* ${LIB_DIR}
	@cp $(UT_CONTROL)/build/$(TARGET)/lib/libut_control.* ${BIN_DIR}
	@${ECHOE} ${GREEN}ut-control LIB Copied to [${BIN_DIR}]${NC}

download_and_build:
	@${ECHOE} ${GREEN}"Ensure ut-core frameworks are present"${NC}
	@${UT_CORE_DIR}/build.sh TARGET=${TARGET}
	@${ECHOE} ${GREEN}Completed${NC}
	@${ECHOE} ${GREEN}"Entering ut-control [TARGET=${TARGET}]"${NC}
	@${MAKE} -C $(UT_CONTROL) TARGET=${TARGET}

# Make the final test binary
test: $(OBJS) createdirs
	@${ECHOE} ${GREEN}Linking $@ $(BUILD_DIR)/$(TARGET_EXEC)${NC}
	@$(CC) $(OBJS) -o $(BUILD_DIR)/$(TARGET_EXEC) $(XLDFLAGS) $(KCFLAGS) $(XCFLAGS)
	@cp $(BUILD_DIR)/$(TARGET_EXEC) $(BIN_DIR)/
ifneq ("$(wildcard $(HAL_LIB_DIR)/*.so)","")
	cp $(HAL_LIB_DIR)/*.so* $(BIN_DIR)
endif

createdirs:
	@$(MKDIR_P) ${BIN_DIR}
	@$(MKDIR_P) ${LIB_DIR}

# Make any c source
$(BUILD_DIR)/%.o: %.c
	@${ECHOE} ${GREEN}Building [${YELLOW}$<${GREEN}]${NC}
	@$(MKDIR_P) $(dir $@)
	@$(CC) $(XCFLAGS) -c $< -o $@

arm:
	make TARGET=arm

linux: framework
	make TARGET=linux

clean:
	@${ECHOE} ${GREEN}Performing Clean for $(TARGET) ${BUILD_DIR} ${NC}
	@$(RM) -rf $(BUILD_DIR)
	@${ECHOE} ${GREEN}Performing Clean for $(TARGET) ${LIB_DIR} ${NC}
	@${RM} -fr ${LIB_DIR}
	@${ECHOE} ${GREEN}Clean Completed${NC}

cleanall: clean 
	@${ECHOE} ${GREEN}Performing Clean on frameworks [$(UT_CORE_DIR)/framework]${NC}
	@$(RM) -rf $(UT_CORE_DIR)/framework
	@${ECHOE} ${GREEN}Performing Clean on frameworks [$(UT_CORE_DIR)/build]${NC}
	@@$(RM) -rf $(UT_CORE_DIR)/build/

list:
	@${ECHOE} --------- ut_core ----------------
	@${ECHOE}
	@${ECHOE} CC:$(CC)
	@${ECHOE}
	@${ECHOE} BUILD_DIR:$(BUILD_DIR)
	@${ECHOE}
	@${ECHOE} BIN_DIR:$(BIN_DIR)
	@${ECHOE}
	@${ECHOE} OBJS:$(OBJS)
	@${ECHOE}
	@${ECHOE} SRCS:$(SRCS)
	@${ECHOE}
	@${ECHOE} UT_CORE_DIR:$(UT_CORE_DIR)
	@${ECHOE}
	@${ECHOE} TOP_DIR:$(TOP_DIR)
	@${ECHOE}
	@${ECHOE} BUILD_DIR:$(BUILD_DIR)
	@${ECHOE}
	@${ECHOE} CFLAGS:$(CFLAGS)
	@${ECHOE}
	@${ECHOE} XCFLAGS:$(XCFLAGS)
	@${ECHOE}
	@${ECHOE} LDFLAGS:$(LDFLAGS)
	@${ECHOE}
	@${ECHOE} YLDFLAGS:$(YLDFLAGS)
	@${ECHOE}
	@${ECHOE} XLDFLAGS:$(XLDFLAGS)
	@${ECHOE}
	@${ECHOE} SRC_DIRS:$(SRC_DIRS)
	@${ECHOE}
	@${ECHOE} INC_DIRS:$(INC_DIRS)
	@${ECHOE}
	@${ECHOE} INC_FLAGS:$(INC_FLAGS)
	@${ECHOE}
	@${ECHOE} DEPS:$(DEPS)
	@${ECHOE}
	@${ECHOE} --------- ut_control ----------------
	@${MAKE} -C $(UT_CONTROL) list

-include $(DEPS)
