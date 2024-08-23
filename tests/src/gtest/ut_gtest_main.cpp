#include <gtest/gtest.h>
#include <ut_kvp_profile.h>  // Include the wrapper class

#define KVP_VALID_TEST_NO_FILE "assets/this_does_not_exist.yaml"
#define KVP_VALID_TEST_YAML_FILE "assets/test_kvp.yaml"

// Test fixture class for setting up and tearing down the test environment.
class UTKVPProfileTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Any necessary setup before each test is run.
    }

    void TearDown() override {
        // Ensure the profile is closed after each test to clean up.
        ut_kvp_profile_close();
    }
};

TEST_F(UTKVPProfileTest, TestProfileOpenSuccess) {
    const char* validFileName = KVP_VALID_TEST_YAML_FILE;
    ut_kvp_status_t status = ut_kvp_profile_open((char*)validFileName);
    EXPECT_EQ(status, UT_KVP_STATUS_SUCCESS);
}

TEST_F(UTKVPProfileTest, TestProfileOpenFailure) {
    // Test with an invalid file name.
    const char* invalidFileName = KVP_VALID_TEST_NO_FILE;
    ut_kvp_status_t status = ut_kvp_profile_open((char*)invalidFileName);
    EXPECT_EQ(status, UT_KVP_STATUS_FILE_OPEN_ERROR);

    // Test with a null file name.
    status = ut_kvp_profile_open(nullptr);
    EXPECT_EQ(status, UT_KVP_STATUS_INVALID_PARAM);
}

TEST_F(UTKVPProfileTest, TestProfileClose) {
    // Open and then close to ensure no exceptions or errors occur.
    ut_kvp_status_t status = ut_kvp_profile_open((char*)KVP_VALID_TEST_YAML_FILE);
    EXPECT_EQ(status, UT_KVP_STATUS_SUCCESS);

    ut_kvp_profile_close();
    // Since close doesn't return a status, we assume success if no exceptions were thrown.
    SUCCEED();
}

TEST_F(UTKVPProfileTest, TestProfileGetInstance) {
    ut_kvp_status_t status = ut_kvp_profile_open((char*)KVP_VALID_TEST_YAML_FILE);
    EXPECT_EQ(status, UT_KVP_STATUS_SUCCESS);

    ut_kvp_instance_t* instance = ut_kvp_profile_getInstance();
    EXPECT_NE(instance, nullptr);
}

TEST_F(UTKVPProfileTest, TestGetBoolField) {
    ut_kvp_status_t status = ut_kvp_profile_open((char*)KVP_VALID_TEST_YAML_FILE);
    EXPECT_EQ(status, UT_KVP_STATUS_SUCCESS);

    bool value = UT_KVP_PROFILE_GET_BOOL("decodeTest/checkBoolFalse");
    EXPECT_FALSE(value);
}

TEST_F(UTKVPProfileTest, TestGetUInt8Field) {
    ut_kvp_status_t status = ut_kvp_profile_open((char*)KVP_VALID_TEST_YAML_FILE);
    EXPECT_EQ(status, UT_KVP_STATUS_SUCCESS);

    uint8_t value = UT_KVP_PROFILE_GET_UINT8("decodeTest.checkUint8IsDeHex");
    EXPECT_EQ(value, 0xde);
}

TEST_F(UTKVPProfileTest, TestGetUInt16Field) {
    ut_kvp_status_t status = ut_kvp_profile_open((char*)KVP_VALID_TEST_YAML_FILE);
    EXPECT_EQ(status, UT_KVP_STATUS_SUCCESS);

    uint16_t value = UT_KVP_PROFILE_GET_UINT16("decodeTest.checkUint16IsDeadHex");
    EXPECT_EQ(value, 0xdead);
}

TEST_F(UTKVPProfileTest, TestGetUInt32Field) {
    ut_kvp_status_t status = ut_kvp_profile_open((char*)KVP_VALID_TEST_YAML_FILE);
    EXPECT_EQ(status, UT_KVP_STATUS_SUCCESS);

    uint32_t value = UT_KVP_PROFILE_GET_UINT32("decodeTest.checkUint32IsDeadBeefHex");
    EXPECT_EQ(value, 0xdeadbeef);
}

TEST_F(UTKVPProfileTest, TestGetUInt64Field) {
    ut_kvp_status_t status = ut_kvp_profile_open((char*)KVP_VALID_TEST_YAML_FILE);
    EXPECT_EQ(status, UT_KVP_STATUS_SUCCESS);

    uint64_t value = UT_KVP_PROFILE_GET_UINT64("decodeTest.checkUint64IsDeadBeefHex");
    EXPECT_EQ(value, 0xdeadbeefdeadbeef);
}

TEST_F(UTKVPProfileTest, TestGetListCount) {
    ut_kvp_status_t status = ut_kvp_profile_open((char*)KVP_VALID_TEST_YAML_FILE);
    EXPECT_EQ(status, UT_KVP_STATUS_SUCCESS);

    uint32_t value = UT_KVP_PROFILE_GET_LIST_COUNT("decodeTest.checkStringList");
    EXPECT_EQ(value, 3);
}

TEST_F(UTKVPProfileTest, TestGetStringField) {
    ut_kvp_status_t status = ut_kvp_profile_open((char*)KVP_VALID_TEST_YAML_FILE);
    EXPECT_EQ(status, UT_KVP_STATUS_SUCCESS);

    char buffer[UT_KVP_MAX_ELEMENT_SIZE] = {0};
    UT_KVP_PROFILE_GET_STRING("decodeTest/checkStringDeadBeef", buffer);
    EXPECT_STREQ(buffer, "the beef is dead");
}

