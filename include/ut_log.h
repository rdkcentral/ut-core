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

#ifndef UT_LOG_H
#define UT_LOG_H

/* System Includes */
#include <stdarg.h>

#define UT_LOG_MAX_LINE_SIZE (255) /**!< Max string size displayed by `UT_LOG`. */
#define UT_LOG_MAX_PATH      (260) /**!< Default maximum path length. */

// ANSI Escape Codes for Colored Output
#define UT_LOG_ASCII_BLACK    "\033[0;30m"    /**!< Black text color. */
#define UT_LOG_ASCII_RED      "\033[0;31m"    /**!< Red text color. */
#define UT_LOG_ASCII_GREEN    "\033[0;32m"    /**!< Green text color. */
#define UT_LOG_ASCII_YELLOW   "\033[0;33m"    /**!< Yellow text color. */
#define UT_LOG_ASCII_BLUE     "\033[0;34m"    /**!< Blue text color. */
#define UT_LOG_ASCII_MAGENTA  "\033[0;35m"    /**!< Magenta text color. */
#define UT_LOG_ASCII_CYAN     "\033[0;36m"    /**!< Cyan text color. */
#define UT_LOG_ASCII_WHITE    "\033[0;37m"    /**!< White text color. */
#define UT_LOG_ASCII_NC       "\033[0m"       /**!< Reset text color. */

// Macro for Basic Logging
/**! Logs a message with file, line, and optional prefix. */
#define UT_LOG(format, ...)                 UT_logPrefix(__FILE__, __LINE__, UT_LOG_ASCII_MAGENTA"LOG   "UT_LOG_ASCII_NC, format, ## __VA_ARGS__)
/**! Logs a message with a custom prefix (file and line are included). */
#define UT_LOG_PREFIX(prefix, format, ...)  UT_logPrefix(__FILE__, __LINE__, prefix, format, ## __VA_ARGS__)

// Macros for Different Log Levels
/**! Logs a step in a test sequence. */
#define UT_LOG_STEP(format, ...)            UT_logPrefix(__FILE__, __LINE__, UT_LOG_ASCII_BLUE"STEP  "UT_LOG_ASCII_NC, format, ## __VA_ARGS__)
/**! Logs informational messages. */
#define UT_LOG_INFO(format, ...)            UT_logPrefix(__FILE__, __LINE__, UT_LOG_ASCII_CYAN"INFO  "UT_LOG_ASCII_NC, format, ## __VA_ARGS__)
/**! Logs debug-level messages. */
#define UT_LOG_DEBUG(format, ...)           UT_logPrefix(__FILE__, __LINE__, UT_LOG_ASCII_MAGENTA"DEBUG "UT_LOG_ASCII_NC, format, ## __VA_ARGS__)
/**! Logs warning messages. */
#define UT_LOG_WARNING(format, ...)         UT_logPrefix(__FILE__, __LINE__, UT_LOG_ASCII_YELLOW"WARN  "UT_LOG_ASCII_NC, format, ## __VA_ARGS__)
/**! Logs error messages. */
#define UT_LOG_ERROR(format, ...)           UT_logPrefix(__FILE__, __LINE__, UT_LOG_ASCII_RED"ERROR "UT_LOG_ASCII_NC, format, ## __VA_ARGS__)
/**! Logs assertion failure messages with a prefix. */
#define UT_LOG_ASSERT(prefix, format, ...)  UT_logPrefix(__FILE__, __LINE__, UT_LOG_ASCII_RED"ASSERT"UT_LOG_ASCII_NC, UT_LOG_ASCII_RED#prefix":"UT_LOG_ASCII_NC #format, ## __VA_ARGS__)


/**!
 * @brief Sets the path for the active log file.
 *
 * Internally it caculates the correct logging filename to be used for the testing
 *
 * @see UT_log_getLogFilename() to retrieve the active log filename including path
 *
 * @param inputFilePath - Path to the log file
 */
void UT_log_setLogFilePath(char *inputFilePath);

/**! @brief Returns the name of the active log file. */
const char *UT_log_getLogFilename(void);


/**!
 * @brief Logs a message with timestamp, file, line, and function name.
 *
 * This function is the core logging implementation, adding metadata to the message before writing it to the log file.
 *
 * @param function - Name of the calling function.
 * @param line - Line number in the source file.
 * @param format - Format string for the log message (printf style).
 * @param ... - Variable arguments for the format string.
 */
void UT_log(const char *function, int line, const char *format, ...);

/**!
 * @brief Logs a message with timestamp, file, line, function name, and prefix.
 *
 * Similar to `UT_log()`, but allows for a custom prefix to be added to the log message.
 *
 * @param function - Name of the calling function.
 * @param line - Line number in the source file.
 * @param prefix - Prefix string to be added before the message.
 * @param format - Format string for the log message (printf style).
 * @param ... - Variable arguments for the format string.
 */
void UT_logPrefix(const char *function, int line, const char *prefix, const char *format, ...);

#endif /* UT_LOG_H */
