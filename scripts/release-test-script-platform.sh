#!/bin/bash

SCRIPT_EXEC="$(realpath $0)"
MY_DIR="$(dirname $SCRIPT_EXEC)"

# ANSI color codes
GREEN='\033[0;32m'
RED='\033[0;31m'
MAGENTA='\033[0;35m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to print usage
usage() {
    echo -e "${YELLOW}Usage: $0 -u <Platform_REPO_URL> -c <ut_core_branch_name w/o feature> -e <environment>${NC}"
}

# Parse command-line arguments
while getopts "u:c:t:e:" opt; do
    case $opt in
        u) Platform_REPO_URL="$OPTARG" ;;
        c) ut_core_branch_name="$OPTARG" ;;
        e) environment="$OPTARG" ;;
        *) usage ;;
    esac
done

# Function to print error message and exit
error_exit() {
    echo -e "${RED}$1${NC}" 1>&2
    exit 1
}

if [ -z "$Platform_REPO_URL" ]; then
    Platform_REPO_URL=git@github.com:rdkcentral/rdk-halif-power_manager.git
fi

# Set default compiler type if not provided
if [ "$environment" == "ubuntu" ]; then
    compiler_type="linux"
    elif [ "$environment" == "VM-SYNC" ]; then
    compiler_type="linux"
    elif [ "$environment" == "dunfell" ]; then
    compiler_type="arm"
else
    echo "Unknown environment: $environment"
    echo "Environments are : ubuntu, VM-SYNC, dunfell"
    usage
    exit 1
fi

GLOBAL_RUN_ON_UBUNTU=${GLOBAL_RUN_ON_UBUNTU:-0}
GLOBAL_RUN_ON_VM_SYNC=${GLOBAL_RUN_ON_VM_SYNC:-0}
GLOBAL_RUN_ON_DUNFELL=${GLOBAL_RUN_ON_DUNFELL:-0}

# Clone the repository
echo -e "${YELLOW}Cloning repository from $Platform_REPO_URL also usage is${NC}"
usage
REPO_NAME=$(basename "$Platform_REPO_URL" .git)
PLAT_DIR=$REPO_NAME-$environment
git clone "$Platform_REPO_URL" $PLAT_DIR || error_exit "Error: Failed to clone repository."
GIT_URL="    git clone ${Platform_REPO_URL} $PLAT_DIR"
echo -e "${GREEN}GIT_URL = $GIT_URL${NC}"

# Change to the repository directory

pushd ${PLAT_DIR} > /dev/null

#command to run for downloading ut/
command_to_run="./build_ut.sh TARGET=$compiler_type > build_log_temp.txt 2>&1"
echo -e "${GREEN}command_to_run = $command_to_run${NC}"
#($command_to_run) &
eval "$command_to_run &"
command_pid=$!


# Check if the command started successfully
# if [ -z "$command_pid" ]; then
#     error_exit "Failed to start the command."
# fi

echo "Started command with PID $command_pid"

# Wait for the specified time
sleep 10

# Stop the command
kill -9 "$command_pid" || error_exit "Failed to stop the command."

sleep 10

# Remove ut-core
# rm -rf ut/ut-core
# echo -e "${YELLOW}Removing ut-core so that, next build will download the correct branch${NC}"
if [ -d ut/ut-core ]; then
    echo -e "${YELLOW}switching to the branch specified and cleaning{NC}"
    cd ut/ut-core; git checkout feature/${ut_core_branch_name}; make cleanall
    cd ../../;
fi

sleep 5
# Edit ut-control branch if provided
if [ -n "$ut_core_branch_name" ]; then
    sed -i "62s/.*/    git checkout feature\/$ut_core_branch_name/" ut/build.sh
fi
sleep 5

echo -e "${YELLOW}You may also tail the output in another shell using ${NC}"
echo -e "${MAGENTA}tail -f 100 $PWD/build_log.txt ${NC}"
build_output=$(./build_ut.sh TARGET="$compiler_type" > build_log.txt 2>&1)

# # Check the build log for "Error"
# grep_output=$(grep -nr "Error " build_log.txt )

# # Print the results of the grep command
# if [ -n "$grep_output" ]; then
#     echo -e "${RED}Errors found in build_log.txt:${NC}"
#     echo "$grep_output"
#     exit 1
# else
#     echo -e "${GREEN}No unexpected errors found in build_log.txt${NC}"
#     echo -e "${GREEN}build_ut command executed successfully.${NC}"
# fi

popd > /dev/null

pushd ${PLAT_DIR} > /dev/null
echo -e "current path during test is : $PWD"
echo -e "${RED}==========================================================${NC}"
CURL_STATIC_LIB="ut/ut-core/framework/ut-control/framework/curl/curl-8.8.0/build/lib/libcurl.a"
OPENSSL_STATIC_LIB="ut/ut-core/framework/ut-control/framework/openssl/openssl-OpenSSL_1_1_1w/build/lib/libssl.a"
CMAKE_HOST_BIN="ut/ut-core/framework/ut-control/host-tools/CMake-3.30.0/build/bin/cmake"

echo -e "${RED}RESULTS for ${environment} ${NC}"

grep_output=$(grep -nr "${ut_core_branch_name}" ut/build.sh)
if [ -n "$grep_output" ]; then
     echo -e "${GREEN}on the branch $ut_core_branch_name. PASS${NC}"
else
    error_exit "but, branch is not switched. FAIL"
fi

# Test for CURL
if [ -f "$CURL_STATIC_LIB" ]; then
    echo -e "${GREEN}$CURL_STATIC_LIB exists. PASS${NC}"
else
    echo -e "${RED}CURL static lib does not exist. FAIL ${NC}"
fi

# Test for Openssl
if [ "$environment" == "ubuntu" ]; then
    if [ ! -f "$OPENSSL_STATIC_LIB" ]; then
        echo -e "${GREEN}Openssl static lib does not exist.PASS ${NC}"
    else
        echo -e "${RED}Openssl static lib does exist.FAIL ${NC}"
    fi
    elif [ "$environment" == "VM-SYNC" ]; then
    if [ -f "$OPENSSL_STATIC_LIB" ]; then
        echo -e "${GREEN}$OPENSSL_STATIC_LIB does exist.PASS ${NC}"
    else
        echo -e "${RED}Openssl static lib does not exist.FAIL ${NC}"
    fi
    elif [ "$environment" == "dunfell" ]; then
    if [ -f "$OPENSSL_STATIC_LIB" ]; then
        echo -e "${GREEN}$OPENSSL_STATIC_LIB does exist.PASS ${NC}"
    else
        echo -e "${RED}Openssl static lib does not exist.FAIL ${NC}"
    fi
fi

# Test for CMAKE HOST Binary
if [ "$environment" == "ubuntu" ]; then
    GLOBAL_RUN_ON_UBUNTU=$((GLOBAL_RUN_ON_UBUNTU + 1))
    if [ ! -f "$CMAKE_HOST_BIN" ]; then
        echo -e "${GREEN}cmake host binary does not exist.PASS ${NC}"
    else
        echo -e "${RED}cmake host binary does exist.FAIL ${NC}"
    fi
    elif [ "$environment" == "VM-SYNC" ]; then
    GLOBAL_RUN_ON_VM_SYNC=$((GLOBAL_RUN_ON_VM_SYNC + 1))
    if [ -f "$CMAKE_HOST_BIN" ]; then
        echo -e "${GREEN}$CMAKE_HOST_BIN does exist.PASS ${NC}"
    else
        echo -e "${RED}cmake host binary does not exist.FAIL ${NC}"
    fi
    elif [ "$environment" == "dunfell" ]; then
    GLOBAL_RUN_ON_DUNFELL=$((GLOBAL_RUN_ON_DUNFELL + 1))
    if [ ! -f "$CMAKE_HOST_BIN" ]; then
        echo -e "${GREEN}cmake host binary does not exist.PASS ${NC}"
    else
        echo -e "${RED}cmake host binary does exist.FAIL ${NC}"
    fi
fi
echo -e "${RED}==========================================================${NC}"
popd > /dev/null

# Running the script in different environments

if [ $GLOBAL_RUN_ON_UBUNTU -eq 0 ]; then
    pushd ${MY_DIR} > /dev/null
    echo -e "PATH=$PWD"
    echo -e "GLOBAL_RUN_ON_UBUNTU: $GLOBAL_RUN_ON_UBUNTU"
    echo -e "GLOBAL_RUN_ON_VM_SYNC: $GLOBAL_RUN_ON_VM_SYNC"
    echo -e "GLOBAL_RUN_ON_DUNFELL: $GLOBAL_RUN_ON_DUNFELL"
    export GLOBAL_RUN_ON_UBUNTU
    export GLOBAL_RUN_ON_VM_SYNC
    export GLOBAL_RUN_ON_DUNFELL
    echo -e "${YELLOW}Executing: $0 -c $ut_core_branch_name -e ubuntu${NC}"
    $0 -c $ut_core_branch_name -e ubuntu
    popd > /dev/null

elif [ $GLOBAL_RUN_ON_DUNFELL -eq 0 ]; then
    pushd ${MY_DIR} > /dev/null
    echo -e "PATH=$PWD"
    export GLOBAL_RUN_ON_UBUNTU
    export GLOBAL_RUN_ON_VM_SYNC
    export GLOBAL_RUN_ON_DUNFELL
    echo -e "GLOBAL_RUN_ON_UBUNTU: $GLOBAL_RUN_ON_UBUNTU"
    echo -e "GLOBAL_RUN_ON_VM_SYNC: $GLOBAL_RUN_ON_VM_SYNC"
    echo -e "GLOBAL_RUN_ON_DUNFELL: $GLOBAL_RUN_ON_DUNFELL"
    echo -e "${YELLOW}Executing: $0 -c $ut_core_branch_name -e dunfell${NC}"
    /bin/bash -c "sc docker run rdk-dunfell 'cd /opt/toolchains/rdk-glibc-x86_64/; \
    . environment-setup-armv7at2hf-neon-rdk-linux-gnueabi; env | grep CC; cd $PWD; \
    export GLOBAL_RUN_ON_UBUNTU=$GLOBAL_RUN_ON_UBUNTU; export GLOBAL_RUN_ON_VM_SYNC=$GLOBAL_RUN_ON_VM_SYNC; \
    export GLOBAL_RUN_ON_DUNFELL=$GLOBAL_RUN_ON_DUNFELL; $0 -c $ut_core_branch_name -e dunfell'"
    popd > /dev/null

elif [ $GLOBAL_RUN_ON_VM_SYNC -eq 0 ]; then
    pushd ${MY_DIR} > /dev/null
    echo -e "PATH=$PWD"
    echo -e "GLOBAL_RUN_ON_UBUNTU: $GLOBAL_RUN_ON_UBUNTU"
    echo -e "GLOBAL_RUN_ON_VM_SYNC: $GLOBAL_RUN_ON_VM_SYNC"
    echo -e "GLOBAL_RUN_ON_DUNFELL: $GLOBAL_RUN_ON_DUNFELL"
    export GLOBAL_RUN_ON_UBUNTU
    export GLOBAL_RUN_ON_VM_SYNC
    export GLOBAL_RUN_ON_DUNFELL
    echo -e "${YELLOW}Executing: $0 -c $ut_core_branch_name -e VM-SYNC${NC}"
    /bin/bash -c "sc docker run rdk-dunfell 'cd /opt/toolchains/rdk-glibc-x86_64/; \
    . environment-setup-armv7at2hf-neon-rdk-linux-gnueabi; env | grep CC; cd $PWD; \
    export GLOBAL_RUN_ON_UBUNTU=$GLOBAL_RUN_ON_UBUNTU; export GLOBAL_RUN_ON_VM_SYNC=$GLOBAL_RUN_ON_VM_SYNC; \
    export GLOBAL_RUN_ON_DUNFELL=$GLOBAL_RUN_ON_DUNFELL; $0 -c $ut_core_branch_name -e VM-SYNC'"
    popd > /dev/null
fi