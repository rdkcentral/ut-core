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
 * UT Unit wrapper to the gtest framework.
 * 
 * Hides the functionality of gtest 
 */
/** @addtogroup UT_GTEST
 * @{
 */

#ifndef __UT_GTEST_H
#define __UT_GTEST_H

#include <gtest/gtest.h>
#include <functional>
#include <string>

/**
 * @brief Verifies that condition is true.
 */
#define UT_ASSERT_TRUE(condition) ASSERT_TRUE(condition)

/**
 * @brief Verifies that condition is false.
 */
#define UT_ASSERT_FALSE(condition) ASSERT_FALSE(condition)

/**
 * @brief Verifies that actual == expected.
 */
#define UT_ASSERT_EQUAL(actual, expected) ASSERT_EQ(expected, actual)

/**
 * @brief Verifies that actual != expected.
 */
#define UT_ASSERT_NOT_EQUAL(actual, expected) ASSERT_NE(expected, actual)

/**
 * @brief Verifies that actual < expected.
 */
#define UT_ASSERT_LESS(actual, expected) ASSERT_LT(actual, expected)

/**
 * @brief Verifies that actual <= expected.
 */
#define UT_ASSERT_LESS_EQUAL(actual, expected) ASSERT_LE(actual, expected)

/**
 * @brief Verifies that actual > expected.
 */
#define UT_ASSERT_GREATER(actual, expected) ASSERT_GT(actual, expected)

/**
 * @brief Verifies that actual >= expected.
 */
#define UT_ASSERT_GREATER_EQUAL(actual, expected) ASSERT_GE(actual, expected)

/**
 * @brief Verifies that ptr is NULL.
 */
#define UT_ASSERT_NULL(ptr) ASSERT_EQ(nullptr, ptr)

/**
 * @brief Verifies that ptr is not NULL.
 */
#define UT_ASSERT_NOT_NULL(ptr) ASSERT_NE(nullptr, ptr)

/**
 * @brief Verifies that two C-style strings are equal.
 */
#define UT_ASSERT_STRING_EQUAL(actual, expected) ASSERT_STREQ(expected, actual)

/**
 * @brief Verifies that two C-style strings are not equal.
 */
#define UT_ASSERT_STRING_NOT_EQUAL(actual, expected) ASSERT_STRNE(expected, actual)

/**
 * @brief Verifies that two strings are equal, ignoring case.
 */
#define UT_ASSERT_STRING_EQUAL_IGNORE_CASE(actual, expected) ASSERT_STRCASEEQ(expected, actual)

/**
 * @brief Verifies that two strings are not equal, ignoring case.
 */
#define UT_ASSERT_STRING_NOT_EQUAL_IGNORE_CASE(actual, expected) ASSERT_STRCASENE(expected, actual)

/**
 * @brief Verifies that two double values are nearly equal.
 */
#define UT_ASSERT_DOUBLE_EQUAL(actual, expected, tolerance) ASSERT_NEAR(expected, actual, tolerance)

/**
 * @brief Verifies that two float values are nearly equal.
 */
#define UT_ASSERT_FLOAT_EQUAL(actual, expected, tolerance) ASSERT_NEAR(expected, actual, tolerance)

/**
 * @brief Verifies that a statement throws an exception of type exception_type.
 */
#define UT_ASSERT_THROW(statement, exception_type) ASSERT_THROW(statement, exception_type)

/**
 * @brief Verifies that a statement throws any exception.
 */
#define UT_ASSERT_ANY_THROW(statement) ASSERT_ANY_THROW(statement)

/**
 * @brief Verifies that a statement does not throw any exception.
 */
#define UT_ASSERT_NO_THROW(statement) ASSERT_NO_THROW(statement)

/**
 * @brief Generates a failure with a message.
 */
#define UT_FAIL(message) FAIL() << message

/**
 * @brief Marks a test as explicitly successful with a message.
 */
#define UT_PASS(message) SUCCEED() << message

/**
 * @brief Verifies that a condition is true, with a message.
 */
#define UT_ASSERT_MESSAGE(condition, message) ASSERT_TRUE(condition) << message

/**
 * @brief Fatal assertion that actual == expected.
 */
#define UT_ASSERT_EQUAL_FATAL(actual, expected) ASSERT_EQ(expected, actual)

/**
 * @brief Fatal assertion that actual != expected.
 */
#define UT_ASSERT_NOT_EQUAL_FATAL(actual, expected) ASSERT_NE(expected, actual)

/**
 * @brief Fatal assertion that condition is true.
 */
#define UT_ASSERT_TRUE_FATAL(condition) ASSERT_TRUE(condition)

/**
 * @brief Fatal assertion that condition is false.
 */
#define UT_ASSERT_FALSE_FATAL(condition) ASSERT_FALSE(condition)

/**
 * @brief Fatal assertion that ptr is NULL.
 */
#define UT_ASSERT_NULL_FATAL(ptr) ASSERT_EQ(nullptr, ptr)

/**
 * @brief Fatal assertion that ptr is not NULL.
 */
#define UT_ASSERT_NOT_NULL_FATAL(ptr) ASSERT_NE(nullptr, ptr)

/**
 * @brief Fatal test failure with a message.
 */
#define UT_FAIL_FATAL(message) FAIL() << message

/**
 * @brief Fatal test success with a message.
 */
#define UT_PASS_FATAL(message) SUCCEED() << message

/**
 * @brief Skips the test execution without marking it as a failure.
 *
 * @note The test will be listed as "[  SKIPPED  ]" in the test results.
 * @note The test function is still compiled and executed, but assertions within it are ignored.
 *
 * @see GTEST_SKIP()
 */
#define UT_IGNORE_TEST() GTEST_SKIP()

/**
 * @brief Macro to add a test case to a test fixture or test suite.
 *
 * This macro is used to define a test case within a test fixture using Google Test framework.
 *
 * @param test_suite_name The name of the test fixture class or the test suite.
 * @param test_name The name of the test case.
 *
 * Example usage:
 * @code
 * UT_ADD_TEST(MyTestFixture, MyTestCase)
 * @endcode
 */
#define UT_ADD_TEST(test_suite_name, test_name) TEST_F(test_suite_name, test_name)

#endif  /* UT -> GTEST - Wrapper */

/** @} */