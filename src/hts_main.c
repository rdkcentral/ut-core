/**
 * @file main.c
 * @author Gerald Weatherup (gerald.weatherup@sky.uk)
 * @brief Main control application for wifi unit testing
 * @version 0.1
 * @date 2022-05-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Stdlib includes */
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

#include "hts.h"

#define TEST_INFO(x) printf x;
#define TRUE  (true)
#define FALSE (false)
#define MAX_STRING_SIZE (32)
#define DEFAULT_FILENAME "wifi_hal"

typedef enum
{
    TEST_MODE_BASIC=0,
    TEST_MODE_AUTOMATED,
    TEST_MODE_CONSOLE
}TestMode_t;

typedef struct
{
    /* option switches */
    TestMode_t  testMode;
    bool        listTest;
    char        filenameRoot[MAX_STRING_SIZE];
    bool        help;
}optionFlags_t;

static optionFlags_t gOptions;

static void usage( void )
{
    TEST_INFO(( "-c - Console Mode (Default)\n" ));
    TEST_INFO(( "-a - Automated Mode\n" ));
    TEST_INFO(( "-b - Basic Mode\n" ));
    TEST_INFO(( "-f - <filename> - set the output filename for automated mode\n" ));
    TEST_INFO(( "-l - List all tests run to a file\n" ));
    TEST_INFO(( "-h - Help\n" ));
}

static bool decodeOptions( int argc, char **argv )
{
    int opt;


    memset(&gOptions,0,sizeof(gOptions));

    gOptions.testMode = TEST_MODE_CONSOLE;
    strcpy( gOptions.filenameRoot, DEFAULT_FILENAME );
    while((opt = getopt(argc, argv, "cabhf:l")) != -1)
    {
        switch(opt)
        {
            case 'c':
                TEST_INFO(("Console Mode\n"));
                gOptions.testMode = TEST_MODE_CONSOLE;
                break;
            case 'b':
                TEST_INFO(("Basic Mode\n"));
                gOptions.testMode = TEST_MODE_BASIC;
                break;
            case 'a':
                TEST_INFO(("Automated Mode\n"));
                gOptions.testMode = TEST_MODE_AUTOMATED;
                break;
            case 'l':
                TEST_INFO(("Automated Mode: List Tests to File\n"));
                gOptions.listTest = TRUE;
                break;
            case 'f':
                TEST_INFO(("Automated Mode: Set Output File Prefix\n"));
                gOptions.testMode = TEST_MODE_AUTOMATED;
                strncpy(gOptions.filenameRoot,optarg,MAX_STRING_SIZE);
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

int main(int argc, char** argv) 
{
    /* Decode the options */
    if ( decodeOptions( argc, argv ) == FALSE )
    {
        return -1;
    }

    return 0;
}

