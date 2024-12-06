/**
 * @file main.c
 * @brief Demonstrates calling a function whose implementation is linked from a library.
 *
 * This file declares a function `my_function` and invokes it in the `main` function.
 * The actual implementation of `my_function` is expected to be provided by an external library.
 */

#include <stdio.h>

/**
 * @brief Function declaration for `my_function`.
 *
 * The implementation of this function is linked from an external library.
 * The behavior of this function depends on the library providing the implementation.
 */
void my_function();

/**
 * @brief Entry point of the program.
 *
 * Calls the `my_function` function, which is implemented in an external library.
 *
 * @returns 0 on successful execution.
 */
int main() {
    my_function();
    return 0;
}
