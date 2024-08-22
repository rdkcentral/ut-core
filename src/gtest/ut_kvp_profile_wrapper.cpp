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

#include "ut_kvp_profile_wrapper.h"
#include <stdexcept>
#include <cassert>

UTKVPProfileWrapper::UTKVPProfileWrapper(const std::string& fileName)
    : fileName_(fileName), isOpen_(false)
{
    open();
}

UTKVPProfileWrapper::~UTKVPProfileWrapper()
{
    close();
}

void UTKVPProfileWrapper::open()
{
    if (!isOpen_)
    {
        ut_kvp_status_t result = ut_kvp_profile_open(const_cast<char *>(fileName_.c_str()));
        if (result == UT_KVP_STATUS_SUCCESS)
        {
            isOpen_ = true;
        }
        else
        {
            throw std::runtime_error("Failed to open KVP profile");
        }
    }
}

void UTKVPProfileWrapper::close()
{
    if (isOpen_)
    {
        ut_kvp_profile_close();
        isOpen_ = false;
    }
}

ut_kvp_instance_t* UTKVPProfileWrapper::getInstance() const
{
    return ut_kvp_profile_getInstance();
}