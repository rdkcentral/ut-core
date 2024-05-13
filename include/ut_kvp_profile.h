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

#ifndef __UT_KVP_PROFILE_H__
#define __UT_KVP_PROFILE_H__

#include <ut.h>
#include <ut_kvp.h>
#include <stdbool.h>
#include <assert.h>

/**!
 * @brief Opens a profile configuration file for processing.
 *
 * @param[in] fileName - Path to the configuration file.
 * @returns Status of the operation:
 * @retval UT_KVP_STATUS_SUCCESS - Success.
 * @retval UT_KVP_STATUS_FILE_OPEN_ERROR - Failed to open the file.
 * @retval UT_KVP_STATUS_INVALID_PARAM - Invalid filename provided.
 * @retval UT_KVP_STATUS_PARSING_ERROR - Error parsing the file.
 */
extern ut_kvp_status_t ut_kvp_profile_open(char *fileName);

/**
 * @brief Closes the currently open profile configuration file.
 * 
 */
extern void ut_kvp_profile_close(void);

/**
 * @brief Retrieves the current KVP instance for the active configuration profile
*/
extern ut_kvp_instance_t *ut_kvp_profile_getInstance(void);

/**! Asserts that a boolean KVP field matches the expected value. */
#define UT_ASSERT_EQUAL_KVP_PROFILE_BOOL(checkValue, key) UT_ASSERT_EQUAL(ut_kvp_getBoolField(ut_kvp_profile_getInstance(), key), checkValue); 

/**! Asserts that a UINT32 KVP field matches the expected value. */
#define UT_ASSERT_KVP_EQUAL_PROFILE_UINT32(checkValue, key) UT_ASSERT_EQUAL(ut_kvp_getUInt32Field(ut_kvp_profile_getInstance(), key), checkValue); 

/**! Asserts that a UINT64 KVP field matches the expected value. */
#define UT_ASSERT_EQUAL_KVP_PROFILE_UINT64(checkValue, key) UT_ASSERT_EQUAL(ut_kvp_getUInt64Field(ut_kvp_profile_getInstance(), key), checkValue); 

/**! Asserts that a string KVP field matches the expected value. */
#define UT_ASSERT_EQUAL_KVP_PROFILE_STRING(checkValue, key) \
    {                                                                       \
        const char* result_kvp = NULL;                                      \
        result_kvp = ut_kvp_getStringField(ut_kvp_profile_getInstance(), key, result_kvp);  \
        UT_ASSERT(result_kvp != NULL);                                        \
        if (result_kvp != NULL) {                                           \
            UT_ASSERT_STRING_EQUAL(checkValue, result_kvp);                       \
        }                                                                   \
    } 


#endif /* __UT_KVP_PROFILE_H__ */
