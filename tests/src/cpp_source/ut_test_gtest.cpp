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

#define KVP_VALID_TEST_NO_FILE "assets/this_does_not_exist.yaml"
#define KVP_VALID_TEST_YAML_FILE "assets/test_kvp.yaml"

class UTKVPProfileTestL1 : public UTCore
{
protected:
    void SetUp() override
    {
        UTCore::SetUp(); // Call base class SetUp
        // Additional setup specific to UTKVPProfileTestL1
    }

    void TearDown() override
    {
        // Additional teardown specific to UTKVPProfileTestL1
        UTCore::TearDown(); // Call base class TearDown
    }
};

UT_ADD_TEST(UTKVPProfileTestL1, TestProfileOpenSuccess)
{
    const char *validFileName = KVP_VALID_TEST_YAML_FILE;
    ut_kvp_status_t status = ut_kvp_profile_open((char *)validFileName);
    UT_ASSERT_EQUAL(status, UT_KVP_STATUS_SUCCESS);
}

UT_ADD_TEST(UTKVPProfileTestL1, TestProfileGetInstance)
{
    // Getinstance from the profile passed via cli
    ut_kvp_instance_t *instance = ut_kvp_profile_getInstance();
    UT_ASSERT_NOT_NULL(instance);
}

UT_ADD_TEST(UTKVPProfileTestL1, TestProfileOpenFailure)
{
    // Test with an invalid file name.
    const char *invalidFileName = KVP_VALID_TEST_NO_FILE;
    ut_kvp_status_t status = ut_kvp_profile_open((char *)invalidFileName);
    UT_ASSERT_EQUAL(status, UT_KVP_STATUS_FILE_OPEN_ERROR);

    // Test with a null file name.
    status = ut_kvp_profile_open(nullptr);
    UT_ASSERT_EQUAL(status, UT_KVP_STATUS_INVALID_PARAM);
}

UT_ADD_TEST(UTKVPProfileTestL1, TestProfileClose)
{
    // Test profile close
    ut_kvp_profile_close();
    ut_kvp_profile_close();
    ut_kvp_profile_close();
    // Since close doesn't return a status, we assume success if no exceptions were thrown.
    UT_PASS("TestProfileClose");
}

// Other test cases as needed...