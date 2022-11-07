/*
##########################################################################
# If not stated otherwise in this file or this component's Licenses.txt
# file the following copyright and licenses apply:
#
# Copyright 2022 RDK Management
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
##########################################################################
*/

#ifndef UT_LOG_H
#define UT_LOG_H

/* System Includes */
#include <stdarg.h> 

#define UT_LOG(format, ...) UT_log(__func__, __LINE__, format, ## __VA_ARGS__)

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
 * @brief Appending to log file 
 * 
 * This function will append the log lines with timestamp, function name and line number
 * 
 * @param function      - name of the calling function
 * @param line          - line number
 * @param format        - string formatting to be applied
 * @param ...           - variable args
 */
void UT_log(const char *function, int line, const char * format, ...);

#endif /* UT_LOG_H */
