/**
 * @file weak_my_function.c
 * @brief Contains the weak implementation of `my_function`.
 *
 * This file provides a weak definition of `my_function`. If a strong definition
 * is linked into the binary, it will override this weak implementation. This function
 * serves as a default behavior when no strong implementation is available.
 */

#include <stdio.h>

/**
 * @brief Weak implementation of the `my_function`.
 *
 * Prints a message to indicate that the weak implementation is being executed.
 * If a strong implementation is linked, this function will be overridden.
 */
void __attribute__((weak)) my_function() {
    printf("This is the weak implementation of my_function.\n");
}
