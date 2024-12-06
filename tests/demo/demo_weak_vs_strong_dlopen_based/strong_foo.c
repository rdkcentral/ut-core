/**
 * @file strong_foo.c
 * @brief Provides a strong implementation of the `foo()` function.
 *
 * This file defines the strong symbol `foo()` to override the weak implementation
 * when dynamically loaded by a program.
 */

#include <stdio.h>

/**
 * @brief Strong symbol implementation of `foo()`.
 *
 * Prints a message indicating that the strong implementation of `foo()` is called.
 */
void foo() {
    printf("Strong symbol: foo() called\n");
}
