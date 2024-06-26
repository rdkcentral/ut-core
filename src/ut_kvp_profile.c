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

#include <ut_kvp_profile.h>
#include <assert.h>

static ut_kvp_instance_t *gKVP_Instance = NULL;

ut_kvp_status_t ut_kvp_profile_open(char *fileName)
{
    ut_kvp_status_t result;

    if (gKVP_Instance == NULL)
    {
        gKVP_Instance = ut_kvp_createInstance();
        assert(gKVP_Instance != NULL);
    }

    result = ut_kvp_open(gKVP_Instance, fileName);
    assert( result == UT_KVP_STATUS_SUCCESS );

    return result;
}

void ut_kvp_profile_close(void)
{
    if ( gKVP_Instance != NULL )
    {
        ut_kvp_destroyInstance(gKVP_Instance);
        gKVP_Instance = NULL;
    }
}

ut_kvp_instance_t *ut_kvp_profile_getInstance( void )
{
    return gKVP_Instance;
}

