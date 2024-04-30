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

#include <gnu_lib_alloc_strings.h>

int asprintf(char **ret, const char *format, ...) 
{
    va_list args;
    int length;
    char *buffer;

    // Find the formatted string's required length using vsnprintf
    va_start(args, format);
    length = vsnprintf(NULL, 0, format, args);
    va_end(args);

    if (length < 0) { 
        return -1; // Encoding or formatting error
    }

    // Allocate memory for the string
    buffer = malloc(length + 1); // +1 for the null terminator
    if (buffer == NULL) {
        return -1; // Memory allocation error
    }

    // Generate the formatted string using vsprintf
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    *ret = buffer;
    return length; 
}

int vasprintf(char **ret, const char *format, va_list args) 
{
    int length;
    char *buffer;

    // Find the formatted string's required length using vsnprintf
    va_copy(args, args); // Make a copy of va_list for two calls to vsnprintf
    length = vsnprintf(NULL, 0, format, args);
    va_end(args);

    if (length < 0) { 
        return -1; // Encoding or formatting error
    }

    // Allocate memory for the string
    buffer = malloc(length + 1); // +1 for null terminator
    if (buffer == NULL) {
        return -1; // Memory allocation error
    }

    // Generate the formatted string using vsprintf
    vsnprintf(buffer, length + 1, format, args); 

    *ret = buffer;
    return length; 
}
