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


static UT_test_suite_t *gpAssertSuite = NULL;
static UT_test_suite_t *gpAssertSuite1 = NULL;

void test_ut_kvp_assert_uint32(void)
{
    uint32_t checkField = 100;
    uint32_t actualField = ut_kvp_getUInt32Field( ut_kvp_assert_getInstance(), "/WifiRadioConfig/0/CsaBeaconCount %s" );
    UT_ASSERT_EQUAL(actualField, checkField);
}

void test_ut_kvp_assert_uint64(void)
{
    uint64_t checkField = 12;
    uint64_t actualField = ut_kvp_getUInt64Field( ut_kvp_assert_getInstance(), "/WifiRadioConfig/0/HwMode %s" );
    UT_ASSERT_EQUAL(actualField, checkField);
}

void test_ut_kvp_assert_string( void)
{
    const char *checkField = "radio1";
    char actualField[20] = {"0"};
    ut_kvp_getStringField( ut_kvp_assert_getInstance(), "/WifiRadioConfig/0/RadioName %s", actualField );
    const char* actualFieldConst = actualField;
    UT_ASSERT_STRING_EQUAL(checkField, actualFieldConst);
}

void test_ut_kvp_assert_bool(void)
{
    /* Test that our macro's work */
    bool checkField = false;
    bool actualField = ut_kvp_getBoolField( ut_kvp_assert_getInstance(), "/WifiRadioConfig/0/DcsEnabled %s" );
    UT_ASSERT_EQUAL(actualField, checkField);
}

void test_ut_kvp_assert_bool_json(void)
{
    /* Test that our macro's work */
    bool checkField = true;
    bool actualField = ut_kvp_getBoolField( ut_kvp_assert_getInstance(), "compilerOptions/allowJs %s" );
    UT_ASSERT_EQUAL(actualField, checkField);
}

void test_ut_kvp_assert_string_json( void)
{
    const char *checkField = "ES2020";
    char actualField[20] = {"0"};
    ut_kvp_getStringField( ut_kvp_assert_getInstance(), "compilerOptions/target %s", actualField );
    const char* actualFieldConst = actualField;
    UT_ASSERT_STRING_EQUAL(checkField, actualFieldConst);
}

void register_kvp_functions(void)
{
    ut_kvp_assert_load("/home/jpn323/workspace/xione.de.yaml");

    gpAssertSuite = UT_add_suite("ut-kvp - yaml", NULL, NULL);
    assert(gpAssertSuite != NULL);

    UT_add_test(gpAssertSuite, "kvp assert uint32", test_ut_kvp_assert_uint32);
    UT_add_test(gpAssertSuite, "kvp assert uint64", test_ut_kvp_assert_uint64);
    UT_add_test(gpAssertSuite, "kvp assert string", test_ut_kvp_assert_string);
    UT_add_test(gpAssertSuite, "kvp assert bool", test_ut_kvp_assert_bool);

    ut_kvp_assert_load( "/home/jpn323/workspace/tsconfig.json" );

    gpAssertSuite1 = UT_add_suite("ut-kvp - json", NULL, NULL);
    assert(gpAssertSuite != NULL);

    UT_add_test(gpAssertSuite1, "kvp assert bool", test_ut_kvp_assert_bool_json);
    UT_add_test(gpAssertSuite1, "kvp assert string", test_ut_kvp_assert_string_json);

}

void unregister_kvp_functions(void)
{
    ut_kvp_assert_unload();
}
