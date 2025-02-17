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

#ifndef __TEST_UT_GTEST_H
#define __TEST_UT_GTEST_H

#include <ut.h>

// Test fixture class
class UTGTestTest : public UTCore
{
protected:
    void SetUp() override
    {
        // Code for setting up each test
    }

    void TearDown() override
    {
        // Code for cleaning up after each test
    }

    // Template function to register the test group with the correct class name
    template <typename T>
    static bool RegisterTest()
    {
        UTCore::UT_set_group(typeid(T).name(), UT_TESTS_L2);
        return true; // Ensures execution during static initialization
    }

    // Static function to register this test suite
    static bool Register()
    {
        return RegisterTest<UTGTestTest>(); // Calls the template with the current class type
    }

    // Additional test cases as needed...
private:
    static bool registered;
};

// Automatically register test suite before test execution
bool UTGTestTest::registered = UTGTestTest::Register();

// Test case for UT_ASSERT_TRUE
UT_TEST(UTGTestTest, UT_ASSERT_TRUE_Test)
{
    int a = 5;
    int b = 5;
    UT_ASSERT_TRUE(a == b);
}

// Test case for UT_ASSERT_FALSE
UT_TEST(UTGTestTest, UT_ASSERT_FALSE_Test)
{
    int a = 5;
    int b = 6;
    UT_ASSERT_FALSE(a == b);
}

// Test case for UT_ASSERT_EQUAL
UT_TEST(UTGTestTest, UT_ASSERT_EQUAL_Test)
{
    int a = 5;
    int b = 5;
    UT_ASSERT_EQUAL(a, b);
}

// Test case for UT_ASSERT_NOT_EQUAL
UT_TEST(UTGTestTest, UT_ASSERT_NOT_EQUAL_Test)
{
    int a = 5;
    int b = 6;
    UT_ASSERT_NOT_EQUAL(a, b);
}

// Test case for UT_ASSERT_LESS
UT_TEST(UTGTestTest, UT_ASSERT_LESS_Test)
{
    int a = 5;
    int b = 6;
    UT_ASSERT_LESS(a, b);
}

// Test case for UT_ASSERT_LESS_EQUAL
UT_TEST(UTGTestTest, UT_ASSERT_LESS_EQUAL_Test)
{
    int a = 5;
    int b = 5;
    UT_ASSERT_LESS_EQUAL(a, b);
}

// Test case for UT_ASSERT_GREATER
UT_TEST(UTGTestTest, UT_ASSERT_GREATER_Test)
{
    int a = 6;
    int b = 5;
    UT_ASSERT_GREATER(a, b);
}

// Test case for UT_ASSERT_GREATER_EQUAL
UT_TEST(UTGTestTest, UT_ASSERT_GREATER_EQUAL_Test)
{
    int a = 5;
    int b = 5;
    UT_ASSERT_GREATER_EQUAL(a, b);
}

// Test case for UT_ASSERT_NULL
UT_TEST(UTGTestTest, UT_ASSERT_NULL_Test)
{
    int *ptr = nullptr;
    UT_ASSERT_NULL(ptr);
}

// Test case for UT_ASSERT_NOT_NULL
UT_TEST(UTGTestTest, UT_ASSERT_NOT_NULL_Test)
{
    int a = 5;
    int *ptr = &a;
    UT_ASSERT_NOT_NULL(ptr);
}

// Test case for UT_ASSERT_STRING_EQUAL
UT_TEST(UTGTestTest, UT_ASSERT_STRING_EQUAL_Test)
{
    const char *str1 = "test";
    const char *str2 = "test";
    UT_ASSERT_STRING_EQUAL(str1, str2);
}

// Test case for UT_ASSERT_STRING_NOT_EQUAL
UT_TEST(UTGTestTest, UT_ASSERT_STRING_NOT_EQUAL_Test)
{
    const char *str1 = "test";
    const char *str2 = "test1";
    UT_ASSERT_STRING_NOT_EQUAL(str1, str2);
}

// Test case for UT_ASSERT_STRING_EQUAL_IGNORE_CASE
UT_TEST(UTGTestTest, UT_ASSERT_STRING_EQUAL_IGNORE_CASE_Test)
{
    const char *str1 = "test";
    const char *str2 = "TEST";
    UT_ASSERT_STRING_EQUAL_IGNORE_CASE(str1, str2);
}

// Test case for UT_ASSERT_STRING_NOT_EQUAL_IGNORE_CASE
UT_TEST(UTGTestTest, UT_ASSERT_STRING_NOT_EQUAL_IGNORE_CASE_Test)
{
    const char *str1 = "test";
    const char *str2 = "TEST1";
    UT_ASSERT_STRING_NOT_EQUAL_IGNORE_CASE(str1, str2);
}

// Test case for UT_ASSERT_DOUBLE_EQUAL
UT_TEST(UTGTestTest, UT_ASSERT_DOUBLE_EQUAL_Test)
{
    double a = 5.0;
    double b = 5.0;
    double tolerance = 0.001;
    UT_ASSERT_DOUBLE_EQUAL(a, b, tolerance);
}

// Test case for UT_ASSERT_FLOAT_EQUAL
UT_TEST(UTGTestTest, UT_ASSERT_FLOAT_EQUAL_Test)
{
    float a = 5.0f;
    float b = 5.0f;
    float tolerance = 0.001f;
    UT_ASSERT_FLOAT_EQUAL(a, b, tolerance);
}

// Test case for UT_ASSERT_THROW
UT_TEST(UTGTestTest, UT_ASSERT_THROW_Test)
{
    UT_ASSERT_THROW(throw std::runtime_error("error"), std::runtime_error);
}

// Test case for UT_ASSERT_ANY_THROW
UT_TEST(UTGTestTest, UT_ASSERT_ANY_THROW_Test)
{
    UT_ASSERT_ANY_THROW(throw std::runtime_error("error"));
}

// Test case for UT_ASSERT_NO_THROW
UT_TEST(UTGTestTest, UT_ASSERT_NO_THROW_Test)
{
    UT_ASSERT_NO_THROW(int a = 5; (void)a;);
}

// Test case for UT_FAIL
UT_TEST(UTGTestTest, UT_FAIL_Test)
{
    UT_FAIL("This test should fail");
}

// Test case for UT_PASS
UT_TEST(UTGTestTest, UT_PASS_Test)
{
    UT_PASS("This test should pass");
}

// Test case for UT_ASSERT_MESSAGE
UT_TEST(UTGTestTest, UT_ASSERT_MESSAGE_Test)
{
    int a = 5;
    int b = 5;
    UT_ASSERT_MESSAGE(a == b, "a should be equal to b");
}

// Test case for UT_ASSERT_EQUAL_FATAL
UT_TEST(UTGTestTest, UT_ASSERT_EQUAL_FATAL_Test)
{
    int a = 5;
    int b = 5;
    UT_ASSERT_EQUAL_FATAL(a, b);
}

// Test case for UT_ASSERT_NOT_EQUAL_FATAL
UT_TEST(UTGTestTest, UT_ASSERT_NOT_EQUAL_FATAL_Test)
{
    int a = 5;
    int b = 6;
    UT_ASSERT_NOT_EQUAL_FATAL(a, b);
}

// Test case for UT_ASSERT_TRUE_FATAL
UT_TEST(UTGTestTest, UT_ASSERT_TRUE_FATAL_Test)
{
    int a = 5;
    int b = 5;
    UT_ASSERT_TRUE_FATAL(a == b);
}

// Test case for UT_ASSERT_FALSE_FATAL
UT_TEST(UTGTestTest, UT_ASSERT_FALSE_FATAL_Test)
{
    int a = 5;
    int b = 6;
    UT_ASSERT_FALSE_FATAL(a == b);
}

// Test case for UT_ASSERT_NULL_FATAL
UT_TEST(UTGTestTest, UT_ASSERT_NULL_FATAL_Test)
{
    int *ptr = nullptr;
    UT_ASSERT_NULL_FATAL(ptr);
}

// Test case for UT_ASSERT_NOT_NULL_FATAL
UT_TEST(UTGTestTest, UT_ASSERT_NOT_NULL_FATAL_Test)
{
    int a = 5;
    int *ptr = &a;
    UT_ASSERT_NOT_NULL_FATAL(ptr);
}

// Test case for UT_FAIL_FATAL
UT_TEST(UTGTestTest, UT_FAIL_FATAL_Test)
{
    UT_FAIL_FATAL("This test should fail");
}

// Test case for UT_PASS_FATAL
UT_TEST(UTGTestTest, UT_PASS_FATAL_Test)
{
    UT_PASS_FATAL("This test should pass");
}

UT_TEST(UTGTestTest, IgnoredTest)
{
    UT_IGNORE_TEST();   // This test will be skipped at runtime
    ASSERT_TRUE(false); // This line will never be executed
}

#endif  // __TEST_UT_GTEST_H