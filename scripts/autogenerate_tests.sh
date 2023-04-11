#!/usr/bin/env bash

#* ************************************************************************
#*
#*   ** Project      : Unit Test Script
#*   ** @addtogroup  : ut
#*   ** @file        : shell
#*   ** @author      : anjali.thampi@sky.uk
#*   ** @date        : 12/01/2023
#*   **
#*   ** @brief : Shell script to autogenerate L1 and L2 tests' framework
#*   **
#*
#* ************************************************************************

# Init function for tests' generation
function AGT_tests_init()
{
        # API_DEF name as first command line argument
        AGT_APIDEF_NAME=$1

        # Call file which has the common variables and function
        source autogenerate_shared.sh ${AGT_APIDEF_NAME}

        # UT's src dir inside worksapce dir
        AGT_UT_SRC=${AGT_UT_HOME}/src

        # Variables
        AGT_TEMP_FUNC_DEF_FILE="funcs_list.txt"
        AGT_TEMP_FUNC_NAMES_FILE="func_names.txt"
        AGT_SCRIPTS_TEMPLATES="${AGT_SCRIPTS_HOME}/templates/"
        AGT_COPYRIGHT_TEMPLATE="${AGT_SCRIPTS_TEMPLATES}/AGT_copywrite_template.txt"
        AGT_L1_FUNCTION_TEMPLATE="${AGT_SCRIPTS_TEMPLATES}/AGT_L1_function_template.txt"
        AGT_L2_FUNCTION_TEMPLATE="${AGT_SCRIPTS_TEMPLATES}/AGT_L2_function_template.txt"
        AGT_REGISTER_FUNCTION_TEMPLATE="${AGT_SCRIPTS_TEMPLATES}/AGT_register_function_template.txt"
        AGT_CONFIG_TEMPLATE="${AGT_SCRIPTS_TEMPLATES}/AGT_register_config_template.sh"
        AGT_FILE_BANNER_TEMPLATE="${AGT_SCRIPTS_TEMPLATES}/AGT_file_banner.txt"
        AGT_MAIN_FILE_TEMPLATE="${AGT_SCRIPTS_TEMPLATES}/AGT_main_file_template.txt"
        AGT_TEST_REGISTER_FILE_TEMPLATE="${AGT_SCRIPTS_TEMPLATES}/AGT_test_register_file_template.txt"
        AGT_WORKSPACE_CONFIG_FILE="${AGT_UT_WORKSPACE}/UT_register_config.sh"
        AGT_UT_MAIN_FILE="${AGT_UT_SRC}/main.c"
        AGT_UT_TEST_REGISTER_FILE="${AGT_UT_SRC}/test_register.c"
        AGT_REGISTER_FUNCTION_KEY="AGT_REGISTER_FUNCTION"
        AGT_TEST_LEVEL_KEY="AGT_TEST_LEVEL"
        AGT_SUITE_NAME_KEY="AGT_SUITE_NAME"
        AGT_FUNCTION_NAME_KEY="AGT_FUNCTION_NAME"
        AGT_FILE_NAME_KEY="AGT_FILE_NAME"
        AGT_L1_TEST="L1"
        AGT_L2_TEST="L2"
        AGT_L1_FILE_PREFIX="test_l1"
        AGT_L2_FILE_PREFIX="test_l2"
        AGT_POSITIVE_TEST_CASES="positive"
        AGT_NEGATIVE_TEST_CASES="negative"
        AGT_ADD_L1_TEST_TO_SUITE=""
        AGT_ADD_L2_TEST_TO_SUITE=""
        AGT_EXTERN_TEST_REGISTER_L1=""
        AGT_TEST_REGISTER_L1=""
        AGT_EXTERN_TEST_REGISTER_L2=""
        AGT_TEST_REGISTER_L2=""
}

# Function to add headers to the files
# @param [in] Filename to copied into
# @param [in] true/false if ut_log.h needs to be copied
function AGT_add_headers_to_files()
{
        if [ $2 = true ]; then
                echo -e '#include <string.h>' >> ${1}
                echo -e '#include <stdlib.h>\n' >> ${1}
        fi

        echo "#include <ut.h>" >> ${1}

        if [ $2 = true ]; then
                echo -e '#include <ut_log.h>\n' >> ${1}
        else
                echo -e ''  >> ${1}
        fi
}

# Function to add copyright lines from template to file
# @param [in] Filename to copied into
function AGT_add_copyright_to_files()
{
        temp=`cat ${AGT_COPYRIGHT_TEMPLATE}`
        eval "echo \"${temp}\"" > $1
        echo -e ''  >> $1
}

# Function to add file banner with file details like author, reviewer, etc
# @param [in] Filename to be copied into
function AGT_add_file_banner_to_files()
{
        banner_file_name=`echo ${1##*/}`
        AGT_update_config ${AGT_FILE_NAME_KEY} ${banner_file_name}

        # Add the register function to the file using the template and config key values
        source ${AGT_WORKSPACE_CONFIG_FILE}
        tempfile=`cat ${AGT_FILE_BANNER_TEMPLATE}`
        eval "echo \"${tempfile}\"" >> $1
        echo -e ''  >> $1
}

# Function to add headers and generic comments to the start of the tests
# @param [in] Filename to copied into
function AGT_add_common_to_tests_file()
{
        AGT_add_copyright_to_files "${1}.c"
        AGT_add_file_banner_to_files "${1}.c"
        AGT_add_headers_to_files "${1}.c" true
}

# Function to make a copy of the config template and update it for each test
# @param [in] Keyname to be changed
# @param [in] Value to be changed into
function AGT_update_config()
{
        cp -u ${AGT_CONFIG_TEMPLATE} ${AGT_WORKSPACE_CONFIG_FILE}

        # Update the value based on the key
        input=${AGT_WORKSPACE_CONFIG_FILE}
        while IFS= read -r line
        do
                key=${line%%=*}
                value=${line#*=}

                case $key in
                       ${AGT_REGISTER_FUNCTION_KEY} | ${AGT_TEST_LEVEL_KEY} | ${AGT_SUITE_NAME_KEY} | ${AGT_FUNCTION_NAME_KEY} | ${AGT_FILE_NAME_KEY} )
                                if [ $1 = $key ]; then
                                        sed -i "s/\($key=*\).*/\1$2/" ${input}
                                fi
                        ;;
                esac
        done < "$input"
}

# Function to add register function to each test file
# @param [in] The name of the register function in the test file
# @param [in] The test level
# @param [in] The name of the suite in the test file
function AGT_add_register_function()
{
        file=$1
        level=$2
        suite=$3
        # Update the value of AGT_REGISTER_FUNCTION key in the config file
        AGT_update_config ${AGT_REGISTER_FUNCTION_KEY} ${file}
        # Update the value of AGT_TEST_LEVEL key in the config file
        AGT_update_config ${AGT_TEST_LEVEL_KEY} ${level}
        # Update the value of AGT_SUITE_NAME key in the config file
        AGT_update_config ${AGT_SUITE_NAME_KEY} ${suite}

        # Add the register function to the file using the template and config key values
        source ${AGT_WORKSPACE_CONFIG_FILE}
        template="$(cat ${AGT_REGISTER_FUNCTION_TEMPLATE})"
        eval "echo \"${template}\"" >> ${file}".c"
}

# Function to add main file in UT src from templates
function AGT_add_main_file()
{
        AGT_add_copyright_to_files ${AGT_UT_MAIN_FILE}

        AGT_add_file_banner_to_files ${AGT_UT_MAIN_FILE}

        AGT_add_headers_to_files ${AGT_UT_MAIN_FILE} false

        # Copy the main file template to the ut src's main.c
        cat ${AGT_MAIN_FILE_TEMPLATE} >> ${AGT_UT_MAIN_FILE}
}

# Function to add test register file from the templates
function AGT_add_test_register_file()
{
        AGT_add_copyright_to_files ${AGT_UT_TEST_REGISTER_FILE}

        AGT_add_headers_to_files ${AGT_UT_TEST_REGISTER_FILE} false

        # Copy the test register template to the ut src's test_register.c with data in the following variables
        # 1. AGT_EXTERN_TEST_REGISTER_L1
        # 2. AGT_EXTERN_TEST_REGISTER_L2
        # 3. AGT_TEST_REGISTER_L1
        # 4. AGT_TEST_REGISTER_L2
        template="$(cat ${AGT_TEST_REGISTER_FILE_TEMPLATE})"
        eval "echo \"${template}\"" >> ${AGT_UT_TEST_REGISTER_FILE}
}

# Function to add the specific function (L1 - positive or negative [or] L2)
# @param [in] The name of the test file
# @param [in] AGT_POSITIVE_TEST_CASES [or] AGT_NEGATIVE_TEST_CASES [or] empty string (for L2)
# @param [in] The name of the header function
# @param [in] AGT_L2_TEST [or] AGT_L2_TEST and "_"
function AGT_add_functions_to_test()
{
        # Local variables
        local file_name=$1
        local test_type=$2
        local header_function=$3
        local test_level=$4
        local test_function_name=""
        local test_name=""

        # Reset the variables for each function added to test
        AGT_FUNCTION_DESCRIPTION=""
        AGT_TEST_PROCEDURE=""

        if [ ${test_level} = ${AGT_L2_TEST} ]; then
                # Test function name for respective L2 tests
                # L2 Testname : filename
                test_function_name="${file_name}"

                # Test name for respective L2 files
                # L2 testname : filename
                test_name=${file_name}

                # Update the value of AGT_FUNCTION_NAME key in the config file
                AGT_update_config ${AGT_FUNCTION_NAME_KEY} ${test_function_name}

                # Add to variable which stores all the "UT_add_test" lines for L2 tests
                AGT_ADD_L2_TEST_TO_SUITE+=`echo "UT_add_test( pSuite, "\"${test_name}\"" ,${test_function_name} );:"`
                # Add to variable (used in AGT_function_template.txt) which stores the test's steps
                AGT_TEST_PROCEDURE+=${AGT_TEST_PROCEDURE_L2}
        else
                # Test function name for respective L1 (positive / negative) tests
                # L1 positive testname : filename_positive_headerfunction
                # L1 negative testname : filename_negative_headerfunction
                test_function_name="${file_name}_${test_type}_${header_function}"

                # Test name for respective L1 (positive / negative) functions
                # L1 positive testname : headerfunction_positive_L1
                # L1 negative testname : headerfunction_negative_L1
                test_name=${header_function}_${4}${test_type}

                if [ "${l1_line_indent}" = true ]; then
                        AGT_ADD_L1_TEST_TO_SUITE+=`echo -e '\t'`
                fi

                # Add to variable (used in AGT_register_function_template.txt) which stores all the "UT_add_test" lines for L1 tests
                AGT_ADD_L1_TEST_TO_SUITE+=`echo "UT_add_test( pSuite, "\"${test_name}\"" ,${test_function_name} );:"`
                # Add to variable (used in AGT_function_template.txt) which stores the test's steps
                AGT_TEST_PROCEDURE+=${AGT_TEST_PROCEDURE_L1}
        fi

        # Update the value of AGT_FUNCTION_NAME key in the config file
        AGT_update_config ${AGT_FUNCTION_NAME_KEY} ${test_function_name}
        source ${AGT_WORKSPACE_CONFIG_FILE}

        if [ ${test_level} = ${AGT_L2_TEST} ]; then
                # Copy the L2 function template for each header function with AGT_FUNCTION_NAME in the config file
                template="$(cat ${AGT_L2_FUNCTION_TEMPLATE})"
        else
                # Copy the L1 function template for each header function with AGT_FUNCTION_NAME in the config file
                template="$(cat ${AGT_L1_FUNCTION_TEMPLATE})"
        fi

        eval "echo \"${template}\"" >> ${file_name}".c"
        echo -e ''  >> ${file_name}".c"

}

# Function to create the UT's src directory if it does not exist
function AGT_create_ut_src()
{
        AGT_task_message ${AGT_TASK_START} "Creating UT's src dir"

        # If UT's src dir does not exist, create it
        if [ ! -d ${AGT_UT_SRC} ]; then
                AGT_WARNING "The UT's src directory [../workspace/${AGT_APIDEF_NAME}/ut/src] DOES NOT EXIST."
                mkdir -p ${AGT_UT_SRC}
                AGT_INFO_BLUE "The UT's src directory [../workspace/${AGT_APIDEF_NAME}/ut/src] is now CREATED."
        else
                # If UT's src is not empty, then get user input to rewrite or not
                if [ ! -z "$(ls -A ${AGT_UT_HOME}/src/*)" ]; then
                        AGT_WARNING "The UT's src (test cases) directory [../workspace/${AGT_APIDEF_NAME}/ut/src] is NOT EMPTY."
                        cd ${AGT_UT_SRC}
                        ls  | grep -xv "main.c" | xargs rm
                        cd - 1>& /dev/null # Else prints the scripts dir full path
                        AGT_INFO_CYAN "The UT's src directory [../workspace/${AGT_APIDEF_NAME}/ut/src] is now EMPTY."
                fi
                AGT_task_message ${AGT_TASK_END} "Creating UT's src dir"
        fi
}

# Function to add data to the following variables : AGT_EXTERN_TEST_REGISTER_L1 and AGT_TEST_REGISTER_L1
# @param [in] The name of the L1 file
function AGT_add_to_L1_test_register_variables()
{
        L1_filename=$1

        # Variable in test register template which holds the list of L1 register functions
        AGT_EXTERN_TEST_REGISTER_L1+="extern int ${L1_filename}_register( void );:"

        # If register_l1_test_line_indent is set to true , then add indentation
        if [ "${register_l1_test_line_indent}" = true ]; then
                AGT_TEST_REGISTER_L1+=`echo -e '\t'`
        fi

        # Variable in test register template which holds the call to L1 register functions
        AGT_TEST_REGISTER_L1+="registerFailed |= ${L1_filename}_register();:"

        # If register_l1_test_line_indent is set to false (first line), then set it to true
        # as the remaining lines need indentation
        if [ "${register_l1_test_line_indent}" = false ]; then
                register_l1_test_line_indent=true
        fi
}

# Function to add data to the following variables : AGT_EXTERN_TEST_REGISTER_L2 and AGT_TEST_REGISTER_L2
# @param [in] The name of the L2 file
function AGT_add_to_L2_test_register_variables()
{
        L2_filename=$1

        # Variable in test register template which holds the list of L2 register functions
        AGT_EXTERN_TEST_REGISTER_L2+="extern int ${L2_filename}_register( void );:"

        # Variable in test register template which holds the call to L2 register functions
        AGT_TEST_REGISTER_L2+="registerFailed |= ${L2_filename}_register();:"
}

# Function to isolate the functions and generate the L1 and ${AGT_L2_TEST} tests
function AGT_generate_l1_l2_tests()
{
        AGT_task_message ${AGT_TASK_START} "Creating UT's ${AGT_L1_TEST} and L2 tests"

        cd ${AGT_SKELETONS_SRC}

        # Initialise variables to false
        register_l1_test_line_indent=false

        # Add to variable (used to update AGT_TEST_PROCEDURE for L1 functions) which stores the test's steps
        AGT_TEST_PROCEDURE_L1=`echo -e '* TODO: Add the steps to run this test and add a line in the below table for each input variation tried in this function.@n#'`
        AGT_TEST_PROCEDURE_L1+=`echo -e '* | Variation / Step | Description | Test Data | Expected Result | Notes |#'`
        AGT_TEST_PROCEDURE_L1+=`echo -e '* | :-------: | ------------- | --------- | --------------- | ----- |#'`
        # Add to variable (used to update AGT_TEST_PROCEDURE for L1 functions) which stores the test's steps
        AGT_TEST_PROCEDURE_L1+=`echo -e '* | 01 | First set of conditions | What is input data to be tested | How to gauge success, is it a success variable? | Should be successful |#'`
        # Add to variable (used to update AGT_TEST_PROCEDURE for L2 functions) which stores the test's steps
        AGT_TEST_PROCEDURE_L2=`echo "Refer to UT specification documentation [L2 Test Specification](\ref l2testspecification)"`
        # Iterate through all c files
        echo -e ""
        for filename in *.c; 
        do
                # Initialise variables to false for every file
                l1_line_indent=false

                # Copy all the function definition lines into a temp file
                cat "${filename}"  | grep -Ev  "^<|.h\"$" | grep "(*)$"  > ${AGT_TEMP_FUNC_DEF_FILE}

                # Move to next file in the list if there are no functions
                if [ ! -s ${AGT_TEMP_FUNC_DEF_FILE} ]; then
                        continue
                fi

                # Get the list of functions and copy into another temp file
                cat ${AGT_TEMP_FUNC_DEF_FILE} | cut -d"(" -f1 | cut -d" " -f2  > ${AGT_TEMP_FUNC_NAMES_FILE}

                # Set up the L1 filename
                new_L1_filename="${AGT_L1_FILE_PREFIX}_`echo $filename| cut -d"." -f1`"
                # Call function to add common lines to L1 file
                AGT_add_common_to_tests_file ${new_L1_filename}
                # Call function to update the test register L1 variables
                AGT_add_to_L1_test_register_variables ${new_L1_filename}

                 # Set up the L2 filename
                new_L2_filename="${AGT_L2_FILE_PREFIX}_`echo $filename| cut -d"." -f1`"
                 # Call function to add common lines to L2 file
                AGT_add_common_to_tests_file ${new_L2_filename}
                # Call function to update the test register L2 variables
                AGT_add_to_L2_test_register_variables ${new_L2_filename}

                # Add the function to the L2 file
                AGT_add_functions_to_test ${new_L2_filename} "" "" ${AGT_L2_TEST}

                # Iterate through the list of functions
                input=${AGT_TEMP_FUNC_NAMES_FILE}
                while read -r header_function
                do
                        # Add the positive function to the L1 file
                        AGT_add_functions_to_test ${new_L1_filename} ${AGT_POSITIVE_TEST_CASES} ${header_function} "${AGT_L1_TEST}_"
                        if [ "${l1_line_indent}" = false ]; then
                                l1_line_indent=true
                        fi
                        # Add the negative function to the L1 file
                        AGT_add_functions_to_test ${new_L1_filename} ${AGT_NEGATIVE_TEST_CASES} ${header_function} "${AGT_L1_TEST}_"

                done < "$input"

                # Temporary variable to copy the content of AGT_ADD_L2_TEST_TO_SUITE (variable which stores all the "UT_add_test" lines for L1 tests)
                temp_AGT_ADD_L2_TEST_TO_SUITE=${AGT_ADD_L2_TEST_TO_SUITE}
                # Reset AGT_ADD_L2_TEST_TO_SUITE (used in AGT_register_function_template.txt) so that the L1 suite name is not appended with the L2 suite name
                AGT_ADD_L2_TEST_TO_SUITE=""
                # Call function to add register function to each L1 test file
                AGT_add_register_function ${new_L1_filename} ${AGT_L1_TEST} `echo $filename| cut -d"." -f1`
                # Reset AGT_ADD_L1_TEST_TO_SUITE (used in AGT_register_function_template.txt) so that the L2 suite name is not appended with the L1 suite name
                AGT_ADD_L1_TEST_TO_SUITE=""
                # Copy back from temporary variable
                AGT_ADD_L2_TEST_TO_SUITE=$temp_AGT_ADD_L2_TEST_TO_SUITE
                # Call function to add register function to each L2 test file
                AGT_add_register_function ${new_L2_filename} ${AGT_L2_TEST} `echo $filename| cut -d"." -f1`
                # Reset AGT_ADD_L2_TEST_TO_SUITE (used in AGT_register_function_template.txt)
                AGT_ADD_L2_TEST_TO_SUITE=""

                # Move the files to UT's src directory
                mv "${AGT_SKELETONS_SRC}/${new_L1_filename}.c" "${AGT_UT_SRC}/${new_L1_filename}.c" ;
                mv "${AGT_SKELETONS_SRC}/${new_L2_filename}.c" "${AGT_UT_SRC}/${new_L2_filename}.c" ;
                AGT_INFO_CYAN "${AGT_L1_TEST} and L2 tests for '`echo $filename| cut -d"." -f1`.h' is created "
                echo -e ""
        done

        # Remove all temp files created
        rm -f ${AGT_SKELETONS_SRC}/${AGT_TEMP_FUNC_DEF_FILE} ${AGT_SKELETONS_SRC}/${AGT_TEMP_FUNC_NAMES_FILE}
        cd ${AGT_SCRIPTS_HOME}

        AGT_task_message ${AGT_TASK_END} "Creating UT's ${AGT_L1_TEST} and L2 tests"
}



AGT_tests_init $1
AGT_create_ut_src
AGT_add_main_file
AGT_generate_l1_l2_tests
AGT_add_test_register_file
