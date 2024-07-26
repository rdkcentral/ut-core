#!/bin/bash

# ANSI color codes
GREEN='\033[0;32m'
RED='\033[0;31m'
MAGENTA='\033[0;35m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to print usage
usage() {
    echo -e "${YELLOW}Usage: $0 -u <Platform_REPO_URL> -c <ut_core_branch_name w/o feature> -p <compiler_type>${NC}"
}

# Parse command-line arguments
while getopts "u:c:t:p:" opt; do
    case $opt in
        u) Platform_REPO_URL="$OPTARG" ;;
        c) ut_core_branch_name="$OPTARG" ;;
        p) compiler_type="$OPTARG" ;;
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
if [ -z "$compiler_type" ]; then
    compiler_type=linux
else
    compiler_type=arm
fi

# Clone the repository
echo -e "${YELLOW}Cloning repository from $Platform_REPO_URL also usage is${NC}"
usage
git clone "$Platform_REPO_URL" || error_exit "Error: Failed to clone repository."
GIT_URL="    git clone ${Platform_REPO_URL}"
echo -e "${GREEN}GIT_URL = $GIT_URL${NC}"

# Change to the repository directory
REPO_NAME=$(basename "$Platform_REPO_URL" .git)
cd "$REPO_NAME" || error_exit "Error: Failed to change directory to $REPO_NAME."

#command to run for downloading ut/
command_to_run="./build_ut.sh TARGET=$compiler_type 2>&1 | tee build_log_temp.txt"
echo -e "${GREEN}command_to_run = $command_to_run${NC}"
($command_to_run) &
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

# Edit ut-control branch if provided
if [ -n "$ut_core_branch_name" ]; then
    sed -i "62s/.*/    git checkout feature\/$ut_core_branch_name/" ut/build.sh
fi

sleep 10

# Execute build.sh
clean_build=$(./build_ut.sh clean)
echo -e "${YELLOW}Running build.sh after doing a clean build${NC}"
echo -e "${YELLOW}You may also tail the output in another shell using ${NC}"
echo -e "${MAGENTA}tail -f 100 $PWD/build_log.txt ${NC}"
build_output=$(./build_ut.sh TARGET="$compiler_type" 2>&1 | tee build_log.txt)

# Check the build log for "Error"
grep_output=$(grep -nr "Error " build_log.txt )

# Print the results of the grep command
if [ -n "$grep_output" ]; then
    echo -e "${RED}Errors found in build_log.txt:${NC}"
    echo "$grep_output"
    exit 1
else
    echo -e "${GREEN}No unexpected errors found in build_log.txt${NC}"
    echo -e "${GREEN}build_ut command executed successfully.${NC}"
fi

if [ $compiler_type = "linux" ]; then
    echo -e "${GREEN}All commands executed successfully for $compiler_type compiler ${NC}"
    echo -e "${YELLOW}Follow below commands for arm compiler ${NC}"
    echo -e "${YELLOW}sc docker run rdk-dunfell /bin/bash ${NC}"
    echo -e "${YELLOW}cd /opt/toolchains/rdk-glibc-x86_64/ ${NC}"
    echo -e "${YELLOW}. environment-setup-armv7at2hf-neon-rdk-linux-gnueabi ${NC}"
    echo -e "${YELLOW}cd <folder with changes> ${NC}"
    echo -e "${YELLOW}./release-test-script-platform.sh -c <ut_core_branch_name w/o feature> -p arm ${NC}"
    echo -e "${YELLOW}or for your installed toolchain ${NC}"
else
    echo -e "${GREEN}All commands executed successfully for $compiler_type compiler ${NC}"
fi