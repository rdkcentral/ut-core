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
    echo -e "${YELLOW}Usage: $0 -u <REPO_URL> -c <ut_core_branch_name> -t <ut_control_branch_name w/o feature> -e <environment>${NC}"
    exit 1
}

# Parse command-line arguments
while getopts "u:c:t:e:" opt; do
    case $opt in
        u) REPO_URL="$OPTARG" ;;
        c) ut_core_branch_name="$OPTARG" ;;
        t) ut_control_branch_name="$OPTARG" ;;
        e) environment="$OPTARG" ;;
        *) usage ;;
    esac
done

# Function to print error message and exit
error_exit() {
    echo -e "${RED}$1${NC}" 1>&2
    exit 1
}

# if [ -z "$ut_control_branch_name" ]; then
#     echo "ut_control_branch_name is empty"
#     usage
#     exit 1
# fi

if [ -z "$REPO_URL" ]; then
    REPO_URL=git@github.com:rdkcentral/ut-core.git
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
    exit 1
fi

GLOBAL_RUN_ON_UBUNTU=${GLOBAL_RUN_ON_UBUNTU:-0}
GLOBAL_RUN_ON_VM_SYNC=${GLOBAL_RUN_ON_VM_SYNC:-0}
GLOBAL_RUN_ON_DUNFELL=${GLOBAL_RUN_ON_DUNFELL:-0}

echo -e "GLOBAL_RUN_ON_UBUNTU: $GLOBAL_RUN_ON_UBUNTU"
echo -e "GLOBAL_RUN_ON_VM_SYNC: $GLOBAL_RUN_ON_VM_SYNC"
echo -e "GLOBAL_RUN_ON_DUNFELL: $GLOBAL_RUN_ON_DUNFELL"

# Check if URL is provided
REPO_NAME=$(basename "$REPO_URL" .git)
if [ -z "$ut_core_branch_name" ]; then
    echo -e "${YELLOW}Cloning repository from $REPO_URL${NC}"
    git clone "$REPO_URL" $REPO_NAME-$environment|| error_exit "Error: Failed to clone repository."
else
    # Clone the repository
    echo -e "${YELLOW}Cloning repository from $REPO_URL and branch $ut_core_branch_name${NC}"
    git clone "$REPO_URL" -b "$ut_core_branch_name" $REPO_NAME-$environment || error_exit "Error: Failed to clone repository."
    GIT_URL="    git clone ${REPO_URL} -b  ${ut_core_branch_name} ut-core"
    echo -e "${GREEN}GIT_URL = $GIT_URL${NC}"
fi

# Change to the repository directory
echo -e "current path is : $PWD"
cd "$REPO_NAME-$environment" || error_exit "Error: Failed to change directory to $REPO_NAME-$environment."
UT_CORE_DIR=${MY_DIR}/$REPO_NAME-$environment

# Edit ut-control branch if provided
if [ -n "$ut_control_branch_name" ]; then
    GIT_URL_FOR_BUILD="git clone git@github.com:rdkcentral\/ut-control.git -b feature\/$ut_control_branch_name ut-control"
    echo -e "${GREEN}GIT_URL_FOR_BUILD = $GIT_URL_FOR_BUILD${NC}"
    sed -i "83s/.*/        $GIT_URL_FOR_BUILD/" build.sh
    sed -i '86s/.*/        # git checkout ${UT_CONTROL_PROJECT_VERSION}/' build.sh
fi

# Execute make framework
echo -e "${YELLOW}Running make framework...logs tee'd to $PWD/make_framework_log.txt${NC}"
make_output=$(make TARGET="$compiler_type" framework 2>&1 | tee make_framework_log.txt)
make_exit_code=$?

if [ $make_exit_code -eq 0 ]; then
    echo -e "${GREEN}Make framework command executed successfully.${NC}"
fi

if [ -n "$ut_core_branch_name" ]; then
    sed -i "59s/.*/$GIT_URL/" framework/ut-control/tests/build.sh
    sed -i '62s/.*/    # git checkout ${UT_CORE_PROJECT_VERSION}/' framework/ut-control/tests/build.sh
fi

# Execute make
echo -e "${YELLOW}Running make...logs tee'd to $PWD/make_log.txt${NC}"
make_output=$(make TARGET="$compiler_type" 2>&1 | tee make_log.txt)
make_exit_code=$?

if [ $make_exit_code -ne 0 ]; then
    if echo "$make_output" | grep -q "undefined reference to \`main'"; then
        echo -e "${YELLOW}Make failed with 'undefined reference to \`main\''. Treating it as pass.${NC}"
    else
        echo -e "${RED}Error: make command failed with the following output:${NC}"
        echo "$make_output"
        exit 1
    fi
else
    echo -e "${GREEN}Make command executed successfully.${NC}"
fi

# Execute make -C tests/
echo -e "${YELLOW}Running make -C tests/...logs tee'd to $PWD/make_test_log.txt ${NC}"
make_output=$(make TARGET="$compiler_type" -C tests/ 2>&1 | tee make_test_log.txt)
make_exit_code=$?

if [ $make_exit_code -eq 0 ]; then
    echo -e "${GREEN}Make -C tests/ command executed successfully.${NC}"
fi

#Change to ut-control
UT_CONTROL=framework/ut-control
cd "$UT_CONTROL" || error_exit "Error: Failed to change directory to $UT_CONTROL "

# Execute make in ut-control
echo -e "${YELLOW}Running make in ut-control....logs tee'd to $PWD/ut-control-make_log.txt${NC}"
make_output=$(make TARGET="$compiler_type" 2>&1 | tee ut-control-make_log.txt)
make_exit_code=$?

if [ $make_exit_code -eq 0 ]; then
    echo -e "${GREEN}Make command executed successfully.${NC}"
fi

# Execute make -C tests/ in ut-control
echo -e "${YELLOW}Running make -C tests/ in ut-control ... logs $PWD/ut-control-make_test_log.txt ${NC}"
make_output=$(make TARGET="$compiler_type" -C tests/ 2>&1 | tee ut-control-make_test_log.txt)
make_exit_code=$?

if [ $make_exit_code -eq 0 ]; then
    echo -e "${GREEN}Make -C tests/ command executed successfully.${NC}"
fi

pushd ${UT_CORE_DIR} > /dev/null
echo -e "current path during test is : $PWD"
echo -e "${RED}==========================================================${NC}"
CURL_STATIC_LIB="framework/ut-control/framework/curl/curl-8.8.0/build/lib/libcurl.a"
OPENSSL_STATIC_LIB="framework/ut-control/framework/openssl/openssl-OpenSSL_1_1_1w/build/lib/libssl.a"
CMAKE_HOST_BIN="framework/ut-control/host-tools/CMake-3.30.0/build/bin/cmake"

echo -e "${RED}RESULTS for ${environment} ${NC}"

current_branch=$(git branch | grep '\*' | sed 's/* //')

# Compare with the target branch name
if [ "$current_branch" == "${ut_core_branch_name}" ]; then
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

if [ $GLOBAL_RUN_ON_UBUNTU -eq 0 ]; then
    pushd ${MY_DIR} > /dev/null
    echo -e "PATH=$PWD"
    echo -e "GLOBAL_RUN_ON_UBUNTU: $GLOBAL_RUN_ON_UBUNTU"
    echo -e "GLOBAL_RUN_ON_VM_SYNC: $GLOBAL_RUN_ON_VM_SYNC"
    echo -e "GLOBAL_RUN_ON_DUNFELL: $GLOBAL_RUN_ON_DUNFELL"
    export GLOBAL_RUN_ON_UBUNTU
    export GLOBAL_RUN_ON_VM_SYNC
    export GLOBAL_RUN_ON_DUNFELL
    if [ -z "$ut_core_branch_name" ]; then
        echo -e "${YELLOW}Executing: $0 -t $ut_control_branch_name -e ubuntu${NC}"
        $0 -t $ut_control_branch_name -e ubuntu
    elif [ -z "$ut_control_branch_name" ]; then
        echo -e "${YELLOW}Executing: $0 -c $ut_core_branch_name -e ubuntu${NC}"
        $0 -c $ut_core_branch_name -e ubuntu
    else
        echo -e "${YELLOW}Executing: $0 -c $ut_core_branch_name -t $ut_control_branch_name -e ubuntu${NC}"
        $0 -c $ut_core_branch_name -t $ut_control_branch_name -e ubuntu
    fi
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
    if [ -z "$ut_core_branch_name" ]; then
        echo -e "${YELLOW}Executing: $0 -t $ut_control_branch_name -e dunfell${NC}"
        /bin/bash -c "sc docker run rdk-dunfell 'cd /opt/toolchains/rdk-glibc-x86_64/; \
        . environment-setup-armv7at2hf-neon-rdk-linux-gnueabi; env | grep CC; cd $PWD; \
        export GLOBAL_RUN_ON_UBUNTU=$GLOBAL_RUN_ON_UBUNTU; export GLOBAL_RUN_ON_VM_SYNC=$GLOBAL_RUN_ON_VM_SYNC; \
        export GLOBAL_RUN_ON_DUNFELL=$GLOBAL_RUN_ON_DUNFELL; $0 -t $ut_control_branch_name -e dunfell'"
    elif [ -z "$ut_control_branch_name" ]; then
        echo -e "${YELLOW}Executing: $0 -c $ut_core_branch_name -e dunfell${NC}"
        /bin/bash -c "sc docker run rdk-dunfell 'cd /opt/toolchains/rdk-glibc-x86_64/; \
        . environment-setup-armv7at2hf-neon-rdk-linux-gnueabi; env | grep CC; cd $PWD; \
        export GLOBAL_RUN_ON_UBUNTU=$GLOBAL_RUN_ON_UBUNTU; export GLOBAL_RUN_ON_VM_SYNC=$GLOBAL_RUN_ON_VM_SYNC; \
        export GLOBAL_RUN_ON_DUNFELL=$GLOBAL_RUN_ON_DUNFELL; $0 -c $ut_core_branch_name -e dunfell'"
    else
        echo -e "${YELLOW}Executing: $0 -c $ut_core_branch_name -t $ut_control_branch_name -e dunfell${NC}"
        /bin/bash -c "sc docker run rdk-dunfell 'cd /opt/toolchains/rdk-glibc-x86_64/; \
        . environment-setup-armv7at2hf-neon-rdk-linux-gnueabi; env | grep CC; cd $PWD; \
        export GLOBAL_RUN_ON_UBUNTU=$GLOBAL_RUN_ON_UBUNTU; \
        export GLOBAL_RUN_ON_VM_SYNC=$GLOBAL_RUN_ON_VM_SYNC; export GLOBAL_RUN_ON_DUNFELL=$GLOBAL_RUN_ON_DUNFELL;\
        $0 -c $ut_core_branch_name -t $ut_control_branch_name -e dunfell'"
    fi
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
    if [ -z "$ut_core_branch_name" ]; then
        echo -e "${YELLOW}Executing: $0 -t $ut_control_branch_name -e VM-SYNC${NC}"
        /bin/bash -c "sc docker run --local vm-sync 'export GLOBAL_RUN_ON_UBUNTU=$GLOBAL_RUN_ON_UBUNTU; \
        export GLOBAL_RUN_ON_VM_SYNC=$GLOBAL_RUN_ON_VM_SYNC; export GLOBAL_RUN_ON_DUNFELL=$GLOBAL_RUN_ON_DUNFELL; \
        $0 -t $ut_control_branch_name -e VM-SYNC'"
    elif [ -z "$ut_control_branch_name" ]; then
        echo -e "${YELLOW}Executing: $0 -c $ut_core_branch_name -e VM-SYNC${NC}"
        /bin/bash -c "sc docker run rdk-dunfell 'cd /opt/toolchains/rdk-glibc-x86_64/; \
        . environment-setup-armv7at2hf-neon-rdk-linux-gnueabi; env | grep CC; cd $PWD; \
        export GLOBAL_RUN_ON_UBUNTU=$GLOBAL_RUN_ON_UBUNTU; export GLOBAL_RUN_ON_VM_SYNC=$GLOBAL_RUN_ON_VM_SYNC; \
        export GLOBAL_RUN_ON_DUNFELL=$GLOBAL_RUN_ON_DUNFELL; $0 -c $ut_core_branch_name -e VM-SYNC'"
    else
        echo -e "${YELLOW}Executing: $0 -c $ut_core_branch_name -t $ut_control_branch_name -e VM-SYNC${NC}"
        /bin/bash -c "sc docker run --local vm-sync 'echo $PWD; export GLOBAL_RUN_ON_UBUNTU=$GLOBAL_RUN_ON_UBUNTU; \
        export GLOBAL_RUN_ON_VM_SYNC=$GLOBAL_RUN_ON_VM_SYNC; export GLOBAL_RUN_ON_DUNFELL=$GLOBAL_RUN_ON_DUNFELL; \
        $0 -c $ut_core_branch_name -t $ut_control_branch_name -e VM-SYNC'"
    fi
    popd > /dev/null
fi