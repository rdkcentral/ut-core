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
#include "ut_log.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define UT_MAX_PATH (260)

/* #FIXME: By default all filenames MUST assume current running directory and linux, not target, for all development the target is LINUX */
static char logFileName[UT_MAX_PATH] = "./log_baseline.log";    /*!< Path + Filename of the currently active log file*/

void UT_log_setLogFilePath(char *inputFilePath)
{
    char        time_now[20] = {'\0'};
    time_t      now;
    struct tm   *tmp ;

    time(&now);
    tmp = localtime(&now);
    strftime(time_now, sizeof(time_now), "%Y-%m-%d-%X", tmp);
    strcpy(logFileName, inputFilePath);
    strcat(logFileName, "/log_");
    strcat(logFileName, time_now);
    strcat(logFileName, ".log");
}

void UT_log(const char *function, int line, const char * format, ...)
{
    char        time_now[20] = { '\0' };
    FILE        *fp = NULL;
    va_list     list;
    time_t      now;
    struct tm   *tmp;

    time(&now);

    /* #FIXME : This will all need rework, we shouldn't be opening logs constantly */
    fp = fopen(logFileName, "a");
    if (fp == NULL)
    { 
        printf("\nUnable to open file for logging...");
        return;
    }

    tmp = localtime(&now);
    strftime(time_now, sizeof(time_now), "%Y-%m-%d-%X", tmp);
    fprintf(fp,"\n3PE [%s] [%s] [%d] : ", time_now, function, line);
    va_start(list, format);
    vfprintf(fp, format, list);
    va_end(list);
    fclose(fp);
}
