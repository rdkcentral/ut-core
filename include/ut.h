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

/**
 * @brief Status codes to support the UT system
 * 
 * @note in the future these error codes will be expanded
 */

typedef enum
{
    UT_STATUS_OK=0,     /*!< Success / OK Results */
    UT_STATUS_FAILURE,  /*!< General failure */
}UT_status_t;

/* Typedefs */

typedef void (UT_test_suite_t);   /*!< UT Test Suite Object Pointer */
typedef void (UT_test_t);         /*!< UT test object pointer */

/**
 * @brief UT Test Function
 * 
 */
typedef void (*UT_TestFunction_t)(void);

/**
 * @brief UT Test Clean up function
 * 
 */
typedef int (*UT_TestCleanupFunction_t)(void);

/**
 * @brief UT Test Suite Init Function
 * 
 */
typedef int (*UT_InitialiseFunction_t)(void);

/**
 * @brief UT Test Suite Clean up function
 * 
 */
typedef int (*UT_CleanupFunction_t)(void);

/* *********************/
/* Function Prototypes */
/* *********************/

/**
 * @brief parent UT-main
 * 
 * This function is to be called from the main of the upper suite
 * The upper suite should register tests as required, then call hts_main to decode param switches
 * 
 * @param[in] argc - argument count
 * @param[in] argv - argument list
 * 
 * @return UT_status_t
 * @retval UT_STATUS_OK - success
 * @retval UT_STATUS_FAILURE - general failure
 * @see UT_run_tests()
 */
UT_status_t UT_init(int argc, char** argv);

/**
 * @brief run the registered tests as required
 * In order for tests to be registered and ran the client will use
 * - UT_add_suite
 * - UT_add_test
 * @return UT_status_t
 * @retval UT_STATUS_OK - success
 * @retval UT_STATUS_FAILURE - general failure
 * @see UT_add_suite(),  UT_add_test()
 */
UT_status_t UT_run_tests( void );

/**
 * @brief Register a test suite with the system
 * 
 * @param[in] pTitle - name for the suite
 * @param[in] pInitFunction - suite init function if required, or NULL otherwise
 * @param[in] pCleanupFunction - suite clean function if required or NULL otherwise
 
 * @return UT_test_suite_t - test suite handle, NULL on error
*/
UT_test_suite_t *UT_add_suite( const char *pTitle, UT_InitialiseFunction_t pInitFunction, UT_CleanupFunction_t pCleanupFunction);

/**
 * @brief Register a test suite
 * 
 * @param[in] pTitle - pointer to the created suite
 * @param[in] pFunction - pointer to the test function
 * @return UT_test_t - pointer to the function handle, NULL on error
 */
UT_test_t *UT_add_test( UT_test_suite_t *pSuite, const char *pTitle, UT_TestFunction_t pFunction);

/**
 * @brief Get the test suite title from the suite name
 * 
 * @param pSuite - pointer to the suite to get the title from
 * @return const char* - string data, or null if unknown
 */
const char *UT_getTestSuiteTitle( UT_test_suite_t *pSuite );

/**
 * @brief Register a test suite cleanup function
 * 
 * @param[in] pFunction - pointer to the test function
 * @return UT_test_t - pointer to the function handle, NULL on error
 */
void UT_regsiter_test_cleanup_function( UT_test_suite_t *pSuite,  UT_TestCleanupFunction_t pFunction);

#endif  /*  __UT_H  */
/** @} */
