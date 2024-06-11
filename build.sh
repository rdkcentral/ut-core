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

set -e # error out if required
SCRIPT_EXEC="$(realpath $0)"
MY_DIR="$(dirname $SCRIPT_EXEC)"


FRAMEWORK_DIR=${MY_DIR}/framework
UT_CONTROL_LIB_DIR=${FRAMEWORK_DIR}/ut-control

pushd ${MY_DIR} > /dev/null
# Clone CUnit
if [ -d "${FRAMEWORK_DIR}/CUnit-2.1-3" ]; then
    echo "Framework CUnit already exists"
else
    echo "Clone Framework"
    wget https://sourceforge.net/projects/cunit/files/CUnit/2.1-3/CUnit-2.1-3.tar.bz2 --no-check-certificate -P framework/
    tar xvfj framework/CUnit-2.1-3.tar.bz2 -C ./framework/
    cp framework/CUnit-2.1-3/CUnit/Headers/CUnit.h.in framework/CUnit-2.1-3/CUnit/Headers/CUnit.h
    echo "Patching Framework"
    cd ${FRAMEWORK_DIR}
    cp ../src/cunit/cunit_lgpl/patches/CorrectBuildWarningsInCunit.patch  .
    patch -u CUnit-2.1-3/CUnit/Sources/Framework/TestRun.c -i CorrectBuildWarningsInCunit.patch
    echo "Patching Complete"
fi
popd > /dev/null # ${MY_DIR}

pushd ${FRAMEWORK_DIR} > /dev/null
if [ -d "${UT_CONTROL_LIB_DIR}" ]; then
    echo "Framework ut-control already exists"
else
    if [ "$1" != "no_ut_control" ]; then
        echo "Clone ut_control in ${UT_CONTROL_LIB_DIR}"
        #TODO: change the git url to correct one
        git clone https://github.com/rdkcentral/ut-control.git -b feature/gh20-control-plane-part2-librarycreation
        #clone the external libraries
        cd ut-control
        ./install.sh
        #make lib
    else
        echo "$1 requested, hence ut-control is not required to be cloned"
    fi
fi
popd > /dev/null # ${FRAMEWORK_DIR}
