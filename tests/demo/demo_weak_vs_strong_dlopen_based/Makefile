# Makefile for demonstrating weak vs strong symbols using dlopen

CC = gcc
CFLAGS = -Wall -fPIC
LDFLAGS = -ldl

# Targets
TARGET = main
SHARED_LIB = libstrongfoo.so

# Source files
MAIN_SRC = main.c
STRONG_SRC = strong_foo.c

.PHONY: all clean

all: $(TARGET) $(SHARED_LIB)

$(TARGET): $(MAIN_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(SHARED_LIB): $(STRONG_SRC)
	$(CC) $(CFLAGS) -shared -o $@ $^

clean:
	rm -f $(TARGET) $(SHARED_LIB)

