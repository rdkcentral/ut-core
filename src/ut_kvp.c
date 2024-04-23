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

/* Standard Libraries */
#include <errno.h>
#include <string.h>

/* Application Includes */
#include <ut_kvp.h>
#include <ut_log.h>
#include <assert.h>

/* External libraries */
#include <libfyaml.h>

ut_kvp_instance_t gKVP_Instance = NULL;

/* TODO: This fields may not be used and should be removed if so*/
#define UT_KVP_MAX_STRING_ELEMENT_SIZE (256)    /*!< Max String Size Supported */
#define UT_KVP_MAX_ARRAY_SIZE (32)              /*!< Max Array Size Supported */

typedef struct
{
    uint32_t magic;
    FILE *inputFilePtr;
    struct fy_document *fy_handle;
} ut_kvp_instance_internal_t;

/* Static functions */
static ut_kvp_instance_internal_t *validateInstance(ut_kvp_instance_t *pInstance);
static bool str_to_bool(const char *string);

ut_kvp_instance_t ut_kvp_createInstance(void)
{
    ut_kvp_instance_internal_t *pInstance = malloc(sizeof(ut_kvp_instance_internal_t));
    memset(pInstance, 0, sizeof(ut_kvp_instance_internal_t));

    pInstance->magic = 0xdeadbeef;

    return (ut_kvp_instance_t)pInstance;
}

void ut_kvp_destroyInstance(ut_kvp_instance_t *pInstance)
{
    // ut_kvp_instance_t *pInstance = get_ut_kvp_instance;
    ut_kvp_instance_internal_t *pInternal = validateInstance(pInstance);

    assert(pInstance != NULL);
    assert(pInternal->fy_handle != NULL);
    assert(pInternal->inputFilePtr != NULL);

    if (pInternal->fy_handle != NULL)
    {
        fy_document_destroy(pInternal->fy_handle);
    }
    if (pInternal->inputFilePtr != NULL)
    {
        fclose(pInternal->inputFilePtr);
    }

    memset(pInternal, 0, sizeof(ut_kvp_instance_internal_t));

    free(pInstance);
}

ut_kvp_status_t ut_kvp_read(ut_kvp_instance_t *pInstance, char *fileName)
{
    ut_kvp_instance_internal_t *pInternal = validateInstance(pInstance);

    assert(pInternal != NULL);
    if (pInternal == NULL)
    {
        return UT_KVP_INVALID_PARAM;
    }

    if (pInternal->inputFilePtr != NULL)
    {
        fclose(pInternal->inputFilePtr);
        pInternal->inputFilePtr = NULL;
    }

    pInternal->inputFilePtr = fopen(fileName, "r");
    if (NULL == pInternal->inputFilePtr)
    {
        UT_LOG_ERROR("Unable to open file\n");
        return UT_KVP_FILE_OPEN_ERROR;
    }
    pInternal->fy_handle = fy_document_build_from_fp(NULL, pInternal->inputFilePtr);
    if (NULL == pInternal->fy_handle)
    {
        UT_LOG_ERROR("Unable to build file\n");
        return UT_KVP_FILE_READ_ERROR;
    }

    return UT_KVP_STATUS_OK;
}

const char *ut_kvp_getField(ut_kvp_instance_t *pInstance, const char *pString)
{
    ut_kvp_instance_internal_t *pInternal = validateInstance(pInstance);
    const char *result = NULL;

    assert(pInternal != NULL);
    if (pInternal == NULL)
    {
        return NULL;
    }
    fy_document_scanf(pInternal->fy_handle, pString, result);

    return result;
}

bool ut_kvp_getBoolField( ut_kvp_instance_t *pInstance, const char *pString )
{
    const char *pField = ut_kvp_getField(pInstance, pString);

    return str_to_bool(pField);
}

uint32_t ut_kvp_getUInt32Field( ut_kvp_instance_t *pInstance, const char *pString )
{
    const char *pField;
    char *pEndptr;
    uint32_t uValue;
    errno = 0; // Clear the stdlib errno

    pField = ut_kvp_getField(pInstance, pString);
    uValue = strtoul(pField, &pEndptr, 10);

    // Error checking
    if (pField == pEndptr) 
    {
        UT_LOG_ERROR("No conversion performed!");
        assert(true);
        return 0;
    }
    else if (*pEndptr != '\0') 
    {
        UT_LOG_ERROR("Invalid characters in the string.");
        assert(true);
        return 0;
    } 
    else if (errno == ERANGE || uValue > UINT32_MAX) 
    {
        UT_LOG_ERROR("Value out of range for uint32_t.\n");
        assert(true);
        return 0;
    } 
    UT_LOG_DEBUG("Converted value: %u", uValue);
    return uValue;
}

uint64_t ut_kvp_getUInt64Field( ut_kvp_instance_t *pInstance, const char *pString )
{
    const char *pField;
    char *pEndptr;
    uint64_t uValue;

    errno = 0; // Clear the stdlib errno
    pField = ut_kvp_getField(pInstance, pString);

    uValue = strtoull(pField, &pEndptr, 10); // Base 10 conversion

    // Error checking
    if (pField == pEndptr) 
    {
        UT_LOG_ERROR("No conversion performed!");
        assert(true);
        return 0;
    }
    else if (*pEndptr != '\0') 
    {
        UT_LOG_ERROR("Invalid characters in the string.");
        assert(true);
        return 0;
    } 
    else if (errno == ERANGE || uValue > UINT64_MAX) 
    {
        UT_LOG_ERROR("Value out of range for uint64_t.");
        assert(true);
        return 0;
    }

    UT_LOG_DEBUG("Converted value: %llu", uValue);
    return uValue;
}

/** Static Functions */
static ut_kvp_instance_internal_t *validateInstance(ut_kvp_instance_t *pInstance)
{
    ut_kvp_instance_internal_t *pInternal = (ut_kvp_instance_internal_t *)pInstance;

    assert(pInternal != NULL);
    assert(pInternal->magic == 0xdeadbeef);

    if (pInternal->magic != 0xdeadbeef)
    {
        return NULL;
    }

    return pInternal;
}

static bool str_to_bool(const char *string)
{
    if ( strcasecmp( string, "true" ) == 0 )
    {
        return true;
    }
    if ( strcasecmp( string, "false" ) == 0 )
    {
        return false;
    }
    /* String is neither true or false, ensure we inform the caller*/
    assert(true);
    return false;
}