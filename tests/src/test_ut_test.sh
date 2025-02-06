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

# Function to run the test and validate the expected test count
run_test() {
    local CMD="$1"
    local EXPECTED="$2"

    # Run the command and capture output
    OUTPUT=$($CMD 2>&1)

    # Extract the total number of tests run
    TOTAL_TESTS=$(echo "$OUTPUT" | grep -oP '\[==========\] \K[0-9]+(?= tests from)')

    # Ensure the expected number of tests ran
    if [[ "$TOTAL_TESTS" -eq "$EXPECTED" ]]; then
        echo "✅ Success: $CMD ran exactly $EXPECTED tests."
    else
        echo "❌ Error: $CMD expected $EXPECTED tests, but found $TOTAL_TESTS."
        exit 1
    fi
}

# Run both test cases
run_test "./run.sh -d 1 -a" 5
run_test "./run.sh -d 1 -d 2 -a" 2
run_test "./run.sh -e 1 -d 2 -a" 6
run_test "./run.sh -d 1 -e 2 -a" 5
run_test "./run.sh -e 1 -a" 9

echo "✅ All tests validated successfully."
exit 0
