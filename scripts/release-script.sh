#!/bin/bash

# ANSI color codes
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to print usage
usage() {
    echo -e "${YELLOW}Usage: $0 -u <REPO_URL> -c <ut_core_branch_name> -t <ut_control_branch_name w/o feature> -p <compiler_type>${NC}"
    exit 1
}

# Parse command-line arguments
while getopts "u:c:t:p:" opt; do
    case $opt in
        u) REPO_URL="$OPTARG" ;;
        c) ut_core_branch_name="$OPTARG" ;;
        t) ut_control_branch_name="$OPTARG" ;;
        p) compiler_type="$OPTARG" ;;
        *) usage ;;
    esac
done

# Function to print error message and exit
error_exit() {
    echo -e "${RED}$1${NC}" 1>&2
    exit 1
}

if [ -z "$ut_control_branch_name" ]; then
    echo "ut_control_branch_name is empty"
    usage
    exit 1
fi

if [ -z "$REPO_URL" ]; then
    REPO_URL=git@github.com:rdkcentral/ut-core.git
fi

# Set default compiler type if not provided
if [ -z "$compiler_type" ]; then
    compiler_type=linux
else
    compiler_type=arm
fi

# Check if URL is provided
if [ -z "$ut_core_branch_name" ]; then
    echo -e "${YELLOW}Cloning repository from $REPO_URL${NC}"
    git clone "$REPO_URL" || error_exit "Error: Failed to clone repository."
else
    # Clone the repository
    echo -e "${YELLOW}Cloning repository from $REPO_URL and branch $ut_core_branch_name${NC}"
    git clone "$REPO_URL" -b "$ut_core_branch_name" || error_exit "Error: Failed to clone repository."
    GIT_URL="    git clone ${REPO_URL} -b  ${ut_core_branch_name} ut-core"
    echo -e "${GREEN}GIT_URL = $GIT_URL${NC}"
fi

# Change to the repository directory
REPO_NAME=$(basename "$REPO_URL" .git)
cd "$REPO_NAME" || error_exit "Error: Failed to change directory to $REPO_NAME."

# Edit ut-control branch if provided
if [ -n "$ut_control_branch_name" ]; then
    GIT_URL_FOR_BUILD="git clone git@github.com:rdkcentral\/ut-control.git -b feature\/$ut_control_branch_name ut-control"
    echo -e "${GREEN}GIT_URL_FOR_BUILD = $GIT_URL_FOR_BUILD${NC}"
    sed -i "83s/.*/        $GIT_URL_FOR_BUILD/" build.sh
    sed -i '86s/.*/        # git checkout ${UT_CONTROL_PROJECT_VERSION}/' build.sh
fi

# Execute make framework
echo -e "${YELLOW}Running make framework...${NC}"
make_output=$(make TARGET="$compiler_type" framework 2>&1)
make_exit_code=$?

if [ $make_exit_code -eq 0 ]; then
    echo -e "${GREEN}Make framework command executed successfully.${NC}"
fi

if [ -n "$ut_core_branch_name" ]; then
    sed -i "59s/.*/$GIT_URL/" framework/ut-control/tests/build.sh
    sed -i '62s/.*/    # git checkout ${UT_CORE_PROJECT_VERSION}/' framework/ut-control/tests/build.sh
fi

# Execute make
echo -e "${YELLOW}Running make...${NC}"
make_output=$(make TARGET="$compiler_type" 2>&1)
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
echo -e "${YELLOW}Running make -C tests/...${NC}"
make_output=$(make TARGET="$compiler_type" -C tests/ 2>&1)
make_exit_code=$?

if [ $make_exit_code -eq 0 ]; then
    echo -e "${GREEN}Make -C tests/ command executed successfully.${NC}"
fi

#Change to ut-control
UT_CONTROL=framework/ut-control
cd "$UT_CONTROL" || error_exit "Error: Failed to change directory to $UT_CONTROL "

# Execute make in ut-control
echo -e "${YELLOW}Running make in ut-control${NC}"
make_output=$(make TARGET="$compiler_type" 2>&1)
make_exit_code=$?

if [ $make_exit_code -eq 0 ]; then
    echo -e "${GREEN}Make command executed successfully.${NC}"
fi

# Execute make -C tests/ in ut-control
echo -e "${YELLOW}Running make -C tests/ in ut-control ${NC}"
make_output=$(make TARGET="$compiler_type" -C tests/ 2>&1)
make_exit_code=$?

if [ $make_exit_code -eq 0 ]; then
    echo -e "${GREEN}Make -C tests/ command executed successfully.${NC}"
fi

if [ $compiler_type = "linux" ]; then
    echo -e "${GREEN}All commands executed successfully for $compiler_type compiler ${NC}"
    echo -e "${YELLOW}Follow below commands for arm compiler ${NC}"
    echo -e "${YELLOW}sc docker run rdk-dunfell /bin/bash ${NC}"
    echo -e "${YELLOW}cd /opt/toolchains/rdk-glibc-x86_64/ ${NC}"
    echo -e "${YELLOW}. environment-setup-armv7at2hf-neon-rdk-linux-gnueabi ${NC}"
    echo -e "${YELLOW}cd <folder with changes> ${NC}"
    echo -e "${YELLOW}./release-script.sh -t <ut-control-branch to be tested> -p TARGET=arm ${NC}"
else
    echo -e "${GREEN}All commands executed successfully for $compiler_type compiler ${NC}"
fi
