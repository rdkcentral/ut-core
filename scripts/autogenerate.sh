#!/usr/bin/env bash

#* *************************************************************************************************
#*
#*   ** Project      : Unit Test Script
#*   ** @addtogroup  : ut
#*   ** @file        : shell
#*   ** @author      : anjali.thampi@sky.uk
#*   ** @date        : 12/01/2023
#*   **
#*   ** @brief : Shell script to initiate autogeneration of skeletons and L1 and L2 tests' framework
#*   **
#*
#* **************************************************************************************************

set -e

# Validates if the url is correct and exits script if it isn't
AGT_validate_url()
{
	result=$(git clone $1  --depth 1 -q AGT_clone_check; echo $?)
	if [ ${result} != 0 ]; then
		AGT_ERROR "Unknown url : '$1'"
		${AGT_EXIT_ERROR}
	fi
	${AGT_RMRF} AGT_clone_check
}

# Function to create workspace dir
function AGT_create_workspace()
{
	AGT_INFO_GREEN "Creating Workspace .."

	# Create workspace if it doesnt exist already
	if [ ${AGT_WORKSPACE_EXISTS} = false ]; then
		${AGT_MKDIR_IF_EXISTS} ${AGT_UT_WORKSPACE}
		AGT_INFO "Workspace directory [../workspace/] is now created"
		AGT_WORKSPACE_EXISTS=true
	else
		AGT_INFO_YELLOW "Workspace directory already exists"
	fi

	AGT_INFO_CYAN "Creating Workspace .. COMPLETE"
}

# Function to clone the API definition repo
function AGT_clone_apidef()
{
	AGT_INFO_GREEN "Cloning API Definition repo .."

	# Getting the base url and org unit from the url
	echo ${AGT_APIDEF_URL} | sed 's|\(.*\)/.*|\1|' 1>& /dev/null

	# Clone if API Definition dir does not exist already
	if [ "$AGT_APIDEF_EXISTS" = false ]; then
	    AGT_INFO "API Definition url is '${AGT_APIDEF_URL}'"

		cd ${AGT_UT_WORKSPACE}
		git clone ${AGT_APIDEF_URL}
		cd ${AGT_SCRIPTS_HOME}

		AGT_INFO "API Definition repo is now cloned"
	else
		# Exit if include dir is empty
		if [ -d "${AGT_APIDEF_HOME}" ]; then
			if [ -z "$(ls ${AGT_INCLUDE_DIR}/*.h)" ]; then
				AGT_ERROR "${AGT_APIDEF_NAME}'s API Definition include dir exists, but HAS NO HEADER FILES"
				${AGT_EXIT_ERROR}
			fi
		else
			# Exit if include dir does not exist
			AGT_ERROR "${AGT_APIDEF_NAME}'s API Definition include dir DOES NOT EXIST"
			${AGT_EXIT_ERROR}
		fi
	fi
	AGT_APIDEF_EXISTS=true

	AGT_INFO_CYAN "Cloning API Definition repo .. COMPLETE "
}

# Function to checkout API Definition branch if -b option is specified
function AGT_checkout_branch()
{
	# Exit with error if remote branch does not exist
	if [ ! `git ls-remote --heads "${AGT_APIDEF_URL}" "${AGT_APIDEF_BRANCH}" | awk '{print $2}' ` ]; then
		AGT_ERROR "Branch [${AGT_APIDEF_BRANCH}] does not exist!"
		${AGT_EXIT_ERROR}
	fi

	cd ${AGT_APIDEF_HOME}
	git checkout ${AGT_APIDEF_BRANCH}
	AGT_INFO_GREEN "Branch [${AGT_APIDEF_BRANCH}] is now checked out"
	cd -
}

# Function to clone the ut-core and doxygen repos to copy their templates
function AGT_clone_doxygen_repo()
{
	AGT_INFO_GREEN "Cloning doxygen repo .."

	if [ ! -d "${AGT_UT_WORKSPACE}/${AGT_DOXYGEN_DIR}"  ]; then
		# Clone doxygen template into doxy dir in workspace/api-def dir
		git clone ${AGT_BASE_URL}/${AGT_DOXY_REPO} ${AGT_UT_WORKSPACE}/${AGT_DOXYGEN_DIR}
		AGT_INFO_BLUE "Doxygen repo has been cloned in [../workspace/${AGT_DOXYGEN_DIR}]"
	fi

	cd ${AGT_SCRIPTS_HOME}

	AGT_INFO_CYAN "Cloning doxygen repo .. COMPLETE"
}

# Function to copy from templates both for ut-core(API Definition/ut template) and doxygen (docs template)
# input - First param is the path for the template
# input - Second param is location to be copied to
function AGT_copy_and_display_uncopied_files()
{
		directory=$1
		template=$2

		# Copy only missing files or files whose timestamps in the source are newer
		case "$directory" in
			${AGT_DIR_APIDEF})
				if [[ "$template" == "$AGT_TEMPLATE_APIDEF" ]]; then
				    # Copy all files as source and not symbolic links
					cp -Lurv ${AGT_BASEDIR}/${AGT_APIDEF_TEMPLATE_PATH}/* .
				else
				    # If docs or docs/pages or docs/images don't exist, create them
				    ${AGT_MKDIR_IF_EXISTS} ${AGT_DOCS_PAGES_DIR}
				    ${AGT_MKDIR_IF_EXISTS} ${AGT_DOCS_PAGES_DIR}/images
					cp -urv ${AGT_UT_WORKSPACE}/${AGT_DOXYGEN_DIR}/${AGT_DOXYGEN_GENERATE_FILE} ./${AGT_DOCS_DIR}
					cp -urv ${AGT_UT_WORKSPACE}/${AGT_DOXYGEN_DIR}/${AGT_DOXYGEN_TEMPLATE_FILES} ./${AGT_DOCS_PAGES_DIR}
				fi
			;;
			${AGT_DIR_UT})
				if [[ "$template" = "$AGT_TEMPLATE_UT" ]]; then
				    # Copy all files from ut template
					cp -urv ${AGT_BASEDIR}/${AGT_UT_TEMPLATE_PATH}/* .
					cd ${AGT_BASEDIR}/${AGT_UT_TEMPLATE_PATH}
					# Get the files (with .md extenstion) common to ${AGT_UT_HOME} and UT's docs/pages folder
					# CONTRIBUTING, README, NOTICE, LICENSE
				    `comm -12 <(ls | sed -e 's/\.md$//' ) <(ls ./docs/pages | sed -e 's/\.md$//' ) > ${AGT_TEMP_FILE1}`
					# Get list of files in UT's docs/pages folder
					`ls ./${AGT_DOCS_PAGES_DIR} | sed -e 's/\.md$//' > ${AGT_TEMP_FILE2}`
					for file in `cat ${AGT_TEMP_FILE2}`
					do
					    # If the file in UT's docs/pages folder is not CONTRIBUTING, README, NOTICE, LICENSE
						# then copy source file and not symbolic link
						if [ -z `grep $file ${AGT_TEMP_FILE1}` ]; then
							cp -Lurv ./${AGT_DOCS_PAGES_DIR}/$file* ${AGT_UT_HOME}/${AGT_DOCS_PAGES_DIR}/
						else
							cp -urv ./${AGT_DOCS_PAGES_DIR}/$file* ${AGT_UT_HOME}/${AGT_DOCS_PAGES_DIR}/
						fi
					done
					${AGT_RM} ${AGT_TEMP_FILE1} ${AGT_TEMP_FILE2}
					cd -
				else
				        # If docs or docs/pages or docs/images don't exist, create them
				        ${AGT_MKDIR_IF_EXISTS} ${AGT_DOCS_PAGES_DIR}
				        ${AGT_MKDIR_IF_EXISTS} ${AGT_DOCS_PAGES_DIR}/images
					cp -urv ${AGT_UT_WORKSPACE}/${AGT_DOXYGEN_DIR}/${AGT_DOXYGEN_GENERATE_FILE} ./${AGT_DOCS_DIR}
				fi
			;;
		esac
}

# Function to copy the templates to the respective folders
function AGT_copy_templates()
{
	AGT_INFO_GREEN "Copying template for '${1}' (if missing any).."

	case "$1" in
		${AGT_DIR_APIDEF})
			AGT_INFO_BLUE "Copying from ut-core template .."
			cd ${AGT_APIDEF_HOME}
			# Copy the missing files and dirs from ut-core template
			AGT_copy_and_display_uncopied_files ${AGT_DIR_APIDEF} ${AGT_TEMPLATE_APIDEF}
			AGT_INFO_GREEN "Copying from ut-core template .. COMPLETE"

			AGT_INFO_BLUE "Copying from doxygen template .."
			# Copy the missing files and dirs from doxygen template
			AGT_copy_and_display_uncopied_files ${AGT_DIR_APIDEF} ${AGT_TEMPLATE_DOXYGEN}
			cd ${AGT_SCRIPTS_HOME} &> /dev/null

			AGT_INFO_GREEN "Copying from doxygen template .. COMPLETE"
		;;
		${AGT_DIR_UT})
			AGT_INFO_BLUE "Copying from ut-core template .."
			cd ${AGT_UT_HOME}
			# Copy the missing files and dirs from ut-core template
			AGT_copy_and_display_uncopied_files  ${AGT_DIR_UT} ${AGT_TEMPLATE_UT}
			AGT_INFO_GREEN "Copying from ut-core template .. COMPLETE"
			AGT_INFO_BLUE "Copying from doxygen template .."
			# Copy the missing files and dirs from doxygen template
			AGT_copy_and_display_uncopied_files ${AGT_DIR_UT} ${AGT_TEMPLATE_DOXYGEN}
			cd ${AGT_SCRIPTS_HOME} &> /dev/null
			AGT_INFO_GREEN "Copying from doxygen template .. COMPLETE"
		;;
		*) AGT_ERROR "Invalid input"
			${AGT_EXIT_ERROR}
		;;

	esac

	AGT_INFO_CYAN "Copying template for '${1}'.. COMPLETE"
}

# Function to clone the ut dir
function AGT_clone_ut()
{
	AGT_INFO_GREEN "Building UT .."

	# If ut dir exists, then exit this function
	if [ "${AGT_UT_EXISTS}" = true ]; then
		AGT_WARNING "UT dir already EXISTS"
		return 0
	fi

	# Default UT repo url
	UT_URL=$(echo "$AGT_APIDEF_URL" | sed 's,hal,haltest,g')
	AGT_WARNING " The url for UT clone is : ${UT_URL} "

	# Check with user if ut repo url is correct , else get the correct one
	read -p "$( AGT_INFO_BLUE Is this correct? )" yn
	case $yn in
		[Yy]* ) ;;
		[Nn]* ) read -p "$(AGT_INFO_YELLOW Please input the correct url : ) " UT_URL
		;;
		* ) AGT_WARNING "No input added. Keeping original url "
		;;
	esac


	AGT_INFO "UT url is '${UT_URL}'"
	# Validate if UT url is correct
	AGT_validate_url ${UT_URL} ${AGT_UT}
	# Clone the UT repo
	git clone ${UT_URL} ${AGT_APIDEF_HOME}/ut
	AGT_INFO "UT repo is now cloned"
	AGT_UT_EXISTS=true

	AGT_INFO_CYAN "Building UT .. COMPLETE"
}

# Function to autogenerate the tests framework
AGT_generate_all()
{
	AGT_create_workspace
	AGT_clone_apidef
	if [ ! -z "${AGT_APIDEF_BRANCH}" ]; then
		AGT_checkout_branch
	fi
	AGT_clone_doxygen_repo
	AGT_copy_templates ${AGT_DIR_APIDEF}
	AGT_clone_ut
	AGT_copy_templates ${AGT_DIR_UT}
	./autogenerate_skeletons.sh ${AGT_APIDEF_URL}
	./autogenerate_tests.sh ${AGT_APIDEF_URL}
	AGT_INFO_YELLOW "GENERATED TESTS AVAILABLE IN [../workspace/${AGT_APIDEF_NAME}/ut/src]"
}

# Function to clear workspace dir
AGT_clean_workspace()
{
	AGT_INFO_GREEN "Deleting workspace .."

	# Delete workspace dir if it exists
	if [ "${AGT_WORKSPACE_EXISTS}" = true ]; then
		${AGT_RMRF} ${AGT_UT_WORKSPACE}
		AGT_INFO "The workspace directory is now DELETED"
	else
		AGT_INFO_YELLOW "The workspace directory does not exist"
	fi

	AGT_INFO_CYAN "Deleting workspace .. COMPLETE"
}

# Command Usage
function AGT_show_usage()
{
	AGT_INFO "Autogen Script : Please run this script from inside the scripts dir"
	AGT_INFO_YELLOW "./autogenerate.sh <api-def-repo-url> <clean/help/branch>"
	AGT_INFO_GREEN "	api-def-repo-url - The API Definition url"
	AGT_INFO_GREEN "	clean/-c - Cleans the workspace directory"
	AGT_INFO_GREEN "	branch/-b - Use this switch along with API definition branch to be checked out"
	AGT_INFO_GREEN "	help/-h - Shows the usage"
	echo
}

# Function for the switch cases for second command line argument
function AGT_decode_switches()
{
	for switch in "$2";
	do
		case "$switch" in
			"clean"|"-c")
				AGT_clean_workspace
				${AGT_EXIT_SUCCESS};
			;;

			"help"|"-h")
				AGT_show_usage
				${AGT_EXIT_SUCCESS};
			;;

			"branch"|"-b")
				if [ ! -z "$3" ]; then
					AGT_APIDEF_BRANCH=$3
				else
					AGT_ERROR "Branch name is empty"
					AGT_show_usage
					${AGT_EXIT_ERROR}
				fi

			;;

			[a-zA-Z0-9]|[^a-zA-Z0-9]|*)
				AGT_ERROR "[$switch] unknown switch"
				AGT_show_usage
				${AGT_EXIT_ERROR}
			;;

		esac
	done
	AGT_generate_all
}

# Init function for autogen
function AGT_init()
{
	# URL of API definition repo to be cloned as first command line argument
	AGT_APIDEF_URL=$1

	# Call file which has the common variables and function
	source autogenerate_shared.sh ${AGT_APIDEF_URL}

	# If no arguments provided, then exit script
	if [ $# -eq 0 ]; then
		>&2 AGT_ERROR "No arguments provided"
		AGT_show_usage
		${AGT_EXIT_ERROR}
	fi

	# Variables
	AGT_UT="UT_url"
	AGT_APIDEF="API_definition_url"
	AGT_DOXY_REPO="rdk-components-hal-doxygen.git"
	AGT_APIDEF_BRANCH=""
	AGT_DOXYGEN_DIR=`echo "$(basename "$AGT_DOXY_REPO" .git)"`
	# Dir path for API Defintion template in ut-core
	AGT_APIDEF_TEMPLATE_PATH="template/api_definition_template"
	 # Dir path for UT template in ut-core
	AGT_UT_TEMPLATE_PATH="template/ut_template"
	AGT_DOCS_DIR="docs"
	AGT_DOCS_PAGES_DIR="${AGT_DOCS_DIR}/pages"
	AGT_DOCS_IMAGES_DIR="${AGT_DOCS_PAGES_DIR}/images"
	AGT_DOXYGEN_TEMPLATE_FILES="template/${AGT_DOCS_PAGES_DIR}/*.md"
	AGT_DOXYGEN_GENERATE_FILE="template/docs/generate_docs.sh"

	AGT_DIR_APIDEF="API_DEF"
	AGT_DIR_UT="UT"
	AGT_TEMPLATE_APIDEF="API_Definition_template"
	AGT_TEMPLATE_UT="UT_template"
	AGT_TEMPLATE_DOXYGEN="Doxygen_template"
	AGT_TEMP_FILE1="AGT_TEMP_FILE1.sh"
	AGT_TEMP_FILE2="AGT_TEMP_FILE2.sh"
	AGT_validate_url ${AGT_APIDEF_URL} ${AGT_APIDEF}

	# Call the shared function to update the respective variables
	AGT_update_variables ${AGT_UT_WORKSPACE}
	AGT_update_variables ${AGT_APIDEF_HOME}
	AGT_update_variables ${AGT_UT_HOME}
}

AGT_init $1
AGT_decode_switches $@
