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
* @file TODO: test_l1_test_example.c
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
* @brief TODO: Describe the objective of the test
*
* TODO: Add the description of what is tested and why in this test@n
*
* **Test Group ID:** TODO: Basic (for L1): 01 / Module (L2): 02 / Stress (L2): 03)@n
* **Test Case ID:** TODO: Add the ID of the test case so that it can be logically tracked in the logs@n
* **Priority:** (Low/Med/High) TODO: Add the priority for the level of test, how important is the test to overall functionality@n
* @n
* **Pre-Conditions:** TODO: Add pre-conditions, if any@n
* **Dependencies:** TODO: Add dependencies for this test, if any@n
* **User Interaction:** TODO: Add any user interactions required during this test, if any
* @n
* **Test Procedure:**@n
* TODO: Add the steps to run this test and add a line in the below table for each input variation tried in this function.@n
* | Variation / Step | Description | Test Data |Expected Result |Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | First set of conditions | What is input data to be tested | How to gauge success, is it a success variable? | Should be successful |
*/
void test_l1_test_example_positive_function1(void)
{
	UT_FAIL("Need to implement");
} 

/**
* @brief TODO: Describe the objective of the test
*
* TODO: Add the description of what is tested and why in this test@n
*
* **Test Group ID:** TODO: Basic (for L1): 01 / Module (L2): 02 / Stress (L2): 03)@n
* **Test Case ID:** TODO: Add the ID of the test case so that it can be logically tracked in the logs@n
* **Priority:** (Low/Med/High) TODO: Add the priority for the level of test, how important is the test to overall functionality@n
* @n
* **Pre-Conditions:** TODO: Add pre-conditions, if any@n
* **Dependencies:** TODO: Add dependencies for this test, if any@n
* **User Interaction:** TODO: Add any user interactions required during this test, if any
* @n
* **Test Procedure:**@n
* TODO: Add the steps to run this test and add a line in the below table for each input variation tried in this function.@n
* | Variation / Step | Description | Test Data |Expected Result |Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | First set of conditions | What is input data to be tested | How to gauge success, is it a success variable? | Should be successful |
*/
void test_l1_test_example_negative_function1(void)
{
	UT_FAIL("Need to implement");
}

/**
* @brief TODO: Describe the objective of the test
*
* TODO: Add the description of what is tested and why in this test@n
*
* **Test Group ID:** TODO: Basic (for L1): 01 / Module (L2): 02 / Stress (L2): 03)@n
* **Test Case ID:** TODO: Add the ID of the test case so that it can be logically tracked in the logs@n
* **Priority:** (Low/Med/High) TODO: Add the priority for the level of test, how important is the test to overall functionality@n
* @n
* **Pre-Conditions:** TODO: Add pre-conditions, if any@n
* **Dependencies:** TODO: Add dependencies for this test, if any@n
* **User Interaction:** TODO: Add any user interactions required during this test, if any
* @n
* **Test Procedure:**@n
* TODO: Add the steps to run this test and add a line in the below table for each input variation tried in this function.@n
* | Variation / Step | Description | Test Data |Expected Result |Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | First set of conditions | What is input data to be tested | How to gauge success, is it a success variable? | Should be successful |
*/
void test_l1_test_example_positive_function2(void)
{
	UT_FAIL("Need to implement");
} 

/**
* @brief TODO: Describe the objective of the test
*
* TODO: Add the description of what is tested and why in this test@n
*
* **Test Group ID:** TODO: Basic (for L1): 01 / Module (L2): 02 / Stress (L2): 03)@n
* **Test Case ID:** TODO: Add the ID of the test case so that it can be logically tracked in the logs@n
* **Priority:** (Low/Med/High) TODO: Add the priority for the level of test, how important is the test to overall functionality@n
* @n
* **Pre-Conditions:** TODO: Add pre-conditions, if any@n
* **Dependencies:** TODO: Add dependencies for this test, if any@n
* **User Interaction:** TODO: Add any user interactions required during this test, if any
* @n
* **Test Procedure:**@n
* TODO: Add the steps to run this test and add a line in the below table for each input variation tried in this function.@n
* | Variation / Step | Description | Test Data |Expected Result |Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | First set of conditions | What is input data to be tested | How to gauge success, is it a success variable? | Should be successful |
*/
void test_l1_test_example_negative_function2(void)
{
	UT_FAIL("Need to implement");
}

static UT_test_suite_t *pSuite = NULL;

/**
 * @brief Register the main tests for this module
 * 
 * @return int - 0 on success, otherwise failure
 */
int test_l1_test_example_register( void )
{
    /* add a suite to the registry */
    pSuite = UT_add_suite("[L1 test_example]", NULL, NULL);
    if (NULL == pSuite) 
    {
        return -1;
    }

    UT_add_test( pSuite, "function1_L1_positive", test_l1_test_example_positive_function1);
    UT_add_test( pSuite, "function1_L1_negative", test_l1_test_example_negative_function1);
    UT_add_test( pSuite, "function2_L2_positive", test_l1_test_example_positive_function2);
    UT_add_test( pSuite, "function2_L2_negative", test_l1_test_example_negative_function2);

    return 0;
}
