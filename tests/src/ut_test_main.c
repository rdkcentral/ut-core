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

#include <stdlib.h>
#include <ut.h>

extern void register_assert_functions(void);
extern void register_kvp_assert_testing_functions(void);
extern void register_kvp_functions(void);

/**
 * @brief Main launch function for the test app
 * 
 * @param argc - param count from the command line
 * @param argv  - param list from the command line
 * @return int - 0 on success, otherwise failure
 */
int main(int argc, char** argv) 
{
    /* Register tests as required, then call the UT-main to support switches and triggering */
    UT_init( argc, argv );

    register_assert_functions();
    register_kvp_functions();
    register_kvp_assert_testing_functions();
    UT_run_tests();

    return 0;
}

