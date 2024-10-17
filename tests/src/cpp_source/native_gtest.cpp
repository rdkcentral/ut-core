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

#include <gtest/gtest.h>

// Define the test fixture class
class SampleTestSuite : public ::testing::Test
{
protected:
    // Initialization logic can be added here
    // static void SetUpTestSuite()
    // {
    //     // Initialization logic for the test suite
    //     // This runs once before any test in this suite
    //     // e.g., sample_suite_init();
    // }

    // Cleanup logic can be added here
    // static void TearDownTestSuite()
    // {
    //     // Cleanup logic for the test suite
    //     // This runs once after all tests in this suite
    //     // e.g., sample_suite_cleanup();
    // }
};

// Test cases
TEST_F(SampleTestSuite, SampleTestCase1)
{
    EXPECT_EQ(1, 1); // Basic test case
}

TEST_F(SampleTestSuite, SampleTestCase2)
{
    EXPECT_STRNE("hello", "world"); // Another test case
}

// Main test function
// int main(int argc, char **argv)
// {
//     // Initialize Google Test
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }