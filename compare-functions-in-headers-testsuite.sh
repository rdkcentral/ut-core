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


# Color code for red text
RED='\033[0;31m'
NC='\033[0m' # No color (reset)

# Check if the correct number of arguments is provided
if [ "$#" -ne 2 ]; then
  echo "Usage: $0 <header_folder> <object_folder>"
  exit 1
fi

# Assign folders from arguments
header_folder=$1
object_folder=$2

# Check if the header folder exists
if [ ! -d "$header_folder" ]; then
  echo "Error: Header folder '$header_folder' not found!"
  exit 1
fi

# Check if the object folder exists
if [ ! -d "$object_folder" ]; then
  echo "Error: Object folder '$object_folder' not found!"
  exit 1
fi

# Loop through each header file in the provided folder
for header_file in "$header_folder"/*.h; do
  # Check if the header file exists
  if [ ! -f "$header_file" ]; then
    echo "Warning: No header files found in '$header_folder'."
    break
  fi

  #echo "Checking functions in header file: $header_file"    #TODO: Enable for logs
  
  # Extract function names from the header file, excluding typedefs and function pointers
  #function_names=$(grep -P '^\w+\s+\w+\s*\([^)]*\)\s*;' "$header_file" | \
  #  grep -Pv 'typedef|(\w+\s*\*.*\(\))' | \
  #  sed -E 's/\s*\(.*\)//')
  function_names=$(grep -hoP '^\s*(?!typedef)\w+\s+\w+\s*\(.*\)\s*;' "$header_file" | \
                   sed -E 's/^\s*\w+\s+(\w+)\s*\(.*/\1/' | sort | uniq)
 
  # Initialize a list of functions to check if they're found in any object file
  for func in $function_names; do
    found_in_any_object=false

    # Loop through all object files in the object folder
    for object_file in "$object_folder"/*.o; do
      # Check if the object file exists
      if [ ! -f "$object_file" ]; then
        echo "Warning: No object files found in '$object_folder'."
        break
      fi

      # Check if the function is found in the object file (defined or undefined)
      if nm "$object_file" | grep -w " $func$" > /dev/null || nm "$object_file" | grep -w " U $func$" > /dev/null; then
        #echo "  Function '$func' is available in object file '$object_file'.PASS"      #TODO: Enable for logs
        found_in_any_object=true
        break  # No need to check further object files for this function
      fi
    done

    # If the function was not found in any object file, highlight it as missing
    if [ "$found_in_any_object" = false ]; then
      echo -e "${RED}*******************************************************************************${NC}"
      echo -e "${RED}  ** Function '$func' is NOT found in ANY object file! **${NC}"
      echo -e "${RED}*******************************************************************************${NC}"
    fi
  done
done	
