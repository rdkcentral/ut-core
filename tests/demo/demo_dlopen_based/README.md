# Plugin Loader Project using dlopen

## Overview
This project demonstrates how to create a simple C program that dynamically loads a shared library (plugin) and executes its functions. The project consists of :
- a main program (`main.c`) that loads the plugin at runtime
- and interacts with it through an interface defined in `plugin.h`.
- The plugin itself is implemented in `plugin.c`.

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
   Plugin initialized.
   Plugin action performed.
   Plugin cleaned up.
   ```

2. Example Output for arm
   ```bash
   root@xione-uk:/opt/jyo# ./main
   Plugin initialized.
   Plugin action performed.
   Plugin cleaned up.
   ```

3. Example when plugin not found:
   ```bash
   Plugin not found. Running without plugin.
   ```

## Project Details

**main.c:**

This file contains the main program that dynamically loads the libplugin.so shared library and uses the PluginInterface to call the plugin’s functions.

**plugin.c:**

This file defines the implementation of the functions specified in the PluginInterface. The functions include plugin_initialize(), plugin_perform_action(), and plugin_cleanup().

**plugin.h:**

The header file that defines the PluginInterface structure, which declares the function pointers used by the plugin.


## Dependencies

- GCC for compilation.
- `libdl` (part of the standard library) for dynamic linking.

## Notes

- Ensure the shared library (`libplugin.so`) is located in the same directory as the `main` executable, or adjust the `LD_LIBRARY_PATH` environment variable to include its location.
- This example is designed for educational purposes and demonstrates basic concepts of dynamic linking and symbol resolution in C.