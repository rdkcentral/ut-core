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

#* *******************************************************************************************
#*
#*   ** Project      : Unit Test Script
#*   ** @addtogroup  : ut
#*   ** @file        : shell
#*   ** @date        : 12/01/2023
#*   **
#*   ** @brief : Shell script for common functions and variable for all autogeneration scripts
#*   **
#*
#* ********************************************************************************************

# Colour Codes
NOCOLOR="\e[39m"
YELLOW="\e[1;33m"
RED="\e[1;31m"
GREEN="\e[1;32m"
CYAN="\e[1;36m"
BLUE="\e[1;34m"
PURPLE="\e[1;35m"

# Init function for common variables shared across the scripts
function AGT_commons_init()
{
        # Dir for where the scripts are housed
        if [ -L $0 ] ; then
        AGT_SCRIPTS_HOME=$(cd "$(dirname "$(readlink $0)")"; pwd -P) # for symbolic link
        else
        AGT_SCRIPTS_HOME=$(cd "$(dirname "$0")"; pwd -P) # for normal file
        fi

        # Variables set for existence of dirs
        AGT_WORKSPACE_EXISTS=false
        AGT_APIDEF_EXISTS=false
        AGT_UT_EXISTS=false
        AGT_CMOCK_EXISTS=false
        AGT_MOCKS_EXISTS=false

        # Base dir of where the scripts are in ut-core
        AGT_BASEDIR=${AGT_SCRIPTS_HOME}/..
        # Workspace dir in ut-core where the script generates the tests
        AGT_UT_WORKSPACE=${AGT_BASEDIR}/workspace
        # API Definition name inside the worksapce dir
        AGT_APIDEF_NAME=`echo ${1##*/}| cut -d'.' -f1`
        # Base URL for git repos
        AGT_BASE_URL=`echo ${1} | sed 's|\(.*\)/.*|\1|'`
        # API Definition home inside the worksapce dir
        AGT_APIDEF_HOME=${AGT_UT_WORKSPACE}/${AGT_APIDEF_NAME}
        # Include dir inside worksapce dir
        AGT_INCLUDE_DIR=${AGT_APIDEF_HOME}/include
        # UT dir inside worksapce dir 
        AGT_UT_HOME=${AGT_APIDEF_HOME}/ut
        # Cmock dir inside worksapce dir
        AGT_CMOCK_DIR=${AGT_UT_HOME}/cmock
         # Mocks dir inside worksapce dir 
        AGT_MOCKS_DIR=${AGT_UT_HOME}/mocks
        # Skeletons dir inside worksapce dir
        AGT_SKELETONS_DIR=${AGT_UT_HOME}/skeletons
        # Skeleton dir's child dir, src inside worksapce dir
        AGT_SKELETONS_SRC=${AGT_SKELETONS_DIR}/src

}

# Function to update the variables based on their particular directory's existence
function AGT_update_variables()
{
	case $1 in
                ${AGT_UT_WORKSPACE})
                        if [ -d "${AGT_UT_WORKSPACE}" ]; then
                                AGT_WORKSPACE_EXISTS=true
                        fi
                ;;
                ${AGT_APIDEF_HOME})
                        if [ -d "${AGT_APIDEF_HOME}" ]; then
                                AGT_APIDEF_EXISTS=true
                        fi
                ;;
                ${AGT_UT_HOME})
                        if [ -d "${AGT_UT_HOME}" ]; then
                                AGT_UT_EXISTS=true
                        fi
                ;;
                ${AGT_CMOCK_DIR})
                        if [ -d "${AGT_CMOCK_DIR}" ]; then
                                AGT_CMOCK_EXISTS=true
                        fi
                ;;
                ${AGT_MOCKS_DIR})
                        if [ -d "${AGT_MOCKS_DIR}" ]; then
                                AGT_MOCKS_EXISTS=true
                        fi
                ;;
	esac
}

function AGT_DEBUG_START()
{
        if [ ! -z "${AGT_DEBUG}" ]; then
                local message
                message=$1
                AGT_INFO "\nSTARTED: ${message}\n"
                #echo -e ""
        fi
}

function AGT_DEBUG_END()
{
        if [ ! -z "${AGT_DEBUG}" ]; then
                local message
                message=$1
                AGT_INFO "\nCOMPLETED: ${message}\n"
                #echo -e ""
        fi
}

# Colour Codes methods

function AGT_INFO()
{
	echo -e ${GREEN}$1${GREEN}$2${GREEN}$3${NOCOLOR}$4
}

function AGT_ALERT()
{
	echo -e ${YELLOW}$1 ${YELLOW}$2 ${YELLOW}$3 ${YELLOW}$4${YELLOW}$5${YELLOW}$6${YELLOW}$7${YELLOW}$8${NOCOLOR}$9

}

function AGT_SUCCESS()
{
	echo -e ${CYAN}$1${CYAN}$2${CYAN}$3${CYAN}$4${CYAN}$5${CYAN}$6${CYAN}$7${NOCOLOR}$8
}

function AGT_WARNING()
{
	echo -e ${PURPLE}$1${PURPLE}$2${PURPLE}$3${PURPLE}$4${NOCOLOR}$5
}

function AGT_ERROR()
{
	echo -e ${RED}$1${NOCOLOR}
}


AGT_commons_init $1