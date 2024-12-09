# Demonstration of Weak and Strong Symbols in C

This project demonstrates the concept of weak and strong symbols in C, showcasing how symbol resolution order affects program behavior. It includes both static and dynamic linking examples using libraries.

## Project Structure

### Directories:
- `libweak/`: Contains the weak implementations of functions, built as static (`libweak.a`) and shared (`libweak1.so`) libraries.
- `libstrong/`: Contains the strong implementations of functions, built as static (`libstrong.a`) and shared (`libstrong1.so`) libraries.

### Targets:
1. **Static Linking**:
   - `weak_linked_first_static_lib`: Links `main.c` with `libweak.a` first, followed by `libstrong.a`.
   - `strong_linked_first_static_lib`: Links `main.c` with `libstrong.a` first, followed by `libweak.a`.
2. **Dynamic Linking**:
   - `weak_first_link_shared_lib`: Dynamically links `main.c` with `libweak1.so` first, followed by `libstrong1.so`.
   - `strong_first_link_shared_lib`: Dynamically links `main.c` with `libstrong1.so` first, followed by `libweak1.so`.

### Makefile Targets:
- `all`: Builds all executables for both static and dynamic linking examples.
- `clean`: Cleans up all generated files and libraries.

## Build Instructions

1. Build the project for linux:
   ```bash
   make
   ```

2. Build the project for arm:
    ```bash
   make TARGET=arm
   ```

3. Clean the project:
   ```bash
   make clean
   ```

## Usage

### Static Linking
1. Run `weak_linked_first_static_lib`:
   ```bash
   ./weak_linked_first_static_lib
   ```
   Links `libweak.a` first, so weak symbols take precedence.

2. Run `strong_linked_first_static_lib`:
   ```bash
   ./strong_linked_first_static_lib
   ```
   Links `libstrong.a` first, so strong symbols take precedence.

### Dynamic Linking
1. Run `weak_first_link_shared_lib`:
   ```bash
   ./weak_first_link_shared_lib
   ```
   Dynamically links `libweak1.so` first, so weak symbols take precedence unless overridden.

2. Run `strong_first_link_shared_lib`:
   ```bash
   ./strong_first_link_shared_lib
   ```
   Dynamically links `libstrong1.so` first, so strong symbols take precedence.
   
### With run.sh
1. For linux
 ```bash
   ./run.sh 
Using strong_linked_first_static_lib
This is the strong implementation of my_function.
This is the strong implementation of func() weak_prototype from strong.c.
This is the weak implementation of func() weak_implementation_only .
Using weak_linked_first_static_lib
This is the weak implementation of my_function.
This is the weak implementation of func() weak_implementation_only .
Using strong_first_link_shared_lib
This is the strong implementation of my_function.
This is the strong implementation of func() weak_prototype from strong.c.
This is the weak implementation of func() weak_implementation_only .
Using weak_first_link_shared_lib
This is the weak implementation of my_function.
This is the weak implementation of func() weak_implementation_only .

   ```
2. For arm
```bash
root@xione-uk:/opt/jyo# ./run.sh
Using strong_linked_first_static_lib
This is the strong implementation of my_function.
This is the strong implementation of func() weak_prototype from strong.c.
This is the weak implementation of func() weak_implementation_only .
Using weak_linked_first_static_lib
This is the weak implementation of my_function.
This is the weak implementation of func() weak_implementation_only .
Using strong_first_link_shared_lib
This is the strong implementation of my_function.
This is the strong implementation of func() weak_prototype from strong.c.
This is the weak implementation of func() weak_implementation_only .
Using weak_first_link_shared_lib
This is the weak implementation of my_function.
This is the strong implementation of func() weak_prototype from strong.c.
This is the weak implementation of func() weak_implementation_only .
   ```

## Key Concepts

- **Weak Symbols**:
  - Defined with the `__attribute__((weak))` attribute.
  - Serve as a default implementation that can be overridden by strong symbols.

- **Strong Symbols**:
  - Default behavior in C.
  - Override weak symbols during linking.

- **Static Linking**:
  - Symbols are resolved at compile time.
  - Precedence is determined by the order of libraries during the linking process.

- **Dynamic Linking**:
  - Symbols are resolved at runtime.
  - Precedence is determined by the order of linked shared libraries.

## Notes

- Ensure `libweak/` and `libstrong/` directories are built before running the targets.
- For dynamic linking, ensure `libweak1.so` and `libstrong1.so` are in the same directory as the executables or in the library search path (e.g., `LD_LIBRARY_PATH`).
