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

# Terminal colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NC='\033[0m'
ECHOE = /bin/echo -e
MKDIR_P ?= @mkdir -p

# Directories and PATH setup
UT_CORE_DIR := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
export PATH := $(UT_CORE_DIR)/toolchain:$(PATH)

# Set target based on TARGET variable
TARGET := $(if $(TARGET),$(TARGET),linux)

ifeq ($(TARGET),arm)
CUNIT_VARIANT = arm-rdk-linux-gnueabi
#CC := arm-rdk-linux-gnueabi-gcc -mthumb -mfpu=vfp -mcpu=cortex-a9 -mfloat-abi=soft -mabi=aapcs-linux -mno-thumb-interwork -ffixed-r8 -fomit-frame-pointer
# CFLAGS will be overriden by Caller as required
INC_DIRS += $(UT_CORE_DIR)/sysroot/usr/include
else
CUNIT_VARIANT = i686-pc-linux-gnu
endif
export TARGET

# Moveable Directories based on the caller makefile
TOP_DIR ?= $(UT_CORE_DIR)
BIN_DIR ?= $(TOP_DIR)/build/bin
LIB_DIR ?= $(TOP_DIR)/build/$(TARGET)/lib
BUILD_DIR ?= $(TOP_DIR)/build/$(TARGET)/obj

# Non-Moveable Directories
FRAMEWORK_DIR = $(UT_CORE_DIR)/framework
UT_CONTROL = $(FRAMEWORK_DIR)/ut-control

# Flags
XCFLAGS := $(KCFLAGS)
INC_DIRS += $(UT_CORE_DIR)/include $(UT_CONTROL)/include $(UT_CORE_DIR)/src

# Set up compiler and source directories based on DGTEST
ifneq ($(DGTEST),1)
TARGET_EXEC ?= hal_test
ifneq ($(TARGET),arm)
COMPILER = gcc
else
COMPILER = $(CC)
endif
DGTEST = 0

# Enable CUnit Requirements
XCFLAGS += -DUT_CUNIT
CUNIT_DIR = $(FRAMEWORK_DIR)/CUnit-2.1-3/CUnit
INC_DIRS += $(CUNIT_DIR)/Headers $(UT_CORE_DIR)/src/cunit
SRC_DIRS += $(CUNIT_DIR)/Sources/Framework $(UT_CORE_DIR)/src
else
TARGET_EXEC ?= hal_gtest
ifneq ($(TARGET),arm)
COMPILER = g++
else
COMPILER = $(CXX)
endif
DGTEST = 1
SRC_DIRS += $(UT_CORE_DIR)/src
EXCLUDE_DIRS = $(SRCDIR)/cunit
GTEST_SRC = $(FRAMEWORK_DIR)/gtest/$(TARGET)/googletest-1.15.2
INC_DIRS += $(GTEST_SRC)/googletest/include $(UT_CORE_DIR)/src/gtest
endif

# Initial output
$(info $(shell ${ECHOE} ${GREEN}TARGET_EXEC [$(TARGET_EXEC)]${NC}))
$(info TARGET [$(TARGET)])
$(info COMPILER [$(COMPILER)])
$(info $(shell ${ECHOE} ${GREEN}DGTEST [$(DGTEST)]${NC}))

# Linking flags
ifneq ($(DGTEST),0)
XLDFLAGS += -L$(GTEST_SRC)/build/lib/ -L $(UT_CONTROL)/build/$(TARGET)/lib -lgtest -lgtest_main -lut_control -lpthread
else
XLDFLAGS += $(YLDFLAGS) $(LDFLAGS) -L$(UT_CONTROL)/build/$(TARGET)/lib -lut_control -Wl,-rpath, -pthread -lpthread
endif

# Source files and objects
ifeq ($(DGTEST),0)
SRCS := $(shell find $(SRC_DIRS) -name *.c -or -name *.s)
OBJS := $(subst $(TOP_DIR),$(BUILD_DIR),$(SRCS:.c=.o))
else
# Find all source files and header files excluding specific directories
SRCS := $(shell find $(SRC_DIRS) -type f \( -name '*.cpp' -o -name '*.c' \) | grep -v "$(EXCLUDE_DIRS)")
OBJS = $(SRCS:.c=.o) $(SRCS:.cpp=.o)
# Prefix the object files with the build directory
OBJS := $(patsubst $(TOP_DIR)/%, $(BUILD_DIR)/%, $(OBJS))
XLDFLAGS += -L$(FRAMEWORK_DIR)/googletest-1.15.2/build/lib/ -lgtest -lgtest_main -lut_control
endif

OBJS := $(filter %.o, $(sort $(OBJS)))

# Dependency files
DEPS := $(OBJS:.o=.d)

# Final flags
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
VERSION := $(shell git describe --tags | head -n1)
XCFLAGS += $(CFLAGS) $(INC_FLAGS) -D UT_VERSION=\"$(VERSION)\"
CXXFLAGS += $(INC_FLAGS)

VPATH += $(UT_CORE_DIR) $(TOP_DIR)

# Default target
.PHONY: clean cleanall list framework test createdirs all

all: framework $(OBJS)

# Build framework
# Recursive make is needed as src files are not available during the first iteration
framework: createdirs download_and_build
	@${ECHOE} ${GREEN}Framework downloaded and built${NC}
	@make test DGTEST=${DGTEST}
	@cp -r $(UT_CONTROL)/build/$(TARGET)/lib/libut_control.* $(LIB_DIR) $(BIN_DIR)
	@${ECHOE} ${GREEN}ut-control LIB Copied to [${BIN_DIR}]${NC}

download_and_build:
	@${ECHOE} ${GREEN}Ensure ut-core frameworks are present${NC}
	@${UT_CORE_DIR}/build.sh TARGET=${TARGET} DGTEST=${DGTEST}
	@${ECHOE} ${GREEN}Completed${NC}
	@${ECHOE} ${GREEN}Entering ut-control [TARGET=${TARGET}]${NC}
	@${MAKE} -C $(UT_CONTROL) TARGET=${TARGET}

# Build the test binary
test: $(OBJS) createdirs
	@${ECHOE} ${GREEN}Linking $@ $(BUILD_DIR)/$(TARGET_EXEC)${NC}
	@$(COMPILER) $(OBJS) -o $(BUILD_DIR)/$(TARGET_EXEC) $(XCFLAGS) $(XLDFLAGS)
	@cp $(BUILD_DIR)/$(TARGET_EXEC) $(BIN_DIR)/
ifneq ("$(wildcard $(HAL_LIB_DIR)/*.so)","")
	cp $(HAL_LIB_DIR)/*.so* $(BIN_DIR)
endif

# Create necessary directories
createdirs:
	@$(MKDIR_P) ${BIN_DIR} ${LIB_DIR}

# Compilation rules
$(BUILD_DIR)/%.o: %.c
	@${ECHOE} ${GREEN}Building [${YELLOW}$<${GREEN}]${NC}
	@$(MKDIR_P) $(dir $@)
	@$(COMPILER) $(XCFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.cpp
	@${ECHOE} ${GREEN}Building [${YELLOW}$<${GREEN}]${NC}
	@$(MKDIR_P) $(dir $@)
	@$(COMPILER) $(CXXFLAGS) -c $< -o $@

# Clean targets
clean:
	@${ECHOE} ${GREEN}Performing Clean for $(TARGET) ${NC}
	@$(RM) -rf $(BUILD_DIR)
	@${ECHOE} ${GREEN}Clean Completed${NC}

cleanall: clean
	@${ECHOE} ${GREEN}Performing Clean on frameworks [$(UT_CORE_DIR)/framework] and build [$(UT_CORE_DIR)/build]${NC}
	@$(RM) -rf $(UT_CORE_DIR)/framework $(UT_CORE_DIR)/build/

# List target
list:
	@${ECHOE} --------- ut_core ----------------
	@${ECHOE}
	@${ECHOE} COMPILER:$(COMPILER)
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
