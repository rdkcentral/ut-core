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

#* *************************************************************************************************
#*
#*   ** Project      : Unit Test Script
#*   ** @addtogroup  : ut
#*   ** @file        : shell
#*   ** @date        : 12/01/2023
#*   **
#*   ** @brief : Shell script to initiate autogeneration of skeletons and L1 and L2 tests' framework
#*   **
#*
#* **************************************************************************************************

set -e

# Minimum required versions
MIN_RUBY_VERSION="2.7.0"
MIN_BUNDLER_VERSION="2.4.17"

# Function to compare two version numbers
AGT_version_ge() {
    # Returns 0 (true) if $1 >= $2
    [ "$(printf '%s\n' "$1" "$2" | sort -V | head -n1)" == "$2" ]
}

# Function to check Ruby version
AGT_check_ruby_version() {
    # Get the installed Ruby version
    INSTALLED_RUBY_VERSION=$(ruby -v 2>/dev/null | awk '{print $2}')
    if AGT_version_ge "$INSTALLED_RUBY_VERSION" "$MIN_RUBY_VERSION"; then
        AGT_INFO "Ruby version $INSTALLED_RUBY_VERSION meets the minimum requirement of $MIN_RUBY_VERSION."
    else
        AGT_ERROR "Ruby version $MIN_RUBY_VERSION or higher is required. Installed version is $INSTALLED_RUBY_VERSION."
        exit 1
    fi
}

# Function to check Bundler version
AGT_check_bundler_version() {
    # Get the installed Bundler version
    INSTALLED_BUNDLER_VERSION=$(bundle -v 2>/dev/null | awk '{print $3}')
    if AGT_version_ge "$INSTALLED_BUNDLER_VERSION" "$MIN_BUNDLER_VERSION"; then
        AGT_INFO "Bundler version $INSTALLED_BUNDLER_VERSION meets the minimum requirement of $MIN_BUNDLER_VERSION."
    else
        AGT_ERROR "Bundler version $MIN_BUNDLER_VERSION or higher is required. Bundler is not installed or the installed version is $INSTALLED_BUNDLER_VERSION."
        exit 1
    fi
}

# Validate if the branch is correct and exit script if it isn't
AGT_validate_branch()
{
	local url=$1
	local branch=$2
	# Exit with error if remote branch does not exist
	if [ ! `git ls-remote --heads "${url}" "${branch}" | awk '{print $2}' ` ]; then
		AGT_ERROR "Branch [${branch}] does not exist!"
		exit 1
	fi
}

# Clone the API definition repo
function AGT_clone_apidef()
{
	local url=$1
	AGT_DEBUG_START "Cloning API Definition repo"

	# Getting the base url and org unit from the url
	echo ${url} | sed 's|\(.*\)/.*|\1|' 1>& /dev/null

	# Clone if API Definition dir does not exist already
	if [ "$AGT_APIDEF_EXISTS" = false ]; then
	    AGT_INFO "API Definition url is '${url}'"

		cd ${AGT_UT_WORKSPACE}
		git clone ${url} &> /dev/null
		cd ${AGT_SCRIPTS_HOME}

		AGT_SUCCESS "API Definition repo is now cloned"
	else
		# Exit if include dir is empty
		if [ -d "${AGT_APIDEF_HOME}" ]; then
			if [ -z "$(ls ${AGT_INCLUDE_DIR}/*.h)" ]; then
				AGT_ERROR "${AGT_APIDEF_NAME}'s API Definition include dir exists, but HAS NO HEADER FILES"
				exit 1
			fi
		else
			# Exit if include dir does not exist
			AGT_ERROR "${AGT_APIDEF_NAME}'s API Definition include dir DOES NOT EXIST"
			exit 1
		fi
	fi
	AGT_APIDEF_EXISTS=true

	AGT_DEBUG_END "Cloning API Definition repo"
}

# Clone the ut-core and doxygen repos to copy their templates
function AGT_clone_doxygen_repo()
{
	AGT_DEBUG_START "Cloning doxygen repo"

	if [ ! -d "${AGT_DOXYGEN_DIR}"  ]; then
		# Clone doxygen template into doxy dir in workspace/api-def dir
		git clone git@github.com:rdkcentral/hal-doxygen.git ${AGT_DOXYGEN_DIR} &> /dev/null
		AGT_SUCCESS "Doxygen repo has been cloned in [../workspace/${AGT_DOXYGEN_REPO_NAME}]"
	fi

	cd ${AGT_SCRIPTS_HOME}

	AGT_DEBUG_END "Cloning doxygen repo"
}

# Copy from templates both for ut-core(API Definition/ut template) and doxygen (docs template)
# input - First param is the path for the template
# input - Second param is location to be copied to
function AGT_copy_and_display_uncopied_files()
{
		AGT_DOXYGEN_TEMPLATE_FILES="${AGT_DOXYGEN_DIR}/template/${AGT_DOCS_PAGES_DIR}/*.md"
		AGT_DOXYGEN_GENERATE_FILE="${AGT_DOXYGEN_DIR}/template/docs/generate_docs.sh"
		local AGT_TEMP_FILE1
		local AGT_TEMP_FILE2
		AGT_TEMP_FILE1="AGT_TEMP_FILE1.sh"
		AGT_TEMP_FILE2="AGT_TEMP_FILE2.sh"

		directory=$1
		template=$2

		# Copy only missing files or files whose timestamps in the source are newer
		case "$directory" in
			${AGT_DIR_APIDEF})
				if [[ "$template" == "$AGT_TEMPLATE_APIDEF" ]]; then
				    # Copy all files as source and not symbolic links
					cp -Lrv ${AGT_BASEDIR}/${AGT_APIDEF_TEMPLATE_PATH}/* . &> /dev/null
				else
				    # If docs or docs/pages or docs/images don't exist, create them
				    mkdir -p ${AGT_DOCS_PAGES_DIR}
				    mkdir -p ${AGT_DOCS_PAGES_DIR}/images
					cp -rv ${AGT_DOXYGEN_GENERATE_FILE} ./${AGT_DOCS_DIR} &> /dev/null
					cp -rv ${AGT_DOXYGEN_TEMPLATE_FILES} ./${AGT_DOCS_PAGES_DIR} &> /dev/null
				fi
			;;
			${AGT_DIR_UT})
				if [[ "$template" = "$AGT_TEMPLATE_UT" ]]; then
				    # Copy all files from ut template
					cp -rv ${AGT_UT_TEMPLATE_PATH}/* . &> /dev/null
					cd ${AGT_UT_TEMPLATE_PATH} &> /dev/null
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
							cp -Lrv ./${AGT_DOCS_PAGES_DIR}/$file* ${AGT_UT_HOME}/${AGT_DOCS_PAGES_DIR}/ &> /dev/null
						else
							cp -rv ./${AGT_DOCS_PAGES_DIR}/$file* ${AGT_UT_HOME}/${AGT_DOCS_PAGES_DIR}/ &> /dev/null
						fi
					done
					rm -f ${AGT_TEMP_FILE1} ${AGT_TEMP_FILE2}
					cd - &> /dev/null
				else
				        # If docs or docs/pages or docs/images don't exist, create them
				        mkdir -p ${AGT_DOCS_PAGES_DIR}
				        mkdir -p ${AGT_DOCS_PAGES_DIR}/images
					cp -rv ${AGT_DOXYGEN_GENERATE_FILE} ./${AGT_DOCS_DIR} &> /dev/null
				fi
			;;
		esac
}

# Copy the templates to the respective folders
function AGT_copy_templates()
{
	AGT_DEBUG_START "Copying template for '${1}'"

	case "$1" in
		${AGT_DIR_APIDEF})
			AGT_DEBUG_START "Copying from ut-core template"
			cd ${AGT_APIDEF_HOME} &> /dev/null
			# Copy the missing files and dirs from ut-core template
			AGT_copy_and_display_uncopied_files ${AGT_DIR_APIDEF} ${AGT_TEMPLATE_APIDEF}
			AGT_DEBUG_END "Copying from ut-core template"

			AGT_DEBUG_START "Copying from doxygen template"
			# Copy the missing files and dirs from doxygen template
			AGT_copy_and_display_uncopied_files ${AGT_DIR_APIDEF} ${AGT_TEMPLATE_DOXYGEN}
			cd ${AGT_SCRIPTS_HOME} &> /dev/null

			AGT_DEBUG_END "Copying from doxygen template"
		;;
		${AGT_DIR_UT})
			AGT_DEBUG_START "Copying from ut-core template"
			cd ${AGT_UT_HOME} &> /dev/null
			# Copy the missing files and dirs from ut-core template
			AGT_copy_and_display_uncopied_files  ${AGT_DIR_UT} ${AGT_TEMPLATE_UT}
			AGT_DEBUG_END "Copying from ut-core template"
			AGT_DEBUG_START "Copying from doxygen template"
			# Copy the missing files and dirs from doxygen template
			AGT_copy_and_display_uncopied_files ${AGT_DIR_UT} ${AGT_TEMPLATE_DOXYGEN}
			cd ${AGT_SCRIPTS_HOME} &> /dev/null
			AGT_DEBUG_END "Copying from doxygen template"
		;;
		*) AGT_ERROR "Invalid input"
			exit 1
		;;

	esac

	AGT_DEBUG_END "Copying template for '${1}'"
}

# Clone the ut dir
function AGT_clone_ut()
{
	local url=$1
	AGT_DEBUG_START "Building UT"

	# If ut dir exists, then exit this function
	if [ "${AGT_UT_EXISTS}" = true ]; then
		AGT_ALERT "UT dir already EXISTS"
		return 0
	fi

	# Default UT repo url
	UT_URL=$(echo "$url" | sed 's,halif,halif-test,g')
	AGT_WARNING " The url for UT clone is : ${UT_URL} "

	# Check with user if ut repo url is correct , else get the correct one
	read -p "$( AGT_ALERT Is this correct? )" yn
	case $yn in
		[Yy]* )
		# Proceed without prompting
		;;
		[Nn]* )
		# Prompt for inputs
		read -p "$(AGT_ALERT "Please input the URL (or leave blank to skip): ") " UT_INPUT_URL
		read -p "$(AGT_ALERT "Please input the directory (absolute) path (or leave blank to skip): ") " UT_DIR_PATH
		;;
		* )
		AGT_WARNING "No input added. Keeping original URL"
		;;
	esac

	if [[ ! -z "$UT_INPUT_URL" ]]; then
		AGT_SUCCESS "UT url is '${UT_INPUT_URL}'"
		# Clone the UT repo
		git clone ${UT_INPUT_URL} ${AGT_APIDEF_HOME}/ut &> /dev/null
	elif [[ ! -z "$UT_DIR_PATH" ]]; then
		AGT_SUCCESS "UT directory path  is '${UT_DIR_PATH}'"
		mkdir ${AGT_APIDEF_HOME}/ut
		cp -r ${UT_DIR_PATH}/* ${AGT_APIDEF_HOME}/ut/
	else
		AGT_SUCCESS "Original UT url is '${UT_URL}'"
		# Clone the UT repo
		git clone ${UT_URL} ${AGT_APIDEF_HOME}/ut &> /dev/null
	fi

	AGT_SUCCESS "UT repo is now cloned/copied"
	AGT_UT_EXISTS=true

	AGT_DEBUG_END "Building UT"
}

# Autogenerate the tests framework
AGT_generate_all()
{
	local url=$1
	local branch=$2

	# Create workspace if it doesn't exist already
	AGT_DEBUG_START "Creating Workspace"
	mkdir -p ${AGT_UT_WORKSPACE}
	AGT_SUCCESS "Workspace directory available at [../workspace/]"

	AGT_DEBUG_END "Creating Workspace"
	AGT_clone_apidef $url &> /dev/null
	if [ ! -z "${branch}" ]; then
		cd ${AGT_APIDEF_HOME}
		git checkout ${branch} &> /dev/null
		AGT_SUCCESS "Branch [${branch}] is now checked out"
		cd - 1>/dev/null
	fi
	AGT_clone_doxygen_repo
	AGT_copy_templates ${AGT_DIR_APIDEF}
	AGT_clone_ut $url
	AGT_copy_templates ${AGT_DIR_UT}
	./autogenerate_skeletons.sh ${AGT_APIDEF_URL} ${AGT_DEBUG}
	./autogenerate_tests.sh ${AGT_APIDEF_URL} ${AGT_DEBUG}
	echo -e ""
	AGT_ALERT "PLEASE REVIEW, SELECT, EDIT AND COMMIT AS REQUIRED"
	AGT_ALERT "GENERATED TESTS AVAILABLE IN ${PURPLE}[../workspace/${AGT_APIDEF_NAME}/ut/src]"
}

# Clear workspace dir
AGT_delete_workspace()
{
	AGT_DEBUG_START "Deleting workspace"

	# Delete workspace dir if it exists
	rm -rf ${AGT_UT_WORKSPACE}

	AGT_DEBUG_END "Deleting workspace"
}

# Command Usage
function AGT_show_usage()
{
	echo -e ""
	AGT_INFO "Autogen Script : Please run this script from inside the scripts dir"
	echo -e ""
	AGT_ALERT "./autogenerate.sh <api-def-repo-url/-c/-clean/-b/-branch/-h/-help>"
	echo -e ""
	AGT_INFO "	api-def-repo-url\t\t - The API Definition url to be used as first argument separately"
	AGT_INFO "	branch-name\t\t\t - API definition branch to be checked out and used along with branch switch"
	echo -e ""
	AGT_INFO "	-clean / -c\t\t\t - Cleans the workspace directory"
	AGT_INFO "	-branch / -b <branch-name>\t - Use this switch along with branch-name"
	AGT_INFO "	-help / -h\t\t\t - Shows the usage"
	AGT_WARNING "	\n\nPLEASE NOTE: run \`export AGT_DEBUG=1\` on terminal before running this script to get the debug messages"
	echo
}

# Switch cases for first command line argument
function AGT_decode_switches()
{
	for switch in "$1";
	do
		case "$switch" in
			"-clean"|"-c")
				AGT_delete_workspace
				exit 0;
			;;

			"-help"|"-h")
				AGT_show_usage
				exit 0;
			;;

			# Should start with the following valid url starters:
			# - git@github.com
			# - github.com
			# - git://git
			# - ssh://git
			# - http://git
			# - http://git

			git@github.com*|github.com*|git://git*|ssh://git*|http://git*|https://git*)
			;;

			* )
				AGT_ERROR "[$switch] unknown switch"
				AGT_show_usage
				exit 1
			;;
		esac
	done

	if [[ "$2" == "-branch" || "$2" == "-b" ]]; then
		if [[ -z "$3" ]]; then
			AGT_ERROR "Git branch name cannot be empty!"
			AGT_show_usage
			exit 1
		fi
		AGT_validate_branch ${AGT_APIDEF_URL} ${AGT_APIDEF_BRANCH}
	fi
}

# Init for autogen
function AGT_init()
{
	local url=$1

	# Call file which has the common variables and function
	. autogenerate_shared.sh ${url}

	# If no arguments provided, then exit script
	if [ $# -eq 0 ]; then
		>&2 AGT_ERROR "No arguments provided"
		AGT_show_usage
		exit 1
	fi

	# Variables
	AGT_DOXY_REPO="hal-doxygen.git"
	AGT_DOXYGEN_REPO_NAME=`echo "$(basename "$AGT_DOXY_REPO" .git)"`
	AGT_DOXYGEN_DIR=${AGT_UT_WORKSPACE}/${AGT_DOXYGEN_REPO_NAME}
	# Dir path for API Defintion template in ut-core
	AGT_APIDEF_TEMPLATE_PATH="template/api_definition_template"
	 # Dir path for UT template in ut-core
	AGT_UT_TEMPLATE_PATH="${AGT_BASEDIR}/template/ut_template"
	AGT_DOCS_DIR="docs"
	AGT_DOCS_PAGES_DIR="${AGT_DOCS_DIR}/pages"
	AGT_DOCS_IMAGES_DIR="${AGT_DOCS_PAGES_DIR}/images"

	AGT_DIR_APIDEF="API_DEF"
	AGT_DIR_UT="UT"
	AGT_TEMPLATE_APIDEF="API_Definition_template"
	AGT_TEMPLATE_UT="UT_template"
	AGT_TEMPLATE_DOXYGEN="Doxygen_template"

	# Call the shared function to update the respective variables
	AGT_update_variables ${AGT_UT_WORKSPACE}
	AGT_update_variables ${AGT_APIDEF_HOME}
	AGT_update_variables ${AGT_UT_HOME}

	# Check for Ruby and Bundler versions
	AGT_check_ruby_version
	AGT_check_bundler_version

	AGT_INFO "All version checks passed."
	AGT_INFO "Also make sure that you have access to all the git repos like rdkcentral etc"
}
# URL of API definition repo to be cloned as first command line argument
AGT_APIDEF_URL=$1

if [[ ! -z "$3" ]]; then
	AGT_APIDEF_BRANCH=$3
fi
AGT_init ${AGT_APIDEF_URL}
AGT_decode_switches $@
AGT_generate_all ${AGT_APIDEF_URL} ${AGT_APIDEF_BRANCH}
