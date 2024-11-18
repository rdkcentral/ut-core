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

#* *************************************************************************************************************************
#*
#*   ** @file        : shell
#*   ** @date        : 13/11/2024
#*   **
#*   ** @brief : Shell script to compare function available in header files with that of testing suite
#*
#* *************************************************************************************************************************


# Color code for yellow text
YELLOW='\033[0;33m'
NC='\033[0m' # No color (reset)

# Check if the correct number of arguments is provided
if [ "$#" -ne 3 ]; then
  echo "Usage: $0 <header_folder> <binary_file> <target>"
  exit 1
fi

# Assign arguments
header_folder=$1
binary_file=$2
target=$3

# Check if the header folder exists
if [ ! -d "$header_folder" ]; then
  echo "Error: Header folder '$header_folder' not found!"
  exit 1
fi

# Check if the binary file exists
if [ ! -f "$binary_file" ]; then
  echo "Error: Binary file '$binary_file' not found!"
  exit 1
fi

# Loop through each header file in the provided folder
for header_file in "$header_folder"/*.h; do
  # Check if the header file exists
  if [ ! -f "$header_file" ]; then
    echo "Warning: No header files found in '$header_folder'."
    break
  fi

  # Extract function names from the header file, excluding typedefs and function pointers
  function_names=$(grep -hoP '^\s*(?!typedef)\w+\s+\w+\s*\(.*\)\s*;' "$header_file" | \
                   sed -E 's/^\s*\w+\s+(\w+)\s*\(.*/\1/' | sort | uniq)
 
 # Initialize a list of functions to check if they're found in the binary file
  for func in $function_names; do
    # Check if the function is found in the binary file (defined or undefined)
    if ! nm "$binary_file" | grep -wP "\b$func\b" > /dev/null; then
      echo -e "${YELLOW}***************************************************************************************${NC}"
      echo -e "${YELLOW}  ** Function '$func' is NOT found in the binary file on target ${target}! **${NC}"
      echo -e "${YELLOW}***************************************************************************************${NC}"
    fi
  done
done
