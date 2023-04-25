#!/usr/bin/env bash

#* ******************************************************
#*
#*   ** Project      : Unit Test Script
#*   ** @addtogroup  : ut
#*   ** @file        : shell
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
	AGT_DEBUG_START "Installing cmock tool required for skeletons' generation"

        # Install latest cmock tool if it does not already exists in the dir
        git clone --recursive https://github.com/throwtheswitch/cmock.git ${AGT_CMOCK_DIR} &> /dev/null
        cd ${AGT_CMOCK_DIR}
        bundle install &> /dev/null
        AGT_SUCCESS "CMock is now installed"
        cd - &> /dev/null

        AGT_DEBUG_END "Installing cmock tool required for skeletons' generation"
}

# Function to delete mocks dir
AGT_delete_mocks()
{
	AGT_DEBUG_START "Deleting deprecated mocks"

        # If deprecated mocks dir exists, delete it
        if [ "${AGT_MOCKS_EXISTS}" = true ]; then
                rm -rf ${AGT_MOCKS_DIR}
                AGT_WARNING "The mocks directory is now DELETED"
                AGT_MOCKS_EXISTS=false
        # Display to user that mocks dir didn't exist
        else
                AGT_ALERT "The mocks directory does NOT EXIST"
        fi

	AGT_DEBUG_END "Deleting deprecated mocks"
}

# Function to generate skeletons
function AGT_generate_skeletons()
{
        local COUNT_SRC
        AGT_DEBUG_START "Generating skeletons"

        # Install cmock tool required to generate skeletons
        AGT_install_cmock

        # Run the skeleton generate command
        cd ${AGT_UT_HOME}
        ruby ${AGT_CMOCK_DIR}/lib/cmock.rb --skeleton --mock_path=${AGT_SKELETONS_DIR} ${AGT_INCLUDE_DIR}/*.h &> /dev/null

        cd ${AGT_SKELETONS_DIR}

        # Comment cmock include in all generated files
        sed -i '/^#include \"cmock.h\"/d' *.*

        # Move source files to src directory
        COUNT_SRC=`ls -1 *.c 2>/dev/null | wc -l`
        if [ ${COUNT_SRC} != 0 ]; then
                mv *.c src/
        fi

        AGT_SUCCESS "Skeletons are generated successfully"
        cd - &> /dev/null

        # Delete cmock tool which was required to generate skeletons
        AGT_DEBUG_START "Deleting cmock tool required for skeletons' generation"
        rm -rf ${AGT_CMOCK_DIR}
        AGT_DEBUG_END "Deleting cmock tool required for skeletons' generation"

        AGT_DEBUG_END "Generating skeletons"
}

# Check the status of skeletons created for the API Defintion's header files
function AGT_set_up_skeletons()
{
        AGT_DEBUG_START "Creating skeletons and its src directory (if they don't exist)"
        mkdir -p ${AGT_SKELETONS_SRC}
        AGT_DEBUG_END "Creating skeletons and its src directory (if they don't exist)"
        if [ ! -z "$(ls -A ${AGT_SKELETONS_SRC})" ]; then
                AGT_WARNING "The skeletons' src directory IS NOT EMPTY"
                AGT_DEBUG_START "Deleting skeletons' src files"
                rm ${AGT_SKELETONS_SRC}/*
                AGT_DEBUG_END "Deleting skeletons' src files"
        fi
}

# Init function for skeletons' generation
function AGT_skeletons_init()
{
        # API_DEF url as first command line argument
        API_DEF_URL=$1

        # Source the script with the shared variables and functions
        . autogenerate_shared.sh ${API_DEF_URL}

        # Call the shared function to update the respective variables
        AGT_update_variables ${AGT_CMOCK_DIR}
        AGT_update_variables ${AGT_MOCKS_DIR}
}

AGT_skeletons_init $1
# Delete Mocks dir (if it exists)
AGT_delete_mocks
AGT_set_up_skeletons
# Generate skeletons
AGT_generate_skeletons
