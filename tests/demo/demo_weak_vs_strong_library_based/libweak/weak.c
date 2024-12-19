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
void __attribute__((weak)) my_function()
{
    printf("This is the weak implementation of my_function.\n");
}

/**
 * @brief Weak implementation of the `weak_implementation_only`.
 *
 * Prints a message to indicate that the weak implementation is being executed.
 * If a strong implementation is linked, this function will be overridden.
 */
void __attribute__((weak)) weak_implementation_only()
{
    printf("This is the weak implementation of func() weak_implementation_only .\n");
}