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

/* Stdlib includes */
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <assert.h>

#include <ut.h>
#include <ut_log.h>
#include "ut_internal.h"

#define DEFAULT_FILENAME "ut_test"


/* Global External Functions */
extern UT_status_t startup_system( void );

/* Global variables */
optionFlags_t gOptions;  /*!< Control flags */

/* Function prototypes */

static void usage( void )
{
    TEST_INFO(( "-c - Console Mode (Default)\n" ));
    TEST_INFO(( "-a - Automated Mode\n" ));
    TEST_INFO(( "-b - Basic Mode\n" ));
    TEST_INFO(( "-f - <filename> - set the output filename for automated mode\n" ));
    TEST_INFO(( "-l - List all tests run to a file\n" ));
    TEST_INFO(( "-p - Set the log Path\n" ));
    TEST_INFO(( "-h - Help\n" ));
}

static bool decodeOptions( int argc, char **argv )
{
    int opt;
    const char *logFilename;
    size_t length;

    memset(&gOptions,0,sizeof(gOptions));

    gOptions.testMode = UT_MODE_CONSOLE;

    /* Set the default path to ./ and then take the filename back and use that for automated mode */
    UT_log_setLogFilePath( "/tmp/" );

    logFilename = UT_log_getLogFilename();
    assert( logFilename != NULL );

    length = strlen(logFilename);
    length -= strlen(".log");
    strncpy( gOptions.filenameRoot, logFilename, length );

    while((opt = getopt(argc, argv, "cabhf:lp:")) != -1)
    {
        switch(opt)
        {
            case 'c':
                TEST_INFO(("Console Mode\n"));
                gOptions.testMode = UT_MODE_CONSOLE;
                break;
            case 'b':
                TEST_INFO(("Basic Mode\n"));
                gOptions.testMode = UT_MODE_BASIC;
                break;
            case 'a':
                TEST_INFO(("Automated Mode\n"));
                gOptions.testMode = UT_MODE_AUTOMATED;
                break;
            case 'l':
                TEST_INFO(("Automated Mode: List Tests to File\n"));
                gOptions.testMode = UT_MODE_AUTOMATED;
                gOptions.listTest = true;
                break;
            case 'f':
                TEST_INFO(("Automated Mode: Set Output File Prefix\n"));
                gOptions.testMode = UT_MODE_AUTOMATED;
                strncpy(gOptions.filenameRoot,optarg,UT_MAX_FILENAME_STRING_SIZE);
                break;
            case 'p':
                TEST_INFO(("Setting Log Path [%s]\n", optarg));
                UT_log_setLogFilePath(optarg);
                break;
            case 'h':
                TEST_INFO(("Help\n"));
                usage();
                exit(0);
                break;
            case '?':
            case ':':
                TEST_INFO(("unknown option: %c\n", optopt));
                break;
        }
    }

    // optind is for the extra arguments 
    // which are not parsed 
    for(; optind < argc; optind++)
    {
        TEST_INFO(("unknown arguments: %s\n", argv[optind]));
    }

    return true;
}

/**
 * @brief main UT init for the system
 * 
 * This function will also process switches from the command line
 * 
 * @param argc - argc from the command line input
 * @param argv - argv from the command line input
 * @return UT_status_t 
 * @retval UT_STATUS_OK - Success
 * @retval UT_STATUS_FAILURE - failure in decoding switches, or starting up the system
 */
UT_status_t UT_init(int argc, char** argv) 
{
    /* Decode the options */
    if ( decodeOptions( argc, argv ) == false )
    {
        return UT_STATUS_FAILURE;
    }

    if ( startup_system() != UT_STATUS_OK )
    {
        return UT_STATUS_FAILURE;
    }

    return UT_STATUS_OK;
}

