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

#ifndef UT_TEST_FIXTURE_H
#define UT_TEST_FIXTURE_H

#include <gtest/gtest.h>

// UT test fixture class for setting up common test resources
class UTTestFixture : public ::testing::Test
{
protected:
    UTTestFixture()
    {
        // Initialization code if needed
    }

    ~UTTestFixture() override
    {
        // Cleanup code if needed
    }

    void SetUp() override
    {
        // Code to initialize resources before each test
    }

    void TearDown() override
    {
        // Code to clean up resources after each test
    }
};

#endif // UT_TEST_FIXTURE_H