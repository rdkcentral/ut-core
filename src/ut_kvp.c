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

#define NDEBUG // Disable to switch on assert
#include <assert.h>

/* External libraries */
#include <libfyaml.h>

ut_kvp_instance_t *gKVP_Instance = NULL;

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

ut_kvp_instance_t *ut_kvp_createInstance(void)
{
    ut_kvp_instance_internal_t *pInstance = malloc(sizeof(ut_kvp_instance_internal_t));

    if ( pInstance == NULL )
    {
        assert( pInstance != NULL );
        return NULL;
    }

    memset(pInstance, 0, sizeof(ut_kvp_instance_internal_t));

    pInstance->magic = 0xdeadbeef;

    return (ut_kvp_instance_t *)pInstance;
}

void ut_kvp_destroyInstance(ut_kvp_instance_t *pInstance)
{
    ut_kvp_instance_internal_t *pInternal = validateInstance(pInstance);

    if ( pInternal == NULL )
    {
        assert(pInternal == NULL);
        return;
    }

    if ( pInternal -> inputFilePtr == NULL )
    {
        assert(pInternal->inputFilePtr == NULL);
        return;
    }

    ut_kvp_close( pInstance );

    memset(pInternal, 0, sizeof(ut_kvp_instance_internal_t));

    free(pInternal);
    pInternal = NULL;
    pInstance = NULL;
}

ut_kvp_status_t ut_kvp_read(ut_kvp_instance_t *pInstance, char *fileName)
{
    ut_kvp_instance_internal_t *pInternal = validateInstance(pInstance);

    if (pInternal == NULL || fileName == NULL)
    {
        return UT_KVP_INVALID_PARAM;
    }

    if (pInternal->inputFilePtr != NULL)
    {
        ut_kvp_close( pInstance );
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
        ut_kvp_close( pInstance );
        return UT_KVP_FILE_READ_ERROR;
    }

    return UT_KVP_STATUS_OK;
}

void ut_kvp_close(ut_kvp_instance_t *pInstance)
{
    ut_kvp_instance_internal_t *pInternal = validateInstance(pInstance);

    if (pInternal == NULL)
    {
        return;
    }

    if (pInternal->inputFilePtr != NULL)
    {
        fclose(pInternal->inputFilePtr);
        pInternal->inputFilePtr = NULL;
    }

    if ( pInternal->fy_handle != NULL)
    {
        fy_document_destroy(pInternal->fy_handle);
        pInternal->fy_handle = NULL;
    }
}

const char *ut_kvp_getField(ut_kvp_instance_t *pInstance, const char *pszKey, char *pzResult)
{
    ut_kvp_instance_internal_t *pInternal = validateInstance(pInstance);
    char zEntry[UT_KVP_MAX_STRING_ELEMENT_SIZE];
    char *str = "%s";
    int fy_result;

    if (pInternal == NULL)
    {
        assert(pInternal != NULL);
        return NULL;
    }

    snprintf( zEntry, UT_KVP_MAX_STRING_ELEMENT_SIZE, "%s %s", pszKey, str );
    fy_result = fy_document_scanf(pInternal->fy_handle, zEntry, pzResult);
    if ( fy_result <= 0 )
    {
        assert( fy_result > 0 );
        return NULL;
    }

    return pzResult;
}

bool ut_kvp_getBoolField( ut_kvp_instance_t *pInstance, const char *pszKey )
{
    char result[UT_KVP_MAX_STRING_ELEMENT_SIZE];
    const char *pField = ut_kvp_getField(pInstance, pszKey, result);

    return str_to_bool(pField);
}

uint32_t ut_kvp_getUInt32Field( ut_kvp_instance_t *pInstance, const char *pszKey )
{
    const char *pField;
    char *pEndptr;
    uint32_t uValue;
    char result[UT_KVP_MAX_STRING_ELEMENT_SIZE];
    errno = 0; // Clear the stdlib errno

    pField = ut_kvp_getField(pInstance, pszKey, result);

    if (strstr(result, "0x"))
    {
        uValue = strtoul(pField, &pEndptr, 16); // Base 16 conversion
    }
    else
    {
        uValue = strtoul(pField, &pEndptr, 10); // Base 10 conversion
    }

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

uint64_t ut_kvp_getUInt64Field( ut_kvp_instance_t *pInstance, const char *pszKey )
{
    const char *pField;
    char *pEndptr;
    uint64_t uValue;
    char result[UT_KVP_MAX_STRING_ELEMENT_SIZE];

    errno = 0; // Clear the stdlib errno
    pField = ut_kvp_getField(pInstance, pszKey, result);

    if(strstr(result, "0x"))
    {
        uValue = strtoul(pField, &pEndptr, 16); // Base 16 conversion
    }
    else
    {
        uValue = strtoul(pField, &pEndptr, 10); // Base 10 conversion
    }

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

const char *ut_kvp_getStringField(ut_kvp_instance_t *pInstance, const char *pszKey)
{
    char result[UT_KVP_MAX_STRING_ELEMENT_SIZE];
    return ut_kvp_getField(pInstance, pszKey, result);
}

/** Static Functions */
static ut_kvp_instance_internal_t *validateInstance(ut_kvp_instance_t *pInstance)
{
    ut_kvp_instance_internal_t *pInternal = (ut_kvp_instance_internal_t *)pInstance;

    if ( pInstance == NULL )
    {
        assert(pInternal == NULL);
        return NULL;
    }

    if (pInternal->magic != 0xdeadbeef)
    {
        assert(pInternal->magic != 0xdeadbeef);
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