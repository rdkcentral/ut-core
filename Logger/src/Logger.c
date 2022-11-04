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

/* Application Includes */
#include "Logger.h"


/**
 * @brief Create a Log File 
 * IN/OUT : logFileName after appending the timestamp
 * This function will create a new log file with
 * timestamp as suffix for every new execution
 */
void UT_CreateLogfile(char * logFileName)
{
    char time_now[20] = {'\0'};
    time_t now;
    time(&now);
    struct tm *tmp ;
    tmp = localtime(&now);
    strftime(time_now, sizeof(time_now), "%Y-%m-%d-%X", tmp);
    strcat(logFileName, "/tmp/3PE_Log_");
    strcat(logFileName, time_now);
    return;
}

/**
 * @brief Appending to log file 
 * IN : function - name of the function from which Logger is called
 *      line - line number from which Logger is called
 *      format - string formatting to be applied
 * This function will append to the log file with
 * timestamp, function name, line number and log message
 */
void UT_Log (const char *function, int line, const char * format, ...)
{
    char time_now[20] = { '\0' };
    FILE * fp = NULL;
    va_list list;
    time_t now ;
    time(&now);
    struct tm *tmp ;
    tmp = localtime(&now);
    strftime(time_now, sizeof(time_now), "%Y-%m-%d-%X", tmp);

    fp = fopen(logFileName, "a");
    if (fp == NULL)
    { 
        printf("\nUnable to open file for logging...");
        return;
    }
   
    fprintf(fp,"\n3PE [%s] [%s] [%d] : ", time_now, function, line);
    va_start(list, format);
    vfprintf(fp, format, list);
    va_end(list);
    fclose(fp);

    return;
}
