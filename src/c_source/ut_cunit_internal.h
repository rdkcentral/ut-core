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

/** @brief
 * UT Unit testing framework to test code.
 * 
 * Application wrapper for Testing Suite Unit Testing Framework
 *  - This wrapper exists in order to swap Unit testing suites as required
 * Order of calling to produce testing results
 * 
 * - client main(int argc, char **argv )
 * -        calls ut_init(argc,argv)
 * - client register test SUITE with ut_test_suite_add
 * - client register test into the SUITE with ut_test_add
 * - client calls ut_run_tests()
 * - results returned to client 
 */

/** @addtogroup UT_CUNIT
 * @{
 */

/** @brief UT Internal shared header
 * 
 */

#ifndef __UT_CUNIT_INTERNAL_H
#define __UT_CUNIT_INTERNAL_H

/* CUnit functions */
#include <CUnit.h>

extern CU_ErrorCode UT_basic_run_tests(void);
extern void UT_console_run_tests( void );
extern void UT_automated_run_tests(void);

extern void UT_list_tests(CU_pSuite pSuite);

extern CU_ErrorCode UT_automated_list_all_tests(CU_pTestRegistry pRegistry, const char* szFilename);
extern CU_ErrorCode UT_list_tests_to_file();

extern const char *UT_automated_package_name_get();
extern void UT_automated_package_name_set(const char *pName);
extern void UT_automated_enable_junit_xml(CU_BOOL bFlag);

#endif  /*  __UT_CUNIT_INTERNAL_H  */
/** @} */