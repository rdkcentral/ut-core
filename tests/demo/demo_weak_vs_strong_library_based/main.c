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
 * @brief Weak Function declaration for `weak_prototype`.
 *
 * The implementation of this function is linked from an external library.
 * The behavior of this function depends on the library providing the implementation.
 */
void __attribute__((weak)) weak_prototype();

/**
 * @brief Function declaration for `weak_implementation_only`.
 *
 * The implementation of this function is linked from an external library.
 * The behavior of this function depends on the library providing the implementation.
 * Only weak implementation exists for this function
 */
void weak_implementation_only();

/**
 * @brief Entry point of the program.
 *
 * Calls the `my_function` function, which is implemented in an external library.
 *
 * @returns 0 on successful execution.
 */
int main() {
    my_function();
    if (weak_prototype)
    {
        weak_prototype();
    }
    weak_implementation_only();
    return 0;
}
