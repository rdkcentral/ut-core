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
 * @brief Cause to test to pass always & continue processing
 * 
 * @param msg - message to display
 * @note Test will continue to process
 * 
 */
#define UT_PASS(msg)                                                                                    \
    UT_LOG_PREFIX(UT_LOG_ASCII_GREEN "PASS  " UT_LOG_ASCII_NC, UT_LOG_ASCII_GREEN msg UT_LOG_ASCII_NC); \
    CU_PASS(msg);

/**
 * @brief Cause the test to failure always & continue processing
 * @param msg - message to display
 * @note Test will continue to process
 */
#define UT_FAIL(msg)                                                                                \
    UT_LOG_PREFIX(UT_LOG_ASCII_RED "FAIL  " UT_LOG_ASCII_NC, UT_LOG_ASCII_RED msg UT_LOG_ASCII_NC); \
    CU_FAIL(msg);

/**
 * @brief Cause a test to fail and exit
 * @param msg - message to display
 * @note Test will exit
 * 
 */
#define UT_FAIL_FATAL(msg)                                                                          \
    UT_LOG_PREFIX(UT_LOG_ASCII_RED "FAIL  " UT_LOG_ASCII_NC, UT_LOG_ASCII_RED msg UT_LOG_ASCII_NC); \
    CU_FAIL_FATAL(msg);

/**
 * @brief Asset(make sure) the expression evaluates to true, otherwise fail
 * 
 * @param value - expression to evaluate
 */
#define UT_ASSERT(value)                      \
    {                                         \
        typeof(value) _value = (value);                \
        if (!_value)                          \
        {                                     \
            UT_LOG_ASSERT(UT_ASSERT, #value); \
        }                                     \
        CU_ASSERT(_value);                    \
    }

/**
 * @brief Asset (make sure) the expression evaluates to true and fail otherwise and exit the test
 *  This function is fatal
 * @param value - expression to evaluate
 */
#define UT_ASSERT_FATAL(value)                      \
    {                                               \
        typeof(value) _value = (value);                      \
        if (!(_value))                              \
        {                                           \
            UT_LOG_ASSERT(UT_ASSERT_FATAL, #value); \
        }                                           \
        CU_ASSERT_FATAL(_value);                    \
    }

/**
 * @brief Asset (make sure) the pointers passed are equal, otherwise fail
 * 
 * @param value - pointer value actual
 * @param expected - pointer value expected
 */
#define UT_ASSERT_PTR_EQUAL(actual, expected)                       \
    {                                                               \
        const void *_actual = (const void *)(actual);               \
        const void *_expected = (const void *)(expected);           \
        if (_actual != _expected)                                   \
        {                                                           \
            UT_LOG_ASSERT(UT_ASSERT_PTR_EQUAL, #actual, #expected); \
        }                                                           \
        CU_ASSERT_PTR_EQUAL(_actual, _expected);                    \
    }

/**
 * @brief Assert(make sure) the pointers passed are equal, otherwise fail and exit the test
 * 
 * @param value - pointer value actual
 * @param expected - pointer value expected
 */
#define UT_ASSERT_PTR_EQUAL_FATAL(actual, expected)                       \
    {                                                                     \
        const void *_actual = (const void *)(actual);                     \
        const void *_expected = (const void *)(expected);                 \
        if (_actual != _expected)                                         \
        {                                                                 \
            UT_LOG_ASSERT(UT_ASSERT_PTR_EQUAL_FATAL, #actual, #expected); \
        }                                                                 \
        CU_ASSERT_PTR_EQUAL_FATAL(_actual, _expected);                    \
    }

/**
 * @brief Assert(make sure) the pointers passed NOT equal
 * 
 * @param value - pointer value actual
 * @param expected - pointer value expected
 */
#define UT_ASSERT_PTR_NOT_EQUAL(actual, expected)                       \
    {                                                                   \
        const void *_actual = (const void *)(actual);                   \
        const void *_expected = (const void *)(expected);               \
        if (_actual == _expected)                                       \
        {                                                               \
            UT_LOG_ASSERT(UT_ASSERT_PTR_NOT_EQUAL, #actual, #expected); \
        }                                                               \
        CU_ASSERT_PTR_NOT_EQUAL(_actual, _expected);                    \
    }

/**
 * @brief Assert(make sure) the pointers passed NOT equal, otherwise fail and exit the test
 * 
 * @param value - pointer value actual
 * @param expected - pointer value expected
 */
#define UT_ASSERT_PTR_NOT_EQUAL_FATAL(actual, expected)                       \
    {                                                                         \
        const void *_actual = (const void *)(actual);                         \
        const void *_expected = (const void *)(expected);                     \
        if (_actual == _expected)                                             \
        {                                                                     \
            UT_LOG_ASSERT(UT_ASSERT_PTR_NOT_EQUAL_FATAL, #actual, #expected); \
        }                                                                     \
        CU_ASSERT_PTR_NOT_EQUAL_FATAL(_actual, _expected);                    \
    }

/**
 * @brief Assert(make sure) the pointer value is NULL
 * 
 * 
 * @param value - pointer value
 */
#define UT_ASSERT_PTR_NULL(value)                      \
    {                                                  \
        const void *_value = (const void *)(value);    \
        if (NULL != _value)                            \
        {                                              \
            UT_LOG_ASSERT(UT_ASSERT_PTR_NULL, #value); \
        }                                              \
        CU_ASSERT_PTR_NULL(_value);                    \
    }

/**
 * @brief Assert(make sure) the pointer value is NULL, otherwise fail and exit the test
 * 
 * 
 * @param value - pointer value
 */
#define UT_ASSERT_PTR_NULL_FATAL(value)                      \
    {                                                        \
        const void *_value = (const void *)(value);          \
        if (NULL != _value)                                  \
        {                                                    \
            UT_LOG_ASSERT(UT_ASSERT_PTR_NULL_FATAL, #value); \
        }                                                    \
        CU_ASSERT_PTR_NULL_FATAL(_value);                    \
    }

/**
 * @brief Assert(make sure) the pointer value is NOT NULL, otherwise fail
 * 
 * @param value - pointer value
 */
#define UT_ASSERT_PTR_NOT_NULL(value)                      \
    {                                                      \
        const void *_value = (const void *)(value);        \
        if (NULL == _value)                                \
        {                                                  \
            UT_LOG_ASSERT(UT_ASSERT_PTR_NOT_NULL, #value); \
        }                                                  \
        CU_ASSERT_PTR_NOT_NULL(_value);                    \
    }

/**
 * @brief Assert(make sure) the pointer value is NOT NULL, otherwise fail and exit the test
 * 
 * @param value - pointer value
 */
#define UT_ASSERT_PTR_NOT_NULL_FATAL(value)                      \
    {                                                            \
        const void *_value = (const void *)(value);              \
        if (NULL == _value)                                      \
        {                                                        \
            UT_LOG_ASSERT(UT_ASSERT_PTR_NOT_NULL_FATAL, #value); \
        }                                                        \
        CU_ASSERT_PTR_NOT_NULL_FATAL(_value);                    \
    }

/**
 * @brief Assert(make sure) the expression is true, otherwise fail
 * 
 * @param value -  expression to evaluate
 * @see UT_ASSERT - Same condition as UT_ASSERT
 */
#define UT_ASSERT_TRUE(value)                      \
    {                                              \
        const int _value = (value);                \
        if (!_value)                               \
        {                                          \
            UT_LOG_ASSERT(UT_ASSERT_TRUE, #value); \
        }                                          \
        CU_ASSERT_TRUE(_value);                    \
    }

/**
 * @brief Assert(make sure) the expression is true, otherwise fail and exit the test
 * 
 * @param value -  expression to evaluate
 * @see UT_ASSERT - Same condition as UT_ASSERT_FATAL
 */
#define UT_ASSERT_TRUE_FATAL(value)                      \
    {                                                    \
        const int _value = (value);                      \
        if (!_value)                                     \
        {                                                \
            UT_LOG_ASSERT(UT_ASSERT_TRUE_FATAL, #value); \
        }                                                \
        CU_ASSERT_TRUE_FATAL(_value);                    \
    }

/**
 * @brief Assert(make sure) the expression is false, otherwise
 * 
 * @param value -  expression to evaluate
 */
#define UT_ASSERT_FALSE(value)                      \
    {                                               \
        const int _value = (value);                 \
        if (_value)                                 \
        {                                           \
            UT_LOG_ASSERT(UT_ASSERT_FALSE, #value); \
        }                                           \
        CU_ASSERT_FALSE(_value);                    \
    }

/**
 * @brief Assert(make sure) the expression is false, otherwise fail and exit the test
 * 
 * @param value -  expression to evaluate
 */
#define UT_ASSERT_FALSE_FATAL(value)                      \
    {                                                     \
        const int _value = (value);                       \
        if (_value)                                       \
        {                                                 \
            UT_LOG_ASSERT(UT_ASSERT_FALSE_FATAL, #value); \
        }                                                 \
        CU_ASSERT_FALSE_FATAL(_value);                    \
    }

/**
 * @brief Assert(make sure) the two expressions are equal, otherwise fail
 * 
 * @param actual -  actual expression
 * @param expected -  expected expression
 */
#define UT_ASSERT_EQUAL(actual, expected)                       \
    {                                                           \
        const int _actual = (actual);                           \
        const int _expected = (expected);                       \
        if (_actual != _expected)                               \
        {                                                       \
            UT_LOG_ASSERT(UT_ASSERT_EQUAL, #actual, #expected); \
        }                                                       \
        CU_ASSERT_EQUAL(_actual, _expected);                    \
    }

/**
 * @brief Assert(make sure) the two expressions are equal, otherwise fail and exit the test
 * 
 * @param actual -  actual expression
 * @param expected -  expected expression
 */
#define UT_ASSERT_EQUAL_FATAL(actual, expected)                       \
    {                                                                 \
        const int _actual = (actual);                                 \
        const int _expected = (expected);                             \
        if (_actual != _expected)                                     \
        {                                                             \
            UT_LOG_ASSERT(UT_ASSERT_EQUAL_FATAL, #actual, #expected); \
        }                                                             \
        CU_ASSERT_EQUAL_FATAL(_actual, _expected);                    \
    }

/**
 * @brief Assert(make sure) the two expressions are NOT equal, otherwise fail
 * 
 * @param actual -  actual expression
 * @param expected -  expected expression
 */
#define UT_ASSERT_NOT_EQUAL(actual, expected)                       \
    {                                                               \
        const int _actual = (actual);                               \
        const int _expected = (expected);                           \
        if (_actual == _expected)                                   \
        {                                                           \
            UT_LOG_ASSERT(UT_ASSERT_NOT_EQUAL, #actual, #expected); \
        }                                                           \
        CU_ASSERT_NOT_EQUAL(_actual, _expected);                    \
    }

/**
 * @brief Assert(make sure) the two expressions are NOT equal, otherwise fail and exit the test
 * 
 * @param actual -  actual expression
 * @param expected -  expected expression
 */
#define UT_ASSERT_NOT_EQUAL_FATAL(actual, expected)                       \
    {                                                                     \
        const int _actual = (actual);                                     \
        const int _expected = (expected);                                 \
        if (_actual == _expected)                                         \
        {                                                                 \
            UT_LOG_ASSERT(UT_ASSERT_NOT_EQUAL_FATAL, #actual, #expected); \
        }                                                                 \
        CU_ASSERT_NOT_EQUAL_FATAL(_actual, _expected);                    \
    }

/**
 * @brief Assert(make sure) the strings are equal, otherwise fail
 * 
 * @param actual -  actual string
 * @param expected -  expected string
 */
#define UT_ASSERT_STRING_EQUAL(expected, actual)                       \
    {                                                                  \
        const char *_expected = (const char *)(expected);              \
        const char *_actual = (const char *)(actual);                  \
        const int _result = strcmp(_actual, _expected);                \
        if (_result)                                                   \
        {                                                              \
            UT_LOG_ASSERT(UT_ASSERT_STRING_EQUAL, #actual, #expected); \
        }                                                              \
        CU_ASSERT_STRING_EQUAL(_actual, _expected);                    \
    }

/**
 * @brief Assert(make sure) the strings are equal, otherwise fail and exit the test
 * 
 * @param actual -  actual string
 * @param expected -  expected string
 */
#define UT_ASSERT_STRING_EQUAL_FATAL(expected, actual)                 \
    {                                                                  \
        const char *_expected = (const char *)(expected);              \
        const char *_actual = (const char *)(actual);                  \
        const int _result = strcmp(_actual, _expected);                \
        if (_result)                                                   \
        {                                                              \
            UT_LOG_ASSERT(UT_ASSERT_STRING_NOT_EQUAL_FATAL, #actual, #expected); \
        }                                                              \
        CU_ASSERT_STRING_EQUAL_FATAL(_actual, _expected);              \
    }

/**
 * @brief Assert(make sure) the strings are NOT equal, otherwise fail
 * 
 * @param actual -  actual string
 * @param expected -  expected string
 */
#define UT_ASSERT_STRING_NOT_EQUAL(expected, actual)                       \
    {                                                                      \
        const char *_expected = (const char *)(expected);                  \
        const char *_actual = (const char *)(actual);                      \
        const int _result = strcmp(_actual, _expected);                    \
        if (!_result)                                                      \
        {                                                                  \
            UT_LOG_ASSERT(UT_ASSERT_STRING_NOT_EQUAL, #actual, #expected); \
        }                                                                  \
        CU_ASSERT_STRING_NOT_EQUAL(_actual, _expected);                    \
    }

/**
 * @brief Assert(make sure) the strings are NOT equal, otherwise fail and exit the test
 * 
 * @param actual -  actual string
 * @param expected -  expected string
 */
#define UT_ASSERT_STRING_NOT_EQUAL_FATAL(expected, actual)                       \
    {                                                                            \
        const char *_expected = (const char *)(expected);                        \
        const char *_actual = (const char *)(actual);                            \
        const int _result = strcmp(_actual, _expected);                          \
        if (!_result)                                                            \
        {                                                                        \
            UT_LOG_ASSERT(UT_ASSERT_STRING_NOT_EQUAL_FATAL, #actual, #expected); \
        }                                                                        \
        CU_ASSERT_STRING_NOT_EQUAL_FATAL(_actual, _expected);                    \
    }

/**
 * @brief Assert(make sure) the expression is true, otherwise fail with the log message
 * 
 * @param[in] value - the value to check 
 * @param[in] message - message to log, if the assert fails
 * 
 */
#define UT_ASSERT_MSG(value, message)                      \
    {                                                      \
        const int _value = (value);                        \
        if (!_value)                                       \
        {                                                  \
            UT_LOG_ASSERT(UT_ASSERT_MSG, #value, message); \
        }                                                  \
        CU_ASSERT(_value);                                 \
    }

/**
 * @brief Assert(make sure) the expression is true, otherwise fail with the log message and exit the test
 * 
 * @param[in] value - the value to check 
 * @param[in] message - message to log, if the assert fails
 * 
 */
#define UT_ASSERT_MSG_FATAL(value, message)                      \
    {                                                            \
        const int _value = (value);                              \
        if (!_value)                                             \
        {                                                        \
            UT_LOG_ASSERT(UT_ASSERT_MSG_FATAL, #value, message); \
        }                                                        \
        CU_ASSERT_FATAL(_value);                                 \
    }

/**
 * @brief Assert(make sure) the expression is true, otherwise fail with the log message
 * 
 * @param[in] value - expression value to check 
 * @param[in] message - message to log, if the expression is true
 * 
 */
#define UT_ASSERT_TRUE_MSG(value, message)                      \
    {                                                           \
        const int _value = (value);                             \
        if (!_value)                                            \
        {                                                       \
            UT_LOG_ASSERT(UT_ASSERT_TRUE_MSG, #value, message); \
        }                                                       \
        CU_ASSERT_TRUE(_value);                                 \
    }

/**
 * @brief Assert(make sure) the expression is true, otherwise fail with the log message and exit the test
 * 
 * @param[in] value - expression value to check 
 * @param[in] message - message to log, if the expression is true
 * 
 */
#define UT_ASSERT_TRUE_MSG_FATAL(value, message)                      \
    {                                                                 \
        const int _value = (value);                                   \
        if (!_value)                                                  \
        {                                                             \
            UT_LOG_ASSERT(UT_ASSERT_TRUE_MSG_FATAL, #value, message); \
        }                                                             \
        CU_ASSERT_TRUE_FATAL(_value);                                 \
    }

/**
 * @brief Assert(make sure) the expression is false, otherwise fail with the log message
 * 
 * @param[in] value - the value to check 
 * @param[in] message - message to log, if the expression is false
 * 
 */
#define UT_ASSERT_FALSE_MSG(value, message)                      \
    {                                                            \
        const int _value = (value);                              \
        if (_value)                                              \
        {                                                        \
            UT_LOG_ASSERT(UT_ASSERT_FALSE_MSG, #value, message); \
        }                                                        \
        CU_ASSERT_FALSE(_value);                                 \
    }

/**
 * @brief Assert(make sure) the expression is false, otherwise fail with the log message and exit the test
 * 
 * @param[in] value - the value to check 
 * @param[in] message - message to log, if the expression is false
 * 
 */
#define UT_ASSERT_FALSE_MSG_FATAL(value, message)                      \
    {                                                                  \
        const int _value = (value);                                    \
        if (_value)                                                    \
        {                                                              \
            UT_LOG_ASSERT(UT_ASSERT_FALSE_MSG_FATAL, #value, message); \
        }                                                              \
        CU_ASSERT_FALSE_FATAL(_value);                                 \
    }

/**
 * @brief Assert(make sure) the expression is true, otherwise fail but always log the message
 * 
 * @param[in] value - the value to check 
 * @param[in] message - message to log, will always log
 * 
 */
#define UT_ASSERT_LOG(value, message)                      \
    {                                                      \
        const int _value = (value);                        \
        if (!_value)                                       \
        {                                                  \
            UT_LOG_ASSERT(UT_ASSERT_LOG, #value, message); \
        }                                                  \
        else                                               \
        {                                                  \
            UT_LOG_ASSERT(UT_ASSERT_LOG, #value, message); \
        }                                                  \
        CU_ASSERT(_value);                                 \
    }

/**
 * @brief Assert(make sure) the expression is true, otherwise fail and exit the test, but always log the message
 * 
 * @param[in] value - the value to check 
 * @param[in] message - message to log, will always log
 * 
 */
#define UT_ASSERT_LOG_FATAL(value, message)                      \
    {                                                            \
        const int _value = (value);                              \
        if (!_value)                                             \
        {                                                        \
            UT_LOG_ASSERT(UT_ASSERT_LOG_FATAL, #value, message); \
        }                                                        \
        else                                                     \
        {                                                        \
            UT_LOG_ASSERT(UT_ASSERT_LOG_FATAL, #value, message); \
        }                                                        \
        CU_ASSERT_FATAL(_value);                                 \
    }

#endif  /* UT -> CUNIT - Wrapper */

/** @} */
