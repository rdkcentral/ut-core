#!/usr/bin/env bash

# Clone the Unit Testing Suit for this repo, it follows a standard convention
# HAL Name is replaced with haltest, allowing this script to be part of the template for all
TEST_REPO=$(git remote -vv | head -n1 | awk -F ' ' '{print $2}' | sed 's/hal/haltest/g')

# This will look up the last tag in the git repo, depending on the project this may require modification
PROJECT_VERSION=feature/RDKB-43403_v4_hal_creation

UT_DIR="./ut"

# Simple help
if [ "${1}" == "-h" ]; then
    echo "Script to build the unit testing suite"
    echo " build_ut.sh <clean> - clean the testing"
    echo " build_ut.sh TARGET=xxx - build the xxx version of the tests, linux/arm etc."
    echo " build_ut.sh <noswitch> - build the linux version of tests using skeleton & stubs"
    exit 0
fi

# Check if the common document configuration is present, if not clone it
if [ -d ${UT_DIR} ]; then
    pushd ${UT_DIR} > /dev/null
    ./build.sh $@
    popd > /dev/null
else
    echo "Cloning Hal Test Suite for this module"
    git clone ${TEST_REPO} hts
    pushd ${UT_DIR} > /dev/null
    git flow init -d
    git checkout ${PROJECT_VERSION}
    popd > /dev/null
    ${0}
fi
