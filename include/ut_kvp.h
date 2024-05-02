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
#ifndef __UT_KVP_H__
#define __UT_KVP_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/**
 * @brief Status return codes for this module
 * 
 */
typedef enum
{
    UT_KVP_STATUS_SUCCESS=0,        /**!< Status ok */
    UT_KVP_STATUS_FILE_OPEN_ERROR,  /**!< File open error */
    UT_KVP_STATUS_INVALID_PARAM,    /**!< Invalid Param */
    UT_KVP_STATUS_PARSING_ERROR,    /**!< Parsing error */
    UT_KVP_STATUS_NO_DATA,          /**!< No Data to process */
    UT_KVP_STATUS_MAX               /**!< Out of range marker */
}
ut_kvp_status_t;

typedef void ut_kvp_instance_t;    /**!< Instance type*/

#define UT_KVP_MAX_ELEMENT_SIZE (256)    /*!< Max Element Size Supported */

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
 * @brief open and read kvp file and ready for use
 * 
 * @param pInstance[in] - Handle to the instance
 * @param fileName[in] - Zero Terminated filename
 * 
 * @returns ut_kvp_status_t - result status
 * @retval UT_KVP_STATUS_SUCCESS - Success
 * @retval UT_KVP_STATUS_FILE_OPEN_ERROR - File open error
 * @retval UT_KVP_STATUS_INVALID_PARAM   - Invalid param passed
 * @retval UT_KVP_STATUS_PARSING_ERROR   - File parsing error
 */
ut_kvp_status_t ut_kvp_open(ut_kvp_instance_t *pInstance, char* fileName);

/**
 * @brief close kvp file previously read and free the memory
 *
 * @param pInstance[in] - Handle to the instance
 */
void ut_kvp_close(ut_kvp_instance_t *pInstance);

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

/**
 * @brief Get a uint8_t field from a key value pair
 *  This function may assert if a field is not convertible type uint8_t
 * 
 * @param pInstance[in] - Handle to the instance
 * @param pszKey[in] - Zero Terminated String Key
 *
 * @returns uint8_t - int result on success, 0 on error
 */
uint8_t ut_kvp_getUInt8Field( ut_kvp_instance_t *pInstance, const char *pszKey );

/**
 * @brief Get a uint16_t field from a key value pair
 *  This function may assert if a field is not convertible type uint16_t
 * 
 * @param pInstance[in] - Handle to the instance
 * @param pszKey[in] - Zero Terminated String Key
 *
 * @returns uint16_t - int result on success, 0 on error
 */
uint16_t ut_kvp_getUInt16Field( ut_kvp_instance_t *pInstance, const char *pszKey );

/**
 * @brief Get a uint32_t field from a key value pair
 *  This function may assert if a field is not convertible type uint32_t
 * 
 * @param pInstance[in] - Handle to the instance
 * @param pszKey[in] - Zero Terminated String Key
 *
 * @returns uint32_t - int result on success, 0 on error
 */
uint32_t ut_kvp_getUInt32Field(ut_kvp_instance_t *pInstance, const char *pszKey);

/**
 * @brief Get a uint64_t field from a key value pair
 *  This function may assert if a field is not convertible type uint32_t
 * 
 * @param pInstance[in] - Handle to the instance
 * @param pszKey[in] - Zero Terminated String Key
 *
 * @returns uint64_t - int result on success, 0 on error
 */
uint64_t ut_kvp_getUInt64Field(ut_kvp_instance_t *pInstance, const char *pszKey);

/**
 * @brief Get a key value pair from the passed string configuration
 *
 * @param pInstance[in] - Handle to the instance
 * @param pszKey[in] - Zero Terminated String Key
 * @param psValue[out] - Caller must pass const char pointer which will be populated with the result
 *
 * @return const char*  - Returned string value
 */
const char* ut_kvp_getStringField( ut_kvp_instance_t *pInstance, const char *pszKey, const char *psValue );

/* TODO:
* ut_kvp_getInt8Field
* ut_kvp_getInt16Field
* ut_kvp_getInt32Field
* ut_kvp_getInt64Field
*/

#endif /* __UT_KVP_H__ */