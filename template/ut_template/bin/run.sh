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

# Save the original directory where the script was called from
original_dir=$(pwd)

# Function to convert a relative path into an absolute path
# If the path is already absolute, it remains unchanged
convert_to_absolute_path() {
    if [[ "$1" = /* ]]; then
        # If it's an absolute path, just return it
        echo "$1"
    else
        # If it's a relative path, prepend the original directory to it
        echo "$original_dir/$1"
    fi
}

# Loop through arguments to find the profile path (-p) and other arguments
profile_path=""
other_args=()

while [[ $# -gt 0 ]]; do
    case "$1" in
        -p)  # When -p flag is found, resolve the path that follows it
            shift
            profile_path=$(convert_to_absolute_path "$1")
            ;;
        *)   # Collect other arguments
            other_args+=("$1")
            ;;
    esac
    shift
done

# Change to the directory where the script is located
cd "$(dirname "$0")"

# Set necessary library paths
export LD_LIBRARY_PATH=/usr/lib:/lib:/home/root:./

# Run the actual program with the resolved absolute profile path if provided
if [[ -n "$profile_path" ]]; then
    # If profile_path is not empty, include it in the command
    ./hal_test -p "$profile_path" "${other_args[@]}"
else
    # If no profile path, just run the program with other arguments
    ./hal_test "${other_args[@]}"
fi

# Optionally, return to the original directory
cd "$original_dir"