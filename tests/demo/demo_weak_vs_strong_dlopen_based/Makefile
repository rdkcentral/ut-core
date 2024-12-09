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

# Makefile for demonstrating weak vs strong symbols using dlopen

ifeq ($(TARGET),)
CC = gcc
else
#CC = rm-rdk-linux-gnueabi-gcc -mthumb -mfpu=vfp -mcpu=cortex-a9 -mfloat-abi=soft -mabi=aapcs-linux -mno-thumb-interwork -ffixed-r8 -fomit-frame-pointer
endif
CFLAGS = -Wall -fPIC
LDFLAGS = -ldl

# Targets
EXECUTABLE = main
SHARED_LIB = libstrong.so

# Source files
MAIN_SRC = main.c
STRONG_SRC = strong.c

.PHONY: all clean

all: $(EXECUTABLE) $(SHARED_LIB)

$(EXECUTABLE): $(MAIN_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(SHARED_LIB): $(STRONG_SRC)
	$(CC) $(CFLAGS) -shared -o $@ $^

clean:
	rm -f $(EXECUTABLE) $(SHARED_LIB)

