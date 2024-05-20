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

pushd ${MY_DIR} > /dev/null

FRAMEWORK_DIR=${MY_DIR}/framework
LIBYAML_DIR=${FRAMEWORK_DIR}/libfyaml-master
ASPRINTF_DIR=${FRAMEWORK_DIR}/asprintf
LIBWEBSOCKETS_DIR="${FRAMEWORK_DIR}/libwebsockets-4.3.3"

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

if [ -d "${LIBYAML_DIR}" ]; then
    echo "Framework libyaml already exists"
else
    pushd ${FRAMEWORK_DIR} > /dev/null
    echo "Clone libfyaml in ${LIBYAML_DIR}"
    wget https://github.com/pantoniou/libfyaml/archive/refs/heads/master.zip --no-check-certificate
    unzip master.zip

    echo "Patching Framework [${PWD}]"
    cp ../src/libyaml/patches/CorrectWarningsAndBuildIssuesInLibYaml.patch  .
    patch -i CorrectWarningsAndBuildIssuesInLibYaml.patch -p0
    echo "Patching Complete"

#    ./bootstrap.sh
#    ./configure --prefix=${LIBYAML_DIR}
#    make
    popd > /dev/null
fi

pushd ${FRAMEWORK_DIR} > /dev/null
if [ -d "${ASPRINTF_DIR}" ]; then
    echo "Framework libyaml already exists"
else
    echo "Clone asprintf in ${ASPRINTF_DIR}"
    wget https://github.com/jwerle/asprintf.c/archive/refs/heads/master.zip -P asprintf/. --no-check-certificate
    cd asprintf
    unzip master.zip
    rm asprintf.c-master/test.c
fi

pushd ${FRAMEWORK_DIR} > /dev/null
if [ -d "${LIBWEBSOCKETS_DIR}" ]; then
    echo "Framework libyaml already exists"
else
    pushd ${MY_DIR}/framework > /dev/null
    echo "Clone libwebsockets in ${LIBWEBSOCKETS_DIR}"
    wget https://github.com/warmcat/libwebsockets/archive/refs/tags/v4.3.3.zip --no-check-certificate
    unzip v4.3.3.zip
fi
popd > /dev/null # ${FRAMEWORK_DIR}
