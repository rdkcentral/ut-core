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

if [[ "$*" == *"VARIANT=CPP"* ]]; then
    VARIANT="CPP"
else
    VARIANT="C"
fi

if [[ "$*" == *"TARGET=arm"* ]]; then
    TARGET="arm"
else
    TARGET="linux"
fi
echo "TARGET= [$TARGET] from [$0]"
echo "VARIANT= [$VARIANT] from [$0]"

THIRD_PARTY_LIB_DIR=${FRAMEWORK_DIR}/ut-control/build/${TARGET}
GTEST_DIR=${FRAMEWORK_DIR}/gtest/${TARGET}
GTEST_LIB_DIR=${MY_DIR}/build/${TARGET}/cpp_libs
if [ "$VARIANT" == "CPP" ]; then
    COMPILE_TIME_MARKER=${MY_DIR}/build/${TARGET}/obj/src/c_source/ut_cunit.o
else
    COMPILE_TIME_MARKER=${MY_DIR}/build/${TARGET}/obj/hal_test
fi

# Check if the file exists
if [ -f "$COMPILE_TIME_MARKER" ]; then
    echo "********************************************"
    echo "Please compile again after cleaning ut-core"
    echo "********************************************"
    exit 1
fi

pushd ${MY_DIR} > /dev/null
# Clone CUnit
if [[ ! -d "${FRAMEWORK_DIR}/CUnit-2.1-3" && "${VARIANT}" == "C" ]]; then
    echo "Clone Framework"
    wget https://sourceforge.net/projects/cunit/files/CUnit/2.1-3/CUnit-2.1-3.tar.bz2 --no-check-certificate -P ${FRAMEWORK_DIR}
    tar xvfj framework/CUnit-2.1-3.tar.bz2 -C ${FRAMEWORK_DIR}
    cp ${FRAMEWORK_DIR}/CUnit-2.1-3/CUnit/Headers/CUnit.h.in ${FRAMEWORK_DIR}/CUnit-2.1-3/CUnit/Headers/CUnit.h
    echo "Patching Framework"
    cd ${FRAMEWORK_DIR}
    cp ../src/c_source/cunit_lgpl/patches/CorrectBuildWarningsInCunit.patch  .
    patch -u CUnit-2.1-3/CUnit/Sources/Framework/TestRun.c -i CorrectBuildWarningsInCunit.patch
    echo "Patching Complete"
else
    mkdir -p ${FRAMEWORK_DIR}
fi
popd > /dev/null # ${MY_DIR}

# The purpose of this script is on a clean environement, is to give you the latest binary compatible version for testing
# When the major version changes in the ut-control, what that signals is that the testings will have to be upgraded to support that version
# Therefore in that case it warns you but doesnt' chnage to that version, which could cause your tests to break.
# Change this to upgrade your ut-control Major versions. Non ABI Changes 1.x.x are supported, between major revisions

UT_CONTROL_PROJECT_VERSION="1.5.0"  # Fixed version

# Clone the Unit Test Requirements
TEST_REPO=git@github.com:rdkcentral/ut-control.git

# This function checks the latest version of UT core and recommends an upgrade if reuqired
function check_ut_control_revision()
{
    pushd ${UT_CONTROL_LIB_DIR} > /dev/null
    # Set default UT_CONTROL_PROJECT_VERSION to next revision, if it's set then we don't need to tell you again
    if [ -v ${UT_CONTROL_PROJECT_VERSION} ]; then
        UT_CONTROL_PROJECT_VERSION=$(git tag | grep ${UT_CONTROL_PROJECT_CURRENT_VERSION} | sort -r | head -n1)
        UT_NEXT_VERSION=$(git tag | sort -r | head -n1)
        echo -e ${YELLOW}ut-control version selected:[${UT_CONTROL_PROJECT_VERSION}]${NC}
        if [ "${UT_NEXT_VERSION}" != "${UT_CONTROL_PROJECT_VERSION}" ]; then
            echo -e ${RED}--- New Version of ut-control released [${UT_NEXT_VERSION}] consider upgrading ---${NC}
        fi
    fi
    echo -e ${YELLOW}ut-control version selected:[${UT_CONTROL_PROJECT_VERSION}]${NC}
    popd > /dev/null
}

# Check if the common document configuration is present, if not clone it
pushd ${FRAMEWORK_DIR} > /dev/null

configure_ut_control() {
    cd ./ut-control
    git checkout ${UT_CONTROL_PROJECT_VERSION}
    ./configure.sh ${TARGET}
}

if [ -d "${UT_CONTROL_LIB_DIR}" ]; then
    echo "Framework ut-control already exists"
    # ut-control exists so run the makefile from ut
    check_ut_control_revision
    if [ -d "${THIRD_PARTY_LIB_DIR}" ]; then
        echo "Third party libraries are built for ${TARGET}"
    else
        echo "Third party libraries needs to be built for ${TARGET}"
        configure_ut_control
    fi
else
    if [ "$1" != "no_ut_control" ]; then
        echo "Clone ut_control in ${UT_CONTROL_LIB_DIR}"
        git clone ${TEST_REPO} ut-control
        check_ut_control_revision
        configure_ut_control
    else
        echo "$1 requested, hence ut-control is not required to be cloned"
    fi
fi
popd > /dev/null # ${FRAMEWORK_DIR}


CMAKE_BIN=${UT_CONTROL_LIB_DIR}/host-tools/CMake-3.30.0/build/bin/cmake
if [ ! -f "$CMAKE_BIN" ]; then
    CMAKE_BIN=$CMAKE_VAR
fi

pushd ${MY_DIR} > /dev/null
#Clone GTEST
if [[ ! -d "${GTEST_DIR}/googletest-1.15.2" && "${VARIANT}" == "CPP" ]]; then
    wget https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip --no-check-certificate -P ${GTEST_DIR}
    cd ${GTEST_DIR}/
    unzip v1.15.2.zip
    cd googletest-1.15.2/
    mkdir -p ${GTEST_LIB_DIR}
    cd ${GTEST_LIB_DIR}
    if command -v cmake &> /dev/null; then
        cmake ${GTEST_DIR}/googletest-1.15.2/
    else
        ${CMAKE_BIN} ${GTEST_DIR}/googletest-1.15.2/
    fi
    make
fi
popd > /dev/null # ${MY_DIR}
