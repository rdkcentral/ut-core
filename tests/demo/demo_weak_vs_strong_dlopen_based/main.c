#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>

// Weak symbol declaration
void __attribute__((weak)) foo() {
    printf("Weak symbol: foo() called\n");
}

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

