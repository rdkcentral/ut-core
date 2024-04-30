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

#include <ut_kvp_assert.h>

#define KVP_VALID_TEST_ASSERT_YAML_FILE "/home/jpn323/workspace/rdk-halif-hdmi_cec/ut/ut-core/src/test_kvp_assert.yaml"
#define KVP_VALID_TEST_ASSERT_JSON_FILE "/home/jpn323/workspace/rdk-halif-hdmi_cec/ut/ut-core/src/test_kvp_assert.json"

static UT_test_suite_t *gpAssertSuite1 = NULL;
static UT_test_suite_t *gpAssertSuite2 = NULL;
static UT_test_suite_t *gpAssertSuite3 = NULL;

void test_ut_kvp_assert_uint32(void)
{
    uint32_t checkField = 0xdeadbeef;
    UT_ASSERT_EQUAL_KVP_UINT32( checkField, "decodeTest/checkUint32IsDeadBeef" );
}

void test_ut_kvp_assert_uint64(void)
{
    uint64_t checkField = 0xdeadbeefdeadbeef;
    UT_ASSERT_EQUAL_KVP_UINT64( checkField, "decodeTest/checkUint64IsDeadBeef" );
}

void test_ut_kvp_assert_string(void)
{
    const char *checkField = "the_beef_is_dead";
    UT_ASSERT_EQUAL_KVP_STRING( checkField, "decodeTest/checkStringDeadBeef");
}

void test_ut_kvp_assert_bool(void)
{
    UT_ASSERT_EQUAL_KVP_BOOL( false, "decodeTest/checkBoolFalse" );
    UT_ASSERT_EQUAL_KVP_BOOL( true, "decodeTest/checkBoolTrue" );
}

void test_ut_kvp_assert_load( void )
{
    ut_kvp_assert_load( KVP_VALID_TEST_ASSERT_YAML_FILE );
    ut_kvp_assert_load( KVP_VALID_TEST_ASSERT_YAML_FILE );
    ut_kvp_assert_load( KVP_VALID_TEST_ASSERT_YAML_FILE );
}

void test_ut_kvp_assert_unload( void )
{
    ut_kvp_assert_unload();
    ut_kvp_assert_unload();
    ut_kvp_assert_unload();
}

int test_ut_kvp_assert_init_yaml( void )
{
    ut_kvp_assert_load( KVP_VALID_TEST_ASSERT_YAML_FILE );
    return 0;
}

int test_ut_kvp_assert_init_json( void )
{
    ut_kvp_assert_load( KVP_VALID_TEST_ASSERT_JSON_FILE );
    return 0;
}

int test_ut_kvp_assert_cleanup( void )
{
    ut_kvp_assert_unload();
    return 0;
}

void register_kvp_assert_testing_functions(void)
{
    gpAssertSuite1 = UT_add_suite("ut-kvp - assert load / unload", NULL, NULL);
    assert(gpAssertSuite1 != NULL);
    UT_add_test(gpAssertSuite1, "kvp assert load()", test_ut_kvp_assert_load);
    UT_add_test(gpAssertSuite1, "kvp assert unload()", test_ut_kvp_assert_unload);

    gpAssertSuite2 = UT_add_suite("ut-kvp - assert testing yaml ", test_ut_kvp_assert_init_yaml, test_ut_kvp_assert_cleanup);
    assert(gpAssertSuite2 != NULL);

    UT_add_test(gpAssertSuite2, "kvp assert uint32", test_ut_kvp_assert_uint32);
    UT_add_test(gpAssertSuite2, "kvp assert uint64", test_ut_kvp_assert_uint64);
    UT_add_test(gpAssertSuite2, "kvp assert string", test_ut_kvp_assert_string);
    UT_add_test(gpAssertSuite2, "kvp assert bool", test_ut_kvp_assert_bool);

    gpAssertSuite3 = UT_add_suite("ut-kvp - assert testing json ", test_ut_kvp_assert_init_json, test_ut_kvp_assert_cleanup);
    assert(gpAssertSuite3 != NULL);

    UT_add_test(gpAssertSuite3, "kvp assert uint32", test_ut_kvp_assert_uint32);
    UT_add_test(gpAssertSuite3, "kvp assert uint64", test_ut_kvp_assert_uint64);
    UT_add_test(gpAssertSuite3, "kvp assert string", test_ut_kvp_assert_string);
    UT_add_test(gpAssertSuite3, "kvp assert bool", test_ut_kvp_assert_bool);
    
}
