#!/usr/bin/env bash

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

# Define colors
GREEN="\033[0;32m"
RED="\033[0;31m"
YELLOW="\033[0;33m"
NC="\033[0m"  # No Color

# Define the Makefile target (optional)
MAKE_TARGET="all"  # Default target, can be overridden

# Capture additional arguments passed to the script
USER_ARGS="$@"

# Path to check for the library
LIB_PATH="build/bin/libweak_stubs_libs.so"

# Function to clean the build
clean_build() {
    echo -e "${YELLOW}Running 'make clean'...${NC}"
    if make clean; then
        echo -e "${GREEN}'make clean' completed successfully.${NC}"
    else
        echo -e "${RED}'make clean' failed.${NC}"
        exit 1
    fi
}

# Scenario 1: Call make without overriding the variable
echo -e "${YELLOW}Running 'make' without overriding BUILD_WEAK_STUBS_SRC...${NC}"
clean_build
if make $MAKE_TARGET $USER_ARGS; then
    echo -e "${GREEN}Scenario 1: 'make' ran successfully.${NC}"
    # Check if the library exists
    if [[ -f $LIB_PATH ]]; then
        echo -e "${GREEN}Library created successfully at: $LIB_PATH${NC}"
    else
        echo -e "${RED}Library not found at: $LIB_PATH${NC}"
        exit 1
    fi
else
    echo -e "${RED}Scenario 1: 'make' failed.${NC}"
    exit 1
fi

# Scenario 2: Call make with BUILD_WEAK_STUBS_SRC unset
echo -e "${YELLOW}Running 'make' with BUILD_WEAK_STUBS_SRC unset...${NC}"
clean_build
if make $MAKE_TARGET BUILD_WEAK_STUBS_SRC= $USER_ARGS; then
    echo -e "${GREEN}Scenario 2: 'make' ran successfully.${NC}"
    # Check if the library is not created
    if [[ -f $LIB_PATH ]]; then
        echo -e "${RED}Library should not be created, but found at: $LIB_PATH${NC}"
        exit 1
    else
        echo -e "${GREEN}Library not created as expected.${NC}"
    fi
else
    echo -e "${RED}Scenario 2: 'make' failed.${NC}"
    exit 1
fi

echo -e "${GREEN}Both scenarios completed successfully.${NC}"