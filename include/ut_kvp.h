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

/**
 * @brief Status return codes for this module
 * 
 */
typedef enum
{
    UT_KVP_STATUS_OK=0,         /**!< Status ok */
    UT_KVP_FILE_OPEN_ERROR,     /**!< File open error */
    UT_KVP_INVALID_PARAM,       /**!< Invalid Param */
    UT_KVP_FILE_READ_ERROR      /**!< Read Error */
}
ut_kvp_status_t;

typedef void ut_kvp_instance_t;    /**!< Instance type*/

/**
 * @brief create instance handle to the kvp object
 * 
 * @returns ut_kvp_instance_t - Handle to the instance
 */
ut_kvp_instance_t *ut_kvp_createInstance( void );

/**
 * @brief destroy instance handle to the kvp object
 * 
 * @param pInstance[in] - Handle to the instance
 */
void ut_kvp_destroyInstance( ut_kvp_instance_t *pInstance );

/**
 * @brief read kvp file and ready for use
 * 
 * @param pInstance[in] - Handle to the instance
 * @param fileName[in] - Zero Terminated filename
 * @returns ut_kvp_status_t
 * @retval UT_KVP_STATUS_OK - Success
 * @retval UT_KVP_FILE_OPEN_ERROR - File open error
 * @retval UT_KVP_INVALID_PARAM - Invalid param passed
 * @retval UT_KVP_FILE_READ_ERROR - File read error
 */
ut_kvp_status_t ut_kvp_read(ut_kvp_instance_t *pInstance, char* fileName);

/**
 * @brief close kvp file previously read and free the memory
 *
 * @param pInstance[in] - Handle to the instance
 */
void ut_kvp_close(ut_kvp_instance_t *pInstance);

/**
 * @brief Get a key value pair from the passed configuration
 *
 * @param pInstance[in] - Handle to the instance
 * @param pszKey[in] - Zero Terminated String Key
 *
 * @return ut_kvp_value_t  - Returned structure
 * @retval type == UT_KVP_INVALID_ELEMENT if the element is not found
 */
//const char *ut_kvp_getField(ut_kvp_instance_t *pInstance, const char *pszKey);
const char *ut_kvp_getField(ut_kvp_instance_t *pInstance, const char *pszKey, char *result);

/**
 * @brief Get a bool key value pair from a passed configuration
 * 
 * @param pInstance[in] - Handle to the instance
 * @param pszKey[in] - Zero Terminated String Key
 *
 * @return true 
 * @return false 
 */
bool ut_kvp_getBoolField(ut_kvp_instance_t *pInstance, const char *pszKey);

const char *ut_kvp_getStringField(ut_kvp_instance_t *pInstance, const char *pszKey);

/* TODO:
* ut_kvp_getUInt8Field
* ut_kvp_getUInt16Field
* ut_kvp_getInt8Field
* ut_kvp_getInt16Field
* ut_kvp_getInt32Field
* ut_kvp_getInt64Field
*/

/**
 * @brief Get a uint32_t field from a key value pair
 *  This function may assert if a field is not convertible type uint32_t
 * 
 * @param pInstance[in] - Handle to the instance
 * @param pszKey[in] - Zero Terminated String Key
 *
 * @returns uint32_t - int result
 */
uint32_t ut_kvp_getUInt32Field(ut_kvp_instance_t *pInstance, const char *pszKey);

/**
 * @brief Get a uint64_t field from a key value pair
 *  This function may assert if a field is not convertible type uint32_t
 * 
 * @param pInstance[in] - Handle to the instance
 * @param pszKey[in] - Zero Terminated String Key
 *
 * @returns uint64_t - int result
 */
uint64_t ut_kvp_getUInt64Field(ut_kvp_instance_t *pInstance, const char *pszKey);

/* TOOD: We will need int32 & int64 field functions also */