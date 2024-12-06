/**
 * @file strong_my_function.c
 * @brief Contains the strong implementation of `my_function`.
 *
 * This file provides the implementation of `my_function`, overriding any weak definitions
 * if linked in the same binary. The function outputs a message to indicate that the strong
 * implementation has been invoked.
 */

#include <stdio.h>

/**
 * @brief Strong implementation of the `my_function`.
 *
 * Prints a message to indicate that the strong implementation is being executed.
 */
void my_function() {
    printf("This is the strong implementation of my_function.\n");
}
