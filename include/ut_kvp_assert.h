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

//Usage for KVP Asserting

#include <ut.h>
#include <ut_kvp.h>

// Macro Setup, will perform type checking
#define UT_KVP_GET_BOOL( key ) \
{ \
    ut_kvp_value_t (kvpValue); \
    ut_kvp_handle_t *p_Handle = NULL; \
    FILE *inputfileptr = NULL; \
    p_Handle = ut_kvp_createInstance_from_file_ptr(inputfileptr); \
    kvpValue = ut_kvp_document_get_bool( p_Handle, key ); \
    UT_ASSERT( kvpValue.type == UT_KVP_BOOL ); \
    ut_kvp_pHandledestoryInstance_from_file_ptr(p_Handle); \
    kvpValue.u.bValue; \
} \

#define UT_KVP_GET_INT(key, kvpValue, pHandle) \
{ \
    kvpValue = ut_kvp_document_get_int( key, pHandle ); \
    UT_ASSERT( kvpValue.type == UT_KVP_INT ); \
    kvpValue.u.intValue \
} \


//.. Expand as required

#define UT_ASSERT_EQUAL_KVP_BOOL( checkValue, key ) \
{ \
    UT_ASSERT( UT_KVP_GET_BOOL( key ) == checkValue ); \
} \

#define UT_ASSERT_EQUAL_KVP_INT(checkValue, key, kvpValue, pHandle ) \
{ \
    UT_ASSERT( UT_KVP_GET_INT( key, kvpValue, pHandle )) == checkValue ); \
} \

