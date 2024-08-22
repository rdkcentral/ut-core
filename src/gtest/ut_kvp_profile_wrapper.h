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

#ifndef UT_KVP_PROFILE_WRAPPER_H
#define UT_KVP_PROFILE_WRAPPER_H

#include <string>
#include <stdexcept>
#include <ut_kvp_profile.h>

class UTKVPProfileWrapper
{
public:
    // Constructor: Opens the KVP profile with the provided file name
    explicit UTKVPProfileWrapper(const std::string &fileName);

    // Destructor: Ensures the profile is closed upon object destruction
    ~UTKVPProfileWrapper();

    // Method to explicitly open the KVP profile
    void open();

    // Method to explicitly close the KVP profile
    void close();

    // Method to retrieve the underlying C instance of the KVP profile
    ut_kvp_instance_t *getInstance() const;

private:
    std::string fileName_; // Profile file name
    bool isOpen_;          // Flag indicating if the profile is open
};

#endif // UT_KVP_PROFILE_WRAPPER_H