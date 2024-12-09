/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2023 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file main.c
 * @brief Demonstrates the usage of weak symbols and dynamic loading with `dlopen` and `dlsym`.
 *
 * This program attempts to load strong implementations of functions from a shared library.
 * If a strong implementation is not found, the weak implementations in this file are used.
 */

#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>

/**
 * @brief Weak implementation of the function `foo`.
 *
 * If a strong implementation is not found in the loaded shared library, this function is called.
 */
void __attribute__((weak)) foo();

/**
 * @brief A weak function prototype that demonstrates fallback behavior.
 *
 * This function is always defined in this file to demonstrate behavior when no strong symbol exists.
 */
void __attribute__((weak)) weak_prototype();

/**
 * @brief A weak implementation with no prototype, demonstrating a fallback scenario.
 *
 * This function is called if no strong implementation is found in the shared library.
 */
void __attribute__((weak)) weak_implementation_only();

/**
 * @brief Entry point of the program.
 *
 * The program dynamically loads a shared library and attempts to retrieve strong implementations
 * of the functions `foo`, `weak_prototype`, and `weak_implementation_only`. If strong implementations
 * are not found, the weak implementations defined in this file are used instead.
 *
 * @returns 0 on success, 1 on failure to load the shared library.
 */
int main()
{
    char *error;

    // Open the shared library
    void *handle = dlopen("./libstrong.so", RTLD_LAZY);
    if (!handle)
    {
        fprintf(stderr, "Error: %s\n", dlerror());
        return 1;
    }

    // Obtain a pointer to the strong implementation of foo
    void (*strong_foo)() = dlsym(handle, "foo");
    if ((error = dlerror()) != NULL)
    {
        fprintf(stderr, "Error: %s\n", error);
    }

    // Obtain a pointer to the strong implementation of weak_prototype
    void (*strong_weak_prototype)() = dlsym(handle, "weak_prototype");
    if ((error = dlerror()) != NULL)
    {
        fprintf(stderr, "Error: %s\n", error);
    }

    // Obtain a pointer to the strong implementation of weak_implementation_only
    void (*strong_weak_implementation_only)() = dlsym(handle, "weak_implementation_only");
    if ((error = dlerror()) != NULL)
    {
        fprintf(stderr, "Error: %s\n", error);
    }

    // Call the appropriate implementation of foo
    if (strong_foo == NULL)
    {
        foo(); // Call weak implementation
    }
    else
    {
        strong_foo(); // Call strong implementation
    }

    // Call the appropriate implementation of weak_prototype
    if (strong_weak_prototype == NULL)
    {
        weak_prototype(); // Call weak implementation
    }
    else
    {
        strong_weak_prototype(); // Call strong implementation
    }

    // Call the appropriate implementation of weak_implementation_only
    if (strong_weak_implementation_only == NULL)
    {
        weak_implementation_only(); // Call weak implementation
    }
    else
    {
        strong_weak_implementation_only(); // Call strong implementation
    }

    // Close the library
    dlclose(handle);

    return 0;
}

/**
 * @brief Weak implementation of `foo`.
 *
 * This is called when no strong implementation of `foo` is found.
 */
void __attribute__((weak)) foo()
{
    printf("foo() : Weak symbol: foo() called\n");
}

/**
 * @brief Implementation of `weak_prototype`.
 *
 * Always displays a message since this function is defined here as a fallback.
 */
void weak_prototype()
{
    printf("Strong Symbol : weak_prototype() from main.c\n");
}

/**
 * @brief Weak implementation of `weak_implementation_only`.
 *
 * This is called when no strong implementation of `weak_implementation_only` is found.
 */
void __attribute__((weak)) weak_implementation_only()
{
    printf("weak_implementation_only(): Weak symbol: weak_implementation_only() called\n");
}