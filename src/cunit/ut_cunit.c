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

/* stdlib */
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <assert.h>

/* CUnit functions */
#include <CUnit.h>
#include <Basic.h>
#include <Console.h>
#include <Automated.h>
#include <MyMem.h>

#include <ut.h>
#include "ut_internal.h"
#include "ut_cunit_internal.h"

typedef struct
{
    CU_pSuite pSuite;
    UT_groupID_t groupId;
} UT_test_group_t;

typedef struct
{
    UT_test_group_t *groups[MAX_GROUPS];
    int count;
} UT_group_list_t;

UT_group_list_t group_list = {.count = 0};

/** Pointer to the currently running suite. */
static int gRegisterFailed;     /*!< Global Registration failed counter */
static TestMode_t  gTestMode;
static groupFlag_t gGroupFlag;

static int internalInit( void );
static int internalClean( void );
static void releaseGroups( void );

/**
 * @brief Startup the system
 * 
 * This function will startup the system based on the CUNIT requirements
 * 
 */
UT_status_t startup_system( void )
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
 * @brief sets the test mode from ut_main
 * 
 */
void UT_set_test_mode(TestMode_t  mode)
{
    gTestMode = mode;
}

/**
 * @brief gets the test mode
 *
 */
static TestMode_t get_test_mode( void )
{
    return gTestMode;
}

void UT_Manage_Suite_Activation(int groupID, bool enable_disable)
{
    if(gGroupFlag.group_flag_count > MAX_OPTIONS)
    {
        UT_LOG_ERROR("Too many -d or -e options\n");
        return;
    }

    int i = gGroupFlag.group_flag_count;
    gGroupFlag.group_value[i] = groupID;
    gGroupFlag.switch_value[i] = enable_disable;
    gGroupFlag.group_flag_count++;
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

    if(gGroupFlag.group_flag_count)
    {
        for(int i = 0; i < gGroupFlag.group_flag_count; i++)
        {
            UT_toggle_suite_activation_based_on_groupID(gGroupFlag.group_value[i], gGroupFlag.switch_value[i]);
        }
    }

    UT_LOG( UT_LOG_ASCII_GREEN"---- start of test run ----"UT_LOG_ASCII_NC );
    switch( get_test_mode() )
    {
        case UT_MODE_BASIC:
        {
            /* Run all tests using the Basic interface */
            UT_basic_run_tests();
        }
        break;

        case UT_MODE_CONSOLE:
        {
            UT_console_run_tests();
        }
        break;

        case UT_MODE_AUTOMATED:
        {
            UT_automated_enable_junit_xml( CU_TRUE );
            UT_automated_run_tests();
        }
        break;
    }

    CU_cleanup_registry();
    releaseGroups();
    error = CU_get_error();

    /* #BUG: There's a bug here to be investigated, the suites are not counting as failed when tests fail.*/
    /* #TODO: Should display message PASS / FAIL depending on the flag state */
    UT_LOG( UT_LOG_ASCII_GREEN"Logfile"UT_LOG_ASCII_NC":["UT_LOG_ASCII_YELLOW"%s"UT_LOG_ASCII_NC"]",UT_log_getLogFilename() );
    if ( error != CUE_SUCCESS )
    {
        // #TODO: Latest date to upgrade to add a translate error function
        UT_LOG( UT_LOG_ASCII_GREEN"---- end of test run ----\n"UT_LOG_ASCII_NC ); 
        return UT_STATUS_FAILURE;
    }

    UT_LOG( UT_LOG_ASCII_GREEN"---- end of test run ----\n"UT_LOG_ASCII_NC );

    UT_exit();

    return UT_STATUS_OK;
}

UT_test_suite_t *UT_add_suite(const char *pTitle, UT_InitialiseFunction_t pInitFunction, UT_CleanupFunction_t pCleanupFunction)
{
    return UT_add_suite_withGroupID(pTitle, pInitFunction, pCleanupFunction, UT_TESTS_UNKNOWN);
}

UT_test_suite_t *UT_add_suite_withGroupID( const char *pTitle, UT_InitialiseFunction_t pInitFunction, UT_CleanupFunction_t pCleanupFunction,  UT_groupID_t groupId )
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

    if (group_list.count >= MAX_GROUPS)
    {
        gRegisterFailed++;
        return NULL;
    }

    pSuite = CU_add_suite(pTitle, (CU_InitializeFunc)pInitFunction, (CU_CleanupFunc)pCleanupFunction);

    if ( pSuite == NULL )
    {
        gRegisterFailed++;
    }

    UT_test_group_t* newGroup = (UT_test_group_t*)malloc(sizeof(UT_test_group_t));
    if (newGroup == NULL) {
        gRegisterFailed++;
        return NULL;
    }

    newGroup->pSuite = pSuite;
    newGroup->groupId = groupId;

    group_list.groups[group_list.count++]=newGroup;

    return (UT_test_suite_t *)pSuite;

}

void UT_toggle_suite_activation_based_on_groupID(UT_groupID_t groupId, bool enable_disable)
{
    if (groupId >= UT_TESTS_MAX)
    {
        return;
    }

    for (int i = 0; i < group_list.count; ++i)
    {
        if (group_list.groups[i]->groupId == groupId)
        {
            CU_set_suite_active(group_list.groups[i]->pSuite, (CU_BOOL)enable_disable);
        }
    }
}

void UT_toggle_all_suites(bool enable_disable)
{
    for (int i = 0; i < group_list.count; ++i)
    {
        CU_set_suite_active(group_list.groups[i]->pSuite, (CU_BOOL) enable_disable);
    }
}

UT_test_t *UT_add_test(UT_test_suite_t *pSuite, const char *pTitle, UT_TestFunction_t pTestFunction)
{
    CU_pTest pTest;
    
    pTest = CU_add_test((CU_pSuite)pSuite, pTitle, (CU_TestFunc)pTestFunction);
    /* In our case this should never happen */
    assert( pTest != NULL );
    if ( pTest == NULL )
    {
        gRegisterFailed++;
    }

    return (UT_test_t *)pTest;
}

const char *UT_getTestSuiteTitle( UT_test_suite_t *pSuite )
{
    CU_pTest pTest;

    pTest = (CU_pTest)pSuite;

    return pTest->pName;
}

static int internalInit( void )
{
    return 0;
}

static int internalClean( void )
{
    return 0;
}

static void releaseGroups( void )
{
    for (int i = 0; i < group_list.count; ++i)
    {
        free(group_list.groups[i]);
    }
    group_list.count = 0;
}


