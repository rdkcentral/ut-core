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

#include <ut.h>
#include <stdbool.h>
#include <stdint.h>

static UT_test_suite_t * gpLogSuite = NULL;
static UT_test_suite_t * gpAssertSuite = NULL;

int ut_init_function( void )
{
    UT_LOG( "--- Start of Suite ---" );
    return 0;
}

int ut_clean_function( void )
{
    UT_LOG( "--- End of Suite ---\n" );
    return 0;
}

void test_ut_logging( void )
{
    const char string[]="I'm a string";

    UT_LOG( "This message should print out with an [I'm a string, in the next box] -> [%s]\n", string );
}

void test_ut_assert( void )
{
    UT_LOG( "Step 1: is expected to pass" );
    UT_ASSERT( true==true );

    UT_LOG( "Step 2: is expected to fail :" );
    UT_ASSERT( true==false );   /* This line should assert */

    UT_LOG("### This line should never be seen\n");
}

void test_ut_assert_pass_fail( void )
{
    UT_LOG( "Step 1: Test UT_PASS");
    UT_PASS("UT PASS MESSAGE\n");

    UT_LOG( "Step 2: Test UT_FAIL");
    UT_FAIL("UT FAIL MESSAGE - Should Fail test will not continue:"); /* This line should assert */

    UT_LOG("+++ This line should be seen\n");
}

void test_ut_assert_fatal( void )
{
    UT_LOG( "Step 1: UT_FAIL_FATAL");
    UT_FAIL_FATAL("Test UT_FAIL_FATAL"); /* This line should assert */

    UT_LOG("### This line should never be seen\n");
}

void test_ut_assert_ptr_NULL()
{
    int *pTR = (int *)1;

    UT_LOG( "Step 1: UT_ASSERT_PTR_NULL (pTR == NULL )");
    UT_ASSERT_PTR_NULL( NULL );

    UT_LOG( "Step 2: UT_ASSERT_PTR_NULL (pTR == 1 )");
    UT_ASSERT_PTR_NULL( pTR );   /* Should Fail */

    UT_LOG("### This line should never be seen\n");
}

void test_ut_assert_ptr_NOT_NULL()
{
    int *pTR = (int*) 1;

    UT_LOG( "Step 1: UT_ASSERT_PTR_NOT_NULL (pTR = 1 )");
    UT_ASSERT_PTR_NOT_NULL( pTR );

    UT_LOG( "Step 2: UT_ASSERT_PTR_NOT_NULL (pTR == NULL )");
    UT_ASSERT_PTR_NOT_NULL( NULL );   /* Should Fail */

    UT_LOG("### This line should never be seen\n");
}

void test_ut_assert_TRUE()
{
    UT_LOG( "Step 1: UT_ASSERT_TRUE ( false!=true ) : No Assert ");
    UT_ASSERT_TRUE( true==true );

    UT_LOG( "Step 2: UT_ASSERT_TRUE ( true=true ) : fail :"); 
    UT_ASSERT_TRUE( true!=true ); /* Should Fail */

    UT_LOG("### This line should never be seen\n");
}

void test_ut_assert_FALSE()
{
    UT_LOG( "Step 1: UT_ASSERT_FALSE ( true==false ) : No Assert ");
    UT_ASSERT_FALSE( true==false );

    UT_LOG( "Step 2: UT_ASSERT_FALSE ( true!=false ) : fail"); 
    UT_ASSERT_FALSE( true!=false ); /* Should fail */

    UT_LOG("### This line should never be seen\n");
}

void test_ut_assert_EQUAL( void )
{
    UT_LOG( "Step 1: UT_ASSERT_EQUAL ( true, true ) : PASS"); 
    UT_ASSERT_EQUAL( true, true );

    UT_LOG( "Step 2: UT_ASSERT_EQUAL ( true, false ) : FAIL");
    UT_ASSERT_EQUAL( true, false );  /* Should FAIL */

    UT_LOG("### This line should never be seen\n");
}

void test_ut_assert_STRING_EQUAL( void )
{
    UT_LOG( "Step 1: UT_ASSERT_STRING_EQUAL ( bob, bob ) : No Assert ");
    UT_ASSERT_STRING_EQUAL( "bob", "bob" );

    UT_LOG( "Step 2: UT_ASSERT_STRING_EQUAL ( bob, fred ) : fail"); 
    UT_ASSERT_STRING_EQUAL( "bob", "fred" ); /* Should FAil */

    UT_LOG("### This line should never be seen\n");
}

void test_ut_assert_STRING_NOT_EQUAL( void )
{
#define UT_ASSERT_STRING_NOT_EQUAL(expected, actual) CU_ASSERT_STRING_NOT_EQUAL_FATAL(actual,expected)
    UT_LOG( "Step 2: UT_ASSERT_STRING_NOT_EQUAL ( bob, bob ) : fail");
    UT_ASSERT_STRING_NOT_EQUAL( "bob", "fred" );

    UT_LOG( "Step 1: UT_ASSERT_STRING_NOT_EQUAL ( bob, fred ) : No Assert "); 
    UT_ASSERT_STRING_NOT_EQUAL( "bob", "bob" ); /* Should FAIL */

    UT_LOG("### This line should never be seen\n");
}

void test_ut_assert_msg( void )
{
    UT_LOG( "Step 1: is expected not to assert" );
    UT_ASSERT_MSG( true==true, "should not assert");

    UT_LOG( "This tests should cause 2 failures, if so then it's passed, it will quite the test after the first failure" );

    UT_LOG( "Step 2: is expected to fail" );
    UT_ASSERT_MSG( false==true, "Step 2 : Asserted Correctly if you see this message :");   /* This line should assert */

    UT_LOG("### This line should never be seen\n");
}

void test_ut_assert_log( void )
{
    UT_ASSERT_LOG( true==true, "Step 3: ASSERT_LOG : should log and not fail :")
    UT_ASSERT_LOG( true==false, "Step 4: ASSERT_LOG : should log and fail :")   /* This line should assert */

    UT_LOG("### This line should never be seen\n");
}

/**
 * @brief Register the main tests for this module
 *
 * @return int - 0 on success, otherwise failure
 */

/**
 * @brief Main launch function for the test app
 * 
 * @param argc - param count from the command line
 * @param argv  - param list from the command line
 * @return int - 0 on success, othersise failure
 */
int main(int argc, char** argv) 
{
    /* Register tests as required, then call the UT-main to support switches and triggering */
    UT_init( argc, argv );

    /* add a suite to the registry */
    gpLogSuite = UT_add_suite("ut-core-log-tests", ut_init_function, ut_clean_function);
    if (NULL == gpLogSuite)
    {
        return -1;
    }

    UT_add_test( gpLogSuite, "UT_LOG Tests", test_ut_logging);

    gpAssertSuite = UT_add_suite("ut-core-assert-tests", ut_init_function, ut_clean_function);
    if (NULL == gpAssertSuite)
    {
        return -1;
    }

    UT_add_test( gpAssertSuite, "UT_ASSERT Tests", test_ut_assert);
    UT_add_test( gpAssertSuite, "UT_ASSERT Msg", test_ut_assert_msg);
    UT_add_test( gpAssertSuite, "UT_ASSERT Log", test_ut_assert_log);
    UT_add_test( gpAssertSuite, "UT_ASSERT Pass/Fail", test_ut_assert_pass_fail);
    UT_add_test( gpAssertSuite, "UT_ASSERT FATAL", test_ut_assert_fatal);
    UT_add_test( gpAssertSuite, "UT_ASSERT ptr_NULL", test_ut_assert_ptr_NULL);
    UT_add_test( gpAssertSuite, "UT_ASSERT ptr_NOT_NULL", test_ut_assert_ptr_NOT_NULL);
    UT_add_test( gpAssertSuite, "UT_ASSERT TRUE", test_ut_assert_TRUE);
    UT_add_test( gpAssertSuite, "UT_ASSERT FALSE", test_ut_assert_FALSE);
    UT_add_test( gpAssertSuite, "UT_ASSERT EQUAL", test_ut_assert_EQUAL);
    UT_add_test( gpAssertSuite, "UT_ASSERT STRING_EQUAL", test_ut_assert_STRING_EQUAL);
    UT_add_test( gpAssertSuite, "UT_ASSERT STRING_NOT_EQUAL", test_ut_assert_STRING_NOT_EQUAL);

    /* Begin test executions */
    UT_run_tests();

    return 0;
}
