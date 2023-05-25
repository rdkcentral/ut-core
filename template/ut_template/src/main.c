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
 * @file main.c
 * @page module_name TODO: Required field, name of the main module
 * @subpage sub_page_name TODO: If submodule, required field, name of the functional group
 * @author TODO: Required field, author of this file; for OpenSource can be obfuscated email
                 or ideally real name
 * **Component Owner:** TODO: Required field, Lead Engineer of the component (Component Expert)
 *                      who must review; for OpenSource can be obfuscated email or ideally real name
 * **Component Architect:** TODO: Required field, Single Architect of the component who must review;
 *                          for OpenSource can be obfuscated email or ideally real name
 * **Review Team:** TODO: Review Team required to sign off the component changes; for OpenSource can
 *                  be obfuscated email or ideally real name (Min 2 reviewers)
 *
 * ## Module's Role
 *  TODO: Explain the module's role in the system in general
 *
 * **Pre-Conditions:** TODO: e.g. Component has not been started, if the component has started then
                            it must be manually shutdown before test execution can begin
 * **Dependencies:** TODO: Add dependencies if any
 *
 * Please refer to specification documentation : halSpec.md
 */

#include <ut.h>

/* L1 Testing Functions */
extern int test_l1_test_example_register( void );

/* L2 Testing Functions */
extern int test_l2_test_example_register( void );

int main(int argc, char** argv) 
{
    int registerFailed=0;

    /* Register tests as required, then call the UT-main to support switches and triggering */
    UT_init( argc, argv );

    registerFailed |= test_test_example_l1_register();
    registerFailed |= test_test_example_l2_register();

    UT_run_tests();
}
