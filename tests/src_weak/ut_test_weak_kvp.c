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


#include <stdint.h>
#include <stdbool.h>
#include <stddef.h> // For NULL
#include <stdlib.h> // For malloc/free
#include "ut_kvp.h"

/**
 * @brief Weak implementation of ut_kvp_createInstance.
 *
 * Creates a new KVP instance.
 * Always returns NULL to indicate failure.
 *
 * @returns NULL.
 */
ut_kvp_instance_t *__attribute__((weak)) ut_kvp_createInstance(void)
{
    printf("Weak implementation of [%s]", __func__);
    return NULL; // Default failure
}

/**
 * @brief Weak implementation of ut_kvp_destroyInstance.
 *
 * Destroys a KVP instance.
 * Does nothing by default.
 *
 * @param[in] pInstance - Handle to the instance to destroy.
 */
void __attribute__((weak)) ut_kvp_destroyInstance(ut_kvp_instance_t *pInstance)
{
    printf("Weak implementation of [%s]", __func__);
    (void)pInstance; // Default no-op
}

/**
 * @brief Weak implementation of ut_kvp_open.
 *
 * Opens and parses a KVP file into a KVP instance.
 * Always returns UT_KVP_STATUS_INVALID_INSTANCE.
 *
 * @param[in] pInstance - Handle to the KVP instance.
 * @param[in] fileName - Path to the KVP file.
 * @returns UT_KVP_STATUS_INVALID_INSTANCE.
 */
ut_kvp_status_t __attribute__((weak)) ut_kvp_open(ut_kvp_instance_t *pInstance, char *fileName)
{
    printf("Weak implementation of [%s]", __func__);
    return UT_KVP_STATUS_INVALID_INSTANCE;
}

/**
 * @brief Weak implementation of ut_kvp_openMemory.
 *
 * Opens and parses a memory block into a KVP instance.
 * Always returns UT_KVP_STATUS_INVALID_INSTANCE.
 *
 * @param[in] pInstance - Handle to the KVP instance.
 * @param[in] pData - Pointer to memory containing KVP data.
 * @param[in] length - Size of the KVP data.
 * @returns UT_KVP_STATUS_INVALID_INSTANCE.
 */
ut_kvp_status_t __attribute__((weak)) ut_kvp_openMemory(ut_kvp_instance_t *pInstance, char *pData, uint32_t length)
{
    printf("Weak implementation of [%s]", __func__);
    return UT_KVP_STATUS_INVALID_INSTANCE;
}

/**
 * @brief Weak implementation of ut_kvp_close.
 *
 * Closes a previously opened KVP instance.
 * Does nothing by default.
 *
 * @param[in] pInstance - Handle to the KVP instance.
 */
void __attribute__((weak)) ut_kvp_close(ut_kvp_instance_t *pInstance)
{
    (void)pInstance; // Default no-op
    printf("Weak implementation of [%s]", __func__);
}

/**
 * @brief Weak implementation of ut_kvp_getBoolField.
 *
 * Always returns false.
 *
 * @param[in] pInstance - Handle to the KVP instance.
 * @param[in] pszKey - Key to search for.
 * @returns false.
 */
bool __attribute__((weak)) ut_kvp_getBoolField(ut_kvp_instance_t *pInstance, const char *pszKey)
{
    printf("Weak implementation of [%s]", __func__);
    return false; // Default behavior
}

/**
 * @brief Weak implementation of ut_kvp_getUInt8Field.
 *
 * Always returns 0.
 *
 * @param[in] pInstance - Handle to the KVP instance.
 * @param[in] pszKey - Key to search for.
 * @returns 0.
 */
uint8_t __attribute__((weak)) ut_kvp_getUInt8Field(ut_kvp_instance_t *pInstance, const char *pszKey)
{
    printf("Weak implementation of [%s]", __func__);
    return 0;
}

/**
 * @brief Weak implementation of ut_kvp_getUInt16Field.
 *
 * Always returns 0.
 *
 * @param[in] pInstance - Handle to the KVP instance.
 * @param[in] pszKey - Key to search for.
 * @returns 0.
 */
uint16_t __attribute__((weak)) ut_kvp_getUInt16Field(ut_kvp_instance_t *pInstance, const char *pszKey)
{
    printf("Weak implementation of [%s]", __func__);
    return 0;
}

/**
 * @brief Weak implementation of ut_kvp_getUInt32Field.
 *
 * Always returns 0.
 *
 * @param[in] pInstance - Handle to the KVP instance.
 * @param[in] pszKey - Key to search for.
 * @returns 0.
 */
uint32_t __attribute__((weak)) ut_kvp_getUInt32Field(ut_kvp_instance_t *pInstance, const char *pszKey)
{
    printf("Weak implementation of [%s]", __func__);
    return 0;
}

/**
 * @brief Weak implementation of ut_kvp_getUInt64Field.
 *
 * Always returns 0.
 *
 * @param[in] pInstance - Handle to the KVP instance.
 * @param[in] pszKey - Key to search for.
 * @returns 0.
 */
uint64_t __attribute__((weak)) ut_kvp_getUInt64Field(ut_kvp_instance_t *pInstance, const char *pszKey)
{
    printf("Weak implementation of [%s]", __func__);
    return 0;
}

/**
 * @brief Weak implementation of ut_kvp_getStringField.
 *
 * Always returns UT_KVP_STATUS_KEY_NOT_FOUND.
 *
 * @param[in] pInstance - Handle to the KVP instance.
 * @param[in] pszKey - Key to search for.
 * @param[out] pszReturnedString - Buffer to store the string.
 * @param[in] uStringSize - Size of the buffer.
 * @returns UT_KVP_STATUS_KEY_NOT_FOUND.
 */
ut_kvp_status_t __attribute__((weak)) ut_kvp_getStringField(
    ut_kvp_instance_t *pInstance,
    const char *pszKey,
    char *pszReturnedString,
    uint32_t uStringSize)
{
    (void)pInstance;
    (void)pszKey;
    (void)pszReturnedString;
    (void)uStringSize;
    printf("Weak implementation of [%s]", __func__);
    return UT_KVP_STATUS_KEY_NOT_FOUND;
}

/**
 * @brief Weak implementation of ut_kvp_getFloatField.
 *
 * Always returns 0.0f.
 *
 * @param[in] pInstance - Handle to the KVP instance.
 * @param[in] pszKey - Key to search for.
 * @returns 0.0f.
 */
float __attribute__((weak)) ut_kvp_getFloatField(ut_kvp_instance_t *pInstance, const char *pszKey)
{
    (void)pInstance;
    (void)pszKey;
    printf("Weak implementation of [%s]", __func__);
    return 0.0f;
}

/**
 * @brief Weak implementation of ut_kvp_getDoubleField.
 *
 * Always returns 0.0.
 *
 * @param[in] pInstance - Handle to the KVP instance.
 * @param[in] pszKey - Key to search for.
 * @returns 0.0.
 */
double __attribute__((weak)) ut_kvp_getDoubleField(ut_kvp_instance_t *pInstance, const char *pszKey)
{
    (void)pInstance;
    (void)pszKey;
    printf("Weak implementation of [%s]", __func__);
    return 0.0;
}

/**
 * @brief Weak implementation of ut_kvp_fieldPresent.
 *
 * Always returns false.
 *
 * @param[in] pInstance - Handle to the KVP instance.
 * @param[in] pszKey - Key to search for.
 * @returns false.
 */
bool __attribute__((weak)) ut_kvp_fieldPresent(ut_kvp_instance_t *pInstance, const char *pszKey)
{
    (void)pInstance;
    (void)pszKey;
    printf("Weak implementation of [%s]", __func__);
    return false;
}

/**
 * @brief Weak implementation of ut_kvp_getData.
 *
 * Always returns NULL.
 *
 * @param[in] pInstance - Handle to the KVP instance.
 * @returns NULL.
 */
char *__attribute__((weak)) ut_kvp_getData(ut_kvp_instance_t *pInstance)
{
    (void)pInstance;
    printf("Weak implementation of [%s]", __func__);
    return NULL;
}

/**
 * @brief Weak implementation of ut_kvp_getListCount.
 *
 * Always returns 0.
 *
 * @param[in] pInstance - Handle to the KVP instance.
 * @param[in] pszKey - Key to search for.
 * @returns 0.
 */
uint32_t __attribute__((weak)) ut_kvp_getListCount(ut_kvp_instance_t *pInstance, const char *pszKey)
{
    (void)pInstance;
    (void)pszKey;
    printf("Weak implementation of [%s]", __func__);
    return 0;
}

/**
 * @brief Weak implementation of ut_kvp_getDataBytes.
 *
 * Always returns NULL and sets size to 0.
 *
 * @param[in] pInstance - Handle to the KVP instance.
 * @param[in] pszKey - Key to search for.
 * @param[out] size - Pointer to store the size of the data.
 * @returns NULL.
 */
unsigned char *__attribute__((weak)) ut_kvp_getDataBytes(ut_kvp_instance_t *pInstance, const char *pszKey, int *size)
{
    (void)pInstance;
    (void)pszKey;
    if (size)
    {
        *size = 0;
    }
    printf("Weak implementation of [%s]", __func__);
    return NULL;
}