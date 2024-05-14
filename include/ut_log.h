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

#define UT_LOG_MAX_LINE_SIZE (255)  /*!< Max String size that UT_LOG will display */
#define UT_LOG_MAX_PATH      (260)  /*!< Default max path length */

#define UT_LOG_ASCII_BLACK    "\033[0;30m"    /*!< BLACK Ascii Code */
#define UT_LOG_ASCII_RED      "\033[0;31m"    /*!< RED Ascii Code */
#define UT_LOG_ASCII_GREEN    "\033[0;32m"    /*!< Green Ascii Code */
#define UT_LOG_ASCII_YELLOW   "\033[0;33m"    /*!< Yellow Ascii Code */
#define UT_LOG_ASCII_BLUE     "\033[0;34m"    /*!< Blue Ascii Code */
#define UT_LOG_ASCII_MAGENTA  "\033[0;35m"    /*!< Magenta Ascii Code */
#define UT_LOG_ASCII_CYAN     "\033[0;36m"    /*!< Cyan Ascii Code */
#define UT_LOG_ASCII_WHITE    "\033[0;37m"    /*!< White Ascii Code */
#define UT_LOG_ASCII_NC       "\033[0m"       /*!< Reset Colour Ascii Code */

/**
 * @brief macro with va_args to display a string
 * 
 * @param format        - string formatting to be applied
 * @param ...           - variable args
 */
#define UT_LOG(format, ...)                 UT_logPrefix(__FILE__, __LINE__, UT_LOG_ASCII_MAGENTA"LOG   "UT_LOG_ASCII_NC, format, ## __VA_ARGS__)
#define UT_LOG_PREFIX(prefix, format, ...)  UT_logPrefix(__FILE__, __LINE__, prefix, format, ## __VA_ARGS__)
#define UT_LOG_STEP(format, ...)            UT_logPrefix(__FILE__, __LINE__, UT_LOG_ASCII_BLUE"STEP  "UT_LOG_ASCII_NC, format, ## __VA_ARGS__)
#define UT_LOG_INFO(format, ...)            UT_logPrefix(__FILE__, __LINE__, UT_LOG_ASCII_CYAN"INFO  "UT_LOG_ASCII_NC, format, ## __VA_ARGS__)
#define UT_LOG_DEBUG(format, ...)           UT_logPrefix(__FILE__, __LINE__, UT_LOG_ASCII_MAGENTA"DEBUG "UT_LOG_ASCII_NC, format, ## __VA_ARGS__)
#define UT_LOG_WARNING(format, ...)         UT_logPrefix(__FILE__, __LINE__, UT_LOG_ASCII_YELLOW"WARN  "UT_LOG_ASCII_NC, format, ## __VA_ARGS__)
#define UT_LOG_ERROR(format, ...)           UT_logPrefix(__FILE__, __LINE__, UT_LOG_ASCII_RED"ERROR "UT_LOG_ASCII_NC, format, ## __VA_ARGS__)
#define UT_LOG_ASSERT(prefix, format, ...)  UT_logPrefix(__FILE__, __LINE__, UT_LOG_ASCII_RED"ASSERT"UT_LOG_ASCII_NC, UT_LOG_ASCII_RED#prefix":"UT_LOG_ASCII_NC #format, ## __VA_ARGS__)

/**
 * @brief Set the path of the active logfile
 * 
 * This function set the path for the active logfile
 * 
 * @param inputFilePath - output logfile
 *
 */
void UT_log_setLogFilePath(char * inputFilePath);

/**
 * @brief Get the log filename
 * 
 * @return const char* 
 */
const char *UT_log_getLogFilename( void );

/**
 * @brief Appending to log file 
 * 
 * This function will append the log lines with timestamp, function name and line number
 * @note: LF will be added to the start of each line by this function
 * 
 * @param function      - name of the calling function
 * @param line          - line number
 * @param format        - string formatting to be applied
 * @param ...           - variable args
 */
void UT_log(const char *function, int line, const char * format, ...);

/**
 * @brief Appending to log file 
 * 
 * This function will append the log lines with timestamp, function name and line number
 * It will also add a prefix message before the message output
 * @note: LF will be added to the start of each line by this function
 * 
 * @param function      - name of the calling function
 * @param line          - line number
 * @param prefix        - Prefix Column string
 * @param format        - string formatting to be applied
 * @param ...           - variable args
 */
void UT_logPrefix(const char *function, int line, const char *prefix, const char * format, ...);

#endif /* UT_LOG_H */
