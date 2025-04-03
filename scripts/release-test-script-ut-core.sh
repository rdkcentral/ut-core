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

#set -x

SCRIPT_EXEC="$(realpath $0)"
MY_DIR="$(dirname $SCRIPT_EXEC)"

# ANSI color codes
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function: usage
# Description: This function displays the correct usage of the script.
# Usage: usage
# Returns: None
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

# Function to handle errors and exit the script
# Usage: error_exit <error_message>
# Arguments:
#   error_message: The message to display before exiting the script
# Returns: None
# Example: error_exit "Failed to clone repository."
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
fi

# If repo_url is not passed by user set it to default
if [ -z "$REPO_URL" ]; then
    REPO_URL=git@github.com:rdkcentral/ut-core.git
fi
REPO_NAME=$(basename "$REPO_URL" .git)

# Function: run_git_clone
# Usage: run_git_clone <environment> <variant>
# Description: This function clones the git repository and changes to the repository directory.
# Parameters:
#   environment: The environment in which the repository is to be cloned.
#   variant: The variant of the repository (C or CPP).
# Returns: None
# Example: run_git_clone "ubuntu" "C"
run_git_clone(){
    local environment="$1"
    local variant="$2"
    echo "environment=$environment"
    echo "variant=$variant"

    # Git clone the branch of ut-control to carry out testing
    REPO_NAME=$(basename "$REPO_URL" .git)
    if [ ! -z "$UT_CORE_BRANCH_NAME" ]; then
        # Clone the repository
        echo -e "${YELLOW}Cloning repository from $REPO_URL and branch ${UT_CORE_BRANCH_NAME}${NC}"
        GIT_URL="git clone ${REPO_URL} -b ${UT_CORE_BRANCH_NAME} ut-core"
        git clone "$REPO_URL" -b "$UT_CORE_BRANCH_NAME" "$REPO_NAME-$environment-$variant" || error_exit "Error: Failed to clone repository."
        echo -e "${GREEN}GIT_URL = $GIT_URL${NC}"
    fi

    # Change to the repository directory
    echo -e "Current path is: $PWD"
    cd "$REPO_NAME-$environment-$variant" || error_exit "Error: Failed to change directory to $REPO_NAME-$environment-$variant."
    UT_CNTRL_DIR=${MY_DIR}/$REPO_NAME-$environment-$variant

    if [ ! -z "$UT_CONTROL_BRANCH_NAME" ]; then
        sed -i "s|git checkout .* # MARKER:.*|git checkout $UT_CONTROL_BRANCH_NAME # MARKER: Branch=$UT_CONTROL_BRANCH_NAME|" build.sh
        # the sed command ensures that the build.sh will check out the branch specified by the $UT_CONTROL_BRANCH_NAME variable during execution
        # instead of the tagged version of the ut-control repository
        # the line number 120 is the line number of the git checkout command in the build.sh file :
        # https://github.com/rdkcentral/ut-core/blob/develop/build.sh#L120
        # Also, the change only happens for the ut-core repository cloned for the test environment
        # It ensures, that the "said" ut-control branch is checked out during the build process
    fi
}

# Function: run_make_with_logs_for_C
# Description: This function runs the 'make' command and captures the logs for C Variant.
# Usage: Call this function to compile the C Variant and save the logs for later review.
# Parameters:
#   architecture_type: The architecture type of the environment.
# Returns: None
# Example: run_make_with_logs_for_C "linux"
run_make_with_logs_for_C() {
    local architecture_type="$1"
    echo "architecture_type=$architecture_type"
    UT_CORE_BIN="build/bin/hal_test"
    UT_CORE_TEST_BIN="tests/build/bin/ut-test"
    vername=$(echo "${PWD##*/}" | sed 's/ut-core-//')

    # Execute make and redirect the log to a file
    echo -e "${YELLOW}Running make ...logs redirected to $PWD/make_log.txt${NC}"
    make clean; make TARGET="$architecture_type" > make_log_C.txt 2>&1
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}Make command executed successfully for $vername .${NC}"
    else
        echo -e "${RED}Make command failed for $vername. Check the logs in make_log_C.txt${NC}"
    fi

    # Execute make -C tests/ and redirect the log to a file
    echo -e "${YELLOW}Running make -C tests/...logs redirected to $PWD/make_test_log.txt${NC}"
    make -C tests/ clean; make TARGET="$architecture_type" -C tests/ > make_test_log_C.txt 2>&1
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}Make -C tests/ command executed successfully for $vername.${NC}"
    else
        echo -e "${RED}Make -C tests/ command failed for $vername. Check the logs in make_test_log_C.txt${NC}"
    fi
}

# Function: run_make_with_logs_for_CPP
# Description: This function executes the 'make' command for a C++ Variant and logs the output.
# Usage: run_make_with_logs_for_CPP
# Parameters:
#   architecture_type: The architecture type of the environment.
# Returns: None
# Example: run_make_with_logs_for_CPP "linux"
run_make_with_logs_for_CPP() {
    local architecture_type="$1"
    echo "architecture_type=$architecture_type"
    UT_CORE_BIN="build/bin/hal_test"
    UT_CORE_TEST_BIN="tests/build/bin/ut-test"
    vername=$(echo "${PWD##*/}" | sed 's/ut-core-//')

    echo -e "${YELLOW}Running make for CPP...logs redirected to $PWD/make_cpp_log.txt${NC}"
    make clean; make TARGET="$architecture_type" VARIANT=CPP > make_cpp_log.txt 2>&1
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}Make command executed successfully for $vername.${NC}"
    else
        echo -e "${RED}Make command failed for $vername. Check the logs in make_cpp_log.txt${NC}"
    fi

    echo -e "${YELLOW}Running make -C tests/ for CPP...logs redirected to $PWD/make_cpp_log.txt${NC}"
    make TARGET="$architecture_type" -C tests/ VARIANT=CPP > make_test_cpp_log.txt 2>&1
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}Make -C tests/ command executed successfully for $vername.${NC}"
    else
        echo -e "${RED}Make -C tests/ command failed for $vername. Check the logs in make_test_cpp_log.txt${NC}"
    fi
}

export -f run_make_with_logs_for_C
export -f run_make_with_logs_for_CPP

# Description: This function validates the CURL static library for dunfell_linux.
validate_curl_dunfell_linux() {
    # We do not expect CURL static library to be rebuilt in dunfell_linux
    local curl_static_lib="$1"
    local environment="$2"

    if [[ "$environment" == "dunfell_linux" ]]; then
        if [ -f "$curl_static_lib" ]; then
            echo -e "${RED}$curl_static_lib exists. FAIL${NC}"
        else
            echo -e "${GREEN}CURL static lib does not exist. PASS${NC}"
        fi
    fi
}

# Description: This function validates the CURL static library for Ubuntu when libcurl.a is not in /usr/.
validate_curl_ubuntu_no_system_lib() {
    # We expect CURL static library to be rebuilt in ubuntu, as libcurl.a is not in /usr/
    local curl_static_lib="$1"
    local environment="$2"
    local system_curl_lib="$3"

    if [[ -z "$system_curl_lib" && "$environment" == "ubuntu" ]]; then
        if [ -f "$curl_static_lib" ]; then
            echo -e "${GREEN}$curl_static_lib exists. PASS${NC}"
        else
            echo -e "${RED}CURL static lib does not exist. FAIL${NC}"
        fi
    fi
}

# Description: This function validates the CURL static library for Ubuntu when libcurl.a is in /usr/.
validate_curl_ubuntu_with_system_lib() {
    # We do not expect CURL static library to be rebuilt in ubuntu, as libcurl.a is in /usr/
    local curl_static_lib="$1"
    local environment="$2"
    local system_curl_lib="$3"

    if [[ -n "$system_curl_lib" && "$environment" == "ubuntu" ]]; then
        if [ -f "$curl_static_lib" ]; then
            echo -e "${RED}$curl_static_lib exists. FAIL${NC}"
        else
            echo -e "${GREEN}CURL static lib does not exist. PASS${NC}"
        fi
    fi
}


# Description: This function validates the CURL static library for other platforms.
validate_curl_all_other_platforms() {
    # We expect CURL static library to be rebuilt in all other platforms
    local curl_static_lib="$1"
    local environment="$2"

    if [[ "$environment" != "dunfell_linux" && "$environment" != "ubuntu" ]]; then
        if [ -f "$curl_static_lib" ]; then
            echo -e "${GREEN}$curl_static_lib exists. PASS${NC}"
        else
            echo -e "${RED}CURL static lib does not exist. FAIL${NC}"
        fi
    fi
}


# Description: This function validates the CURL static library based on the environment.
validate_curl_library_created_correctly() {
    local environment="$1"
    local curl_static_lib="framework/ut-control/build/${architecture_type}/curl/lib/libcurl.a"
    local system_curl_lib

    system_curl_lib=$(find /usr/ -iname "libcurl.a" 2>/dev/null)

    # Validate and print results for CURL static library for dunfell_linux
    validate_curl_dunfell_linux "$curl_static_lib" "$environment"

    # Validate and print results for CURL static library for ubuntu when libcurl.a is not in /usr/
    validate_curl_ubuntu_no_system_lib "$curl_static_lib" "$environment" "$system_curl_lib"

    # Validate and print results for CURL static library for ubuntu when libcurl.a is in /usr/
    validate_curl_ubuntu_with_system_lib "$curl_static_lib" "$environment" "$system_curl_lib"

    # Validate and print results for CURL static library for all other platforms
    validate_curl_all_other_platforms "$curl_static_lib" "$environment"
}



# Description: This function performs a series of validations to ensure the build is successful or not.
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
    UT_CORE_BIN="build/bin/hal_test"
    UT_CORE_TEST_BIN="tests/build/bin/ut-test"

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
    validate_curl_library_created_correctly "$environment"

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
    elif [[ "$environment" == "kirkstone_arm" ]]; then
        if [ -f "$OPENSSL_STATIC_LIB" ]; then
            echo -e "${GREEN}$OPENSSL_STATIC_LIB exists. PASS ${NC}"
        else
            echo -e "${RED}Openssl static lib does not exist. FAIL ${NC}"
        fi
    elif [[ "$environment" == "kirkstone_linux" ]]; then
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
    elif [[ "$environment" == "kirkstone_arm" ]]; then
        if [ ! -f "$CMAKE_HOST_BIN" ]; then
            echo -e "${GREEN}CMake host binary does not exist. PASS ${NC}"
        else
            echo -e "${RED}CMake host binary exists. FAIL ${NC}"
        fi
    elif [[ "$environment" == "kirkstone_linux" ]]; then
        if [ ! -f "$CMAKE_HOST_BIN" ]; then
            echo -e "${GREEN}CMake host binary does not exist. PASS ${NC}"
        else
            echo -e "${RED}CMake host binary exists. FAIL ${NC}"
        fi
    fi

    if [ "$4" == "CPP" ]; then
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

        # Test for ut-core binary
        if [ -f "$UT_CORE_BIN" ]; then
            echo -e "${GREEN}$UT_CORE_BIN exists. PASS${NC}"
        else
            echo -e "${RED}UT_CORE_BIN does not exist. FAIL ${NC}"
        fi

        # Test for ut-core test binary
        if [ -f "$UT_CORE_TEST_BIN" ]; then
            echo -e "${GREEN}$UT_CORE_TEST_BIN exists. PASS${NC}"
        else
            echo -e "${RED}UT_CORE_TEST_BIN does not exist. FAIL ${NC}"
        fi
    fi

    if [ "$4" == "C" ]; then
        # Test for ut-core binary
        if [ -f "$UT_CORE_BIN" ]; then
            echo -e "${RED}$UT_CORE_BIN exists. FAIL${NC}"
        else
            echo -e "${GREEN}hal_test does not exist. PASS ${NC}"
        fi

        # Test for ut-core test binary
        if [ -f "$UT_CORE_TEST_BIN" ]; then
            echo -e "${GREEN}$UT_CORE_TEST_BIN exists. PASS${NC}"
        else
            echo -e "${RED}UT_CORE_TEST_BIN does not exist. FAIL ${NC}"
        fi
    fi

    echo -e "${RED}==========================================================${NC}"
}

# Function: print_results
# Description: This function prints the results of the release tests for the UT core.
# Usage: Call this function to display the results of the release tests for the UT core.
# Parameters:
#   variant: The variant of the UT core (C or CPP).
# Returns: None
# Example: print_results "C"
print_results() {

    if [ "$1" == "C" ]; then
        echo -e "${RED}==================== RESULTS FOR C VARIANT ====================${NC}"
        elif [ "$1" == "CPP" ]; then
        echo -e "${RED}==================== RESULTS FOR CPP VARIANT ====================${NC}"
    fi

    pushd ${MY_DIR} > /dev/null

    #Results for ubuntu
    PLAT_DIR="${REPO_NAME}-ubuntu-$1"
    pushd ${PLAT_DIR} > /dev/null
    run_checks "ubuntu" "linux" "$UT_CORE_BRANCH_NAME" $1
    popd > /dev/null

    #Results for VM_SYNC
    PLAT_DIR="${REPO_NAME}-VM-SYNC-$1"
    pushd ${PLAT_DIR} > /dev/null
    run_checks "VM-SYNC" "linux" $UT_CORE_BRANCH_NAME $1
    popd > /dev/null

    #Results for dunfell-arm
    PLAT_DIR="${REPO_NAME}-dunfell_arm-$1"
    pushd ${PLAT_DIR} > /dev/null
    run_checks "dunfell_arm" "arm" $UT_CORE_BRANCH_NAME $1
    popd > /dev/null

    #Results for dunfell-linux
    PLAT_DIR="${REPO_NAME}-dunfell_linux-$1"
    pushd ${PLAT_DIR} > /dev/null
    run_checks "dunfell_linux" "linux" $UT_CORE_BRANCH_NAME $1
    popd > /dev/null

    #Results for kirkstone-arm
    PLAT_DIR="${REPO_NAME}-kirkstone_arm-$1"
    pushd ${PLAT_DIR} > /dev/null
    run_checks "kirkstone_arm" "arm" $UT_CORE_BRANCH_NAME $1
    popd > /dev/null

    #Results for kirkstone-linux
    PLAT_DIR="${REPO_NAME}-kirkstone_linux-$1"
    pushd ${PLAT_DIR} > /dev/null
    run_checks "kirkstone_linux" "linux" $UT_CORE_BRANCH_NAME $1
    popd > /dev/null

    popd > /dev/null

}

# Function: run_make_with_logs
# Description: This function selects the appropriate make command based on the variant and logs the output.
# Usage: run_make_with_logs <variant>
# Parameters:
#   variant: The variant of the UT core (C or CPP).
# Returns: None
# Example: run_make_with_logs "C", run_make_with_logs "CPP"
run_make_with_logs() {
    if [ "$1" == "C" ]; then
        run_make_with_logs_for_C "$2"
    elif [ "$1" == "CPP" ]; then
        run_make_with_logs_for_CPP "$2"
    else
        echo "Error: Invalid argument. Use 'C' or 'CPP'."
        exit 1
    fi
}

# Environment-specific setups and execution
export -f run_make_with_logs_for_C
export -f run_make_with_logs_for_CPP
export -f run_make_with_logs
export -f run_checks
export -f usage
export -f error_exit

# Function: run_on_ubuntu_linux
# Description: This function git clones and builds ut-core on an Ubuntu Linux environment.
# Usage: Call this function within the script to execute the defined operations on Ubuntu Linux.
# Parameters:
#   variant: The variant of the UT core (C or CPP).
# Returns: None
# Example: run_on_ubuntu_linux "C"
run_on_ubuntu_linux() {
    pushd ${MY_DIR} > /dev/null
    run_git_clone "ubuntu" $1
    run_make_with_logs $1 "linux"
    run_checks "ubuntu" "linux" "$UT_CORE_BRANCH_NAME"
    popd > /dev/null
}

# Description: This function git clones and builds ut-core on a specified platform.
run_on_platform() {
    local PLATFORM=$1
    local TARGET=$2
    local VARIANT_FLAG=$3
    local LOG_SUFFIX=$3

    pushd ${MY_DIR} > /dev/null
    run_git_clone "${PLATFORM}_${TARGET}" "${VARIANT_FLAG}"

    echo "Running make for ${PLATFORM}_${TARGET} (${VARIANT_FLAG}) variant"

    local DOCKER_IMAGE="rdk-${PLATFORM}"
    [[ "$PLATFORM" == "VM-SYNC" ]] && DOCKER_IMAGE="vm-sync"

    local TARGET_FLAG=""
    [[ "$TARGET" == "arm" ]] && TARGET_FLAG="TARGET=arm"

    local ENV_SETUP=""
    if [[ "$TARGET" == "arm" ]]; then
        if [[ "$PLATFORM" == "dunfell" ]]; then
            # For Dunfell ARM
            ENV_SETUP='[ -z "$OECORE_TARGET_OS" ] && source /opt/toolchains/rdk-glibc-x86_64-arm-toolchain/environment-setup-armv7at2hf-neon-oe-linux-gnueabi;'
        else
            # For Kirkstone ARM
            ENV_SETUP='[ -z "$OECORE_TARGET_OS" ] && source /opt/toolchains/rdk-glibc-x86_64-arm-toolchain/environment-setup-armv7vet2hf-neon-oe-linux-gnueabi;'
        fi
    fi

    /bin/bash -c "sc docker run ${DOCKER_IMAGE} \
    '${ENV_SETUP} echo \$CC; \
    make ${TARGET_FLAG} VARIANT=${VARIANT_FLAG} > make_log_${LOG_SUFFIX}.txt 2>&1; \
    make -C tests/ ${TARGET_FLAG} VARIANT=${VARIANT_FLAG} > make_test_log_${LOG_SUFFIX}.txt 2>&1'"

    run_checks "${PLATFORM}_${TARGET}" "${TARGET}" $UT_CORE_BRANCH_NAME
    popd > /dev/null
}

# Run tests in different environments
run_on_ubuntu_linux "C"
run_on_platform "dunfell" "linux" "C"
run_on_platform "kirkstone" "linux" "C"
run_on_platform "VM-SYNC" "linux" "C"
run_on_platform "dunfell" "arm" "C"
run_on_platform "kirkstone" "arm" "C"

run_on_ubuntu_linux "CPP"
run_on_platform "dunfell" "linux" "CPP"
run_on_platform "kirkstone" "linux" "CPP"
un_on_platform "VM-SYNC" "linux" "CPP"
run_on_platform "dunfell" "arm" "CPP"
run_on_platform "kirkstone" "arm" "CPP"

# Print the results for C and CPP variants
print_results "C"
print_results "CPP"
