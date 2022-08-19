/*
 *  CSUnit - Application wrapper for Comcast Sky Unit Testing Framework
 *         - This wrapper exists in order to swap functionality based on requirements
 *  - Expectation is that this will be replaced asap, when we confirm
 *  - cunit, or unity, gtest?
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/** @brief
 * CSUnit Unit testing macros to test code.
 * Macro Definations to support testing
 */
/** @addtogroup CSUnit
 * @{
 */

#ifndef __CSUNIT_H
#define __CSUNIT_H

#include <string.h>

#define CS_VERSION "1.0.0"

#define CUNIT

#ifdef CUNIT /* CSUnit -> CUnit Wrapper */

#include <TestRun.h>
#include <CUnit.h>

#define CS_TEST_PASS(msg) CU_PASS(msg)

/* Fatal test macros */
#define CS_TEST_FAIL(msg) CU_FAIL_FATAL(msg)
#define CS_TEST_ASSERT(value) CU_ASSERT_FATAL(msg)
#define CS_TEST_ASSERT_PTR_NULL(value) CU_ASSERT_PTR_NULL_FATAL(value)
#define CS_TEST_ASSERT_PTR_NOT_NULL(value) CU_ASSERT_PTR_NOT_NULL_FATAL(value)

#define CS_TEST_ASSERT_TRUE(value) CU_ASSERT_TRUE_FATAL(value)
#define CS_TEST_ASSERT_FALSE(value) CU_ASSERT_FALSE_FATAL(value)

#define CS_TEST_ASSERT_STRING_EQUAL(expected, actual) CU_ASSERT_STRING_EQUAL_FATAL(actual,expected)
#define CS_TEST_ASSERT_STRING_NOT_EQUAL(expected, actual) CU_ASSERT_STRING_NOT_EQUAL_FATAL(actual,expected)

/* Registration functions are only used when the module supports it */
#define CS_TEST_ADD_SUITE(title,init,clean) void *CU_add_suite(title,(CU_InitializeFunc)init,(CU_CleanupFunc)clean)
#define CS_TEST_ADD(suite,title,function) void *CU_add_test(suite, title, (CU_TestFunc)function);

#endif  /* CUNIT - Wrapper */

#endif  /*  __CSUNIT_H  */

/** @} */
