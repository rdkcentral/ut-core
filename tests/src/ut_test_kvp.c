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
#include <ut_kvp_assert.h>

static UT_test_suite_t *gpLogSuite = NULL;
static UT_test_suite_t *gpAssertSuite = NULL;

void test_ut_kvp_log( )
{
    ut_kvp_handle_t *p_Handle = NULL;
    FILE *inputfileptr = NULL;

    p_Handle = ut_kvp_createInstance_from_file_ptr(inputfileptr);
    if (NULL == p_Handle)
    {
        UT_LOG_STEP("Unable to build YAML file\n");
        return;
    }

    ut_kvp_document_get_int(p_Handle, "/WifiRadioConfig/0/RadioIndex %d ");
    ut_kvp_document_get_int(p_Handle, "/WifiRadioConfig/1/RadioIndex %d ");

    ut_kvp_document_get_string(p_Handle, "/WifiRadioConfig/0/RadioName %s");
    ut_kvp_document_get_string(p_Handle, "/WifiRadioConfig/1/RadioName %s");

    ut_kvp_document_get_bool(p_Handle, "/WifiRadioConfig/0/AutoChannelEnabled %s");
    ut_kvp_document_get_bool(p_Handle, "/WifiRadioConfig/0/DcsEnabled %s");

    ut_kvp_pHandledestoryInstance_from_file_ptr(p_Handle);
}

#if 0
void test_ut_kvp_assert_int( void )
{
    ut_kvp_handle_t *p_Handle = NULL;
    FILE *inputfileptr = NULL;
    ut_kvp_value_t kvpValue;

    p_Handle = ut_kvp_createInstance_from_file_ptr(inputfileptr);
    kvpValue = ut_kvp_document_get_int(p_Handle, "/WifiRadioConfig/0/RadioIndex %d");
    UT_ASSERT( kvpValue.type == UT_KVP_INT);
    ut_kvp_pHandledestoryInstance_from_file_ptr(p_Handle);
    UT_ASSERT(kvpValue.u.bValue == 1);
    ut_kvp_pHandledestoryInstance_from_file_ptr(p_Handle);
}
#endif

void register_kvp_functions(void)
{
    gpAssertSuite = UT_add_suite("ut-kvp", NULL, NULL);
    if (NULL == gpAssertSuite)
    {
        return -1;
    }

    UT_add_test(gpAssertSuite, "kvp reference test", test_ut_kvp_log);
    //UT_add_test(gpAssertSuite, "kvp assert int", test_ut_kvp_assert_int);
}
