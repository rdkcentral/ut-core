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
TARGET_EXEC ?= hal_test

# Set up compiler and source directories based on VARIANT
ifneq ($(VARIANT),CPP) # CUNIT case
  VARIANT = C

  # Enable CUnit Requirements
  XCFLAGS += -DUT_CUNIT
  CUNIT_DIR = $(FRAMEWORK_DIR)/CUnit-2.1-3/CUnit
  INC_DIRS += $(CUNIT_DIR)/Headers $(UT_CORE_DIR)/src/c_source
  SRC_DIRS += $(CUNIT_DIR)/Sources/Framework $(UT_CORE_DIR)/src
  XLDFLAGS += $(YLDFLAGS) $(LDFLAGS) -L$(UT_CONTROL)/build/$(TARGET)/lib -lut_control -Wl,-rpath, -pthread -lpthread

  # Source files
  SRCS := $(shell find $(SRC_DIRS) -name *.c -or -name *.s)
else # GTEST case
  VARIANT = CPP

  SRC_DIRS += $(UT_CORE_DIR)/src
  EXCLUDE_DIRS = $(SRCDIR)/c_source
  GTEST_SRC = $(FRAMEWORK_DIR)/gtest/$(TARGET)/googletest-1.15.2
  INC_DIRS += $(GTEST_SRC)/googletest/include $(UT_CORE_DIR)/src/cpp_source $(UT_CORE_DIR)/src
  TEST_LIB_DIR = $(UT_CORE_DIR)/build/$(TARGET)/cpp_libs/lib/
  XLDFLAGS += $(YLDFLAGS) $(LDFLAGS) -L$(UT_CONTROL)/build/$(TARGET)/lib -L$(TEST_LIB_DIR) -lgtest_main -lgtest -lut_control -lpthread

  # Source files
  SRCS := $(shell find $(SRC_DIRS) -type f \( -name '*.cpp' -o -name '*.c' \) | grep -v "$(EXCLUDE_DIRS)")
endif

VARIANT_FILE := .variant

ifeq ($(TARGET),arm)
COMPILER := $(if $(filter CPP,$(VARIANT)),$(CXX),$(CC))
#CC := arm-rdk-linux-gnueabi-gcc -mthumb -mfpu=vfp -mcpu=cortex-a9 -mfloat-abi=soft -mabi=aapcs-linux -mno-thumb-interwork -ffixed-r8 -fomit-frame-pointer
# CFLAGS will be overriden by Caller as required
INC_DIRS += $(UT_CORE_DIR)/sysroot/usr/include
else
COMPILER := $(if $(filter CPP,$(VARIANT)),g++ -ggdb -o0 -Wall, gcc -ggdb -o0 -Wall)
endif

# Common object file setup
OBJS = $(SRCS:.c=.o) $(SRCS:.cpp=.o)
OBJS := $(patsubst $(TOP_DIR)/%, $(BUILD_DIR)/%, $(OBJS)) # Prefix the object files with the build directory using pattern substitution
OBJS := $(filter %.o, $(sort $(OBJS))) # Filter and sort the object files and remove duplicates

# Dependency files
DEPS := $(OBJS:.o=.d)

# Final flags
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
VERSION := $(shell git describe --tags | head -n1)
XCFLAGS += $(CFLAGS) $(INC_FLAGS) -D UT_VERSION=\"$(VERSION)\"
CXXFLAGS += $(INC_FLAGS)

# Initial output
$(info $(shell ${ECHOE} ${GREEN}TARGET_EXEC [$(TARGET_EXEC)]${NC}))
$(info TARGET [$(TARGET)])
$(info COMPILER [$(COMPILER)])
$(info $(shell ${ECHOE} ${GREEN}VARIANT [$(VARIANT)]${NC}))

# Search for prerequisites (like .c, .cpp, or .h files) in specified directories when those files are not in the current directory.
VPATH += $(UT_CORE_DIR) $(TOP_DIR)

# Default target
.PHONY: clean list arm linux framework test createdirs all printenv

all: framework $(OBJS)

# Build framework
# Recursive make is needed as src files are not available during the first iteration
framework: checkvariantchange createdirs download_and_build
	@${ECHOE} ${GREEN}Framework downloaded and built${NC}
	@make test VARIANT=${VARIANT}
	@cp -r $(UT_CONTROL)/build/$(TARGET)/lib/libut_control.* $(LIB_DIR) $(BIN_DIR)
	@${ECHOE} ${GREEN}ut-control LIB Copied to [${BIN_DIR}]${NC}

download_and_build:
	@${ECHOE} ${GREEN}Ensure ut-core frameworks are present${NC}
	@${UT_CORE_DIR}/build.sh TARGET=${TARGET} VARIANT=${VARIANT}
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
	@echo "$(VARIANT)" > $(VARIANT_FILE)
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

# Rule to check if the VARIANT has changed
checkvariantchange:
	@if [ -f "$(VARIANT_FILE)" ]; then \
		PREVIOUS_VARIANT=$$(cat $(VARIANT_FILE)); \
		if [ "$$PREVIOUS_VARIANT" != "$(VARIANT)" ]; then \
			${ECHOE} ${RED}********************************************************************${NC}; \
			${ECHOE} ${RED}Error: VARIANT has changed from $$PREVIOUS_VARIANT to $(VARIANT)${NC}; \
			${ECHOE} ${RED}Please ${YELLOW} make clean TARGET=${TARGET}\; make -C tests\/ clean TARGET=${TARGET} ${NC}; \
			${ECHOE} ${RED}the build and try again.${NC}; \
			${ECHOE} ${RED}********************************************************************${NC}; \
			exit 1; \
		fi \
	fi

arm:
	make TARGET=arm

linux: framework
	make TARGET=linux

# Clean targets
clean:
	@${ECHOE} ${GREEN}Performing Clean for $(TARGET) ${BUILD_DIR} ${NC}
	@$(RM) -rf $(BUILD_DIR)
	@${ECHOE} ${GREEN}Performing Clean for $(TARGET) $(VARIANT_FILE) ${NC}
	@$(RM) -rf $(VARIANT_FILE)
	@${ECHOE} ${GREEN}Performing Clean for $(TARGET) ${LIB_DIR} ${NC}
	@${RM} -rf ${LIB_DIR}
	@${ECHOE} ${GREEN}Clean Completed${NC}

cleanall: clean 
	@${ECHOE} ${GREEN}Performing Clean on [$(UT_CORE_DIR)/framework]${NC}
	@$(RM) -rf $(UT_CORE_DIR)/framework
	@${ECHOE} ${GREEN}Performing Clean on [$(UT_CORE_DIR)/build]${NC}
	@$(RM) -rf $(UT_CORE_DIR)/build/
	@${ECHOE} ${GREEN}Performing Clean on [$(TOP_DIR)/build/]${NC}
	@$(RM) -rf $(TOP_DIR)/build/
	@${ECHOE} ${GREEN}Clean Completed${NC}

printenv:
	@${ECHOE} "Environment variables: [ut-core]"
	@${ECHOE} "---------------------------"
	@$(foreach v, $(.VARIABLES), $(info $(v) = $($(v))))
	@${ECHOE} "---------------------------"

# List target
list:
	@${ECHOE} --------- ut_core ----------------
	@${ECHOE}
	@${ECHOE} ${YELLOW}CC:${NC} $(CC)
	@${ECHOE}
	@${ECHOE} COMPILER:$(COMPILER)
	@${ECHOE}
	@${ECHOE} ${YELLOW}UT_CORE_DIR:${NC} $(UT_CORE_DIR)
	@${ECHOE}
	@${ECHOE} ${YELLOW}TOP_DIR:${NC} $(TOP_DIR)
	@${ECHOE}
	@${ECHOE} ${YELLOW}BIN_DIR:${NC} $(BIN_DIR)
	@${ECHOE}
	@${ECHOE} ${YELLOW}LIB_DIR:${NC} $(LIB_DIR)
	@${ECHOE}
	@${ECHOE} ${YELLOW}BUILD_DIR:${NC} $(BUILD_DIR)
	@${ECHOE}
	@${ECHOE} ${YELLOW}CFLAGS:${NC} $(CFLAGS)
	@${ECHOE}
	@${ECHOE} ${YELLOW}XCFLAGS:${NC} $(XCFLAGS)
	@${ECHOE}
	@${ECHOE} ${YELLOW}LDFLAGS:${NC} $(LDFLAGS)
	@${ECHOE}
	@${ECHOE} ${YELLOW}YLDFLAGS:${NC} $(YLDFLAGS)
	@${ECHOE}
	@${ECHOE} ${YELLOW}XLDFLAGS:${NC} $(XLDFLAGS)
	@${ECHOE}
	@${ECHOE} ${YELLOW}OBJS:${NC} $(OBJS)
	@${ECHOE}
	@${ECHOE} ${YELLOW}SRCS:${NC} $(SRCS)
	@${ECHOE}
	@${ECHOE} ${YELLOW}SRC_DIRS:${NC} $(SRC_DIRS)
	@${ECHOE}
	@${ECHOE} ${YELLOW}INC_DIRS:${NC} $(INC_DIRS)
	@${ECHOE}
	@${ECHOE} ${YELLOW}INC_FLAGS:${NC} $(INC_FLAGS)
	@${ECHOE}
	@${ECHOE} ${YELLOW}DEPS:${NC} $(DEPS)
	@${ECHOE}
	@${ECHOE} --------- ut_control ----------------
	@${MAKE} -C $(UT_CONTROL) list

-include $(DEPS)