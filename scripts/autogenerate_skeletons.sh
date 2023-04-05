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
	AGT_INFO_GREEN "Installing cmock tool required for skeletons' generation .."

        # Install latest cmock tool if it does not already exists in the dir
	if [ "${AGT_CMOCK_EXISTS}" = false ]; then
		git clone --recursive https://github.com/throwtheswitch/cmock.git ${AGT_CMOCK_DIR}
		cd ${AGT_CMOCK_DIR}
		bundle install
		AGT_INFO "CMock is now installed"
		AGT_CMOCK_EXISTS=true
		cd -
	else
		AGT_INFO_YELLOW "Cmock dir in UT already EXIST."
	fi

        AGT_INFO_CYAN "Installing cmock tool required for skeletons' generation .. .. COMPLETED"
}

# Function to delete cmock dir
AGT_delete_cmock()
{
	AGT_INFO_GREEN "Deleting cmock tool required for skeletons' generation .."

        # If cmock directory exists, delete it
        if [ "${AGT_CMOCK_EXISTS}" = true ]; then
                ${AGT_RMRF} ${AGT_CMOCK_DIR}
                AGT_INFO "The cmock directory  is now DELETED"
                AGT_CMOCK_EXISTS=false
        else
                AGT_INFO "The cmock directory does NOT EXIST"
        fi

        AGT_INFO_CYAN "Deleting cmock tool required for skeletons' generation .. COMPLETE"
}

# Function to delete mocks dir
AGT_delete_mocks()
{
	AGT_INFO_GREEN "Deleting deprecated mocks .."

        # If mocks dir exist , prompt user to allow deletion of the same
        if [ "${AGT_MOCKS_EXISTS}" = true ]; then
                # Check with user if the deprecated mocks dir can be deleted
                AGT_get_user_response ${AGT_DELETE_MOCKS}
                # If user response is "YES", delete mocks dir
                if [ "${AUTO_RESPONSE}" = ${AGT_YES} ]; then
                        ${AGT_RMRF} ${AGT_MOCKS_DIR}
                        AGT_INFO "The mocks directory is now DELETED"
                        AGT_CMOCK_EXISTS=false
                # If user response is "YES", keep the mocks dir and warn user again
                else
                        AGT_WARNING "NOT DELETED mocks directory"
                        AGT_WARNING "Again, it is highly recommended to delete the deprecated mocks directory!"
                fi
        # Display to user that mocks dir didn't exist
        else
                AGT_INFO "The mocks directory does NOT EXIST"
        fi

	AGT_INFO_CYAN "Deleting deprecated mocks .. COMPLETE"
}

# Function to generate skeletons
function AGT_generate_skeletons()
{
        AGT_INFO_GREEN "Generating skeletons .."

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

        AGT_INFO "Skeletons are created successfully"
        cd - 2>/dev/null

        # Delete cmock tool which was required to generate skeletons
        AGT_delete_cmock

        AGT_INFO_CYAN "Generating skeletons .. COMPLETE"
}

# Check the status of skeletons created for the API Defintion's header files
function AGT_check_skeleton_status()
{
        # If skeleton dir does not exist, create it along with its src child dir
        if [ ! -d ${AGT_SKELETONS_DIR} ]; then
                ${AGT_MKDIR_IF_EXISTS} ${AGT_SKELETONS_SRC}
                AGT_INFO "The skeletons' directory is now created"
        else
                # If skeleton's child src dir exist
                if [ -d ${AGT_SKELETONS_SRC} ]; then
                        # If src dir is not empty, check with user if the skeletons need to be rewritten
                        if [ ! -z "$(ls -A ${AGT_SKELETONS_SRC})" ]; then
                                AGT_WARNING "The skeletons' src directory IS NOT EMPTY"
                                AGT_get_user_response ${AGT_REWRITE}
                                # If user response is NO, then exit this function
                                if [ "${AUTO_RESPONSE}" = ${AGT_NO} ]; then
                                        return 0
                                fi
                        else
                                AGT_INFO "The skeletons' src directory IS EMPTY"
                        fi
                # If skeleton's child src dir does not exist, create it
                else
                        ${AGT_MKDIR_IF_EXISTS} ${AGT_SKELETONS_SRC}
                        AGT_INFO "The skeletons' src directory  is now created"
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