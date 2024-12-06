/**
 * @file main.c
 * @brief Demonstrates the use of weak and strong symbols in C with dynamic linking.
 *
 * This program defines a weak symbol `foo()` and dynamically loads a shared library
 * to override it with a strong implementation if specified. The program calls the
 * appropriate implementation of `foo()` based on the input arguments.
 */

#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>

/**
 * @brief Weak symbol implementation of `foo()`.
 *
 * Prints a message indicating that the weak implementation of `foo()` is called.
 */
void __attribute__((weak)) foo() {
    printf("Weak symbol: foo() called\n");
}

/**
 * @brief Entry point for the program.
 *
 * This function checks the command-line arguments to determine whether to load
 * a shared library containing a strong implementation of `foo()`. If the first
 * argument is '1', it dynamically loads the shared library and calls the strong
 * implementation. Otherwise, it calls the weak implementation.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line argument strings.
 *
 * @returns 0 on success, or 1 if an error occurs while loading the shared library
 *          or resolving the strong implementation of `foo()`.
 */

int main(int argc, char *argv[]) {
    

	if (argc > 1 && argv[1][0] == '1') {
		printf("\nLoading the shared library with a strong implementation of foo()...\n");

		// Load the shared library at runtime
		void *handle = dlopen("./libstrongfoo.so", RTLD_LAZY);
		if (!handle) {
			fprintf(stderr, "Error: %s\n", dlerror());
			return 1;
		}

		// Obtain a pointer to the strong implementation of foo
		void (*strong_foo)() = dlsym(handle, "foo");
		char *error;
		if ((error = dlerror()) != NULL) {
			fprintf(stderr, "Error: %s\n", error);
			dlclose(handle);
			return 1;
		}

		printf("Calling the strong implementation of foo():\n");
		strong_foo();

		// Close the shared library
		dlclose(handle);
	} else {
	    // Call the weak implementation
	    printf("Calling the weak implementation of foo():\n");
	    foo();
	    printf("\nTo call the strong implementation, run: %s 1\n", argv[0]);
    }

    return 0;
}

