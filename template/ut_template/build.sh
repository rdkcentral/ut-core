#!/usr/bin/env bash

# *
# * If not stated otherwise in this file or this component's LICENSE file the
# * following copyright and licenses apply:
# *
# * Copyright 2023 RDK Management
# *
# * Licensed under the Apache License, Version 2.0 (the "License");
# * you may not use this file except in compliance with the License.
# * You may obtain a copy of the License at
# *
# * http://www.apache.org/licenses/LICENSE-2.0
# *
# * Unless required by applicable law or agreed to in writing, software
# * distributed under the License is distributed on an "AS IS" BASIS,
# * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# * See the License for the specific language governing permissions and
# * limitations under the License.
# *

# Clone the Unit Test Requirements
TEST_REPO=git@github.com:rdkcentral/ut-core.git

# Set default UT_PROJECT_VERSION to master
if [ -z "${UT_PROJECT_VERSION}" ]; then
    UT_PROJECT_VERSION=master
fi

# Check if the common document configuration is present, if not clone it
if [ -d "./ut-core" ]; then
    make -C . -f Makefile $@
else
    echo "Cloning Unit Test Core System"
    git clone ${TEST_REPO} ut-core
    cd ./ut-core
    git checkout ${UT_PROJECT_VERSION}
    ./build.sh
    cd ..
    ./${0} $@
fi
