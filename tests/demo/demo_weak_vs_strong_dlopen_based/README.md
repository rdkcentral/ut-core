# Dynamic Linking and Weak/Strong Symbols Example

This project demonstrates the use of weak and strong symbols in C programming, along with dynamic linking using the `dlopen` and `dlsym` functions. The goal is to showcase how a weak symbol can be overridden by a strong implementation provided by a shared library.

## Files

### `main.c`
- Defines a weak implementation of the function `foo()`.
- Dynamically loads a shared library (`libstrongfoo.so`) if the user specifies a command-line argument.
- Calls either the weak or strong implementation of `foo()` based on the input.

### `strong_foo.c`
- Contains the strong implementation of the function `foo()`.
- Compiled into a shared library (`libstrongfoo.so`) to override the weak implementation when dynamically loaded.

### `Makefile`
- Automates the build process for the project.
- Targets:
  - `all`: Builds the main executable and the shared library.
  - `main`: Compiles `main.c` into the executable.
  - `libstrongfoo.so`: Compiles `strong_foo.c` into a shared library.
  - `clean`: Cleans up the generated files.

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

1. Run the program without arguments to call the weak implementation of `foo()`:
   ```bash
   ./main
   ```
   **Output:**
   ```
   Calling the weak implementation of foo():
   Weak symbol: foo() called

   To call the strong implementation, run: ./main 1
   ```

2. Run the program with `1` as the argument to load the shared library and call the strong implementation of `foo()`:
   ```bash
   ./main 1
   ```
   **Output:**
   ```
   Loading the shared library with a strong implementation of foo()...
   Calling the strong implementation of foo():
   Strong symbol: foo() called
   ```

## Key Concepts

1. **Weak Symbols**: 
   - Defined using the `__attribute__((weak))` attribute.
   - Provide a default implementation that can be overridden by a strong symbol at runtime or link time.

2. **Strong Symbols**:
   - Provide an explicit implementation of a function or variable.
   - Override weak symbols when dynamically or statically linked.

3. **Dynamic Linking**:
   - Achieved using `dlopen` to load a shared library and `dlsym` to resolve symbols at runtime.
   - Enables runtime flexibility by allowing programs to load specific implementations as needed.

## Dependencies

- GCC for compilation.
- `libdl` (part of the standard library) for dynamic linking.

## Notes

- Ensure the shared library (`libstrongfoo.so`) is located in the same directory as the `main` executable, or adjust the `LD_LIBRARY_PATH` environment variable to include its location.
- This example is designed for educational purposes and demonstrates basic concepts of dynamic linking and symbol resolution in C.

