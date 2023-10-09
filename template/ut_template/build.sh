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

RED="\e[0;31m"
YELLOW="\e[1;33m"
GREEN="\e[0;32m"
NC="\e[39m"

UT_PROJECT_MAJOR_VERSION="1."    # Change this to upgrade your UT-Core Major versions. Non ABI Changes 1.x.x are supported

# Clone the Unit Test Requirements
TEST_REPO=git@github.com:rdkcentral/ut-core.git

# This function checks if we're not sitting on the latest revision.
function check_next_revision()
{
    pushd ./ut-core 2&>/dev/null
    # Set default UT_PROJECT_VERSION to next revision
    if [ -v ${UT_PROJECT_VERSION} ]; then
        UT_PROJECT_VERSION=$(git tag | grep ${UT_PROJECT_MAJOR_VERSION} | sort -r | head -n1)
        UT_NEXT_VERSION=$(git tag | sort -r | head -n1)
        echo -e ${YELLOW}ut-core version selected:[${UT_PROJECT_VERSION}]${NC}
        if [ "${UT_NEXT_VERSION}" != "${UT_PROJECT_VERSION}" ]; then
            echo -e ${RED}--- New Version of ut-core released [${UT_NEXT_VERSION}] consider upgrading ---${NC}
        fi
    fi
    popd 2&>/dev/null
}

# Check if the common document configuration is present, if not clone it
if [ -d "./ut-core" ]; then
    # ut-core exists so run the makefile from ut
    check_next_revision
    make -C . -f Makefile $@
else
    echo "Cloning Unit Test Core System"
    git clone ${TEST_REPO} ut-core
    check_next_revision
    cd ./ut-core
    git checkout ${UT_PROJECT_VERSION}
    ./build.sh
    cd ..
    ./${0} $@
fi
