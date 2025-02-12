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

#include <ut_kvp_profile.h>
#include <ut.h>

class UTGTestL1 : public UTCore
{
public:
    UTGTestL1() : UTCore() {}

    ~UTGTestL1() override = default;

    void SetUp() override
    {
        // Code to set up resources before each test
    }

    void TearDown() override
    {
        // Code to clean up resources after each test
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
        return RegisterTest<UTGTestL1>(); // Calls the template with the current class type
    }

private:
    static bool registered;
};

// Automatically register test suite before test execution
bool UTGTestL1::registered = UTGTestL1::Register();


TEST_F(UTGTestL1, TestGtestL1Equal)
{
    EXPECT_EQ(1, 1); // Basic test case
}

TEST_F(UTGTestL1, TestGtestL1NotEqual)
{
    EXPECT_NE(1, 2);
}

TEST_F(UTGTestL1, TestGtestL1GreaterThan)
{
    EXPECT_GT(2, 1);
}

// Other test cases as needed...