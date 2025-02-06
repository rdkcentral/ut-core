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
protected:

    // Static member ensures suite is registered at startup
    static bool registered;

    void SetUp() override
    {
        UTCore::SetUp(); // Call base class SetUp
    }

    void TearDown() override
    {
        // Additional teardown specific to UTGTestL1
        UTCore::TearDown(); // Call base class TearDown
    }
};

// Define and initialize static variable
bool UTGTestL1::registered = (TestGroupManager::RegisterSuite("UTGTestL1", UT_TESTS_L2), true);

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