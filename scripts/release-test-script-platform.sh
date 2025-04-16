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
    #exit 1
fi

echo "UT_CONTROL_BRANCH_NAME = $UT_CONTROL_BRANCH_NAME"
# Check if the branch name is passed else display the usage
if [ -z "$UT_CONTROL_BRANCH_NAME" ]; then
    echo "UT_CONTROL_BRANCH_NAME is empty"
fi

# If repo_url is not passed by user set it to default
if [ -z "$REPO_URL" ]; then
    REPO_URL=git@github.com:rdkcentral/rdk-halif-hdmi_cec.git
fi

#git clone and change dir
run_git_clone(){
    local environment="$1"
    local target="$2"
    echo "environment=$environment"
    echo "target=$target"
    
    # Git clone the branch of ut-control to carry out testing
    REPO_NAME=$(basename "$REPO_URL" .git)
    # Clone the repository
    echo -e "${YELLOW}Cloning repository from $REPO_URL${NC}"
    
    PLAT_DIR=$REPO_NAME-$environment
    git clone "$REPO_URL" $PLAT_DIR || error_exit "Error: Failed to clone repository."
    GIT_URL="    git clone ${Platform_REPO_URL} $PLAT_DIR"
    echo -e "${GREEN}GIT_URL = $GIT_URL${NC}"
    
    sleep 5
    
}

# Function to run the build process
run_build() {
    local environment="$1"
    local target="$2"
    local UT_CORE_BRANCH_NAME="$3"
    local UT_CONTROL_BRANCH_NAME="$4"

    echo "environment=$environment"
    echo "target=$target"
    echo "UT_CORE_BRANCH_NAME=$UT_CORE_BRANCH_NAME"
    echo "REPO_NAME-$environment=$REPO_NAME-$environment"

    error_exit() {
        echo "[ERROR] $1"
        exit 1
    }

    if [ ! -z "$UT_CORE_BRANCH_NAME" ]; then
        cd $REPO_NAME-$environment
        echo "Directory: $PWD"
        # Just run it in background directly
        ./build_ut.sh TARGET=$target > $PWD/build_log_temp.txt 2>&1 &
        command_pid=$!
        echo "Started command with PID $command_pid"
        sleep 10
        kill -9 "$command_pid" || echo "Warning: Could not kill process $command_pid (might have finished)"
        sleep 5

        if [ ! -z "$UT_CONTROL_BRANCH_NAME" ]; then
            cd ut/ut-core || error_exit "ut-core not found"
            git checkout "$UT_CORE_BRANCH_NAME"
            git pull
            cd -

            cd ut/ut-core/framework/ut-control || error_exit "ut-control not found"
            git checkout "$UT_CONTROL_BRANCH_NAME"
            git pull
            cd -

            rm -rf ut/build/ ut/ut-core/framework/ut-control/build/ ut/ut-core/framework/ut-control/framework/ ut/ut-core/framework/ut-control/host-tools/
        else
            echo "UT_CORE_BRANCH_NAME = $UT_CORE_BRANCH_NAME"
            sed -i "s| git checkout .*|    git checkout $UT_CORE_BRANCH_NAME # MARKER: Branch=$UT_CORE_BRANCH_NAME|" ut/build.sh || error_exit "sed failed"
            sleep 5
            chmod -R +w ut/ut-core 2>/dev/null

            # Retry mechanism for stubborn directories
            MAX_ATTEMPTS=5
            ATTEMPT=1
            while [ -d "ut/ut-core" ] && [ $ATTEMPT -le $MAX_ATTEMPTS ]; do
                rm -rf ut/ut-core 2>/dev/null
                if [ -d "ut/ut-core" ]; then
                    echo "[WARN] ut-core not fully removed. Retrying ($ATTEMPT/$MAX_ATTEMPTS)..."
                    sleep 1
                fi
                ATTEMPT=$((ATTEMPT+1))
            done

            # Final check
            if [ -d "ut/ut-core" ]; then
                echo "[ERROR] Failed to remove ut-core after $MAX_ATTEMPTS attempts."
                exit 1
            fi

            sleep 5
        fi
    fi

    echo "You may also tail the output in another shell using:"
    echo "tail -f 100 $PWD/build_log.txt"

    if [ ! -f ./build_ut.sh ]; then
        error_exit "./build_ut.sh not found"
    fi

    if [ "$environment" = "ubuntu" ] && [ "$target" = "linux" ]; then
        ./build_ut.sh TARGET="$target" | tee "$PWD/build_log.txt" 2>&1
        if [ $? -eq 0 ]; then
            echo "Build command executed successfully."
        else
            echo "[ERROR] Build command failed. Check build_log.txt"
        fi
    fi
}

# Description: Check if CURL static library exists based on environment
validate_curl_library_dunfell_linux() {
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

# Description: Check if CURL static library exists based on environment
validate_curl_library_ubuntu_no_system_lib() {
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

# Description: Check if CURL static library exists based on environment
validate_curl_library_ubuntu_with_system_lib() {
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

# Description: Check if CURL static library exists based on environment
validate_curl_library_other_platforms() {
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

# Description: Validates if CURL static library exists based on environment
validate_curl_library_created_correctly() {
    local environment="$1"
    local curl_static_lib="$2"
    local system_curl_lib

    system_curl_lib=$(find /usr/ -iname "libcurl.a" 2>/dev/null)

    # Validate and print results for CURL static library for dunfell_linux
    validate_curl_library_dunfell_linux "$curl_static_lib" "$environment"

    # Validate and print results for CURL static library for ubuntu when libcurl.a is not in /usr/
    validate_curl_library_ubuntu_no_system_lib "$curl_static_lib" "$environment" "$system_curl_lib"

    # Validate and print results for CURL static library for ubuntu when libcurl.a is in /usr/
    validate_curl_library_ubuntu_with_system_lib "$curl_static_lib" "$environment" "$system_curl_lib"

    # Validate and print results for CURL static library for other platforms
    validate_curl_library_other_platforms "$curl_static_lib" "$environment"
}

# This function runs the checks for the build process
run_checks() {
    # Parameters to be passed to the function
    environment=$1
    architecture_type=$2
    UT_CORE_BRANCH_NAME=$3
    UT_CONTROL_BRANCH_NAME=$4
    
    echo -e "${RED}==========================================================${NC}"
    # Variables to test existence of different librararies
    CURL_STATIC_LIB="ut/ut-core/framework/ut-control/build/${architecture_type}/curl/lib/libcurl.a"
    OPENSSL_STATIC_LIB="ut/ut-core/framework/ut-control/build/${architecture_type}/openssl/lib/libssl.a"
    CMAKE_HOST_BIN="ut/ut-core/framework/ut-control/host-tools/CMake-3.30.0/build/bin/cmake"
    HAL_BIN=$(find ut/bin/ -iname "hal_test*" 2>/dev/null)
    # CPP_STATIC_LIB="build/${architecture_type}/cpp_libs/lib/libgtest.a"
    # CPP_MAIN_STATIC_LIB="build/${architecture_type}/cpp_libs/lib/libgtest_main.a"

    # Search for libcurl.a in /usr/
    SYSTEM_CURL_LIB=$(find /usr/ -iname "libcurl.a" 2>/dev/null)
    
    echo -e "${RED}RESULTS for ${environment} ${NC}"
    
    if [ ! -z "$UT_CORE_BRANCH_NAME" ]; then
        cd ut/ut-core/
        
        current_branch=$(git branch | grep '\*' | sed 's/* //')
        
        # Compare with the target branch name
        if [ "${current_branch}" == "${UT_CORE_BRANCH_NAME}" ]; then
            echo -e "${GREEN}On the branch $UT_CORE_BRANCH_NAME
            . PASS${NC}"
        else
            echo -e "${RED}Branch is not switched. FAIL${NC}"
        fi
        
        cd -
    fi

    if [ ! -z "$UT_CONTROL_BRANCH_NAME" ]; then
        cd ut/ut-core/framework/ut-control/

        current_branch=$(git branch | grep '\*' | sed 's/* //')

        # Compare with the target branch name
        if [ "${current_branch}" == "${UT_CONTROL_BRANCH_NAME}" ]; then
            echo -e "${GREEN}On the branch $UT_CONTROL_BRANCH_NAME
            . PASS${NC}"
        else
            echo -e "${RED}Branch is not switched. FAIL${NC}"
        fi

        cd -
    fi
    
    # Test for HAL binary
    if [ -f *"$HAL_BIN"* ]; then
        echo -e "${GREEN}$HAL_BIN exists. PASS${NC}"
    else
        echo -e "${RED}Hal binary does not exist. FAIL ${NC}"
    fi
    
    # Test for CURL static library
    validate_curl_library_created_correctly "$environment" "$CURL_STATIC_LIB"
    
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
    
    # # Test for CPP static library
    # if [ -f "$CPP_STATIC_LIB" ]; then
    #     echo -e "${GREEN}$CPP_STATIC_LIB exists. PASS${NC}"
    # else
    #     echo -e "${RED}CPP static lib does not exist. FAIL ${NC}"
    # fi
    
    # # Test for CPP main static library
    # if [ -f "$CPP_MAIN_STATIC_LIB" ]; then
    #     echo -e "${GREEN}$CPP_MAIN_STATIC_LIB exists. PASS${NC}"
    # else
    #     echo -e "${RED}CPP main static lib does not exist. FAIL ${NC}"
    # fi
    
    echo -e "${RED}==========================================================${NC}"
}

# Environment-specific setups and execution
export -f run_git_clone
export -f run_checks
export -f run_build
export -f usage
export -f error_exit

REPO_NAME=$(basename "$REPO_URL" .git)

run_on_ubuntu_linux() {
    
    pushd ${MY_DIR} > /dev/null
    run_git_clone "ubuntu" "linux"
    # Change to the repository directory
    PLAT_DIR="${REPO_NAME}-ubuntu"
    pushd ${PLAT_DIR} > /dev/null
    run_build "ubuntu" "linux" "$UT_CORE_BRANCH_NAME" "$UT_CONTROL_BRANCH_NAME"
    run_checks "ubuntu" "linux" "$UT_CORE_BRANCH_NAME" "$UT_CONTROL_BRANCH_NAME"
    popd > /dev/null
    popd > /dev/null
}



# Description: This function git clones and builds ut-core on a specified platform.
run_on_platform() {
    local PLATFORM=$1
    local TARGET=$2
    local VARIANT_FLAG=$3
    local LOG_SUFFIX=$3

    pushd "${MY_DIR}" > /dev/null
    run_git_clone "${PLATFORM}_${TARGET}" "${VARIANT_FLAG}"

    echo "Running make for ${PLATFORM}_${TARGET} (${VARIANT_FLAG}) variant"

    local DOCKER_IMAGE="rdk-${PLATFORM}"
    [[ "$PLATFORM" == "VM-SYNC" ]] && DOCKER_IMAGE="vm-sync"

    local TARGET_FLAG=""
    [[ "$TARGET" == "arm" ]] && TARGET_FLAG="TARGET=arm"

    local ENV_SETUP=""
    if [[ "$TARGET" == "arm" ]]; then
        if [[ "$PLATFORM" == "dunfell" ]]; then
            ENV_SETUP='[ -z "$OECORE_TARGET_OS" ] && source /opt/toolchains/rdk-glibc-x86_64-arm-toolchain/environment-setup-armv7at2hf-neon-oe-linux-gnueabi;'
        else
            ENV_SETUP='[ -z "$OECORE_TARGET_OS" ] && source /opt/toolchains/rdk-glibc-x86_64-arm-toolchain/environment-setup-armv7vet2hf-neon-oe-linux-gnueabi;'
        fi
    fi

    run_build "${PLATFORM}_${TARGET}" "${TARGET}" "$UT_CORE_BRANCH_NAME" "$UT_CONTROL_BRANCH_NAME"

    /bin/bash -c "sc docker run ${DOCKER_IMAGE} \
    '${ENV_SETUP} echo \$CC; \
    ./build_ut.sh TARGET=${TARGET} | tee \$PWD/build_log.txt 2>&1; \
    if [ \$? -eq 0 ]; then \
        echo \"Build command executed successfully.\"; \
    else \
        echo \"[ERROR] Build command failed. Check build_log.txt\"; \
    fi'"

    run_checks "${PLATFORM}_${TARGET}" "${TARGET}" $UT_CORE_BRANCH_NAME
    popd > /dev/null
}

# Description: This function prints the results of the tests
print_results() {
    pushd ${MY_DIR} > /dev/null
    
    #Results for ubuntu
    PLAT_DIR="${REPO_NAME}-ubuntu"
    pushd ${PLAT_DIR} > /dev/null
    run_checks "ubuntu" "linux" "$UT_CORE_BRANCH_NAME"
    popd > /dev/null
    
    #Results for VM_SYNC
    PLAT_DIR="${REPO_NAME}-VM-SYNC_linux"
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

    #Results for kirkstone-arm
    PLAT_DIR="${REPO_NAME}-kirkstone_arm"
    pushd ${PLAT_DIR} > /dev/null
    run_checks "kirkstone_arm" "arm" $UT_CORE_BRANCH_NAME
    popd > /dev/null

    #Results for kirkstone-linux
    PLAT_DIR="${REPO_NAME}-kirkstone_linux"
    pushd ${PLAT_DIR} > /dev/null
    run_checks "kirkstone_linux" "linux" $UT_CORE_BRANCH_NAME
    popd > /dev/null
    
    popd > /dev/null
    
}

# Run tests in different environments
run_on_ubuntu_linux
run_on_platform "dunfell" "linux"
run_on_platform "kirkstone" "linux"
run_on_platform "VM-SYNC" "linux"
run_on_platform "dunfell" "arm"
run_on_platform "kirkstone" "arm"

print_results
