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

#* ************************************************************************
#*
#*   ** Project      : Test Autogenerate Script
#*   ** @addtogroup  : ut
#*   ** @file        : shell
#*   ** @date        : 08/11/2024
#*   **
#*   ** @brief : Shell script to test the functionality of autogenerate script
#*   **
#*
#* ************************************************************************


# Initialize counters for total, success, and failure counts
total_tests=0
success_count=0
failure_count=0
all_success=true

# Function to run a command, check its exit status, and update counters
run_command() {
  ((total_tests++))  # Increment total test count
  eval "$@"
  local status=$?
  if [ $status -ne 0 ]; then
    echo "Command '$*' failed with exit status $status"
    ((failure_count++))  # Increment failure count
    all_success=false
  else
    ((success_count++))  # Increment success count
  fi
}

#Run each command with the function
#Test 1
run_command 'echo "y" | ./autogenerate.sh git@github.com:rdkcentral/rdk-halif-device_settings.git'
run_command './autogenerate.sh -c'
sleep 2

#Test 2
run_command 'echo "y" | ./autogenerate.sh https://github.com/rdkcentral/RdkWanManager.git'
run_command './autogenerate.sh -c'
sleep 2

#Test 3
git clone https://github.com/rdkcentral/RdkWanManager.git rdkwanmanager
run_command 'echo "y" | ./autogenerate.sh $(readlink -f rdkwanmanager/)'
run_command './autogenerate.sh -c'
rm -rf rdkwanmanager/
sleep 2

#Test 4
run_command './autogenerate.sh https://github.com/rdkcentral/RdkWanManager.git <<EOF
N
https://github.com/rdkcentral/RdkWanManager.git

EOF'
run_command './autogenerate.sh -c'
sleep 2

#Test 5
git clone https://github.com/rdkcentral/RdkWanManager.git rdkwanmanager
run_command './autogenerate.sh https://github.com/rdkcentral/RdkWanManager.git <<EOF
N

$(readlink -f rdkwanmanager/)
EOF'
run_command './autogenerate.sh -c'
rm -rf rdkwanmanager/
sleep 2

#Test 6
run_command 'echo "y" | ./autogenerate.sh https://github.com/rdkcentral/RdkWanManager.git -b RDKB-56270-gtest'
run_command './autogenerate.sh -c'

# Display consolidated results
echo
echo "Total tests run: $total_tests"
echo "Successful tests: $success_count"
echo "Failed tests: $failure_count"
echo
# Give a consolidated result
if [ "$all_success" = true ]; then
  echo "*************************************"
  echo "All commands executed successfully."
  echo "*************************************"
else
  echo "Some commands failed. Check the output above for details."
fi

