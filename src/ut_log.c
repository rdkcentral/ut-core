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

/* Application Includes */
#include "ut_log.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <libgen.h>

#define UT_MAX_TIME_STRING (20)

/* #FIXME: By default all filenames MUST assume current running directory and linux, not target, for all development the target is LINUX */
#define UT_LOG_DEFAULT_PATH "/tmp/"  // Move to "./" in the future, when 
static char gLogFileName[UT_LOG_MAX_PATH] = {0};    /*!< Path + Filename of the currently active log file*/
static bool gLogInit = false;
static char* UT_stripColorCode(char *in_string);

void UT_log_setLogFilePath(char *inputFilePath)
{
    char        time_now[UT_MAX_TIME_STRING] = {'\0'};
    time_t      now;
    struct tm   *tmp;
    int         length;

    length = strlen(inputFilePath);
    time(&now);
    tmp = localtime(&now);
    strftime(time_now, sizeof(time_now), "%F_%H%M%S", tmp);
    if ( inputFilePath[length-1] == '/' )
    {
        /* Dont append / because path has one */
        snprintf(gLogFileName, UT_LOG_MAX_PATH, "%sut-log_%s.log", inputFilePath, time_now);
    }
    else
    {
        /* Append / because path doesn't have one */
        snprintf(gLogFileName, UT_LOG_MAX_PATH, "%s/ut-log_%s.log", inputFilePath, time_now);
    }
    gLogInit = true;
}

const char *UT_log_getLogFilename( void )
{
    return (const char *)&gLogFileName[0];
}

void UT_log(const char *function, int line, const char * format, ...)
{
    char        time_now[20] = { '\0' };
    FILE        *fp = NULL;
    va_list     list;
    time_t      now;
    struct tm   *tmp;
    char        singleLineBuffer[UT_LOG_MAX_LINE_SIZE+1]={0};
    size_t      lineSize;

    time(&now);

    if ( gLogInit == false )
    {
        /* If ut-core has never called to set the log to a fixed location then let's set it */
        UT_log_setLogFilePath(UT_LOG_DEFAULT_PATH);
    }

    /* #FIXME : This will need rework, we shouldn't be opening logs constantly */
    fp = fopen(gLogFileName, "a");
    if (fp == NULL)
    { 
        printf("\nUnable to open file for logging...");
        return;
    }

    tmp = localtime(&now);
    strftime(time_now, sizeof(time_now), "%Y-%m-%d-%X", tmp);
#if 0
    snprintf( singleLineBuffer, UT_LOG_MAX_LINE_SIZE, "\n%s, %s,%6d : ", time_now, function, line );
#else
    snprintf( singleLineBuffer, UT_LOG_MAX_LINE_SIZE, "\n%s, %s, %6d : ", time_now, UT_LOG_ASCII_MAGENTA"LOG   "UT_LOG_ASCII_NC, line );
#endif
    lineSize = strlen( singleLineBuffer );

    va_start(list, format);
    vsnprintf(&singleLineBuffer[lineSize], UT_LOG_MAX_LINE_SIZE-lineSize-1, format, list);
    va_end(list);

    /* Print the line to stdout */
    printf( "%s", singleLineBuffer );

    /* Print the data to the stream */
    fprintf( fp, "%s", singleLineBuffer );
    fclose(fp);
}

void UT_logPrefix(const char *file, int line, const char *prefix, const char * format, ...)
{
    char        time_now[20] = { '\0' };
    FILE        *fp = NULL;
    va_list     list;
    time_t      now;
    struct tm   *tmp;
    char        singleLineBuffer[UT_LOG_MAX_LINE_SIZE+1]={0};
    size_t      lineSize;

    time(&now);

    if ( gLogInit == false )
    {
        /* If ut-core has never called to set the log to a fixed location then let's set it */
        UT_log_setLogFilePath(UT_LOG_DEFAULT_PATH);
    }

    /* #FIXME : This will need rework, we shouldn't be opening logs constantly */
    fp = fopen(gLogFileName, "a");
    if (fp == NULL)
    { 
        printf("\nUnable to open file for logging...");
        return;
    }

    tmp = localtime(&now);
    strftime(time_now, sizeof(time_now), "%Y-%m-%d-%X", tmp);
#if 1
    snprintf( singleLineBuffer, UT_LOG_MAX_LINE_SIZE, "\n%s, %*s, %s,%6d : ", time_now, 16, prefix, basename((char *)file), line );
#else
    file=file;  /* unused */
    snprintf( singleLineBuffer, UT_LOG_MAX_LINE_SIZE, "\n%s, %s, %6d : ", time_now, prefix, line );
#endif
    lineSize = strlen( singleLineBuffer );

    va_start(list, format);
    vsnprintf(&singleLineBuffer[lineSize], UT_LOG_MAX_LINE_SIZE-lineSize-1, format, list);
    va_end(list);

    /* Print the line to stdout */
    printf( "%s", singleLineBuffer );

    /* Print the data to the stream */
    fprintf( fp, "%s", UT_stripColorCode(singleLineBuffer));
    fclose(fp);
}

static char* UT_stripColorCode(char *in_string)
{
    int i = 0, j = 0;
    while (in_string[i])
    {
        if (in_string[i] == '\033')
        {
            while (in_string[i] != 'm' && in_string[i])
            {
                i++;
            }
            if (in_string[i] == '\0')
            {
                break;
            }
            i++;
        }
        else
        {
            in_string[j++] = in_string[i++];
        }
    }
    in_string[j] = '\0';
    return in_string;
}
