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
#include <ut_kvp_profile.h>
#include <ut_internal.h>


#define DEFAULT_FILENAME "ut_test"
#ifndef UT_VERSION
#define UT_VERSION "Not Defined"
#endif

/* Global variables */
static optionFlags_t gOptions;  /*!< Control flags, should not be exposed outside of this file */

const char* UT_LOG_DEFAULT_PATH = "/tmp/"; /*Converted to const char inorder to make it compatible with CPP compiler */

/* Function prototypes */

static void usage( void )
{
    TEST_INFO(( "-c - Console Mode (Default)\n" ));
    TEST_INFO(( "-a - Automated Mode\n" ));
    TEST_INFO(( "-b - Basic Mode\n" ));
    TEST_INFO(( "-d - <group id> - Disable Group \n" ));
    TEST_INFO(( "-e - <group id> - Enable Group \n" ));
    TEST_INFO(( "-t - List all tests run to a file\n" ));
    TEST_INFO(( "-l - Set the log Path\n" ));
    TEST_INFO(( "-p - <profile_filename> - specify the profile to load YAML or JSON, also used by kvp_assert\n" ));
    TEST_INFO(("- Group IDs are as below\n"));
    TEST_INFO(("- Group 1  : Level 1 basic tests are expected to be in this group\n"));
    TEST_INFO(("- Group 2  : Level 2 advanced tests are expected to be in this group\n"));
    TEST_INFO(("- Group 3  : Level 3 Module tests are expected to be in this group\n"));
    TEST_INFO(("- Group 4  : Level 4 Module Control functions (e.g. start module/ stop module), will not be ran as a testing suite\n"));
    TEST_INFO(("- Group 5  : Level 2 Suite Requires Human interaction\n"));
    TEST_INFO(("- Group 6  : Level 3 Suite Requires Human interaction\n"));
    TEST_INFO(("- Group 7  : Level 4 Suite Requires Human interaction\n"));
    TEST_INFO(("- Group 8  : Level 3 Suite for setup specific test and cannot be run on real device\n"));
    TEST_INFO(("- Group 9  : For currently existing suites\n"));
    TEST_INFO(( "-h - Help\n" ));
}

static bool decodeOptions( int argc, char **argv )
{
    int opt;
    int option_index = 0;
    ut_kvp_status_t status;

    memset(&gOptions,0,sizeof(gOptions));

    /* Console mode is always enabled we don't need a switch for that */
    gOptions.testMode = UT_MODE_CONSOLE;
    
    // Define long options
    static struct option long_options[] = {
        {"gtest_output", required_argument, 0, 0}, // 0 is used as a placeholder for gtest_output
        {0, 0, 0, 0} // Terminator
    };

    /* Set the filepath always by default to /tmp/ */
    UT_log_setLogFilePath((char* )UT_LOG_DEFAULT_PATH);
    UT_set_results_output_filename( UT_log_getLogFilename() );

    while ((opt = getopt_long(argc, argv, "cabhf:l:tp:d:e:", long_options, &option_index)) != -1)
    {
        switch(opt)
        {
            case 'b':
                TEST_INFO(("Basic Mode\n"));
                gOptions.testMode = UT_MODE_BASIC;
                break;
            case 'a':
                TEST_INFO(("Automated Mode\n"));
                gOptions.testMode = UT_MODE_AUTOMATED;
                break;
            case 't':
                TEST_INFO(("Automated Mode: List Tests to File\n"));
                gOptions.testMode = UT_MODE_AUTOMATED;
                gOptions.listTest = true;
                break;
            case 'l':
                TEST_INFO(("Setting Log Path [%s]\n", optarg));
                UT_log_setLogFilePath(optarg);
                UT_set_results_output_filename( UT_log_getLogFilename() );
                break;
            case 'd':
                if (atoi(optarg) >= UT_TESTS_MAX)
                {
                    TEST_INFO(("Invalid group [%d]\n", atoi(optarg)));
                    break;
                }
                TEST_INFO(("Disable group [%d]\n", atoi(optarg)));
                UT_Manage_Suite_Activation(atoi(optarg), false);
                break;
            case 'e':
                if (atoi(optarg) >= UT_TESTS_MAX)
                {
                    TEST_INFO(("Invalid group [%d]\n", atoi(optarg)));
                    break;
                }
                TEST_INFO(("Enable group [%d]\n", atoi(optarg)));
                UT_Manage_Suite_Activation(atoi(optarg), true);
                break;
            case 'p':
                TEST_INFO(("Using Profile[%s]\n", optarg));
                status = ut_kvp_profile_open(optarg);
                if ( status != UT_KVP_STATUS_SUCCESS )
                {
                    UT_LOG_ERROR("Failed to Load [%s]", optarg);
                }
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

    UT_set_test_mode(gOptions.testMode);
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
    printf("\nUT CORE Version: \033[0;32m%s\033[0m\n",UT_VERSION);

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

void UT_exit( void )
{
    ut_kvp_profile_close();
}
