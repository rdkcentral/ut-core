/*
* This library is free software; you can redistribute it and/or modify it
* under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 2.1 of the License, or (at
* your option) any later version.
*
* This library is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
* USA.
* 
* https://www.gnu.org/software/libc/manual/html_node/Copying.html
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
