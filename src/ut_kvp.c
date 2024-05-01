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
#include <limits.h>

/* Application Includes */
#include <ut_kvp.h>
#include <ut_log.h>
#include <assert.h>

/* External libraries */
#include <libfyaml.h>

ut_kvp_instance_t *gKVP_Instance = NULL;

typedef struct
{
    uint32_t magic;
    FILE *inputFilePtr;
    struct fy_document *fy_handle;
} ut_kvp_instance_internal_t;

/* Static functions */
static ut_kvp_instance_internal_t *validateInstance(ut_kvp_instance_t *pInstance);
static unsigned long getUIntField( ut_kvp_instance_t *pInstance, const char *pszKey, unsigned long maxRange );
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

ut_kvp_status_t ut_kvp_open(ut_kvp_instance_t *pInstance, char *fileName)
{
    ut_kvp_instance_internal_t *pInternal = validateInstance(pInstance);

    if (pInstance == NULL)
    {
        return UT_KVP_STATUS_INVALID_PARAM;
    }

    if (fileName == NULL)
    {
        assert( fileName != NULL );
        UT_LOG_ERROR( "Invalid Param [fileName]" );
        return UT_KVP_STATUS_INVALID_PARAM;
    }

    if (pInternal->inputFilePtr != NULL)
    {
        /* Current file was open we'll close it now */
        ut_kvp_close( pInstance );
    }

    pInternal->inputFilePtr = fopen(fileName, "r");
    if (NULL == pInternal->inputFilePtr)
    {
        UT_LOG_ERROR("Unable to open file");
        return UT_KVP_STATUS_FILE_OPEN_ERROR;
    }
    pInternal->fy_handle = fy_document_build_from_fp(NULL, pInternal->inputFilePtr);
    if (NULL == pInternal->fy_handle)
    {
        UT_LOG_ERROR("Unable to parse file");
        ut_kvp_close( pInstance );
        return UT_KVP_STATUS_PARSING_ERROR;
    }

    return UT_KVP_STATUS_SUCCESS;
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

ut_kvp_status_t ut_kvp_getField(ut_kvp_instance_t *pInstance, const char *pszKey, char *pzResult)
{
    ut_kvp_instance_internal_t *pInternal = validateInstance(pInstance);
    char zEntry[UT_KVP_MAX_ELEMENT_SIZE];
    char *str = "%s";
    int fy_result;

    if (pInternal == NULL)
    {
        assert(pInternal != NULL);
        return UT_KVP_STATUS_INVALID_PARAM;
    }

    if (pszKey == NULL)
    {
        assert(pszKey != NULL);
        UT_LOG_ERROR("Invalid Param - pszKey");
        return UT_KVP_STATUS_INVALID_PARAM;
    }

    if (pzResult == NULL)
    {
        assert(pzResult != NULL);
        UT_LOG_ERROR("Invalid Param - pzResult");
        return UT_KVP_STATUS_INVALID_PARAM;
    }

    if ( pInternal->fy_handle == NULL )
    {
        assert(pInternal->fy_handle != NULL);
        UT_LOG_ERROR("No Data File open");
        return UT_KVP_STATUS_NO_DATA;
    }

    snprintf( zEntry, UT_KVP_MAX_ELEMENT_SIZE, "%s %s", pszKey, str );
    fy_result = fy_document_scanf(pInternal->fy_handle, zEntry, pzResult);
    if ( fy_result <= 0 )
    {
        assert( fy_result > 0 );
        UT_LOG_ERROR("Parsing Error");
        return UT_KVP_STATUS_PARSING_ERROR;
    }

    return UT_KVP_STATUS_SUCCESS;
}

bool ut_kvp_getBoolField( ut_kvp_instance_t *pInstance, const char *pszKey )
{
    char result[UT_KVP_MAX_ELEMENT_SIZE];
    ut_kvp_status_t status;
    bool bValue;

    status = ut_kvp_getField(pInstance, pszKey, result);
    if ( status != UT_KVP_STATUS_SUCCESS )
    {
        assert(status == UT_KVP_STATUS_SUCCESS);
        return false;
    }

    bValue = str_to_bool(result);
    return bValue;
}

static unsigned long getUIntField( ut_kvp_instance_t *pInstance, const char *pszKey, unsigned long maxRange )
{
    char *pEndptr;
    unsigned long uValue;
    char result[UT_KVP_MAX_ELEMENT_SIZE];
    ut_kvp_status_t status;
    char *pField = &result[0];

    errno = 0; // Clear the stdlib errno

    status = ut_kvp_getField(pInstance, pszKey, result);
    if ( status != UT_KVP_STATUS_SUCCESS )
    {
        assert(status != UT_KVP_STATUS_SUCCESS);
        return 0;
    }

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
    else if (errno == ERANGE || uValue > maxRange)
    {
        UT_LOG_ERROR("Value out of range for maxRange [0x%x,%d].\n", maxRange, maxRange);
        assert(true);
        return 0;
    }

    //UT_LOG_DEBUG("Converted value: %u", uValue);
    return uValue;
}

uint8_t ut_kvp_getUInt8Field( ut_kvp_instance_t *pInstance, const char *pszKey )
{
    unsigned long value;
    value = getUIntField( pInstance, pszKey, UINT8_MAX );

    return (uint8_t)value;
}

uint16_t ut_kvp_getUInt16Field( ut_kvp_instance_t *pInstance, const char *pszKey )
{
    unsigned long value;
    value = getUIntField( pInstance, pszKey, UINT16_MAX );

    return (uint16_t)value;
}

uint32_t ut_kvp_getUInt32Field( ut_kvp_instance_t *pInstance, const char *pszKey )
{
    unsigned long value;
    value = getUIntField( pInstance, pszKey, UINT32_MAX );

    return (uint32_t)value;
}

uint64_t ut_kvp_getUInt64Field( ut_kvp_instance_t *pInstance, const char *pszKey )
{
    char *pEndptr;
    uint64_t u64Value;
    char result[UT_KVP_MAX_ELEMENT_SIZE];
    ut_kvp_status_t status;
    char *pField = &result[0];

    errno = 0; // Clear the stdlib errno
    status = ut_kvp_getField(pInstance, pszKey, result);
    if ( status != UT_KVP_STATUS_SUCCESS )
    {
        assert(status != UT_KVP_STATUS_SUCCESS);
        return 0;
    }

    if(strstr(result, "0x"))
    {
        u64Value = strtoull(pField, &pEndptr, 16); // Base 16 conversion
    }
    else
    {
        u64Value = strtoull(pField, &pEndptr, 10); // Base 10 conversion
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
    else if (errno == ERANGE || u64Value > UINT64_MAX) 
    {
        UT_LOG_ERROR("Value out of range for uint64_t.");
        assert(true);
        return 0;
    }

    //UT_LOG_DEBUG("Converted value: %llu", u64Value);
    return u64Value;
}

const char* ut_kvp_getStringField( ut_kvp_instance_t *pInstance, const char *pszKey, const char *psValue )
{
    struct fy_node *node = NULL;
    struct fy_node *root = NULL;

    ut_kvp_instance_internal_t *pInternal = validateInstance(pInstance);

     if (pInternal == NULL)
    {
        assert(pInternal != NULL);
        return NULL;
    }

    if (pszKey == NULL)
    {
        assert(pszKey != NULL);
        UT_LOG_ERROR("Invalid Param - pszKey");
        return NULL;
    }

    if ( pInternal->fy_handle == NULL )
    {
        assert(pInternal->fy_handle != NULL);
        UT_LOG_ERROR("No Data File open");
        return NULL;
    }
    // Get the root node
    root = fy_document_root(pInternal->fy_handle);
    if ( root == NULL )
    {
        assert( root != NULL );
        UT_LOG_ERROR("Empty document");
        return NULL;
    }

    // Find the node corresponding to the key
    node = fy_node_by_path(root, pszKey, -1, FYNWF_DONT_FOLLOW);
    if ( node == NULL )
    {
        assert( node != NULL );
        UT_LOG_ERROR("node not found");
        return NULL;
    }

    if (node && fy_node_is_scalar(node)) {
        // Get the string value
        psValue = fy_node_get_scalar0(node);
    }
    return psValue;

}

/** Static Functions */
static ut_kvp_instance_internal_t *validateInstance(ut_kvp_instance_t *pInstance)
{
    ut_kvp_instance_internal_t *pInternal = (ut_kvp_instance_internal_t *)pInstance;

    if ( pInstance == NULL )
    {
        assert(pInternal == NULL);
        UT_LOG_ERROR("Invalid Handle");
        return NULL;
    }

    if (pInternal->magic != 0xdeadbeef)
    {
        assert(pInternal->magic != 0xdeadbeef);
        UT_LOG_ERROR("Invalid Handle - magic failure");
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