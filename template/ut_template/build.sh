#!/usr/bin/env bash

# Clone the Unit Test Requirements
TEST_REPO=git@github.com:comcast-sky/rdk-components-ut-core.git

# This will look up the last tag in the git repo, depending on the project this may require modification
PROJECT_VERSION=master

# Check if the common document configuration is present, if not clone it
if [ -d "./ut-core" ]; then
    make -C . -f Makefile $@
else
    echo "Cloning Unit Test Core System"
    git clone ${TEST_REPO} ut-core
    cd ./ut-core
    git flow init -d
    git checkout ${PROJECT_VERSION}
    ./build.sh
    cd ..
    ./${0}
fi
