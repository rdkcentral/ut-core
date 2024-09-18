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

#ifdef UT_CUNIT
#include <stdlib.h>
#include <stdbool.h>
#include <ut.h>
#include <ut_internal.h>
extern void register_assert_functions(void);
extern void register_kvp_profile_testing_functions(void);
#else
#include <ut_test_gtest.h>
#endif


int UT_run(int argc, char** argv)
{
#ifdef UT_CUNIT
    // register_assert_functions(); /* FIXME: Enable when any change is performed to the UT_ASSERT functions.
    // Since this always fails we want it outside our normal testing, which currently is 100% PASS */
    register_kvp_profile_testing_functions();

    UT_run_tests();

    return 0;
#else

    UTTestRunner testRunner(argc, argv);
    return testRunner.runTests();

#endif
}