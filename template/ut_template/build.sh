#!/usr/bin/env bash

# Clone the Unit Test Requirements
TEST_REPO=git@github.com:comcast-sky/rdk-components-ut-core.git

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
