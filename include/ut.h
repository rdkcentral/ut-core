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

/** @brief
 * UT Unit testing framework to test code.
 * 
 * Application wrapper for Testing Suite Unit Testing Framework
 *  - This wrapper exists in order to swap Unit testing suites as required
 * Order of calling to produce testing results
 * 
 * - client main(int argc, char **argv )
 * -        calls ut_init(argc,argv)
 * - client register test SUITE with ut_test_suite_add
 * - client register test into the SUITE with ut_test_add
 * - client calls ut_run_tests()
 * - results returned to client 
 */
/** @addtogroup UT
 * @{
 */

#ifndef __UT_H
#define __UT_H

#include <string.h>

#ifdef UT_CUNIT
#include <ut_cunit.h>
#endif

/**!
 * @brief Status codes for the unit testing (UT) framework.
 *
 * @note This enum will be extended with more specific error codes in future updates.
 */
typedef enum
{
    UT_STATUS_OK = 0,      /**!< Test case or operation completed successfully. */
    UT_STATUS_FAILURE,    /**!< A general failure occurred. */
} UT_status_t;

typedef enum
{
    UT_TESTS_L1 = 1,   /*!<  Level 1 basic tests are expected to be in this group */
    UT_TESTS_L2,       /*!<  Level 2 advanced tests are expected to be in this group */
    UT_TESTS_L3,       /*!<  Level 3 Module tests are expected to be in this group */
    UT_TESTS_L4,       /*!<  Level 4 Module Control functions (e.g. start module/ stop module ), will not be ran as a testing suite */
    UT_TESTS_HUMAN_L2, /*!<  Level 2 Suite Requires Human interaction */
    UT_TESTS_HUMAN_L3, /*!<  Level 3 Suite Requires Human interaction */
    UT_TESTS_HUMAN_L4, /*!<  Level 4 Suite Requires Human interaction */
    UT_TESTS_MAX       /*!<  Out of range marker (not a valid status). */
} UT_groupID_t;

/* 
* TODO (Error Handling Enhancement):
*   - Extend `UT_status_t` enum with more specific error codes for:
*       - Assertion failures (e.g., `UT_ASSERT_EQUAL_FAILED`, `UT_ASSERT_TRUE_FAILED`)
*       - Setup or teardown errors (e.g., `UT_SETUP_FAILED`, `UT_TEARDOWN_FAILED`)
*       - Other framework-specific issues (e.g., invalid test configuration)
 */ 


/* Typedefs */

typedef void (UT_test_suite_t);   /*!< UT Test Suite Object Pointer */
typedef void (UT_test_t);         /*!< UT test object pointer */

/* Typedefs */

/**! Pointer to a UT Test Suite object. */
typedef void (UT_test_suite_t);

/**! Pointer to a UT test object. */
typedef void (UT_test_t);

/**! Function pointer for a unit test case. */
typedef void (*UT_TestFunction_t)(void);

/**! Function pointer for a unit test case cleanup routine.
 * @returns An integer status code.
 */
typedef int (*UT_TestCleanupFunction_t)(void);

/**! Function pointer for initializing a unit test suite.
 * @returns An integer status code.
 */
typedef int (*UT_InitialiseFunction_t)(void);

/**! Function pointer for cleaning up a unit test suite.
 * @returns An integer status code.
 */
typedef int (*UT_CleanupFunction_t)(void);

/* *********************/
/* Function Prototypes */
/* *********************/

/**!
 * @brief Initializes the unit testing (UT) framework.
 *
 * This function must be called from the main function of the test suite. 
 *
 * Responsibilities:
 *   - Parses command-line arguments for test configuration.
 *   - Initializes the UT framework for subsequent test execution.
 *
 * @param[in] argc - Number of command-line arguments.
 * @param[in] argv - Array of command-line argument strings.
 * 
 * @returns Status of the initialization.
 * @retval UT_STATUS_OK - Initialization successful.
 * @retval UT_STATUS_FAILURE - Initialization failed.
 *
 * @see UT_run_tests() for running the registered test cases.
 */
UT_status_t UT_init(int argc, char** argv);

/**! 
 * @brief Exits the unit testing framework and releases resources. 
 * */
void UT_exit(void);

/**!
 * @brief Runs all registered test cases.
 * 
 * @returns Status of the test execution.
 * @retval UT_STATUS_OK - All tests passed.
 * @retval UT_STATUS_FAILURE - One or more tests failed.
 */
UT_status_t UT_run_tests( void );

/**!
 * @brief Registers a test suite with the unit testing framework.
 * 
 * @param[in] pTitle - Name of the test suite.
 * @param[in] pInitFunction - Optional initialization function for the suite (can be NULL).
 * @param[in] pCleanupFunction - Optional cleanup function for the suite (can be NULL).
 * @returns Handle to the created test suite, or NULL on error.
 */
UT_test_suite_t *UT_add_suite(const char *pTitle, UT_InitialiseFunction_t pInitFunction, UT_CleanupFunction_t pCleanupFunction);

/**!
 * @brief Registers a test suite with the unit testing framework based on the group ID
 *
 * @param[in] pTitle - Name of the test suite.
 * @param[in] pInitFunction - Optional initialization function for the suite (can be NULL).
 * @param[in] pCleanupFunction - Optional cleanup function for the suite (can be NULL).
 * @param[in] groupId - Group ID of the test from enum UT_groupID_t.
 * @returns Handle to the created test suite, or NULL on error.
 */
UT_test_suite_t *UT_add_suite_withGroupID( const char *pTitle, UT_InitialiseFunction_t pInitFunction, UT_CleanupFunction_t pCleanupFunction,  UT_groupID_t groupId );

/**!
 * @brief Adds a test case to a suite.
 *
 * @param[in] pSuite - Handle to the test suite to add the test case to.
 * @param[in] pTitle - Name of the test case.
 * @param[in] pFunction - Function to be executed for this test case.
 * @returns Handle to the added test case, or NULL on error.
 */
UT_test_t *UT_add_test( UT_test_suite_t *pSuite, const char *pTitle, UT_TestFunction_t pFunction);

/**!
 * @brief Adds a test case to a suite with group ID
 *
 * @param[in] pSuite - Handle to the test suite to add the test case to.
 * @param[in] pTitle - Name of the test case.
 * @param[in] pFunction - Function to be executed for this test case.
 * @returns Handle to the added test case, or NULL on error.
 */
UT_test_t *UT_add_test_autoCaseID( UT_test_suite_t *pSuite, const char *pTitle, UT_TestFunction_t pFunction);

/**!
 * @brief Retrieves the title of a test suite.
 *
 * @param pSuite - Pointer to the test suite.
 * @returns Title of the test suite, or NULL if the suite is not found.
 */
const char *UT_getTestSuiteTitle( UT_test_suite_t *pSuite );

/**!
 * @brief Registers a cleanup function for a test suite.
 *
 * The cleanup function will be called after all tests in the suite have finished running.
 *
 * @param[in] pSuite - Handle to the test suite.
 * @param[in] pFunction - Cleanup function to register.
 */
void UT_regsiter_test_cleanup_function( UT_test_suite_t *pSuite, UT_TestCleanupFunction_t pFunction); 


#endif  /*  __UT_H  */
/** @} */
