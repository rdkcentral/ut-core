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

/**! Status codes for the unit testing (UT) Key-Value Pair (KVP) framework. */
typedef enum
{
    UT_KVP_STATUS_SUCCESS = 0,       /**!< Operation successful. */
    UT_KVP_STATUS_FILE_OPEN_ERROR,   /**!< Failed to open the file. */
    UT_KVP_STATUS_INVALID_PARAM,     /**!< Invalid parameter passed. */
    UT_KVP_STATUS_PARSING_ERROR,     /**!< Error parsing the file. */
    UT_KVP_STATUS_KEY_NOT_FOUND,     /**!< Key not found in the KVP instance. */
    UT_KVP_STATUS_NO_DATA,           /**!< No data to process. */
    UT_KVP_STATUS_NULL_PARAM,        /**!< Null parameter passed. */
    UT_KVP_STATUS_INVALID_INSTANCE,  /**!< Invalid KVP instance handle. */
    UT_KVP_STATUS_MAX                /**!< Out of range marker (not a valid status). */
} ut_kvp_status_t;

/**! Handle to a KVP instance. */
typedef void ut_kvp_instance_t;    

#define UT_KVP_MAX_ELEMENT_SIZE (256)  /**!< Maximum size of a single KVP element (in bytes). */

/**!
 * @brief Creates a new KVP instance.
 * @returns Handle to the created KVP instance, or NULL on failure. 
 */ 
ut_kvp_instance_t *ut_kvp_createInstance(void);

/**!
 * @brief Destroys a KVP instance.
 * 
 * Releases resources associated with the KVP instance.
 * 
 * @param[in] pInstance - Handle to the instance to destroy.
 */
void ut_kvp_destroyInstance(ut_kvp_instance_t *pInstance);

/**!
 * @brief Opens and parses a Key-Value Pair (KVP) file into a KVP instance.
 *
 * This function opens the specified KVP file, reads its contents, and parses the key-value pairs into the given KVP instance.
 *
 * @param[in] pInstance - Handle to the KVP instance where the parsed data will be stored.
 * @param[in] fileName - Null-terminated string containing the path to the KVP file.
 *
 * @returns Status of the operation (`ut_kvp_status_t`):
 * @retval UT_KVP_STATUS_SUCCESS - The file was opened and parsed successfully.
 * @retval UT_KVP_STATUS_FILE_OPEN_ERROR - The file could not be opened.
 * @retval UT_KVP_STATUS_INVALID_PARAM - One or more parameters are invalid (e.g., null pointer).
 * @retval UT_KVP_STATUS_PARSING_ERROR - An error occurred while parsing the file contents.
 * @retval UT_KVP_STATUS_INVALID_INSTANCE - The provided `pInstance` is not a valid KVP instance.
 */
ut_kvp_status_t ut_kvp_open(ut_kvp_instance_t *pInstance, char* fileName);

/**!
 * @brief Closes a previously opened KVP profile and frees its memory.
 * 
 * @param[in] pInstance - Handle to the instance to close.
 */
void ut_kvp_close(ut_kvp_instance_t *pInstance);

/**!
 * @brief Gets a boolean value from the KVP profile.
 * 
 * @param[in] pInstance - Handle to the KVP instance.
 * @param[in] pszKey - Null-terminated string representing the key to search for.
 * 
 * @returns `true` if the key is found and its value is "true", `false` otherwise.
 * 
 * @note This function will return false if the key is not found or if the value is not a valid boolean.
 */
bool ut_kvp_getBoolField(ut_kvp_instance_t *pInstance, const char *pszKey);

/**!
 * @brief Gets a uint8_t value from the KVP profile.
 * 
 * @param[in] pInstance - Handle to the KVP instance.
 * @param[in] pszKey - Null-terminated string representing the key to search for.
 * 
 * @returns The `uint8_t` value on success, or 0 on error (check logs for details).
 */
uint8_t ut_kvp_getUInt8Field(ut_kvp_instance_t *pInstance, const char *pszKey);

/**!
 * @brief Gets a uint16_t value from the KVP profile.
 * 
 * @param[in] pInstance - Handle to the KVP instance.
 * @param[in] pszKey - Null-terminated string representing the key to search for.
 * 
 * @returns The `uint16_t` value on success, or 0 on error (check logs for details).
 */
uint16_t ut_kvp_getUInt16Field(ut_kvp_instance_t *pInstance, const char *pszKey);

/**!
 * @brief Gets a uint32_t value from the KVP profile.
 * 
 * @param[in] pInstance - Handle to the KVP instance.
 * @param[in] pszKey - Null-terminated string representing the key to search for.
 * 
 * @returns The `uint32_t` value on success, or 0 on error (check logs for details).
 */
uint32_t ut_kvp_getUInt32Field(ut_kvp_instance_t *pInstance, const char *pszKey);

/**!
 * @brief Gets a uint64_t value from the KVP profile.
 * 
 * @param[in] pInstance - Handle to the KVP instance.
 * @param[in] pszKey - Null-terminated string representing the key to search for.
 * 
 * @returns The `uint64_t` value on success, or 0 on error (check logs for details).
 */
uint64_t ut_kvp_getUInt64Field(ut_kvp_instance_t *pInstance, const char *pszKey);

/**!
 * @brief Retrieves a string value from the KVP profile.
 * 
 * The caller owns the memory passed into `pszReturnedString`, which is filled out by this function. 
 * The `uStringSize` should include space for the null-terminator.
 *
 * @param[in] pInstance - Handle to the KVP instance.
 * @param[in] pszKey - Key of the string value to retrieve (null-terminated string).
 * @param[out] pszReturnedString - Pre-allocated buffer to store the retrieved string.
 * @param[in] uStringSize - Size of the `pszReturnedString` buffer (including space for the null-terminator).
 *
 * @returns Status of the operation (`ut_kvp_status_t`):
 * @retval UT_KVP_STATUS_SUCCESS - String value was found and successfully copied to `pszReturnedString`.
 * @retval UT_KVP_STATUS_INVALID_PARAM - An invalid parameter was passed (e.g. invalid buffer size).
 * @retval UT_KVP_STATUS_PARSING_ERROR - An error occurred while parsing the KVP data.
 * @retval UT_KVP_STATUS_NO_DATA - The KVP instance does not contain any data.
 * @retval UT_KVP_STATUS_KEY_NOT_FOUND - The specified key (`pszKey`) was not found in the KVP data.
 * @retval UT_KVP_STATUS_NULL_PARAM - A null parameter was passed.
 * @retval UT_KVP_STATUS_INVALID_INSTANCE - An invalid KVP instance handle was passed.
 */
ut_kvp_status_t ut_kvp_getStringField( ut_kvp_instance_t *pInstance, const char *pszKey, char *pszReturnedString, uint32_t uStringSize ); 

/* TODO:
 * - Implement functions for getting signed integer values (`ut_kvp_getInt8Field`, `ut_kvp_getInt16Field`, `ut_kvp_getInt32Field`, `ut_kvp_getInt64Field`).
 */
#endif /* __UT_KVP_H__ */