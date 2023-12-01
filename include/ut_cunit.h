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
 * UT Unit wrapper to the cunit framework.
 * 
 * Hides the functionality of cunit 
 */
/** @addtogroup UT_CUNIT
 * @{
 */

#ifndef __UT_CUNIT_H
#define __UT_CUNIT_H

#include <TestRun.h>
#include <CUnit.h>
#include <ut_log.h>

/**
 * @brief Cause to pass always
 * 
 * @param msg - message to display
 * @note Test will continue to process
 * 
 */
#define UT_PASS(msg) \
    UT_LOG_PREFIX( UT_LOG_ASCII_GREEN"PASS  "UT_LOG_ASCII_NC , UT_LOG_ASCII_GREEN msg UT_LOG_ASCII_NC ); \
    CU_PASS(msg);

/**
 * @brief Cause a test to failure always & continue processing
 * @param msg - message to display
 * @note Test will continue to process
 */
#define UT_FAIL(msg) \
    UT_LOG_PREFIX( UT_LOG_ASCII_RED"FAIL  "UT_LOG_ASCII_NC, UT_LOG_ASCII_RED msg UT_LOG_ASCII_NC); \
    CU_FAIL(msg);

/**
 * @brief Cause a test to fail
 * @param msg - message to display
 * @note Test will exit
 * 
 */
#define UT_FAIL_FATAL(msg) \
    UT_LOG_PREFIX( UT_LOG_ASCII_RED"FAIL  "UT_LOG_ASCII_NC, UT_LOG_ASCII_RED msg UT_LOG_ASCII_NC ); \
    CU_FAIL_FATAL(msg);

/**
 * @brief Asset if the expression is true
 * 
 * @param value - expression to evaluate
 */
#define UT_ASSERT(value) \
    if ( !(value) ) \
    { \
        UT_LOG_ASSERT( UT_ASSERT , #value ); \
    } \
    CU_ASSERT(value);

/**
 * @brief Asset if the expression is true
 *  This function is fatal
 * @param value - expression to evaluate
 */
#define UT_ASSERT_FATAL(value) \
    if ( !(value) ) \
    { \
        UT_LOG_ASSERT( UT_ASSERT_FATAL , #value ); \
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
        UT_LOG_ASSERT( UT_ASSERT_PTR_EQUAL, #actual , #expected ); \
    } \
    CU_ASSERT_PTR_EQUAL(actual, expected);

/**
 * @brief Asset if the pointers passed are equal
 * 
 * @param value - pointer value actual
 * @param expected - pointer value expected
 */
#define UT_ASSERT_PTR_EQUAL_FATAL(actual, expected) \
    if ( ((const void*)(actual) != (const void*)(expected)) ) \
    { \
        UT_LOG_ASSERT( UT_ASSERT_PTR_EQUAL_FATAL , #actual , #expected ); \
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
        UT_LOG_ASSERT( UT_ASSERT_PTR_NOT_EQUAL, #actual ,  #expected ); \
    } \
    CU_ASSERT_PTR_NOT_EQUAL(actual, expected);

/**
 * @brief Asset if the pointers passed NOT equal
 * 
 * @param value - pointer value actual
 * @param expected - pointer value expected
 */
#define UT_ASSERT_PTR_NOT_EQUAL_FATAL(actual, expected) \
    if ( ((const void*)(actual) == (const void*)(expected)) ) \
    { \
        UT_LOG_ASSERT( UT_ASSERT_PTR_NOT_EQUAL_FATAL, #actual ,  #expected ); \
    } \
    CU_ASSERT_PTR_NOT_EQUAL_FATAL(actual, expected);

/**
 * @brief Asset if the pointer value is NULL
 * 
 * 
 * @param value - pointer value
 */
#define UT_ASSERT_PTR_NULL(value)  \
    if ( (NULL != (const void*)(value)) ) \
    { \
        UT_LOG_ASSERT( UT_ASSERT_PTR_NULL , #value ); \
    } \
    CU_ASSERT_PTR_NULL(value);

/**
 * @brief Asset if the pointer value is NULL
 * 
 * 
 * @param value - pointer value
 */
#define UT_ASSERT_PTR_NULL_FATAL(value)  \
    if ( (NULL != (const void*)(value)) ) \
    { \
        UT_LOG_ASSERT( UT_ASSERT_PTR_NULL_FATAL , #value ); \
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
        UT_LOG_ASSERT( UT_ASSERT_PTR_NOT_NULL, #value ); \
    } \
    CU_ASSERT_PTR_NOT_NULL(value);

/**
 * @brief Asset if the pointer value is NOT NULL
 * 
 * @param value - pointer value
 */
#define UT_ASSERT_PTR_NOT_NULL_FATAL(value) \
    if ( (NULL == (const void*)(value)) ) \
    { \
        UT_LOG_ASSERT( UT_ASSERT_PTR_NOT_NULL_FATAL, #value ); \
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
        UT_LOG_ASSERT( UT_ASSERT_TRUE, #value ); \
    } \
    CU_ASSERT_TRUE(value);

/**
 * @brief Asset if the expression is true
 * 
 * @param value -  expression to evaluate
 */
#define UT_ASSERT_TRUE_FATAL(value)  \
    if ( !(value) ) \
    { \
        UT_LOG_ASSERT( UT_ASSERT_TRUE_FATAL, #value ); \
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
        UT_LOG_ASSERT( UT_ASSERT_FALSE, #value ); \
    } \
    CU_ASSERT_FALSE(value);

/**
 * @brief Asset if the expression is false
 * 
 * @param value -  expression to evaluate
 */
#define UT_ASSERT_FALSE_FATAL(value) \
    if ( (value) ) \
    { \
        UT_LOG_ASSERT( UT_ASSERT_FALSE_FATAL , #value ); \
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
        UT_LOG_ASSERT( UT_ASSERT_EQUAL , #actual , #expected ); \
    } \
    CU_ASSERT_EQUAL(actual,expected);

/**
 * @brief Asset if the two expressions are equal
 * 
 * @param actual -  actual expression
 * @param expected -  expected expression
 */
#define UT_ASSERT_EQUAL_FATAL(actual,expected) \
    if ( ((actual) != (expected)) ) \
    { \
        UT_LOG_ASSERT( UT_ASSERT_EQUAL_FATAL , #actual , #expected ); \
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
        UT_LOG_ASSERT( UT_ASSERT_NOT_EQUAL, #actual , #expected ); \
    } \
    CU_ASSERT_NOT_EQUAL(actual,expected);

/**
 * @brief Asset if the two expressions are NOT equal
 * 
 * @param actual -  actual expression
 * @param expected -  expected expression
 */
#define UT_ASSERT_NOT_EQUAL_FATAL(actual,expected) \
    if ( ((actual) == (expected)) ) \
    { \
        UT_LOG_ASSERT( UT_ASSERT_NOT_EQUAL_FATAL, actual , expected ); \
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
        UT_LOG_ASSERT( UT_ASSERT_STRING_EQUAL, actual , expected ); \
    } \
    CU_ASSERT_STRING_EQUAL(actual,expected);

/**
 * @brief Asset if the strings are equal
 * 
 * @param actual -  actual string
 * @param expected -  expected string
 */
#define UT_ASSERT_STRING_EQUAL_FATAL(expected, actual) \
    if ( (strcmp((const char*)(actual), (const char*)(expected))) ) \
    { \
        UT_LOG_ASSERT( UT_ASSERT_STRING_EQUAL_FATAL, actual, expected ); \
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
        UT_LOG_ASSERT( UT_ASSERT_STRING_NOT_EQUAL , actual , expected ); \
    } \
    CU_ASSERT_STRING_NOT_EQUAL(actual,expected);

/**
 * @brief Asset if the strings are NOT equal
 * 
 * @param actual -  actual string
 * @param expected -  expected string
 */
#define UT_ASSERT_STRING_NOT_EQUAL_FATAL(expected, actual) \
    if ( !(strcmp((const char*)(actual), (const char*)(expected))) ) \
    { \
        UT_LOG_ASSERT( UT_ASSERT_STRING_NOT_EQUAL_FATAL , #actual , #expected ); \
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
        UT_LOG_ASSERT( UT_ASSERT_MSG , (value,message)  ); \
    } \
    CU_ASSERT(value);

/**
 * @brief UT Assert with Message on failure
 * 
 * @param[in] value - the value to check 
 * @param[in] message - message to log, if the assert fails
 * 
 */
#define UT_ASSERT_MSG_FATAL(value, message) \
    if ( !(value) ) \
    { \
        UT_LOG_ASSERT( UT_ASSERT_MSG_FATAL , (value,message)  ); \
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
        UT_LOG_ASSERT( UT_ASSERT_TRUE_MSG, (value,message) ); \
    } \
    CU_ASSERT_TRUE(value);

/**
 * @brief UT Assert True with Message on failure
 * 
 * @param[in] value - expression value to check 
 * @param[in] message - message to log, if the expression is true
 * 
 */
#define UT_ASSERT_TRUE_MSG_FATAL(value, message) \
    if ( !(value) ) \
    { \
        UT_LOG_ASSERT( UT_ASSERT_TRUE_MSG_FATAL, (value,message) ); \
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
        UT_LOG_ASSERT( UT_ASSERT_FALSE_MSG , (value,message)  ); \
    } \
    CU_ASSERT_FALSE(value);

/**
 * @brief UT Assert True with Message on failure
 * 
 * @param[in] value - the value to check 
 * @param[in] message - message to log, if the expression is false
 * 
 */
#define UT_ASSERT_FALSE_MSG_FATAL(value, message) \
    if ( (value) ) \
    { \
        UT_LOG_ASSERT( UT_ASSERT_FALSE_MSG_FATAL, (value,message)  ); \
    } \
    CU_ASSERT_FALSE_FATAL(value);

/**
 * @brief UT Assert with Log always
 * 
 * @param[in] value - the value to check 
 * @param[in] message - message to log, will always log
 * 
 */
#define UT_ASSERT_LOG(value, message) \
    if ( !(value) ) \
    { \
        UT_LOG_ASSERT( UT_ASSERT_LOG, (value,message) ); \
    } else { \
        UT_LOG_ASSERT( UT_ASSERT_LOG, (value,message) ); \
    } \
    CU_ASSERT(value);

/**
 * @brief UT Assert with Log always
 * 
 * @param[in] value - the value to check 
 * @param[in] message - message to log, will always log
 * 
 */
#define UT_ASSERT_LOG_FATAL(value, message) \
    if ( !(value) ) \
    { \
        UT_LOG_ASSERT( UT_ASSERT_LOG_FATAL, (value,message) ); \
    } else { \
        UT_LOG_ASSERT( UT_ASSERT_LOG_FATAL, (value,message) ); \
    } \
    CU_ASSERT_FATAL(value);

#endif  /* UT -> CUNIT - Wrapper */

/** @} */
