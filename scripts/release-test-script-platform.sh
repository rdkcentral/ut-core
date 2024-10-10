#!/bin/bash

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
    usage
    #exit 1
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

run_build(){
    local environment="$1"
    local target="$2"
    local UT_CORE_BRANCH_NAME="$3"
    local UT_CONTROL_BRANCH_NAME="$4"
    echo "environment=$environment"
    echo "target=$target"
    echo "UT_CORE_BRANCH_NAME=$UT_CORE_BRANCH_NAME"
    
    if [ ! -z "$UT_CORE_BRANCH_NAME" ]; then
        #command to run for downloading ut/
        command_to_run="./build_ut.sh TARGET=$target > build_log_temp.txt 2>&1"
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
        
        # switch ut-core to ut_core_branch_name
        # echo "UT_CORE_BRANCH_NAME = $UT_CORE_BRANCH_NAME"
        # sed -i "62s|.*|    git checkout $UT_CORE_BRANCH_NAME|" ut/build.sh
        # sleep 5
        # rm -rf ut/ut-core || error_exit "Failed to remove ut-core"
        # sleep 5
        cd ut/ut-core
        git checkout $UT_CORE_BRANCH_NAME
        git pull
        cd -

        if [ ! -z "$UT_CONTROL_BRANCH_NAME" ]; then
            cd ut/ut-core/framework/ut-control
            git checkout $UT_CONTROL_BRANCH_NAME
            git pull
            cd -
        fi

        rm -rf ut/build/ ut/ut-core/framework/ut-control/build/ ut/ut-core/framework/ut-control/framework/ ut/ut-core/framework/ut-control/host-tools/
    fi
    
    echo -e "${YELLOW}You may also tail the output in another shell using ${NC}"
    echo -e "${MAGENTA}tail -f 100 $PWD/build_log.txt ${NC}"
    ./build_ut.sh TARGET="$target" | tee build_log.txt 2>&1
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}Build command executed successfully.${NC}"
    else
        echo -e "${RED}Build command failed. Check the logs in build_log.txt${NC}"
    fi
}

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
    HAL_BIN="ut/bin/hal_test"
    # CPP_STATIC_LIB="build/${architecture_type}/cpp_libs/lib/libgtest.a"
    # CPP_MAIN_STATIC_LIB="build/${architecture_type}/cpp_libs/lib/libgtest_main.a"
    
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
    if [ -f "$HAL_BIN" ]; then
        echo -e "${GREEN}$HAL_BIN exists. PASS${NC}"
    else
        echo -e "${RED}Hal binary does not exist. FAIL ${NC}"
    fi
    
    # Test for CURL static library
    if [[ "$environment" == "dunfell_linux" ]]; then
        if [ -f "$CURL_STATIC_LIB" ]; then
            echo -e "${RED}$CURL_STATIC_LIB exists. FAIL${NC}"
        else
            echo -e "${GREEN}CURL static lib does not exist. PASS ${NC}"
        fi
    else
        if [ -f "$CURL_STATIC_LIB" ]; then
            echo -e "${GREEN}$CURL_STATIC_LIB exists. PASS${NC}"
        else
            echo -e "${RED}CURL static lib does not exist. FAIL ${NC}"
        fi
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

run_on_dunfell_linux() {
    pushd ${MY_DIR} > /dev/null
    SETUP_ENV="sc docker run rdk-dunfell"
    run_git_clone "dunfell_linux" "linux"
    # Change to the repository directory
    PLAT_DIR="${REPO_NAME}-dunfell_linux"
    pushd ${PLAT_DIR} > /dev/null
    /bin/bash -c "$SETUP_ENV; $(declare -f run_build); run_build ''dunfell_linux 'linux' '$UT_CORE_BRANCH_NAME' '$UT_CONTROL_BRANCH_NAME'"
    run_checks "dunfell_linux" "linux" $UT_CORE_BRANCH_NAME $UT_CONTROL_BRANCH_NAME
    popd > /dev/null
    popd > /dev/null
}

run_on_dunfell_arm() {
    pushd ${MY_DIR} > /dev/null
    run_git_clone "dunfell_arm" "arm"
    # Change to the repository directory
    PLAT_DIR="${REPO_NAME}-dunfell_arm"
    pushd ${PLAT_DIR} > /dev/null
    /bin/bash -c "sc docker run rdk-dunfell 'cd /opt/toolchains/rdk-glibc-x86_64-arm-toolchain; \
     . environment-setup-armv7at2hf-neon-oe-linux-gnueabi; env | grep CC; cd -; \
    $(declare -f run_build); run_build 'dunfell_arm' 'arm' '$UT_CORE_BRANCH_NAME' '$UT_CONTROL_BRANCH_NAME';exit'"
    run_checks "dunfell_arm" "arm" $UT_CORE_BRANCH_NAME $UT_CONTROL_BRANCH_NAME
    popd > /dev/null
    popd > /dev/null
}

run_on_vm_sync_linux() {
    pushd ${MY_DIR} > /dev/null
    SETUP_ENV="sc docker run vm-sync"
    run_git_clone "VM-SYNC" "linux"
    # Change to the repository directory
    PLAT_DIR="${REPO_NAME}-VM-SYNC"
    pushd ${PLAT_DIR} > /dev/null
    /bin/bash -c "$SETUP_ENV '$(declare -f run_build); run_build 'VM-SYNC' 'linux' '$UT_CORE_BRANCH_NAME' '$UT_CONTROL_BRANCH_NAME';'"
    run_checks "VM-SYNC" "linux" $UT_CORE_BRANCH_NAME $UT_CONTROL_BRANCH_NAME
    popd > /dev/null
    popd > /dev/null
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

# Run tests in different environments
#run_on_ubuntu_linux
#run_on_dunfell_linux
#run_on_vm_sync_linux
#run_on_dunfell_arm
print_results
