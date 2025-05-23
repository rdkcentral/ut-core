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
#include <unordered_map>
#include <unordered_set>

/**
 * @brief Verifies that condition is true.
 */
#define UT_ASSERT_TRUE(condition) EXPECT_TRUE(condition)

/**
 * @brief Verifies that condition is false.
 */
#define UT_ASSERT_FALSE(condition) EXPECT_FALSE(condition)

/**
 * @brief Verifies that actual == expected.
 */
#define UT_ASSERT_EQUAL(actual, expected) EXPECT_EQ(expected, actual)

/**
 * @brief Verifies that actual != expected.
 */
#define UT_ASSERT_NOT_EQUAL(actual, expected) EXPECT_NE(expected, actual)

/**
 * @brief Verifies that actual < expected.
 */
#define UT_ASSERT_LESS(actual, expected) EXPECT_LT(actual, expected)

/**
 * @brief Verifies that actual <= expected.
 */
#define UT_ASSERT_LESS_EQUAL(actual, expected) EXPECT_LE(actual, expected)

/**
 * @brief Verifies that actual > expected.
 */
#define UT_ASSERT_GREATER(actual, expected) EXPECT_GT(actual, expected)

/**
 * @brief Verifies that actual >= expected.
 */
#define UT_ASSERT_GREATER_EQUAL(actual, expected) EXPECT_GE(actual, expected)

/**
 * @brief Verifies that ptr is NULL.
 */
#define UT_ASSERT_NULL(ptr) EXPECT_EQ(nullptr, ptr)

/**
 * @brief Verifies that ptr is not NULL.
 */
#define UT_ASSERT_NOT_NULL(ptr) EXPECT_NE(nullptr, ptr)

/**
 * @brief Verifies that two C-style strings are equal.
 */
#define UT_ASSERT_STRING_EQUAL(actual, expected) EXPECT_STREQ(expected, actual)

/**
 * @brief Verifies that two C-style strings are not equal.
 */
#define UT_ASSERT_STRING_NOT_EQUAL(actual, expected) EXPECT_STRNE(expected, actual)

/**
 * @brief Verifies that two strings are equal, ignoring case.
 */
#define UT_ASSERT_STRING_EQUAL_IGNORE_CASE(actual, expected) EXPECT_STRCASEEQ(expected, actual)

/**
 * @brief Verifies that two strings are not equal, ignoring case.
 */
#define UT_ASSERT_STRING_NOT_EQUAL_IGNORE_CASE(actual, expected) EXPECT_STRCASENE(expected, actual)

/**
 * @brief Verifies that two double values are nearly equal.
 */
#define UT_ASSERT_DOUBLE_EQUAL(actual, expected, tolerance) EXPECT_NEAR(expected, actual, tolerance)

/**
 * @brief Verifies that two float values are nearly equal.
 */
#define UT_ASSERT_FLOAT_EQUAL(actual, expected, tolerance) EXPECT_NEAR(expected, actual, tolerance)

/**
 * @brief Verifies that a statement throws an exception of type exception_type.
 */
#define UT_ASSERT_THROW(statement, exception_type) EXPECT_THROW(statement, exception_type)

/**
 * @brief Verifies that a statement throws any exception.
 */
#define UT_ASSERT_ANY_THROW(statement) EXPECT_ANY_THROW(statement)

/**
 * @brief Verifies that a statement does not throw any exception.
 */
#define UT_ASSERT_NO_THROW(statement) EXPECT_NO_THROW(statement)

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
#define UT_ASSERT_MESSAGE(condition, message) EXPECT_TRUE(condition) << message

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

/**
 * @class UTCore
 * @brief A test fixture class for unit tests using Google Test framework.
 *
 * This class provides setup and teardown functionality for unit tests.
 * It also includes static methods for managing test suites and their associated groups.
 */

class UTCore : public ::testing::Test
{
protected:
    /**
     * @brief Default constructor for the UTCore class.
     */
    UTCore() = default;
    /**
     * @brief Destructor for the UTCore class.
     *
     * This destructor is marked as `override` to ensure it overrides a virtual destructor
     * in the base class. It is also marked as `default` to use the compiler-generated
     * default implementation.
     */
    ~UTCore() override = default;
    /**
     * @brief Sets up the necessary preconditions for each test.
     *
     * This method is called before each test is run. Override this method to
     * initialize any resources or set up any state that is required for the tests.
     */
    void SetUp() override;
    /**
     * @brief Tears down the test environment after each test case.
     *
     * This method is called after each test case is run to perform any necessary
     * cleanup. It overrides the TearDown method from the base class.
     */
    void TearDown() override;

public:
    /**
     * @brief Retrieves the current test filter.
     *
     * This function returns a string representing the current test filter
     * used to select which tests to run.
     *
     * @return A string containing the test filter.
     */
    static std::string UT_get_test_filter();
    /**
     * @brief Adds a test suite with a specified group ID.
     *
     * This function registers a new test suite under the given name and associates it with the specified group ID.
     *
     * @param testSuiteName The name of the test suite to be added.
     * @param group The group ID to associate with the test suite.
     * @return true if the test suite was successfully added, false otherwise.
     */
    static bool UT_add_suite_withGroupID(const std::string& testSuiteName, UT_groupID_t group);

private:
    static std::unordered_map<std::string, UT_groupID_t> suiteToGroup;
};

/**
 * @brief Registers a test suite under a specific group.
 *
 * This macro defines a static boolean variable that ensures the test suite is
 * added to the test framework upon execution. It calls `UTCore::UT_add_suite_withGroupID()`
 * to associate the test suite name with a group ID.
 *
 * @param groupName The identifier of the test suite (must be a valid identifier, not a string).
 * @param groupID   The group ID to which the test suite belongs.
 *
 * @note The `groupName` argument is automatically converted to a string using `#groupName`
 *       when passed to `UT_add_suite_withGroupID()`.
 *
 * @warning `groupName` must be a valid identifier (not a string literal) and unique within the scope.
 *
 * @see UTCore::UT_add_suite_withGroupID()
 */

#define UT_ADD_TEST_TO_GROUP(groupName, groupID) \
    static bool groupName##_group = UTCore::UT_add_suite_withGroupID(#groupName, groupID);

#endif  /* UT -> GTEST - Wrapper */

/** @} */