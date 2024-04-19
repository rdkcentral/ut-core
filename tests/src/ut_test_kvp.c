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

#include <ut_log.h>
#include <ut.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
// #include <ut_kvp.h>
#include <ut_kvp_assert.h>

static UT_test_suite_t *gpLogSuite = NULL;
static UT_test_suite_t *gpAssertSuite = NULL;

void test_ut_kvp_assert_int(void)
{
    // UT_ASSERT_EQUAL_KVP_INT( 0, "/WifiRadioConfig/0/RadioIndex" );
    // { UT_ASSERT_EQUAL({ int kvpValue; kvpValue = ut_kvp_getField( gKVP_Instance, "\"/WifiRadioConfig/0/RadioIndex\"" ); kvpValue; }, "0" ); }
    int kvpValue;
    kvpValue = ut_kvp_getField(gKVP_Instance, "/WifiRadioConfig/0/RadioIndex");
    UT_ASSERT_EQUAL(kvpValue, 0);
}

#if 0
void test_ut_kvp_assert_string( void)
{
    char filename[] = "/xione.de.yaml";
    UT_LOG( "yamlfile = %s", filename);
    UT_ASSERT_EQUAL_KVP_STRING("radio1", "/WifiRadioConfig/0/RadioName" )
}
#endif

void test_ut_kvp_assert_bool(void)
{
    UT_ASSERT_EQUAL_KVP_BOOL(false, "/WifiRadioConfig/0/DcsEnabled %s");
}

void register_kvp_functions(void)
{
    ut_kvp_assert_load("/home/jyotsna/xione.de.yaml");

    gpAssertSuite = UT_add_suite("ut-kvp - yaml", NULL, NULL);
    if (NULL == gpAssertSuite)
    {
        return -1;
    }

    UT_add_test(gpAssertSuite, "kvp assert int", test_ut_kvp_assert_int);
    // UT_add_test(gpAssertSuite, "kvp assert string", test_ut_kvp_assert_string);
    UT_add_test(gpAssertSuite, "kvp assert bool", test_ut_kvp_assert_bool);

#if 0
    ut_kvp_assert_load( "testFile.json" );

    gpAssertSuite = UT_add_suite("ut-kvp - json", NULL, NULL);
    if (NULL == gpAssertSuite)
    {
        return -1;
    }

    UT_add_test(gpAssertSuite, "kvp assert int", test_ut_kvp_assert_int);
    //UT_add_test(gpAssertSuite, "kvp assert string", test_ut_kvp_assert_string);
    UT_add_test(gpAssertSuite, "kvp assert bool", test_ut_kvp_assert_bool);
#endif
    ut_kvp_destoryInstance(gKVP_Instance);
}
