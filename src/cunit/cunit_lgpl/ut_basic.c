/*
 *  CUnit - A Unit testing framework library for C.
 *  Copyright (C) 2004-2006  Jerry St.Clair, Anil Kumar
 *  Copyright (C) 2023 RDK Management
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

/*
 *  Implementation for basic test runner interface.
 *
 *  11-Oct-2023   Changed Wrapper functions to support common logging & overrides. (Ulrond)
 * 
 *  11-Aug-2004   Initial implementation of basic test runner interface.  (JDS)
 *
 *  8-Jan-2005    Fixed reporting bug (bug report cunit-Bugs-1093861).  (JDS)
 *
 *  30-Apr-2005   Added notification of suite cleanup failure.  (JDS)
 *
 *  02-May-2006   Added internationalization hooks.  (JDS)
 */

/** @file
 * Basic interface with output to stdout.
 */
/** @addtogroup Basic
 * @{
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>

#include "CUnit.h"
#include "TestDB.h"
#include "Util.h"
#include "TestRun.h"
#include "Basic.h"
#include "CUnit_intl.h"

#include <ut_log.h>

/*=================================================================
 *  Global/Static Definitions
 *=================================================================*/
/** Pointer to the currently running suite. */
static CU_pSuite f_pRunningSuite = NULL;
/** Current run mode. */
static CU_BasicRunMode f_run_mode = CU_BRM_NORMAL;

/*=================================================================
 *  Forward declaration of module functions *
 *=================================================================*/
static CU_ErrorCode basic_initialize(void);
static CU_ErrorCode basic_run_all_tests(CU_pTestRegistry pRegistry);
static CU_ErrorCode basic_run_suite(CU_pSuite pSuite);
static CU_ErrorCode basic_run_single_test(CU_pSuite pSuite, CU_pTest pTest);

static void basic_test_start_message_handler(const CU_pTest pTest, const CU_pSuite pSuite);
static void basic_test_complete_message_handler(const CU_pTest pTest, const CU_pSuite pSuite, const CU_pFailureRecord pFailureList);
static void basic_all_tests_complete_message_handler(const CU_pFailureRecord pFailure);
static void basic_suite_init_failure_message_handler(const CU_pSuite pSuite);
static void basic_suite_cleanup_failure_message_handler(const CU_pSuite pSuite);

/*=================================================================
 *  Public Interface functions
 *=================================================================*/
CU_ErrorCode UT_basic_run_tests(void)
{
  CU_ErrorCode error;

  if (NULL == CU_get_registry()) {
    if (CU_BRM_SILENT != f_run_mode)
      UT_LOG_ERROR( "\n\n%s\n", _("FATAL ERROR - Test registry is not initialized."));
    error = CUE_NOREGISTRY;
  }
  else if (CUE_SUCCESS == (error = basic_initialize()))
    error = basic_run_all_tests(NULL);

  return error;
}

/*------------------------------------------------------------------------*/
CU_ErrorCode UT_basic_run_suite(CU_pSuite pSuite)
{
  CU_ErrorCode error;

  if (NULL == pSuite)
    error = CUE_NOSUITE;
  else if (CUE_SUCCESS == (error = basic_initialize()))
    error = basic_run_suite(pSuite);

  return error;
}

/*------------------------------------------------------------------------*/
CU_ErrorCode UT_basic_run_test(CU_pSuite pSuite, CU_pTest pTest)
{
  CU_ErrorCode error;

  if (NULL == pSuite)
    error = CUE_NOSUITE;
  else if (NULL == pTest)
    error = CUE_NOTEST;
  else if (CUE_SUCCESS == (error = basic_initialize()))
    error = basic_run_single_test(pSuite, pTest);

  return error;
}

/*------------------------------------------------------------------------*/
void UT_basic_set_mode(CU_BasicRunMode mode)
{
  f_run_mode = mode;
}

/*------------------------------------------------------------------------*/
CU_BasicRunMode UT_basic_get_mode(void)
{
  return f_run_mode;
}

/*------------------------------------------------------------------------*/
void UT_basic_show_failures(CU_pFailureRecord pFailure)
{
  int i;

  for (i = 1 ; (NULL != pFailure) ; pFailure = pFailure->pNext, i++) 
  {
    UT_LOG( "  %d. %s:%u  - %s", i,
        (NULL != pFailure->strFileName) ? pFailure->strFileName : "",
        pFailure->uiLineNumber,
        (NULL != pFailure->strCondition) ? pFailure->strCondition : "");
  }
}

/*=================================================================
 *  Static module functions
 *=================================================================*/
/** Performs inialization actions for the basic interface.
 *  This includes setting output to unbuffered, printing a
 *  welcome message, and setting the test run handlers.
 *  @return An error code indicating the framework error condition.
 */
static CU_ErrorCode basic_initialize(void)
{
  /* Unbuffered output so everything reaches the screen */
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  CU_set_error(CUE_SUCCESS);

#if 0
  if (CU_BRM_SILENT != f_run_mode)
    UT_LOG( "\n\n     %s" CU_VERSION
                      "\n     %s\n\n",
                    _("CUnit - A unit testing framework for C - Version "),
                    _("http://cunit.sourceforge.net/"));
#endif

  CU_set_test_start_handler(basic_test_start_message_handler);
  CU_set_test_complete_handler(basic_test_complete_message_handler);
  CU_set_all_test_complete_handler(basic_all_tests_complete_message_handler);
  CU_set_suite_init_failure_handler(basic_suite_init_failure_message_handler);
  CU_set_suite_cleanup_failure_handler(basic_suite_cleanup_failure_message_handler);

  return CU_get_error();
}

/*------------------------------------------------------------------------*/
/** Runs all tests within the basic interface.
 *  If non-NULL, the test registry is changed to the specified registry
 *  before running the tests, and reset to the original registry when
 *  done.  If NULL, the default CUnit test registry will be used.
 *  @param pRegistry The CU_pTestRegistry containing the tests
 *                   to be run.  If NULL, use the default registry.
 *  @return An error code indicating the error status
 *          during the test run.
 */
static CU_ErrorCode basic_run_all_tests(CU_pTestRegistry pRegistry)
{
  CU_pTestRegistry pOldRegistry = NULL;
  CU_ErrorCode result;

  f_pRunningSuite = NULL;

  if (NULL != pRegistry)
    pOldRegistry = CU_set_registry(pRegistry);
  result = CU_run_all_tests();
  if (NULL != pRegistry)
    CU_set_registry(pOldRegistry);
  return result;
}

/*------------------------------------------------------------------------*/
/** Runs a specified suite within the basic interface.
 *  @param pSuite The suite to be run (non-NULL).
 *  @return An error code indicating the error status
 *          during the test run.
 */
static CU_ErrorCode basic_run_suite(CU_pSuite pSuite)
{
  f_pRunningSuite = NULL;
  return CU_run_suite(pSuite);
}

/*------------------------------------------------------------------------*/
/** Runs a single test for the specified suite within
 *  the console interface.
 *  @param pSuite The suite containing the test to be run (non-NULL).
 *  @param pTest  The test to be run (non-NULL).
 *  @return An error code indicating the error status
 *          during the test run.
 */
static CU_ErrorCode basic_run_single_test(CU_pSuite pSuite, CU_pTest pTest)
{
  f_pRunningSuite = NULL;
  return CU_run_test(pSuite, pTest);
}

/*------------------------------------------------------------------------*/
/** Handler function called at start of each test.
 *  @param pTest  The test being run.
 *  @param pSuite The suite containing the test.
 */
static void basic_test_start_message_handler(const CU_pTest pTest, const CU_pSuite pSuite)
{
  assert(NULL != pSuite);
  assert(NULL != pTest);

  assert(NULL != pTest->pName);
  if ((NULL == f_pRunningSuite) || (f_pRunningSuite != pSuite)) 
  {
    assert(NULL != pSuite->pName);
    UT_LOG( UT_LOG_ASCII_BLUE"Running Suite : "UT_LOG_ASCII_CYAN"%s"UT_LOG_ASCII_NC, pSuite->pName);
    f_pRunningSuite = pSuite;
  }
  UT_LOG( UT_LOG_ASCII_GREEN"     Running Test : "UT_LOG_ASCII_CYAN"\'%s\'"UT_LOG_ASCII_NC, pTest->pName);
}

/*------------------------------------------------------------------------*/
/** Handler function called at completion of each test.
 *  @param pTest   The test being run.
 *  @param pSuite  The suite containing the test.
 *  @param pFailure Pointer to the 1st failure record for this test.
 */
static void basic_test_complete_message_handler(const CU_pTest pTest,
                                                const CU_pSuite pSuite,
                                                const CU_pFailureRecord pFailureList)
{
  CU_pFailureRecord pFailure = pFailureList;
  int i;

  assert(NULL != pSuite);
  assert(NULL != pTest);

  if (NULL == pFailure) {
    if (CU_BRM_VERBOSE == f_run_mode) {
      UT_LOG( _(UT_LOG_ASCII_GREEN"passed"UT_LOG_ASCII_NC));
    }
  }
  else {
    switch (f_run_mode) {
      case CU_BRM_VERBOSE:
        UT_LOG_ERROR( "FAILED" );
        break;
      case CU_BRM_NORMAL:
        assert(NULL != pSuite->pName);
        assert(NULL != pTest->pName);
        UT_LOG( _(UT_LOG_ASCII_CYAN"Suite %s, Test %s had failures:"UT_LOG_ASCII_NC), pSuite->pName, pTest->pName);
        break;
      default:  /* gcc wants all enums covered.  ok. */
        break;
    }
    if (CU_BRM_SILENT != f_run_mode) {
      for (i = 1 ; (NULL != pFailure) ; pFailure = pFailure->pNext, i++) {
        UT_LOG( UT_LOG_ASCII_YELLOW"    %d. %s:%u  - %s"UT_LOG_ASCII_NC, i,
            (NULL != pFailure->strFileName) ? pFailure->strFileName : "",
            pFailure->uiLineNumber,
            (NULL != pFailure->strCondition) ? pFailure->strCondition : "");
      }
    }
  }

  /* Comparing the Addresses rather than the Group Names. */
  UT_LOG( UT_LOG_ASCII_GREEN"     Test Complete : "UT_LOG_ASCII_CYAN"\'%s\'"UT_LOG_ASCII_NC, pTest->pName);

}

/*------------------------------------------------------------------------*/
/** Handler function called at completion of all tests in a suite.
 *  @param pFailure Pointer to the test failure record list.
 */
static void basic_all_tests_complete_message_handler(const CU_pFailureRecord pFailure)
{
  CU_UNREFERENCED_PARAMETER(pFailure); /* not used in basic interface */
  printf("\n\n");
  CU_print_run_results(stdout);
  printf("\n");
}

/*------------------------------------------------------------------------*/
/** Handler function called when suite initialization fails.
 *  @param pSuite The suite for which initialization failed.
 */
static void basic_suite_init_failure_message_handler(const CU_pSuite pSuite)
{
  assert(NULL != pSuite);
  assert(NULL != pSuite->pName);

  if (CU_BRM_SILENT != f_run_mode)
    UT_LOG( _("\nWARNING - Suite initialization failed for '%s'."), pSuite->pName);
}

/*------------------------------------------------------------------------*/
/** Handler function called when suite cleanup fails.
 *  @param pSuite The suite for which cleanup failed.
 */
static void basic_suite_cleanup_failure_message_handler(const CU_pSuite pSuite)
{
  assert(NULL != pSuite);
  assert(NULL != pSuite->pName);

  if (CU_BRM_SILENT != f_run_mode)
    UT_LOG( _("\nWARNING - Suite cleanup failed for '%s'."), pSuite->pName);
}

/** @} */
