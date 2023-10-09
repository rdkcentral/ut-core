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
#include <ut_log.h>

// the configured options and settings
#define UT_CORE_VERSION "1.0.0"

/**
 * @brief Status codes to support the UT system
 * 
 * @note in the future these error codes will be expanded
 */

typedef enum
{
    UT_STATUS_OK=0,    /*!< Success / OK Results */
    UT_STATUS_FAILURE  /*!< General failure */
}UT_status_t;

/* Typedefs */

typedef void (UT_test_suite_t);   /*!< hts_test_suite object pointer */
typedef void (UT_test_t);         /*!< UT test object pointer */

/**
 * @brief UT Test Function
 * 
 */
typedef void (*UT_TestFunction_t)(void);

/**
 * @brief UT Test Init Function
 * 
 */
typedef int (*UT_InitialiseFunction_t)(void);

/**
 * @brief UT Test Clean up function
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
 * @param[in] pInitFunction - init function if required, or NULL otherwise
 * @param[in] pCleanupFunction - clean function if required or NULL otherwise
 
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

#define UT_CUNIT

#ifdef UT_CUNIT 

#include <TestRun.h>
#include <CUnit.h>

/**
 * @brief Cause to pass always
 * 
 * @param msg - message to display
 * @note Test will continue to process
 * 
 */
#define UT_PASS(msg) \
    UT_LOG( "UT_PASS(" #msg ")" ); \
    CU_PASS(msg);

/**
 * @brief Cause a test to failure always & continue processing
 * @param msg - mesage to display
 * @note Test will continue to process
 */
#define UT_FAIL(msg) \
    UT_LOG( ("UT_FAIL(" #msg")") ); \
    CU_FAIL(msg);

/**
 * @brief Cause a test to fail
 * @param msg - mesage to display
 * @note Test will exit
 * 
 */
#define UT_FAIL_FATAL(msg) \
    UT_LOG( ("UT_FAIL_FATAL(" #msg")") ); \
    CU_FAIL_FATAL(msg);

/**
 * @brief Asset if the expression is true
 * 
 * @param value - expression to evaluate
 */
#define UT_ASSERT(value) \
    if ( !(value) ) \
    { \
        UT_LOG( ("UT_ASSERT(" #value")") ); \
    } \
    CU_ASSERT_FATAL(value);

/**
 * @brief Asset if the pointers passed are equal
 * 
 * @param value - pointer value actual
 * @param expected - pointer value expected
 */
#define UT_ASSERT_PTR_EQUAL(actual, expected) \
    if ( ((const void*)(actual) != (const void*)(expected)) ) \
    { \
        UT_LOG( ("UT_ASSERT_PTR_EQUAL(" #actual ","  #expected ")") ); \
    } \
    CU_ASSERT_PTR_EQUAL_FATAL(actual, expected);

/**
 * @brief Asset if the pointers passed NOT equal
 * 
 * @param value - pointer value actual
 * @param expected - pointer value expected
 */
#define UT_ASSERT_PTR_NOT_EQUAL(actual, expected) \
    if ( ((const void*)(actual) == (const void*)(expected)) ) \
    { \
        UT_LOG( ("UT_ASSERT_PTR_NOT_EQUAL(" #actual ","  #expected ")") ); \
    } \
    CU_ASSERT_PTR_NOT_EQUAL_FATAL(actual, expected);

/**
 * @brief Asset if the pointer value is NULL
 * 
 * @param value - pointer value
 */
#define UT_ASSERT_PTR_NULL(value)  \
    if ( (NULL != (const void*)(value)) ) \
    { \
        UT_LOG( ("UT_ASSERT_PTR_NULL(" #value")") ); \
    } \
    CU_ASSERT_PTR_NULL_FATAL(value);

/**
 * @brief Asset if the pointer value is NOT NULL
 * 
 * @param value - pointer value
 */
#define UT_ASSERT_PTR_NOT_NULL(value) \
    if ( (NULL == (const void*)(value)) ) \
    { \
        UT_LOG(("UT_ASSERT_PTR_NOT_NULL(" #value")")); \
    } \
    CU_ASSERT_PTR_NOT_NULL_FATAL(value);

/**
 * @brief Asset if the expression is true
 * 
 * @param value -  expression to evaluate
 */
#define UT_ASSERT_TRUE(value)  \
    if ( !(value) ) \
    { \
        UT_LOG( ("UT_ASSERT_TRUE(" #value")") ); \
    } \
    CU_ASSERT_TRUE_FATAL(value);

/**
 * @brief Asset if the expression is false
 * 
 * @param value -  expression to evaluate
 */
#define UT_ASSERT_FALSE(value) \
    if ( (value) ) \
    { \
        UT_LOG( ("UT_ASSERT_FALSE(" #value")") ); \
    } \
    CU_ASSERT_FALSE_FATAL(value);

/**
 * @brief Asset if the two expressions are equal
 * 
 * @param actual -  actual expression
 * @param expected -  expected expression
 */
#define UT_ASSERT_EQUAL(actual,expected) \
    if ( ((actual) != (expected)) ) \
    { \
        UT_LOG( ("UT_ASSERT_EQUAL(" #actual ","  #expected ")") ); \
    } \
    CU_ASSERT_EQUAL_FATAL(actual,expected);

/**
 * @brief Asset if the two expressions are NOT equal
 * 
 * @param actual -  actual expression
 * @param expected -  expected expression
 */
#define UT_ASSERT_NOT_EQUAL(actual,expected) \
    if ( ((actual) == (expected)) ) \
    { \
        UT_LOG(("UT_ASSERT_NOT_EQUAL(" #actual ","  #expected ")")); \
    } \
    CU_ASSERT_NOT_EQUAL_FATAL(actual,expected);

/**
 * @brief Asset if the strings are equal
 * 
 * @param actual -  actual string
 * @param expected -  expected string
 */
#define UT_ASSERT_STRING_EQUAL(expected, actual) \
    if ( (strcmp((const char*)(actual), (const char*)(expected))) ) \
    { \
        UT_LOG(("UT_ASSERT_STRING_EQUAL(" #actual ","  #expected ")") ); \
    } \
    CU_ASSERT_STRING_EQUAL_FATAL(actual,expected);

/**
 * @brief Asset if the strings are NOT equal
 * 
 * @param actual -  actual string
 * @param expected -  expected string
 */
#define UT_ASSERT_STRING_NOT_EQUAL(expected, actual) \
    if ( !(strcmp((const char*)(actual), (const char*)(expected))) ) \
    { \
        UT_LOG(("UT_ASSERT_STRING_NOT_EQUAL(" #actual ","  #expected ")") ); \
    } \
    CU_ASSERT_STRING_NOT_EQUAL_FATAL(actual,expected);

/**
 * @brief UT Assert with Message on failure
 * 
 * @param[in] value - the value to check 
 * @param[in] message - message to log, if the assert fails
 * 
 */
#define UT_ASSERT_MSG(value, message) \
    if ( !(value) ) \
    { \
        UT_LOG( ("UT_ASSERT_MSG( [" #value "],[" #message "])")  ); \
    } \
    CU_ASSERT_FATAL(value);

/**
 * @brief UT Assert True with Message on failure
 * 
 * @param[in] value - expression value to check 
 * @param[in] message - message to log, if the expression is true
 * 
 */
#define UT_ASSERT_TRUE_MSG(value, message) \
    if ( !(value) ) \
    { \
        UT_LOG( ("UT_ASSERT_TRUE_MSG( ["#value"],[" #message"])") ); \
    } \
    CU_ASSERT_TRUE_FATAL(value);

/**
 * @brief UT Assert True with Message on failure
 * 
 * @param[in] value - the value to check 
 * @param[in] message - message to log, if the expression is false
 * 
 */
#define UT_ASSERT_FALSE_MSG(value, message) \
    if ( (value) ) \
    { \
        UT_LOG( ("UT_ASSERT_FALSE_MSG( ["#value"],[" #message"])")  ); \
    } \
    CU_ASSERT_FALSE_FATAL(value);

/**
 * @brief 
 * 
 */
/**
 * @brief UT Assert with Log always
 * 
 * @param[in] value - the value to check 
 * @param[in] message - message to log, will always log
 * 
 */
#define UT_ASSERT_LOG(value, message) \
    UT_LOG( ("UT_ASSERT_LOG( ["#value"],[" #message"])") ); \
    UT_ASSERT(value);

#endif  /* UT -> CUNIT - Wrapper */

#endif  /*  __UT_H  */

/** @} */
