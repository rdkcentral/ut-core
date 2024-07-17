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

/**
 * @file ut_internal.h
 * @brief Internal header file for the Unit Testing (UT) framework.
 *
 * This header defines common types, enums, and function prototypes used internally by the UT framework.
 */

/** @addtogroup UT
 *  Unit Testing Framework
 *  @{
 */

#ifndef __UT_INTERNAL_H
#define __UT_INTERNAL_H

#include <string.h>

#define UT_MAX_FILENAME_STRING_SIZE (32)
#define MAX_OPTIONS 50
#define MAX_GROUPS 100
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

/**
 * @brief Enumerates the different testing modes supported by the UT framework.
 */
typedef enum
{
    UT_MODE_BASIC=0,    /**< Basic Mode: Runs all tests and outputs results to the console. */
    UT_MODE_AUTOMATED,  /**< Automated Mode: Runs tests without console output, generates xUnit XML report. */
    UT_MODE_CONSOLE     /**< Console Mode: Runs tests and interacts with the user through the console. */
} TestMode_t;

/**
 * @brief Structure to hold configuration options and flags for the UT framework.
 */

typedef struct {
    int d_values[MAX_OPTIONS]; /**< Flag to hold value of optargs -d> */
    int d_count;               /**< Flag to hold index of optargs -d> */
    int e_values[MAX_OPTIONS]; /**< Flag to hold value of optargs -e> */
    int e_count;               /**< Flag to hold index of optargs -e> */
} groupFlag_t;

typedef struct
{
    TestMode_t  testMode; /**< The selected testing mode. */
    bool        listTest; /**< Flag to indicate whether to list available tests. */
    char        filenameRoot[UT_MAX_FILENAME_STRING_SIZE]; /**< Base filename for test results. */
    char        filename[UT_MAX_FILENAME_STRING_SIZE];  /**< Full filename for test results. */
    bool        help; /**< Flag to indicate whether to display help information. */
    groupFlag_t groupFlag; /**< Group flag info> */
} optionFlags_t;

#define TEST_INFO(x) printf x; // Consider replacing with a more robust logging mechanism

/**
 * @brief Sets the output file name for the test summary results.
 *
 * @param szFilenameRoot The base filename (including path) for the results file.
 */
extern void UT_set_results_output_filename(const char* szFilenameRoot);

/**
 * @brief Sets the testing mode for the UT framework.
 *
 * @param mode The desired testing mode.
 */
extern void UT_set_test_mode(TestMode_t mode);

/**
 * @brief Enables or disables suites based on group Ids
 *
 * @param flag flag to enable or disable suites
 * @param groupID group ID to determine the suite which needs to be enabled or disabled
 */
extern void UT_enable_disable_suites_with_groupIDs(bool flag, UT_groupID_t groupId);

/**
 * @brief Sets the groupFlag_t structure values
 *
 * @param groupFlag flag with values from optarg
 */
extern void UT_set_option_value(groupFlag_t* groupFlag);

/**
 * @brief Deactivate all suites with group ids
 *
 */
extern void UT_deactivate_suites_with_group_ids();

#endif  /*  __UT_INTERNAL_H  */
/** @} */ // End of UT group
