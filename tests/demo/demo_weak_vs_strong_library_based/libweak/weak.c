#include <stdio.h>

// Weak definition of my_function
void __attribute__((weak)) my_function() {
    printf("This is the weak implementation of my_function.\n");
}
