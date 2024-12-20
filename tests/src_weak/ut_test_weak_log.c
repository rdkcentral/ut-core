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

/* Standard Libraries */
#include <stdio.h>
#include <stdarg.h>

/* Module Includes */
#include <ut_log.h>

/**
 * @brief Weak implementation of UT_log_setLogFilePath.
 *
 * This function provides a default, weak implementation that does nothing.
 * Override this function by defining your own implementation elsewhere.
 *
 * @param inputFilePath Path to the log file.
 */
void __attribute__((weak)) UT_log_setLogFilePath(char *inputFilePath)
{
    // Default no-op implementation
    printf("Weak implementation of [%s]", __func__);
}

/**
 * @brief Weak implementation of UT_log_getLogFilename.
 *
 * This function provides a default, weak implementation that returns a static filename.
 * Override this function by defining your own implementation elsewhere.
 *
 * @returns A static string representing a default log filename.
 */
const char *__attribute__((weak)) UT_log_getLogFilename(void)
{
    printf("Weak implementation of [%s]", __func__);
    return "default_log_file.log"; // Default filename
}

/**
 * @brief Weak implementation of UT_log.
 *
 * This function provides a default, weak implementation that does nothing.
 * Override this function by defining your own implementation elsewhere.
 *
 * @param function Name of the calling function.
 * @param line Line number in the source file.
 * @param format Format string for the log message.
 * @param ... Variable arguments for the format string.
 */
void __attribute__((weak)) UT_log(const char *function, int line, const char *format, ...)
{
    // Default no-op implementation
    printf("Weak implementation of [%s]", __func__);
}

/**
 * @brief Weak implementation of UT_logPrefix.
 *
 * This function provides a default, weak implementation that does nothing.
 * Override this function by defining your own implementation elsewhere.
 *
 * @param function Name of the calling function.
 * @param line Line number in the source file.
 * @param prefix Prefix string to be added before the message.
 * @param format Format string for the log message.
 * @param ... Variable arguments for the format string.
 */
void __attribute__((weak)) UT_logPrefix(const char *function, int line, const char *prefix, const char *format, ...)
{
    // Default no-op implementation
    printf("Weak implementation of [%s]", __func__);
}