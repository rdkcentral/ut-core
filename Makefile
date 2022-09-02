#* ******************************************************************************
#* Copyright (C) 2022 Sky group of companies, All Rights Reserved
#* * --------------------------------------------------------------------------
#* * THIS SOFTWARE CONTRIBUTION IS PROVIDED ON BEHALF OF SKY PLC. 
#* * BY THE CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
#* * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
#* * A PARTICULAR PURPOSE ARE DISCLAIMED
#* ******************************************************************************
#*
#*   ** Project      : ut-core
#*   ** @addtogroup  : ut-core
#*   ** @file        : makefile
#*   ** @author      : gerald.weatherup@sky.uk
#*   ** @date        : 20/05/2022
#*   **
#*   ** @brief : Makefile for hal_test
#*   ** 
#*
#* ******************************************************************************
TARGET_EXEC ?= hal_test

$(info Module [$(TARGET_EXEC)])

UT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
export PATH := $(shell pwd)/toolchain:$(PATH)
BUILD_DIR := $(TOP_DIR)/obj
BIN_DIR := $(TOP_DIR)/bin

INC_DIRS += $(UT_DIR)/include
SRC_DIRS += $(UT_DIR)/src

MKDIR_P ?= @mkdir -p

ccred=$(echo -e "\033[0;31m")
ccyellow=$(echo -e "\033[0;33m")
ccend=$(echo -e "\033[0m")

$(info TARGET [$(TARGET)])

ifeq ($(TARGET),arm)
CUNIT_VARIANT=arm-rdk-linux-gnueabi
#CC := arm-rdk-linux-gnueabi-gcc -mthumb -mfpu=vfp -mcpu=cortex-a9 -mfloat-abi=soft -mabi=aapcs-linux -mno-thumb-interwork -ffixed-r8 -fomit-frame-pointer 
# CFLAGS will be overriden by Caller as required
INC_DIRS += $(UT_DIR)/sysroot/usr/include
XCFLAGS := $(KCFLAGS)
endif

ifeq ($(TARGET),linux)
CUNIT_VARIANT=i686-pc-linux-gnu
CC := gcc -ggdb -o0 -Wall
endif

CUNIT_LIB_DIR = $(UT_DIR)/framework/cunit/$(CUNIT_VARIANT)/lib
INC_DIRS += $(UT_DIR)/framework/cunit/$(CUNIT_VARIANT)/include/CUnit

XLDFLAGS += -Wl,-rpath,$(CUNIT_LIB_DIR) -L$(CUNIT_LIB_DIR) -lcunit $(YLDFLAGS) $(LDFLAGS) 

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(subst $(TOP_DIR),$(BUILD_DIR), $(SRCS:.c=.o))

INC_DIRS += $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# Library Path

# Final conversions
DEPS += $(OBJS:.o=.d)
XCFLAGS += $(CFLAGS) $(INC_FLAGS) 

VPATH += $(UT_DIR)
VPATH += $(TOP_DIR)

test: $(OBJS)
	@echo Linking $@ $(BUILD_DIR)/$(TARGET_EXEC)
	@$(CC) $(OBJS) -o $(BUILD_DIR)/$(TARGET_EXEC) $(XLDFLAGS) $(KCFLAGS)
	@cp $(BUILD_DIR)/$(TARGET_EXEC) $(BIN_DIR)
	@cp $(CUNIT_LIB_DIR)/*.so* $(BIN_DIR)
	@cp $(CUNIT_LIB_DIR)/*.a $(BIN_DIR)
	@cp $(CUNIT_LIB_DIR)/*.la $(BIN_DIR)
	@touch $(HAL_LIB_DIR)/hal.so.touch
	@cp $(HAL_LIB_DIR)/*.so* $(BIN_DIR)

# c source
$(BUILD_DIR)/%.o: %.c
	@$(MKDIR_P) $(dir $@)
	@echo Building $<
	@$(CC) $(XCFLAGS) -c $< -o $@

.PHONY: clean list arm linux

arm:
	make TARGET=arm

linux:
	make TARGET=arm

clean:
	@echo Performing Clean
	@$(RM) -rf $(BUILD_DIR)
	@echo Clean Completed

list:
	@echo 
	@echo CC:$(CC)
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
