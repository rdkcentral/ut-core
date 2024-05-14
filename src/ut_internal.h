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

/** @brief UT Internal shared header
 * 
 */

/** @addtogroup UT
 * @{
 */

#ifndef __UT_INTERNAL_H
#define __UT_INTERNAL_H

#include <string.h>

#define UT_MAX_FILENAME_STRING_SIZE (128)

/**
 * @brief Testing Modes
 * 
 */
typedef enum
{
    UT_MODE_BASIC=0,    /*!< Basic Mode, run all tests and output results */
    UT_MODE_AUTOMATED,  /*!< Automated only mode, no output but generate xUnit xml output*/
    UT_MODE_CONSOLE     /*!< Requires console, input from terminal */
}TestMode_t;

typedef struct
{
    /* option switches */
    TestMode_t  testMode;
    bool        listTest;
    char        filenameRoot[UT_MAX_FILENAME_STRING_SIZE];
    char        filename[UT_MAX_FILENAME_STRING_SIZE];
    bool        help;
}optionFlags_t;

extern optionFlags_t gOptions;  /*!< Control flags */

#define TEST_INFO(x) printf x;

#endif  /*  __UT_INTERNAL_H  */
/** @} */
