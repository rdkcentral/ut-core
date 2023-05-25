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

/**
* @file TODO: test_l2_test_example.c
* @page module_name TODO: Required field, name of the main module
* @subpage sub_page_name TODO: Add a function group if relevant
*
*  **Component Owner:** TODO: Required field, Lead Engineer of the component ( Component Expert ) who must review; for OpenSource can be obfuscated email or ideally real name@n
*  **Component Architect:** TODO: Required field, Single Architect of the component who must review; for OpenSource can be obfuscated email or ideally real name@n
*  **Review Team:** TODO:  Review Team required to sign off the component changes; for OpenSource can be obfuscated email or ideally real name (Min 2 reviewers)@n
*
* ## Module's Role
* TODO: Explain the module's role in the system in general
* This is to ensure that the API meets the operational requirements of the module across all vendors.
*
* **Pre-Conditions:**  TODO: Add pre-conditions if any@n
* **Dependencies:** TODO: Add dependencies if any@n
*
* Ref to API Definition specification documentation : [halSpec.md](../../../docs/halSpec.md)
*/

#include <string.h>
#include <stdlib.h>

#include <ut.h>
#include <ut_log.h>

/**
* @brief TODO: Describe the object of the test
*
* TODO: Add the description of what is tested and why in this test
*
* **Test Group ID:** TODO: Add the group this test belongs to - Basic (for L1): 01 / Module (L2): 02 / Stress (L2): 03)@n
* **Test Case ID:** TODO: Add the ID of the test case so that it can be logically tracked in the logs@n
* **Priority:** (Low/Med/High) TODO: Add the priority for the level of test, how important is the test to overall functionality@n
*
* **Test Procedure:**
* Refer to UT specification documentation [l2_test_specification_template.md](l2_test_specification_template.md)
*/
void test_l2_test_example_function1(void)
{
	UT_FAIL("Need to implement");
}

/**
* @brief TODO: Describe the object of the test
*
* TODO: Add the description of what is tested and why in this test
*
* **Test Group ID:** TODO: Add the group this test belongs to - Basic (for L1): 01 / Module (L2): 02 / Stress (L2): 03)@n
* **Test Case ID:** TODO: Add the ID of the test case so that it can be logically tracked in the logs@n
* **Priority:** (Low/Med/High) TODO: Add the priority for the level of test, how important is the test to overall functionality@n
*
* **Test Procedure:**
* Refer to UT specification documentation [l2_test_specification_template.md](l2_test_specification_template.md)
*/
void test_l2_test_example_function2(void)
{
	UT_FAIL("Need to implement");
}

static UT_test_suite_t *pSuite = NULL;

/**
 * @brief Register the main tests for this module
 * 
 * @return int - 0 on success, otherwise failure
 */
int test_l2_test_example_register( void )
{
    /* add a suite to the registry */
    pSuite = UT_add_suite("[L2 test_example]", NULL, NULL);
    if (NULL == pSuite) 
    {
        return UT_STATUS_FAILURE;
    }

    UT_add_test( pSuite, "function1_L2", test_l2_test_example_function1);
    UT_add_test( pSuite, "function2_L2", test_l2_test_example_function2);

    return UT_STATUS_OK;
}
