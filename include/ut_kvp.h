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

// ut_kvp.h, header to be expanded as required

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>


typedef enum
{
    UT_KVP_STATUS_OK=0,
    UT_KVP_FILE_OPEN_ERROR,
    UT_KVP_VALID_PARAM,
    UT_KVP_FILE_READ_ERROR
}
ut_kvp_status_t;

#define UT_KVP_MAX_STRING_ELEMENT_SIZE (256)
#define UT_KVP_MAX_ARRAY_SIZE (32)

typedef void *ut_kvp_instance_t;

extern ut_kvp_instance_t gKVP_Instance;

/**
 * @brief create instance handle to the kvp object
 */
ut_kvp_instance_t *ut_kvp_createInstance( void );

/**
 * @brief destory instance handle to the kvp object
 */
ut_kvp_instance_t ut_kvp_destoryInstance( ut_kvp_instance_t *pInstance );

/**
 * @brief get instance handle to the kvp object
 */
//ut_kvp_instance_t get_ut_kvp_instance();

/**
 * @brief read kvp file and ready for use
 */
ut_kvp_status_t ut_kvp_read(ut_kvp_instance_t *pInstance, char* fileName);

/**
 * @brief Get a key value pair from the passed configuration
 *
 * @param param[in] - Handle to the instance
 * @param key[in] - Zero Terminated String Key
 *
 * @return ut_kvp_value_t  - Returned structure
 * @retval type == UT_KVP_INVALID_ELEMENT if the element is not found
 */
const char *ut_kvp_getField( ut_kvp_instance_t *pInstance, const char *pszKey );
