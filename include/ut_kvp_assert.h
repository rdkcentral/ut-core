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
    bool kvpValue; \
    kvpValue = ut_kvp_getField( gKVP_Instance, key ); \
    kvpValue; \
} \

#define UT_KVP_GET_INT(key) \
{ \
    int kvpValue; \
    kvpValue = ut_kvp_getField( gKVP_Instance, key ); \
    kvpValue; \
} \

#if 0
#define UT_KVP_GET_STRING(key) \
{ \
    const char *kvpValue; \
    kvpValue = ut_kvp_getField( key ); \
    kvpValue; \
} \

#endif

//.. Expand as required

#define UT_ASSERT_EQUAL_KVP_BOOL( checkValue, key ) \
{ \
    UT_ASSERT_EQUAL( UT_KVP_GET_BOOL( #key ), #checkValue ); \
} \

#define UT_ASSERT_EQUAL_KVP_INT( checkValue, key ) \
{ \
    UT_ASSERT_EQUAL( UT_KVP_GET_INT( #key ), #checkValue ); \
} \

#define UT_ASSERT_EQUAL_KVP_STRING( checkValue, key ) \
{ \
    UT_ASSERT_EQUAL_STRING( UT_KVP_GET_STRING( #key ), #checkValue ); \
}
