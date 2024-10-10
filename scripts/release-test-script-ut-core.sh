#!/bin/bash

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

# ANSI color codes
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to print usage
usage() {
    echo -e "${YELLOW}Usage: $0 -u <REPO_URL> -t <UT_CORE_BRANCH_NAME> -c <UT_CONTROL_BRANCH_NAME>${NC}"
    exit 1
}

# Parse command-line arguments
while getopts "u:t:c:" opt; do
    case $opt in
        u) REPO_URL="$OPTARG" ;;
        t) UT_CORE_BRANCH_NAME="$OPTARG" ;;
        c) UT_CONTROL_BRANCH_NAME="$OPTARG" ;;
        *) usage ;;
    esac
done

# Function to print error message and exit
error_exit() {
    echo -e "${RED}$1${NC}" 1>&2
    exit 1
}

echo "UT_CORE_BRANCH_NAME = $UT_CORE_BRANCH_NAME"
# Check if the branch name is passed else display the usage
if [ -z "$UT_CORE_BRANCH_NAME" ]; then
    echo "UT_CORE_BRANCH_NAME is empty"
    usage
    exit 1
fi

echo "UT_CONTROL_BRANCH_NAME = $UT_CONTROL_BRANCH_NAME"
# Check if the branch name is passed else display the usage
if [ -z "$UT_CONTROL_BRANCH_NAME" ]; then
    echo "UT_CONTROL_BRANCH_NAME is empty"
    usage
    #exit 1
fi

# If repo_url is not passed by user set it to default
if [ -z "$REPO_URL" ]; then
    REPO_URL=git@github.com:rdkcentral/ut-core.git
fi
REPO_NAME=$(basename "$REPO_URL" .git)

# # Set compiler type based on the environment passed
# case "$environment" in
#     "ubuntu" | "VM-SYNC" | "dunfell_linux") architecture_type="linux" ;;
#     "dunfell_arm") architecture_type="arm" ;;
#     *)
#         echo "Unknown environment: $environment"
#         echo "Environments are : ubuntu, VM-SYNC, dunfell_arm, dunfell_linux"
#         exit 1
#         ;;
# esac

#git clone and change dir
run_git_clone(){
    local environment="$1"
    echo "environment=$environment"

    # Git clone the branch of ut-control to carry out testing
    REPO_NAME=$(basename "$REPO_URL" .git)
    if [ ! -z "$UT_CORE_BRANCH_NAME" ]; then
        # Clone the repository
        echo -e "${YELLOW}Cloning repository from $REPO_URL and branch ${UT_CORE_BRANCH_NAME}${NC}"
        GIT_URL="git clone ${REPO_URL} -b ${UT_CORE_BRANCH_NAME} ut-core"
        git clone "$REPO_URL" -b "$UT_CORE_BRANCH_NAME" "$REPO_NAME-$environment" || error_exit "Error: Failed to clone repository."
        echo -e "${GREEN}GIT_URL = $GIT_URL${NC}"
    fi
    
    # Change to the repository directory
    echo -e "Current path is: $PWD"
    cd "$REPO_NAME-$environment" || error_exit "Error: Failed to change directory to $REPO_NAME-$environment."
    UT_CNTRL_DIR=${MY_DIR}/$REPO_NAME-$environment

    if [ ! -z "$UT_CONTROL_BRANCH_NAME" ]; then
        sed -i "108s|.*|    git checkout $UT_CONTROL_BRANCH_NAME|" build.sh
    fi
}

# Define a function to run the commands
run_make_with_logs() {
    local architecture_type="$1"
    echo "architecture_type=$architecture_type"

    # Execute make and redirect the log to a file
    echo -e "${YELLOW}Running make ...logs redirected to $PWD/make_log.txt${NC}"
    make TARGET="$architecture_type" > make_log.txt 2>&1
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}Make command executed successfully.${NC}"
    else
        echo -e "Make command failed. Check the logs in make_log.txt"
    fi

    echo -e "${YELLOW}Running make for CPP...logs redirected to $PWD/make_cpp_log.txt${NC}"
    make clean; make TARGET="$architecture_type" VARIANT=CPP > make_cpp_log.txt 2>&1
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}Make command executed successfully.${NC}"
    else
        echo -e "Make command failed. Check the logs in make_cpp_log.txt"
    fi

    # Execute make -C tests/ and redirect the log to a file
    echo -e "${YELLOW}Running make -C tests/...logs redirected to $PWD/make_test_log.txt${NC}"
    make TARGET="$architecture_type" -C tests/ > make_test_log.txt 2>&1
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}Make -C tests/ command executed successfully.${NC}"
    else
        echo -e "Make -C tests/ command failed. Check the logs in make_test_log.txt"
    fi

    echo -e "${YELLOW}Running make -C tests/ for CPP...logs redirected to $PWD/make_cpp_log.txt${NC}"
    make -C tests/ clean;make TARGET="$architecture_type" -C tests/ VARIANT=CPP > make_cpp_log.txt 2>&1
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}Make -C tests/ command executed successfully.${NC}"
    else
        echo -e "Make -C tests/ command failed. Check the logs in make_cpp_log.txt"
    fi
}

run_checks() {
    # Parameters to be passed to the function
    environment=$1
    architecture_type=$2
    UT_CORE_BRANCH_NAME=$3

    echo -e "${RED}==========================================================${NC}"
    # Variables to test existence of different librararies
    CURL_STATIC_LIB="framework/ut-control/build/${architecture_type}/curl/lib/libcurl.a"
    OPENSSL_STATIC_LIB="framework/ut-control/build/${architecture_type}/openssl/lib/libssl.a"
    CMAKE_HOST_BIN="framework/ut-control/host-tools/CMake-3.30.0/build/bin/cmake"
    GTEST_STATIC_LIB="build/${architecture_type}/cpp_libs/lib/libgtest.a"
    GTEST_MAIN_STATIC_LIB="build/${architecture_type}/cpp_libs/lib/libgtest_main.a"

    echo -e "${RED}RESULTS for ${environment} ${NC}"

    current_branch=$(git branch | grep '\*' | sed 's/* //')

    # Compare with the target branch name
    if [ "${current_branch}" == "${UT_CORE_BRANCH_NAME}" ]; then
        echo -e "${GREEN}On the branch $UT_CORE_BRANCH_NAME
. PASS${NC}"
    else
        error_exit "Branch is not switched. FAIL"
    fi

    # Test for CURL static library
    if [ -f "$CURL_STATIC_LIB" ]; then
        echo -e "${GREEN}$CURL_STATIC_LIB exists. PASS${NC}"
    else
        echo -e "${RED}CURL static lib does not exist. FAIL ${NC}"
    fi

    # Test for OpenSSL static library
    if [[ "$environment" == "ubuntu" ]]; then
        if [ ! -f "$OPENSSL_STATIC_LIB" ]; then
            echo -e "${GREEN}Openssl static lib does not exist. PASS ${NC}"
        else
            echo -e "${RED}Openssl static lib exists. FAIL ${NC}"
        fi
    elif [[ "$environment" == "VM-SYNC" ]]; then
        if [ -f "$OPENSSL_STATIC_LIB" ]; then
            echo -e "${GREEN}$OPENSSL_STATIC_LIB exists. PASS ${NC}"
        else
            echo -e "${RED}Openssl static lib does not exist. FAIL ${NC}"
        fi
    elif [[ "$environment" == "dunfell_arm" ]]; then
        if [ -f "$OPENSSL_STATIC_LIB" ]; then
            echo -e "${GREEN}$OPENSSL_STATIC_LIB exists. PASS ${NC}"
        else
            echo -e "${RED}Openssl static lib does not exist. FAIL ${NC}"
        fi
    elif [[ "$environment" == "dunfell_linux" ]]; then
        if [ -f "$OPENSSL_STATIC_LIB" ]; then
            echo -e "${RED}$OPENSSL_STATIC_LIB exists. FAIL ${NC}"
        else
            echo -e "${GREEN}Openssl static lib does not exist. PASS ${NC}"
        fi
    fi

    # Test for CMAKE host binary
    if [[ "$environment" == "ubuntu" ]]; then
        if [ ! -f "$CMAKE_HOST_BIN" ]; then
            echo -e "${GREEN}CMake host binary does not exist. PASS ${NC}"
        else
            echo -e "${RED}CMake host binary exists. FAIL ${NC}"
        fi
    elif [[ "$environment" == "VM-SYNC" ]]; then
        if [ -f "$CMAKE_HOST_BIN" ]; then
            echo -e "${GREEN}$CMAKE_HOST_BIN exists. PASS ${NC}"
        else
            echo -e "${RED}CMake host binary does not exist. FAIL ${NC}"
        fi
    elif [[ "$environment" == "dunfell_arm" ]]; then
        if [ ! -f "$CMAKE_HOST_BIN" ]; then
            echo -e "${GREEN}CMake host binary does not exist. PASS ${NC}"
        else
            echo -e "${RED}CMake host binary exists. FAIL ${NC}"
        fi
    elif [[ "$environment" == "dunfell_linux" ]]; then
        if [ ! -f "$CMAKE_HOST_BIN" ]; then
            echo -e "${GREEN}CMake host binary does not exist. PASS ${NC}"
        else
            echo -e "${RED}CMake host binary exists. FAIL ${NC}"
        fi
    fi

    # Test for gtest static library
    if [ -f "$GTEST_STATIC_LIB" ]; then
        echo -e "${GREEN}$GTEST_STATIC_LIB exists. PASS${NC}"
    else
        echo -e "${RED}GTEST static lib does not exist. FAIL ${NC}"
    fi

    # Test for gtest main static library
    if [ -f "$GTEST_MAIN_STATIC_LIB" ]; then
        echo -e "${GREEN}$GTEST_MAIN_STATIC_LIB exists. PASS${NC}"
    else
        echo -e "${RED}GTEST main static lib does not exist. FAIL ${NC}"
    fi

    echo -e "${RED}==========================================================${NC}"
}

print_results() {
    pushd ${MY_DIR} > /dev/null
    
    #Results for ubuntu
    PLAT_DIR="${REPO_NAME}-ubuntu"
    pushd ${PLAT_DIR} > /dev/null
    run_checks "ubuntu" "linux" "$UT_CORE_BRANCH_NAME"
    popd > /dev/null
    
    #Results for VM_SYNC
    PLAT_DIR="${REPO_NAME}-VM-SYNC"
    pushd ${PLAT_DIR} > /dev/null
    run_checks "VM-SYNC" "linux" $UT_CORE_BRANCH_NAME
    popd > /dev/null
    
    #Results for dunfell-arm
    PLAT_DIR="${REPO_NAME}-dunfell_arm"
    pushd ${PLAT_DIR} > /dev/null
    run_checks "dunfell_arm" "arm" $UT_CORE_BRANCH_NAME
    popd > /dev/null
    
    #Results for dunfell-linux
    PLAT_DIR="${REPO_NAME}-dunfell_linux"
    pushd ${PLAT_DIR} > /dev/null
    run_checks "dunfell_linux" "linux" $UT_CORE_BRANCH_NAME
    popd > /dev/null
    
    popd > /dev/null
    
}

# Environment-specific setups and execution
export -f run_make_with_logs
export -f run_checks
export -f usage
export -f error_exit

run_on_ubuntu_linux() {
    pushd ${MY_DIR} > /dev/null
    run_git_clone "ubuntu"
    run_make_with_logs "linux"
    run_checks "ubuntu" "linux" "$UT_CORE_BRANCH_NAME"
    popd > /dev/null
}

run_on_dunfell_linux() {
    pushd ${MY_DIR} > /dev/null
    SETUP_ENV="sc docker run rdk-dunfell"
    run_git_clone "dunfell_linux"
    /bin/bash -c "$SETUP_ENV; $(declare -f run_make_with_logs); run_make_with_logs 'linux'"
    run_checks "dunfell_linux" "linux" $UT_CORE_BRANCH_NAME
    popd > /dev/null
}

run_on_dunfell_arm() {
    pushd ${MY_DIR} > /dev/null
    run_git_clone "dunfell_arm"
    /bin/bash -c "sc docker run rdk-dunfell 'cd /opt/toolchains/rdk-glibc-x86_64-arm-toolchain; \
     . environment-setup-armv7at2hf-neon-oe-linux-gnueabi; env | grep CC; cd -; \
     $(declare -f run_make_with_logs); run_make_with_logs 'arm';exit'"
    run_checks "dunfell_arm" "arm" $UT_CORE_BRANCH_NAME
    popd > /dev/null
}

run_on_vm_sync_linux() {
    pushd ${MY_DIR} > /dev/null
    SETUP_ENV="sc docker run vm-sync"
    run_git_clone "VM-SYNC"
    /bin/bash -c "$SETUP_ENV '$(declare -f run_make_with_logs); run_make_with_logs 'linux';'"
    run_checks "VM-SYNC" "linux" $UT_CORE_BRANCH_NAME
    popd > /dev/null
}

# Run tests in different environments
#( run_on_ubuntu_linux ) &
#( run_on_dunfell_linux ) &
#wait
#run_on_vm_sync_linux
#run_on_dunfell_arm
print_results
