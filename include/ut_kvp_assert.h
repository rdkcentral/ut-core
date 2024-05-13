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

//Usage for KVP Asserting
#ifndef __UT_KVP_ASSERT_H__
#define __UT_KVP_ASSERT_H__

#include <ut.h>
#include <ut_kvp.h>
#include <stdbool.h>
#include <assert.h>

/**
 * @brief open the assert configuration file for processing
 * 
 * @param fileName[in] - filename to open
 * @returns ut_kvp_status_t - status
 * @retval UT_KVP_STATUS_SUCCESS - Success
 * @retval UT_KVP_STATUS_FILE_OPEN_ERROR - File open error
 * @retval UT_KVP_STATUS_INVALID_PARAM   - Invalid param passed
 * @retval UT_KVP_STATUS_PARSING_ERROR   - File parsing error
 */
extern ut_kvp_status_t ut_kvp_assert_open(char *fileName);

/**
 * @brief close the assert configuration
 * 
 */
extern void ut_kvp_assert_close(void);

/**
 * @brief get the current assert kvp instance
*/
extern ut_kvp_instance_t *ut_kvp_assert_getInstance(void);

/* ASSERT Macros */
#define UT_ASSERT_EQUAL_KVP_BOOL( checkValue, key ) UT_ASSERT_EQUAL( ut_kvp_getBoolField(ut_kvp_assert_getInstance(), key), checkValue );
#define UT_ASSERT_EQUAL_KVP_UINT8( checkValue, key ) UT_ASSERT_EQUAL( ut_kvp_getUInt8Field( ut_kvp_assert_getInstance(), key ), checkValue );
#define UT_ASSERT_EQUAL_KVP_UINT16( checkValue, key ) UT_ASSERT_EQUAL( ut_kvp_getUInt16Field( ut_kvp_assert_getInstance(), key ), checkValue );
#define UT_ASSERT_EQUAL_KVP_UINT32( checkValue, key ) UT_ASSERT_EQUAL( ut_kvp_getUInt32Field( ut_kvp_assert_getInstance(), key ), checkValue );
#define UT_ASSERT_EQUAL_KVP_UINT64( checkValue, key ) UT_ASSERT_EQUAL( ut_kvp_getUInt64Field( ut_kvp_assert_getInstance(), key ), checkValue );
#define UT_ASSERT_EQUAL_KVP_STRING(checkValue, key)                         \
    {                                                                       \
        const char* result_kvp = NULL;                                      \
        result_kvp = ut_kvp_getStringField(ut_kvp_assert_getInstance(), "decodeTest/checkStringDeadBeef", result_kvp);  \
        UT_ASSERT( result_kvp != NULL );    \
        if(result_kvp != NULL)    \
        {    \
            UT_ASSERT_STRING_EQUAL(checkField, result_kvp);    \
        }                         \
    }

#endif /* __UT_KVP_ASSERT_H__ */
