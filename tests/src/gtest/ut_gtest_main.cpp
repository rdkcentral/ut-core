#include <gtest/gtest.h>
#include "ut_kvp_profile_wrapper.h"  // Include the wrapper class

#define KVP_VALID_TEST_ASSERT_YAML_FILE "assets/test_kvp.yaml"

// Test fixture for UTKVPProfileWrapper
class UTKVPProfileWrapperTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Code here will be run before each test
    }

    void TearDown() override {
        // Code here will be run after each test
    }
};

// Example Test Case 1: Test if the profile opens successfully
TEST_F(UTKVPProfileWrapperTest, OpenProfileSuccess) {
    try {
        UTKVPProfileWrapper profile(KVP_VALID_TEST_ASSERT_YAML_FILE);
        EXPECT_NE(profile.getInstance(), nullptr);  // Ensure instance is not null
    } catch (const std::exception &e) {
        FAIL() << "Exception caught: " << e.what();  // Fail the test if exception is thrown
    }
}

// Example Test Case 2: Test if the profile closes correctly
TEST_F(UTKVPProfileWrapperTest, CloseProfileSuccess) {
    try {
        UTKVPProfileWrapper profile(KVP_VALID_TEST_ASSERT_YAML_FILE);
        profile.close();  // Close explicitly
        EXPECT_EQ(profile.getInstance(), nullptr);  // Should be null after closing
    } catch (const std::exception &e) {
        FAIL() << "Exception caught: " << e.what();  // Fail the test if exception is thrown
    }
}

// More tests to be added...

