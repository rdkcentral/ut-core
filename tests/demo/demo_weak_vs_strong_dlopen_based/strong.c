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
 * @file strong.c
 * @brief Provides strong implementations of functions to override weak symbols.
 *
 * This file defines strong implementations for the functions `foo` and `weak_prototype`.
 * When linked or dynamically loaded, these implementations will take precedence over
 * the weak symbols defined in other files.
 */

#include <stdio.h>

/**
 * @brief Strong implementation of the function `foo`.
 *
 * Overrides the weak implementation of `foo` when this file is linked or loaded dynamically.
 * Displays a message indicating the strong implementation is called.
 */
void foo()
{
    printf("Strong symbol: foo() called\n");
}

/**
 * @brief Strong implementation of the function `weak_prototype`.
 *
 * Overrides the weak implementation of `weak_prototype` when this file is linked or loaded dynamically.
 * Displays a message indicating the strong implementation is called.
 */
void weak_prototype()
{
    printf("Strong symbol: weak_prototype() called\n");
}