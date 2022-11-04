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

#ifndef LOGGER_H
#define LOGGER_H

/* System Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h> 

#define UT_LOG(format, ...)\
        UT_Log(__func__, __LINE__, format, ## __VA_ARGS__)

/* Log Fileto which execution logs are to be redirected */
char logFileName[40];

/**
 * @brief Create a Log File 
 * IN/OUT : logFileName after appending the timestamp 
 * This function will create a new log file with
 * timestamp as suffix for every new execution
 */
void UT_CreateLogfile(char * logFileName);

/**
 * @brief Appending to log file 
 * IN : function - name of the function from which Logger is called
 *      line - line number from which Logger is called
 *      format - string formatting to be applied
 * This function will append the log lines with
 * timestamp, function name and line number
 */
void UT_Log (const char *function, int line, const char * format, ...);

#endif /* LOGGER_H */
