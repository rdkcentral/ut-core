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
    UT_KVP_INVALID_ELEMENT=0,
    UT_KVP_BOOL,
    UT_KVP_INT,
    UT_KVP_zSTRING,
    UT_KVP_FLOAT,
}ut_kvp_type_t;

#define UT_KVP_MAX_STRING_ELEMENT_SIZE (256)
#define UT_KVP_MAX_ARRAY_SIZE (32)

typedef struct
{
    ut_kvp_type_t  type;      /*!< Type of the value */
    int    arrayElements;  /*!< number of elements in the array*/
    union
    {
        bool        bValue;       /*!< boolean, true or false using std_bool */
        int           intValue;     /*!< signed int */
        const char  szValue[UT_KVP_MAX_STRING_ELEMENT_SIZE];    /*!< char */
        uint32_t    intArray[UT_KVP_MAX_ARRAY_SIZE];    /*!< element array, using arrayElements as element count */
        float      fValue; /*!< float value */
    }u;
}ut_kvp_value_t;

typedef struct fy_document ut_kvp_handle_t;
typedef void ut_kvp_instance_t;

/**
 * @brief create instance handle to the kvp object
 */
ut_kvp_handle_t *ut_kvp_createInstance( void );

/**
 * @brief destory instance handle to the kvp object
 */
ut_kvp_instance_t ut_kvp_destoryInstance( ut_kvp_handle_t *pHandle );

/**
 * @brief read kvp file and ready for use
 */
ut_kvp_value_t ut_kvp_read( ut_kvp_handle_t *pHandle, const char *pszKvpFile );

/**
 * @brief Get a key value pair from the passed configuration
 *
 * @param param[in] - Handle to the instance
 * @param key[in] - Zero Terminated String Key
 *
 * @return ut_kvp_value_t  - Returned structure
 * @retval type == UT_KVP_INVALID_ELEMENT if the element is not found
 */
ut_kvp_value_t ut_kvp_get( ut_kvp_handle_t *pHandle, const char *pszKey );

ut_kvp_handle_t *ut_kvp_createInstance_from_file_ptr(FILE *inputfileptr);
ut_kvp_handle_t *ut_kvp_createInstance_from_file(char *file);
ut_kvp_value_t ut_kvp_document_get_int(ut_kvp_handle_t *pHandle, const char *string);
ut_kvp_value_t ut_kvp_document_get_string(ut_kvp_handle_t *pHandle, const char *string);
ut_kvp_value_t ut_kvp_document_get_bool(ut_kvp_handle_t *pHandle, const char *string);
/*Other possible examples*/
const char * ut_kvp_getString( ut_kvp_handle_t *pHandle, const char *pszKey );
int ut_kvp_getInt( ut_kvp_handle_t *pHandle, const char *pszKey );
int ut_kvp_getIntFromArray( ut_kvp_handle_t *pHandle, const char *pszKey, uint32_t elementIndex );
int ut_kvp_geLong( ut_kvp_handle_t *pHandle, const char *pszKey );
float ut_kvp_getFloat( ut_kvp_handle_t *pHandle, const char *pszKey );
