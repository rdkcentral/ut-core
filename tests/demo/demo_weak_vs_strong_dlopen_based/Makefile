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
SHARED_LIB = libstrongfoo.so

# Source files
MAIN_SRC = main.c
STRONG_SRC = strong_foo.c

.PHONY: all clean

all: $(EXECUTABLE) $(SHARED_LIB)

$(EXECUTABLE): $(MAIN_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(SHARED_LIB): $(STRONG_SRC)
	$(CC) $(CFLAGS) -shared -o $@ $^

clean:
	rm -f $(EXECUTABLE) $(SHARED_LIB)

