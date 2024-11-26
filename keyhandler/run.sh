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

set -e # error out if required

# Check if $@ is empty
if [ $# -eq 0 ]; then
  echo "Error: No arguments provided."
  echo "Usage: ${0} -c <event_code> -t <event_type> -v <value>"
  exit 1
fi

if [ -d ../build/linux/lib ]; then
  cp ../build/linux/lib/libut_control.so .
fi
export LD_LIBRARY_PATH=/usr/lib:/lib:/home/root:.
./ut_core_key_input -p input-event-codes-5.15.yaml $@
