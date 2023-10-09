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

#include "ut.h"
#include "ut_log.h"

#ifdef UT_CUNIT
/* CUnit functions */
#include <CUnit.h>
#include <Basic.h>
#include <Console.h>
#include <Automated.h>
#endif

#define TEST_INFO(x) printf x;
#define TRUE  (true)
#define FALSE (false)
#define MAX_STRING_SIZE (32)
#define DEFAULT_FILENAME "wifi_hal"

typedef enum
{
    UT_MODE_BASIC=0,
    UT_MODE_AUTOMATED,
    UT_MODE_CONSOLE
}TestMode_t;

typedef struct
{
    /* option switches */
    TestMode_t  testMode;
    bool        listTest;
    char        filenameRoot[MAX_STRING_SIZE];
    bool        help;
}optionFlags_t;

/* Global variables */
static optionFlags_t gOptions;  /*!< Control flags */
static int gRegisterFailed;     /*!< Global Registration failed counter */

/* Function prototypes */
static UT_status_t startup_system( void );
static int internalInit( void );
static int internalClean( void );

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

    memset(&gOptions,0,sizeof(gOptions));

    gOptions.testMode = UT_MODE_CONSOLE;
    strcpy( gOptions.filenameRoot, DEFAULT_FILENAME );
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
                gOptions.listTest = TRUE;
                break;
            case 'f':
                TEST_INFO(("Automated Mode: Set Output File Prefix\n"));
                gOptions.testMode = UT_MODE_AUTOMATED;
                strncpy(gOptions.filenameRoot,optarg,MAX_STRING_SIZE);
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

    return TRUE;
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
    if ( decodeOptions( argc, argv ) == FALSE )
    {
        return UT_STATUS_FAILURE;
    }

    if ( startup_system() != UT_STATUS_OK )
    {
        return UT_STATUS_FAILURE;
    }
 
    return UT_STATUS_OK;
}

#ifdef UT_CUNIT

/**
 * @brief Startup the system
 * 
 * This function will startup the system based on the CUNIT requirements
 * 
 */
static UT_status_t startup_system( void )
{
    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
    {
        int result;
        result = CU_get_error();

        // @FIXME: Check the return status and return a corrected error code
        result = result;
        return UT_STATUS_FAILURE;
    }
    return UT_STATUS_OK;
}

/**
 * @brief run the registered tests as required
 * 
 * For the moment this function will currently output the cunit
 * result format, this will move to xUnit format as required
 * or an UT internal format
 */
UT_status_t UT_run_tests( void )
{
    CU_ErrorCode error;
    
    /* If any registration failed then stop here */
    if ( gRegisterFailed != 0 )
    {
        CU_cleanup_registry();
        error = CU_get_error();
        if ( error != CUE_SUCCESS )
        {
            return UT_STATUS_FAILURE;
        }
        return UT_STATUS_OK;
    }

    CU_set_output_filename(gOptions.filenameRoot);
    UT_LOG( UT_LOG_GREEN"---- start of test run ----\n"UT_LOG_NC );
    switch( gOptions.testMode )
    {
        case UT_MODE_BASIC:
        {
            /* Run all tests using the CUnit Basic interface */
            CU_basic_set_mode(CU_BRM_VERBOSE);
            CU_basic_run_tests();
            TEST_INFO(("\nFailures:-\n"));
            CU_basic_show_failures(CU_get_failure_list());
            TEST_INFO(("\n\n"));
        }
        break;

        case UT_MODE_CONSOLE:
        {
            CU_console_run_tests();
        }
        break;

        case UT_MODE_AUTOMATED:
        {
            //CU_automated_enable_junit_xml( CU_TRUE );
            CU_automated_run_tests();
        }
        break;
    }

    if ( gOptions.listTest == true )
    {
        CU_list_tests_to_file();
    }

    CU_cleanup_registry();
    error = CU_get_error();

    /* #BUG: There's a bug here to be investigated, the suites are not counting as failed when tests fail.*/
    /* #TODO: Should display message PASS / FAIL depending on the flag state */
    UT_LOG( UT_LOG_GREEN"Logfile"UT_LOG_NC":["UT_LOG_YELLOW"%s"UT_LOG_NC"]",UT_log_getLogFilename() );
    if ( error != CUE_SUCCESS )
    {
        // #TODO: Latest date to upgrade to add a translate error function
        UT_LOG( UT_LOG_GREEN"---- end of test run ----\n"UT_LOG_NC ); 
        return UT_STATUS_FAILURE;
    }

    UT_LOG( UT_LOG_GREEN"---- end of test run ----\n"UT_LOG_NC );

    return UT_STATUS_OK;
}

UT_test_suite_t *UT_add_suite(const char *pTitle, UT_InitialiseFunction_t pInitFunction, UT_CleanupFunction_t pCleanupFunction)
{
    CU_pSuite pSuite;

    if ( pInitFunction == NULL )
    {
        pInitFunction = &internalInit;
    }

    if ( pCleanupFunction == NULL )
    {
        pCleanupFunction = &internalClean;
    }

    if ( pTitle == NULL )
    {
        gRegisterFailed++;
        return NULL;
    }

    pSuite = CU_add_suite(pTitle, (CU_InitializeFunc)pInitFunction, (CU_CleanupFunc)pCleanupFunction);

    if ( pSuite == NULL )
    {
        gRegisterFailed++;
    }

    return (UT_test_suite_t *)pSuite;
}

UT_test_t *UT_add_test(UT_test_suite_t *pSuite, const char *pTitle, UT_TestFunction_t pTestFunction)
{
    CU_pTest pTest;
    
    pTest = CU_add_test((CU_pSuite)pSuite, pTitle, (CU_TestFunc)pTestFunction);

    if ( pTest == NULL )
    {
        gRegisterFailed++;
    }

    return (UT_test_t *)pTest;
}

#else // UT_CUNIT

static UT_status_t system_startup( void )
{
    return UT_STATUS_FAILURE; 
}

void UT_run_tests( void )
{

}

UT_test_suite_t *UT_add_suite(const char *pTitle, UT_InitialiseFunction_t pInitFunction, UT_CleanupFunction_t pCleanupFunction)
{
    title=title;
    pInitFunction=pInitFunction;
    pCleanupFunction=pCleanupFunction;
    return NULL;
}

UT_test_t *UT_add_test(UT_test_suite_t *pSuite, const char *pTitle, UT_TestFunction_t pTestFunction)
{
    title=title;
    pSuite=pSuite;
    pTestFunction=pTestFunction;
    return NULL;
}

#endif

static int internalInit( void )
{
    return 0;
}

static int internalClean( void )
{
    return 0;
}


