#!/usr/bin/env bash

#* ******************************************************
#*
#*   ** Project      : Unit Test Script
#*   ** @addtogroup  : ut
#*   ** @file        : shell
#*   ** @author      : anjali.thampi@sky.uk
#*   ** @date        : 12/01/2023
#*   **
#*   ** @brief : Shell script to autogenerate skeletons
#*   **
#*
#* ******************************************************

set -e

# Function to install cmock dir
function AGT_install_cmock()
{
	AGT_task_message ${AGT_TASK_START} "Installing cmock tool required for skeletons' generation"

        # Install latest cmock tool if it does not already exists in the dir
	if [ "${AGT_CMOCK_EXISTS}" = false ]; then
		git clone --recursive https://github.com/throwtheswitch/cmock.git ${AGT_CMOCK_DIR}
		cd ${AGT_CMOCK_DIR}
		bundle install
		AGT_INFO_CYAN "CMock is now installed"
		AGT_CMOCK_EXISTS=true
		cd -
	else
		AGT_INFO_YELLOW "Cmock dir in UT already EXIST."
	fi

        AGT_task_message ${AGT_TASK_END} "Installing cmock tool required for skeletons' generation"
}

# Function to delete cmock dir
AGT_delete_cmock()
{
	AGT_task_message ${AGT_TASK_START} "Deleting cmock tool required for skeletons' generation"

        # If cmock directory exists, delete it
        if [ "${AGT_CMOCK_EXISTS}" = true ]; then
                rm -rf ${AGT_CMOCK_DIR}
                AGT_INFO_CYAN "The cmock directory  is now DELETED"
                AGT_CMOCK_EXISTS=false
        else
                AGT_INFO_YELLOW "The cmock directory does NOT EXIST"
        fi

        AGT_task_message ${AGT_TASK_END} "Deleting cmock tool required for skeletons' generation"
}

# Function to delete mocks dir
AGT_delete_mocks()
{
	AGT_task_message ${AGT_TASK_START} "Deleting deprecated mocks"

        # If deprecated mocks dir exists, delete it
        if [ "${AGT_MOCKS_EXISTS}" = true ]; then
                rm -rf ${AGT_MOCKS_DIR}
                AGT_INFO_WARNING "The mocks directory is now DELETED"
                AGT_CMOCK_EXISTS=false
        # Display to user that mocks dir didn't exist
        else
                AGT_INFO_YELLOW "The mocks directory does NOT EXIST"
        fi

	AGT_task_message ${AGT_TASK_END} "Deleting deprecated mocks"
}

# Function to generate skeletons
function AGT_generate_skeletons()
{
        local COUNT_SRC
        AGT_task_message ${AGT_TASK_START} "Generating skeletons"

        # Install cmock tool required to generate skeletons
        AGT_install_cmock

        # Run the skeleton generate command
        cd ${AGT_UT_HOME}
        ruby ${AGT_CMOCK_DIR}/lib/cmock.rb --skeleton --mock_path=${AGT_SKELETONS_DIR} ${AGT_INCLUDE_DIR}/*.h

        cd ${AGT_SKELETONS_DIR}

        # Comment cmock include in all generated files
        sed -i '/^#include \"cmock.h\"/d' *.*

        # Move source files to src directory
        COUNT_SRC=`ls -1 *.c 2>/dev/null | wc -l`
        if [ ${COUNT_SRC} != 0 ]; then
                mv *.c src/
        fi

        AGT_INFO_CYAN "Skeletons are generated successfully"
        cd - 2>/dev/null

        # Delete cmock tool which was required to generate skeletons
        AGT_delete_cmock

        AGT_task_message ${AGT_TASK_END} "Generating skeletons"
}

# Check the status of skeletons created for the API Defintion's header files
function AGT_check_skeleton_status()
{
        # If skeleton dir does not exist, create it along with its src child dir
        if [ ! -d ${AGT_SKELETONS_DIR} ]; then
                mkdir -p ${AGT_SKELETONS_SRC}
                AGT_INFO_CYAN "The skeletons' directory is now created"
        else
                # If skeleton's child src dir exist
                if [ -d ${AGT_SKELETONS_SRC} ]; then
                        # Check if src dir is not empty
                        if [ ! -z "$(ls -A ${AGT_SKELETONS_SRC})" ]; then
                                AGT_WARNING "The skeletons' src directory IS NOT EMPTY"
                        fi
                # If skeleton's child src dir does not exist, create it
                else
                        mkdir -p ${AGT_SKELETONS_SRC}
                        AGT_INFO_CYAN "The skeletons' src directory is now created"
                fi
        fi

        # Generate skeletons
        AGT_generate_skeletons
}

# Init function for skeletons' generation
function AGT_skeletons_init()
{
        # API_DEF url as first command line argument
        API_DEF_URL=$1

        # Source the script with the shared variables and functions
        source autogenerate_shared.sh ${API_DEF_URL}

        # Call the shared function to update the respective variables
        AGT_update_variables ${AGT_CMOCK_DIR}
        AGT_update_variables ${AGT_MOCKS_DIR}

        # Delete Mocks dir (if it exists)
        AGT_delete_mocks
}

AGT_skeletons_init $1
AGT_check_skeleton_status