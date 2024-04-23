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

#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

#include <ut.h>
#include <ut_log.h>

static UT_test_suite_t * gpLogSuite = NULL;
static UT_test_suite_t * gpAssertSuite = NULL;

int ut_init_function( void )
{
    //UT_LOG( UT_LOG_ASCII_YELLOW"--- Start of Suite ---"UT_LOG_ASCII_NC );
    return 0;
}

int ut_clean_function( void )
{
    //UT_LOG( UT_LOG_ASCII_YELLOW"--- End of Suite ---\n"UT_LOG_ASCII_NC );
    return 0;
}

void test_ut_logging( void )
{
    const char string[]="I'm a string";

    UT_LOG( "This message should print out with an [I'm a string, in the next box] -> [%s]\n", string );
    UT_LOG_STEP("This is a step message ");
    UT_LOG_INFO("This is an info message");
    UT_LOG_DEBUG("This is a debug message");
    UT_LOG_WARNING("This is a warning message");
    UT_LOG_ERROR("This is an error message");
}

void test_ut_logging_too_long_string( void )
{
    const char string[]="The widespread usage of computer-based assessments and individualized learning platforms has resulted "\
                        "in an increased demand for the rapid production of high-quality items. Automated item generation (AIG),"\
                        "the process of using item models to generate new items with the help of computer technology, was proposed"\
                        " to reduce reliance on human subject experts at each step of the process. AIG has been used in test development"\
                        " for some time. Still, the use of machine learning algorithms has introduced the potential to improve the efficiency"\
                        " and effectiveness of the process greatly. The approach presented in this paper utilizes OpenAI's latest transformer-based"\
                        " language model, GPT-3, to generate reading passages. Existing reading passages were used in carefully engineered prompts to"\
                        " ensure the AI-generated text has similar content and structure to a fourth-grade reading passage. For each prompt, we generated"\
                        " multiple passages, the final passage was selected according to the Lexile score agreement with the original passage. In the final"\
                        " round, the selected passage went through a simple revision by a human editor to ensure the text was free of any grammatical and factual"\
                        " errors. All AI-generated passages, along with original passages were evaluated by human judges according to their coher, "\
                        "appropriateness to fourth graders, and readability.";

    UT_LOG_STEP( "This passage must be truncated to [%d] from it's current size of [%d]", UT_LOG_MAX_LINE_SIZE, strlen(string) );
    UT_LOG( "%s", string );

    UT_LOG_STEP( "This test should also have the same affect" );

    UT_LOG( string );
}

void test_ut_logging_colour_test( void )
{
    UT_LOG_STEP( "This is black:   "UT_LOG_ASCII_BLACK"[black]"UT_LOG_ASCII_NC );
    UT_LOG_STEP( "This is red:     "UT_LOG_ASCII_RED"[red]"UT_LOG_ASCII_NC );
    UT_LOG_STEP( "This is green:   "UT_LOG_ASCII_GREEN"[green]"UT_LOG_ASCII_NC );
    UT_LOG_STEP( "This is yellow:  "UT_LOG_ASCII_YELLOW"[yellow]"UT_LOG_ASCII_NC );
    UT_LOG_STEP( "This is blue:    "UT_LOG_ASCII_BLUE"[blue]"UT_LOG_ASCII_NC );
    UT_LOG_STEP( "This is magenta: "UT_LOG_ASCII_MAGENTA"[magenta]"UT_LOG_ASCII_NC );
    UT_LOG_STEP( "This is cyan:    "UT_LOG_ASCII_CYAN"[cyan]"UT_LOG_ASCII_NC );
    UT_LOG_STEP( "This is white:   "UT_LOG_ASCII_WHITE"[white]"UT_LOG_ASCII_NC );
}

void test_ut_assert( void )
{
    UT_LOG_STEP( "1: is expected to pass" );
    UT_ASSERT( true==true );

    UT_LOG_STEP( "2: is expected to fail but not fatal:" );
    UT_ASSERT( true==false );   /* This line should assert */

    UT_LOG_STEP( "3: is expected to assert & FATAL:" );
    UT_ASSERT_FATAL( true==false );   /* This line should assert */

    UT_LOG_ERROR("### This line should never be seen\n");
}

void test_ut_assert_pass( void )
{
    UT_LOG_STEP( "1: Test UT_PASS");
    UT_PASS( "This should be a PASS Message" );
}

void test_ut_assert_fail( void )
{
    UT_LOG_STEP( "1: UT_FAIL");
    UT_FAIL( "UT FAIL MESSAGE - Should Fail but test will continue" ); /* This line should assert */

    UT_LOG_INFO("+++ This line should be seen\n");

    UT_LOG_STEP( "2: UT_FAIL_FATAL");
    UT_FAIL_FATAL("Test UT_FAIL_FATAL"); /* This line should assert */

    UT_LOG_ERROR("### This line should never be seen\n");
}

void test_ut_assert_ptr_NULL()
{
    int *pTR = (int *)1;
    
    UT_LOG_STEP( "1: UT_ASSERT_PTR_NULL (pTR == NULL ) : no assertest_ut_logging_too_long_stringt");
    UT_ASSERT_PTR_NULL( NULL );

    UT_LOG_STEP( "2: UT_ASSERT_PTR_NULL (pTR == 1 ) - This step should assert");
    UT_ASSERT_PTR_NULL( pTR );   /* Should Fail, but not fatal */

    UT_LOG_STEP( "3: UT_ASSERT_PTR_NULL_FATAL (pTR == 1 ) - This step should assert & FATAL");
    UT_ASSERT_PTR_NULL_FATAL( pTR );   /* Should Fail, but not fatal */

    UT_LOG_ERROR("### This line should never be seen\n");
}

void test_ut_assert_ptr_equal()
{
    int *pTR1 = (int *)1;
    int *pTR2 = (int *)1;
    int *pTR3 = (int *)2;

    UT_LOG_STEP( "1: UT_ASSERT_PTR_EQUAL (pTR1 == pTR2 ) : no assert");
    UT_ASSERT_PTR_EQUAL( pTR1, pTR2 );

    UT_LOG_STEP( "2: UT_ASSERT_PTR_EQUAL (pTR != pTR3 ) - this step should assert");
    UT_ASSERT_PTR_EQUAL( pTR1, pTR3 );   /* Should Fail */

    UT_LOG_STEP( "3: UT_ASSERT_PTR_EQUAL (pTR != pTR3 ) - this step should assert & FATAL");
    UT_ASSERT_PTR_EQUAL_FATAL( pTR1, pTR3 );   /* Should Fail */

    UT_LOG_ERROR("### This line should never be seen\n");
}

void test_ut_assert_ptr_NOT_equal()
{
    int *pTR1 = (int *)1;
    int *pTR2 = (int *)2;
    int *pTR3 = (int *)1;

    UT_LOG_STEP( "1: UT_ASSERT_PTR_NOT_EQUAL (pTR1 != pTR2 ) : no assert");
    UT_ASSERT_PTR_NOT_EQUAL( pTR1, pTR2 );

    UT_LOG_STEP( "2: UT_ASSERT_PTR_NOT_EQUAL (pTR1 == pTR3 ) - This step should assert");
    UT_ASSERT_PTR_NOT_EQUAL( pTR1, pTR3 );   /* Should Fail */

    UT_LOG_STEP( "2: UT_ASSERT_PTR_NOT_EQUAL (pTR1 == pTR3 ) - This step should assert & FATAL");
    UT_ASSERT_PTR_NOT_EQUAL_FATAL( pTR1, pTR3 );   /* Should Fail */

    UT_LOG_ERROR("### This line should never be seen\n");
}

void test_ut_assert_ptr_NOT_NULL()
{
    int *pTR = (int*) 1;

    UT_LOG_STEP( "1: UT_ASSERT_PTR_NOT_NULL (pTR = 1 ) : no assert");
    UT_ASSERT_PTR_NOT_NULL( pTR );

    UT_LOG_STEP( "2: UT_ASSERT_PTR_NOT_NULL (pTR == NULL ) : this step should assert");
    UT_ASSERT_PTR_NOT_NULL( NULL );   /* Should Fail */

    UT_LOG_STEP( "3: UT_ASSERT_PTR_NOT_NULL (pTR == NULL ) : this step should assert & FATAL");
    UT_ASSERT_PTR_NOT_NULL_FATAL( NULL );   /* Should Fail */

    UT_LOG_ERROR("### This line should never be seen\n");
}

void test_ut_assert_TRUE()
{
    UT_LOG_STEP( "1: UT_ASSERT_TRUE ( true==true ) : No Assert ");
    UT_ASSERT_TRUE( true==true );

    UT_LOG_STEP( "2: UT_ASSERT_TRUE ( true!=true ) : this step should assert"); 
    UT_ASSERT_TRUE( true!=true ); /* Should Fail */

    UT_LOG_STEP( "3: UT_ASSERT_TRUE ( true!=true ) : this step should assert & FATAL"); 
    UT_ASSERT_TRUE_FATAL( true!=true ); /* Should Fail */

    UT_LOG_ERROR("### This line should never be seen\n");
}

void test_ut_assert_FALSE()
{
    UT_LOG_STEP( "1: UT_ASSERT_FALSE ( true==false ) : No Assert ");
    UT_ASSERT_FALSE( true==false );

    UT_LOG_STEP( "2: UT_ASSERT_FALSE ( true!=false ) : this step should assert"); 
    UT_ASSERT_FALSE( true!=false ); /* Should fail */

    UT_LOG_STEP( "2: UT_ASSERT_FALSE ( true!=false ) : this step should assert & FATAL"); 
    UT_ASSERT_FALSE_FATAL( true!=false ); /* Should fail */

    UT_LOG_ERROR("### This line should never be seen\n");
}

void test_ut_assert_EQUAL( void )
{
    UT_LOG_STEP( "1: UT_ASSERT_EQUAL ( true, true ) : PASS"); 
    UT_ASSERT_EQUAL( true, true );

    UT_LOG_STEP( "2: UT_ASSERT_EQUAL ( true, false ) : : this step should assert " );
    UT_ASSERT_EQUAL( true, false );  /* Should FAIL */

    UT_LOG_STEP( "3: UT_ASSERT_EQUAL ( true, false ) : : this step should assert & FATAL" );
    UT_ASSERT_EQUAL_FATAL( true, false );  /* Should FAIL */

    UT_LOG_ERROR("### This line should never be seen\n");
}

void test_ut_assert_STRING_EQUAL( void )
{
    UT_LOG_STEP( "1: UT_ASSERT_STRING_EQUAL ( bob, bob ) : No Assert ");
    UT_ASSERT_STRING_EQUAL( "bob", "bob" );

    UT_LOG_STEP( "2: UT_ASSERT_STRING_EQUAL ( bob, fred ) : : this step should assert"); 
    UT_ASSERT_STRING_EQUAL( "bob", "fred" ); /* Should FAil */

    UT_LOG_STEP( "3: UT_ASSERT_STRING_EQUAL ( bob, fred ) : : this step should assert & FATAL"); 
    UT_ASSERT_STRING_EQUAL_FATAL( "bob", "fred" ); /* Should FAil */

    UT_LOG_ERROR("### This line should never be seen\n");
}

void test_ut_assert_STRING_NOT_EQUAL( void )
{
    UT_LOG_STEP( "1: UT_ASSERT_STRING_NOT_EQUAL ( bob, fred ) : no assert");
    UT_ASSERT_STRING_NOT_EQUAL( "bob", "fred" );

    UT_LOG_STEP( "2: UT_ASSERT_STRING_NOT_EQUAL ( bob, bob ) : this step should assert"); 
    UT_ASSERT_STRING_NOT_EQUAL( "bob", "bob" ); /* Should FAIL */

    UT_LOG_STEP( "3: UT_ASSERT_STRING_NOT_EQUAL ( bob, bob ) : this step should assert & FATAL"); 
    UT_ASSERT_STRING_NOT_EQUAL_FATAL( "bob", "bob" ); /* Should FAIL */

    UT_LOG_ERROR("### This line should never be seen\n");
}

void test_ut_assert_msg( void )
{
    UT_LOG_STEP( "1: is expected not to assert" );
    UT_ASSERT_MSG( true==true, "should not assert");

    UT_LOG( "This test should cause 2 assert checks, so a test pass, it will quit the test early" );

    UT_LOG_STEP( "2: is expected to fail" );
    UT_ASSERT_MSG( false==true, "Step 2 : Asserted Correctly if you see this message :");   /* This line should assert */

    UT_LOG_STEP( "3: is expected to fail" );
    UT_ASSERT_MSG_FATAL( false==true, "Step 2 : Asserted Correctly if you see this message :");   /* This line should assert & FATAL*/

    UT_LOG_ERROR("### This line should never be seen\n");
}

void test_ut_assert_msg_true( void )
{
    UT_LOG_STEP( "1: is expected not to assert" );
    UT_ASSERT_TRUE_MSG( true==true, "should not assert");

    UT_LOG( "This test should cause 2 assert checks, so a test pass, it will quit the test early" );

    UT_LOG_STEP( "2: is expected to fail" );
    UT_ASSERT_TRUE_MSG( false==true, "Step 2 : Asserted Correctly if you see this message :");   /* This line should assert */

    UT_LOG_STEP( "3: is expected to fail" );
    UT_ASSERT_TRUE_MSG_FATAL( false==true, "Step 3 : Asserted Correctly if you see this message :");   /* This line should assert & FATAL*/

    UT_LOG_ERROR("### This line SHOULD never be seen\n");
}

void test_ut_assert_msg_false( void )
{
    UT_LOG_STEP( "1: is expected not to assert" );
    UT_ASSERT_FALSE_MSG( true==false, "should not assert");

    UT_LOG( "This test should cause 2 assert checks, so a test pass, it will quit the test early" );

    UT_LOG_STEP( "2: is expected to fail" );
    UT_ASSERT_FALSE_MSG( true==true, "Step 2 : Asserted Correctly if you see this message :");   /* This line should assert */

    UT_LOG_STEP( "3: is expected to fail" );
    UT_ASSERT_FALSE_MSG_FATAL( true==true, "Step 3 : Asserted Correctly if you see this message :");   /* This line should assert & FATAL */

    UT_LOG_ERROR("### This line SHOULD never be seen\n");
}

void test_ut_assert_log( void )
{
    UT_ASSERT_LOG( true==true, "Step 1: ASSERT_LOG : should log and NOT fail :");
    UT_ASSERT_LOG( true==false, "Step 2: ASSERT_LOG : should log and NOT fail :");   /* This line should assert */

    UT_LOG_INFO("+++ This line SHOULD be seen\n");
}

/**
 * @brief Main launch function for assert functions
 */
void register_assert_functions(void) 
{
    /* add a suite to the registry */
    gpLogSuite = UT_add_suite("ut-core-log-tests", ut_init_function, ut_clean_function);
    assert(gpLogSuite != NULL);

    UT_add_test( gpLogSuite, "UT_LOG Tests", test_ut_logging);
    UT_add_test( gpLogSuite, "UT_LOG Tests - Too Long String", test_ut_logging_too_long_string);
    UT_add_test( gpLogSuite, "UT_LOG Tests - Colour Test", test_ut_logging_colour_test);

    gpAssertSuite = UT_add_suite("ut-core-assert-tests", ut_init_function, ut_clean_function);
    assert(gpAssertSuite != NULL);

    UT_add_test( gpAssertSuite, "UT_PASS Pass", test_ut_assert_pass);
    UT_add_test( gpAssertSuite, "UT_FAIL Fail", test_ut_assert_fail);
    UT_add_test( gpAssertSuite, "UT_ASSERT Tests", test_ut_assert);
    UT_add_test( gpAssertSuite, "UT_ASSERT ptr_equal", test_ut_assert_ptr_equal);
    UT_add_test( gpAssertSuite, "UT_ASSERT ptr_NOT_equal", test_ut_assert_ptr_NOT_equal);
    UT_add_test( gpAssertSuite, "UT_ASSERT ptr_NULL", test_ut_assert_ptr_NULL);
    UT_add_test( gpAssertSuite, "UT_ASSERT ptr_NOT_NULL", test_ut_assert_ptr_NOT_NULL);
    UT_add_test( gpAssertSuite, "UT_ASSERT TRUE", test_ut_assert_TRUE);
    UT_add_test( gpAssertSuite, "UT_ASSERT FALSE", test_ut_assert_FALSE);
    UT_add_test( gpAssertSuite, "UT_ASSERT EQUAL", test_ut_assert_EQUAL);
    UT_add_test( gpAssertSuite, "UT_ASSERT STRING_EQUAL", test_ut_assert_STRING_EQUAL);
    UT_add_test( gpAssertSuite, "UT_ASSERT STRING_NOT_EQUAL", test_ut_assert_STRING_NOT_EQUAL);
    UT_add_test( gpAssertSuite, "UT_ASSERT Msg", test_ut_assert_msg);
    UT_add_test( gpAssertSuite, "UT_ASSERT_TRUE_MSG", test_ut_assert_msg_true);
    UT_add_test( gpAssertSuite, "UT_ASSERT_FALSE_MSG", test_ut_assert_msg_false);
    UT_add_test( gpAssertSuite, "UT_ASSERT Log", test_ut_assert_log);
}

