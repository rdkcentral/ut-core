#!/usr/bin/env bash

#* *******************************************************************************************
#*
#*   ** Project      : Unit Test Script
#*   ** @addtogroup  : ut
#*   ** @file        : shell
#*   ** @author      : anjali.thampi@sky.uk
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

        # Variables for basic actions
        AGT_TASK_START="START"
        AGT_TASK_END="END"
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

function AGT_task_message()
{
        local message
        message=$2
        case $1 in
                ${AGT_TASK_START}) AGT_INFO_GREEN "\nSTARTED: ${message}"
                echo -e ""
                ;;
                ${AGT_TASK_END}) AGT_INFO_GREEN "\nCOMPLETED: ${message}"
                echo -e ""
                ;;
        esac
}

# Colour Codes methods
function AGT_INFO()
{
	echo -e ${BLUE}$1 ${BLUE}$2 ${BLUE}$3 ${BLUE}$4
}

function AGT_INFO_BLUE()
{
	echo -e ${BLUE}$1 ${BLUE}$2 ${BLUE}$3 ${BLUE}$4
}

function AGT_INFO_GREEN()
{
	echo -e ${GREEN}$1${NOCOLOR}$2${GREEN}$3${NOCOLOR}$4
}

function AGT_INFO_YELLOW()
{
	echo -e ${YELLOW}$1 ${YELLOW}$2 ${YELLOW}$3 ${YELLOW}$4 ${YELLOW}$5 ${YELLOW}$6 ${YELLOW}$7 ${YELLOW}$8 ${PURPLE}$9
}

function AGT_INFO_CYAN()
{
	echo -e ${CYAN}$1 ${CYAN}$2 ${CYAN}$3 ${CYAN}$4 ${CYAN}$5 ${CYAN}$6 ${CYAN}$7
}

function AGT_WARNING()
{
	echo -e ${PURPLE}$1${PURPLE}$2${PURPLE}$3${PURPLE}$4
}

function AGT_ERROR()
{
	echo -e ${RED}$1${NOCOLOR}
}


AGT_commons_init $1