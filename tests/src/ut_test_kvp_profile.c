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

/* Standard Libraries */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

/* Module Includes */
#include <ut.h>
#include <ut_log.h>

#include <ut_kvp_profile.h>

#define KVP_VALID_TEST_ASSERT_YAML_FILE "assets/test_kvp.yaml"
#define KVP_VALID_TEST_ASSERT_JSON_FILE "assets/test_kvp.json"

static UT_test_suite_t *gpAssertSuite1 = NULL;
static UT_test_suite_t *gpAssertSuite2 = NULL;
static UT_test_suite_t *gpAssertSuite3 = NULL;

void test_ut_kvp_profile_uint8(void)
{
    uint8_t checkField = 0xde;
    UT_LOG_STEP( "test_ut_kvp_profile_uint8 - start" );
    UT_ASSERT_KVP_EQUAL_PROFILE_UINT8( checkField, "decodeTest/checkUint8IsDeDec" );
    UT_ASSERT_KVP_EQUAL_PROFILE_UINT8( checkField, "decodeTest/checkUint8IsDeHex" );
    UT_LOG_STEP( "test_ut_kvp_profile_uint8 - end" );
}

void test_ut_kvp_profile_uint16(void)
{
    uint16_t checkField = 0xdead;
    UT_LOG_STEP( "test_ut_kvp_profile_uint16 - start" );
    UT_ASSERT_KVP_EQUAL_PROFILE_UINT16( checkField, "decodeTest/checkUint16IsDeadDec" );
    UT_ASSERT_KVP_EQUAL_PROFILE_UINT16( checkField, "decodeTest/checkUint16IsDeadHex" );
    UT_LOG_STEP( "test_ut_kvp_profile_uint16 - end" );
}

void test_ut_kvp_profile_uint32(void)
{
    uint32_t checkField = 0xdeadbeef;
    UT_LOG_STEP( "test_ut_kvp_profile_uint32 - start" );
    UT_ASSERT_KVP_EQUAL_PROFILE_UINT32( checkField, "decodeTest/checkUint32IsDeadBeefDec" );
    UT_ASSERT_KVP_EQUAL_PROFILE_UINT32( checkField, "decodeTest/checkUint32IsDeadBeefHex" );
    UT_LOG_STEP( "test_ut_kvp_profile_uint32 - end" );
}

void test_ut_kvp_profile_uint64(void)
{
    uint64_t checkField = 0xdeadbeefdeadbeef;
    UT_LOG_STEP( "test_ut_kvp_profile_uint64" );
    UT_ASSERT_EQUAL_KVP_PROFILE_UINT64( checkField, "decodeTest/checkUint64IsDeadBeefDec" );
    UT_ASSERT_EQUAL_KVP_PROFILE_UINT64( checkField, "decodeTest/checkUint64IsDeadBeefHex" );
    UT_LOG_STEP( "test_ut_kvp_profile_uint64 - end" );
}

void test_ut_kvp_profile_string(void)
{
    const char *checkField = "the beef is dead";
    UT_LOG_STEP( "test_ut_kvp_profile_string" );
    UT_ASSERT_EQUAL_KVP_PROFILE_STRING( checkField, "decodeTest/checkStringDeadBeef");
    UT_LOG_STEP( "test_ut_kvp_profile_string - end" );
}

void test_ut_kvp_profile_bool(void)
{
    UT_LOG_STEP( "test_ut_kvp_profile_bool" );
    UT_ASSERT_EQUAL_KVP_PROFILE_BOOL( false, "decodeTest/checkBoolFalse" );
    UT_ASSERT_EQUAL_KVP_PROFILE_BOOL( true, "decodeTest/checkBoolTRUE" );
    UT_LOG_STEP( "test_ut_kvp_profile_bool - end" );
}

void test_ut_kvp_profile_open( void )
{
    UT_LOG_STEP( "test_ut_kvp_profile_open - start" );
    ut_kvp_profile_open( KVP_VALID_TEST_ASSERT_YAML_FILE );
    ut_kvp_profile_open( KVP_VALID_TEST_ASSERT_YAML_FILE );
    ut_kvp_profile_open( KVP_VALID_TEST_ASSERT_YAML_FILE );
    UT_LOG_STEP( "test_ut_kvp_profile_open - end" );
}

void test_ut_kvp_profile_close( void )
{
    UT_LOG_STEP( "test_ut_kvp_profile_close - start" );
    ut_kvp_profile_close();
    ut_kvp_profile_close();
    ut_kvp_profile_close();
    UT_LOG_STEP( "test_ut_kvp_profile_close - end" );
}

int test_ut_kvp_profile_init_yaml( void )
{
    ut_kvp_profile_open( KVP_VALID_TEST_ASSERT_YAML_FILE );
    return 0;
}

int test_ut_kvp_profile_init_json( void )
{
    ut_kvp_profile_open( KVP_VALID_TEST_ASSERT_JSON_FILE );
    return 0;
}

int test_ut_kvp_profile_cleanup( void )
{
    ut_kvp_profile_close();
    return 0;
}

void test_ut_kvp_get_instance( void )
{
    bool result;
    ut_kvp_instance_t *pInstance;

    pInstance = ut_kvp_profile_getInstance();
    UT_ASSERT( pInstance != NULL );
    if(pInstance == NULL)
    {
        UT_LOG_STEP("Check ut_kvp_profile_getInstance() - Negative");
        UT_LOG_ERROR("perhaps platform profile was not passed using -p switch");
        return;
    }
    UT_LOG_STEP("Check ut_kvp_profile_getInstance() - Positive");
    // Check for decoding correctly a field from the platformProfile
    result = ut_kvp_getBoolField( pInstance, "decodeTest/checkBoolTRUE" );
    UT_ASSERT( result == true );
}

void register_kvp_profile_testing_functions(void)
{
    gpAssertSuite1 = UT_add_suite("ut-kvp - assert open / close", NULL, NULL);
    assert(gpAssertSuite1 != NULL);
    UT_add_test(gpAssertSuite1, "kvp profile open()", test_ut_kvp_profile_open);
    UT_add_test(gpAssertSuite1, "kvp profile getInstance()", test_ut_kvp_get_instance);
    UT_add_test(gpAssertSuite1, "kvp profile close()", test_ut_kvp_profile_close);

    gpAssertSuite2 = UT_add_suite("ut-kvp - assert testing yaml ", test_ut_kvp_profile_init_yaml, test_ut_kvp_profile_cleanup);
    assert(gpAssertSuite2 != NULL);

    UT_add_test(gpAssertSuite2, "kvp profile uint8", test_ut_kvp_profile_uint8);
    UT_add_test(gpAssertSuite2, "kvp profile uint16", test_ut_kvp_profile_uint16);
    UT_add_test(gpAssertSuite2, "kvp profile uint32", test_ut_kvp_profile_uint32);
    UT_add_test(gpAssertSuite2, "kvp profile uint64", test_ut_kvp_profile_uint64);
    UT_add_test(gpAssertSuite2, "kvp profile string", test_ut_kvp_profile_string);
    UT_add_test(gpAssertSuite2, "kvp profile bool", test_ut_kvp_profile_bool);

    gpAssertSuite3 = UT_add_suite("ut-kvp - assert testing json ", test_ut_kvp_profile_init_json, test_ut_kvp_profile_cleanup);
    assert(gpAssertSuite3 != NULL);

    UT_add_test(gpAssertSuite3, "kvp profile uint8", test_ut_kvp_profile_uint8);
    UT_add_test(gpAssertSuite3, "kvp profile uint16", test_ut_kvp_profile_uint16);
    UT_add_test(gpAssertSuite3, "kvp profile uint32", test_ut_kvp_profile_uint32);
    UT_add_test(gpAssertSuite3, "kvp profile uint64", test_ut_kvp_profile_uint64);
    UT_add_test(gpAssertSuite3, "kvp profile string", test_ut_kvp_profile_string);
    UT_add_test(gpAssertSuite3, "kvp profile bool", test_ut_kvp_profile_bool);
    
}
