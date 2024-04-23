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

SCRIPT_EXEC="$(realpath $0)"
MY_DIR="$(dirname $SCRIPT_EXEC)"

pushd ${MY_DIR} > /dev/null

FRAMEWORK_DIR=${MY_DIR}/framework
LIBYAML_DIR=${FRAMEWORK_DIR}/libfyaml-master

# Clone CUnit
if [ -d "./framework/" ]; then
    echo "Framework CUnit already exists"
else
    echo "Clone Framework"
    wget https://sourceforge.net/projects/cunit/files/CUnit/2.1-3/CUnit-2.1-3.tar.bz2 --no-check-certificate -P framework/
    tar xvfj framework/CUnit-2.1-3.tar.bz2 -C ./framework/
    cp framework/CUnit-2.1-3/CUnit/Headers/CUnit.h.in framework/CUnit-2.1-3/CUnit/Headers/CUnit.h
    cp src/cunit/cunit_lgpl/patches/CorrectBuildWarningsInCunit.patch  framework/.
    cd framework/
    echo "Patching Framework"
    patch -u CUnit-2.1-3/CUnit/Sources/Framework/TestRun.c -i ../patches/CorrectBuildWarningsInCunit.patch
    echo "Patching Complete"
fi

if [ -d "${LIBYAML_DIR}" ]; then
    echo "Framework libyml already exists"
else
    pushd ${FRAMEWORK_DIR} > /dev/null
    echo "Clone libfyaml in ${LIBYAML_DIR}"
    wget https://github.com/pantoniou/libfyaml/archive/refs/heads/master.zip --no-check-certificate
    unzip master.zip

    echo "Patching Framework"
    patch -u . -i ../../patches/CorrectBuildIssuesInLibyaml.patch
    echo "Patching Complete"

#    ./bootstrap.sh
#    ./configure --prefix=${LIBYAML_DIR}
#    make
    popd > /dev/null
fi
popd > /dev/null
