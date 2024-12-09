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
void my_function()
{
    printf("This is the strong implementation of my_function.\n");
}

/**
 * @brief Strong implementation of the `weak_prototype` function which is declared with weak prototype.
 *
 * Prints a message to indicate that the strong implementation is being executed.
 */
void weak_prototype()
{
    printf("This is the strong implementation of func() weak_prototype from strong.c.\n");
}
