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
#include <ut_kvp.h>
#include <ut_log.h>

#define KVP_VALID_TEST_YAML_FILE "/home/jpn323/workspace/rdk-halif-hdmi_cec/ut/ut-core/src/test_kvp.yaml"

static ut_kvp_instance_t *gpMainTestInstance = NULL;
static UT_test_suite_t *gpKVPSuite = NULL;
static UT_test_suite_t *gpKVPSuite2 = NULL;

void test_ut_kvp_testCreateDestroy( void )
{
    ut_kvp_instance_t *pInstance = NULL;
    ut_kvp_instance_t *pInstance1 = NULL;

    ut_kvp_destroyInstance(NULL);
    UT_ASSERT( pInstance == NULL );

    pInstance = ut_kvp_createInstance();
    UT_ASSERT( pInstance != NULL );

    pInstance1 = ut_kvp_createInstance();
    UT_ASSERT( pInstance1 != NULL );

    UT_ASSERT( pInstance != pInstance1 );

    ut_kvp_destroyInstance(pInstance1);
    ut_kvp_destroyInstance(pInstance);
}

void test_ut_kvp_read( void )
{
    ut_kvp_instance_t *pInstance = NULL;
    ut_kvp_status_t status;

    UT_LOG_STEP("ut_kvp_createInstance");
    pInstance = ut_kvp_createInstance();
    UT_ASSERT( pInstance != NULL );

    UT_LOG_STEP("ut_kvp_read( pInstance, NULL )");
    status = ut_kvp_read( pInstance, NULL );
    UT_ASSERT( status == UT_KVP_INVALID_PARAM );

    UT_LOG_STEP("ut_kvp_read( pInstance, nofile.txt )");
    status = ut_kvp_read( pInstance, "./nofile.txt" );
    UT_ASSERT( status == UT_KVP_FILE_OPEN_ERROR );

    UT_LOG_STEP("ut_kvp_read( pInstance, dummynodatafile.txt )");
    status = ut_kvp_read( pInstance, "/home/jpn323/workspace/rdk-halif-hdmi_cec/ut/ut-core/src/dummynodatafile.txt" );
    UT_ASSERT( status == UT_KVP_FILE_READ_ERROR );

    UT_LOG_STEP("ut_kvp_read( pInstance,  KVP_VALID_TEST_YAML_FILE )");
    status = ut_kvp_read( pInstance, KVP_VALID_TEST_YAML_FILE );
    UT_ASSERT( status == UT_KVP_STATUS_OK );

    /* Test Destory causes close */
    ut_kvp_destroyInstance( pInstance );
    ut_kvp_destroyInstance( pInstance );

    /* Calling close after destory should have no effect */
    ut_kvp_close(pInstance);
    ut_kvp_close(pInstance);

    /* Re-check close is actually ok by walking through and auto running */
    pInstance = ut_kvp_createInstance();
    UT_ASSERT( pInstance != NULL );

    ut_kvp_close(pInstance);
    ut_kvp_close(pInstance);
}

void test_ut_kvp_uint32(void)
{
    uint32_t result;
    uint32_t checkField = 0xdeadbeef;
    result = ut_kvp_getUInt32Field( gpMainTestInstance, "decodeTest/checkUint32IsDeadBeefHex" );
    UT_ASSERT( result == checkField );
}

void test_ut_kvp_uint64(void)
{
    uint64_t result;
    uint64_t checkField = 0xdeadbeefdeadbeef;
    result = ut_kvp_getUInt64Field( gpMainTestInstance, "decodeTest/checkUint64IsDeadBeef" );
    UT_ASSERT( result == checkField );
}

void test_ut_kvp_string(void)
{
    const char *result;
    const char *checkField = "the_beef_is_dead";
    char result_kvp[256] = {0};
    result = ut_kvp_getField( gpMainTestInstance, "decodeTest/checkStringDeadBeef", result_kvp);
    UT_ASSERT_STRING_EQUAL( result, checkField );
}

void test_ut_kvp_bool(void)
{
    bool result;
    result = ut_kvp_getBoolField( gpMainTestInstance, "decodeTest/checkBoolTRUE" );
    UT_ASSERT( result == true );

    result = ut_kvp_getBoolField( gpMainTestInstance, "decodeTest/checkBooltrue" );
    UT_ASSERT( result == true );

    result = ut_kvp_getBoolField( gpMainTestInstance, "decodeTest/checkBoolTRuE" );
    UT_ASSERT( result == true );

    result = ut_kvp_getBoolField( gpMainTestInstance, "decodeTest/checkBoolFalse" );
    UT_ASSERT( result == false );
}

int test_ut_kvp_createGlobalInstance( void )
{
    ut_kvp_status_t status;
    gpMainTestInstance = ut_kvp_createInstance();
    assert( gpMainTestInstance != NULL );

    status = ut_kvp_read( gpMainTestInstance, KVP_VALID_TEST_YAML_FILE );
    assert( status == UT_KVP_STATUS_OK );

    return 0;
}

int test_ut_kvp_freeGlobalInstance( void )
{
    ut_kvp_destroyInstance( gpMainTestInstance );
    return 0;
}

void register_kvp_functions( void )
{
    gpKVPSuite = UT_add_suite("ut-kvp - test functions ", NULL, NULL);
    assert(gpKVPSuite != NULL);

    UT_add_test(gpKVPSuite, "kvp create / destory", test_ut_kvp_testCreateDestroy);
    UT_add_test(gpKVPSuite, "kvp read", test_ut_kvp_read);

    gpKVPSuite2 = UT_add_suite("ut-kvp - test mainfunctions ", test_ut_kvp_createGlobalInstance, test_ut_kvp_freeGlobalInstance);
    assert(gpKVPSuite2 != NULL);

    UT_add_test(gpKVPSuite2, "kvp bool", test_ut_kvp_bool);
    UT_add_test(gpKVPSuite2, "kvp string", test_ut_kvp_string);
    UT_add_test(gpKVPSuite2, "kvp uint32", test_ut_kvp_uint32);
    UT_add_test(gpKVPSuite2, "kvp uint64", test_ut_kvp_uint64);
}