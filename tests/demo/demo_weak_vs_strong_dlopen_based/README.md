# Weak and Strong Symbols Demo

This project demonstrates the use of weak and strong symbols in C, along with dynamic loading of shared libraries using `dlopen` and `dlsym`.

## Overview

In C, weak symbols allow for a fallback implementation of a function when a strong implementation is not provided. This project uses both weak and strong symbols to show how strong implementations from a shared library can override weak ones in the main program.

The project consists of the following components:

- **`main.c`**: The main program, which contains weak implementations of functions and dynamically attempts to load strong implementations from a shared library.
- **`strong.c`**: Provides strong implementations of some functions, intended to override the weak ones.
- **`libstrong.so`**: A shared library compiled from `strong.c`.

## Prerequisites

To build and run this project, you need:

- A C compiler (e.g., `gcc`)
- The `dl` library for dynamic linking (commonly available on Linux)
- GNU `make`

## Compilation Instructions

1. Build the project for the native architecture:
   ```bash
   make
   ```

2. Build the project for an ARM target:
   ```bash
   make TARGET=arm
   ```
   Ensure you have the appropriate cross-compiler toolchain configured for ARM in the `Makefile`.

3. Clean up the generated files:
   ```bash
   make clean
   ```

## Usage

The program dynamically loads the shared library and attempts to use the strong implementations. If a strong implementation is not found, the weak implementation is used instead.

1. Example Output for linux
   ```bash
   ./main
   Error: ./libstrong.so: undefined symbol: weak_implementation_only
   Strong symbol: foo() called
   Strong symbol: weak_prototype() called
   weak_implementation_only(): Weak symbol: weak_implementation_only() called

   ```

2. Example Output for arm
   ```bash
   root@xione-uk:/opt/jyo# ./main
   Error: ./libstrong.so: undefined symbol: weak_implementation_only
   Strong symbol: foo() called
   Strong symbol: weak_prototype() called
   weak_implementation_only(): Weak symbol: weak_implementation_only() called
   ```

## Key Concepts
- **Weak Symbols:** Functions defined with __attribute__((weak)) that act as fallbacks.
- 	**Strong Symbols:** Regular function definitions that override weak symbols when present.
- 	**Dynamic Loading:** Use of dlopen and dlsym to load and resolve functions at runtime.
- 	**Makefile Usage:** Automating build, clean, and dependency management.


## Dependencies

- GCC for compilation.
- `libdl` (part of the standard library) for dynamic linking.

## Notes

- Ensure the shared library (`libstrong.so`) is located in the same directory as the `main` executable, or adjust the `LD_LIBRARY_PATH` environment variable to include its location.
- This example is designed for educational purposes and demonstrates basic concepts of dynamic linking and symbol resolution in C.