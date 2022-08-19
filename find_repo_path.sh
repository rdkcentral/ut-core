#!/usr/bin/env bash

function build_coloned_path()
{
    # Process STM_SDK2_INCLUDE* annotations.
    # Build up coloned path variable.
    # from variables named ${annotation_var_prefix*}
    annotation_var_prefix=$1
    coloned_path=$2
    for include_var in ${!REPO__*} ; do
        case $include_var in
            REPO__${annotation_var_prefix}*)
                # Build up CPATH variable for this project
                # If necessary, append new CPATHs to current CPATH.
                export_string="$coloned_path=\$$coloned_path:$PWD${!include_var:+/}${!include_var}"
                echo "export $export_string"
                echo >&2 "exported: $export_string"
                ;;
        esac
    done
}

function warn_coloned_path()
{
	annotation_var_prefix=$1
	coloned_path=$2
	for include_var in ${!REPO__*} ; do
	    case $include_var in
		REPO__${annotation_var_prefix}*)
		    echo >&2 "WARNING: $annotation_var_prefix annotation is DEPRECATED, $coloned_path is not modified"
		    ;;
	    esac
	done
}

for include_var in ${!REPO__*} ; do
    echo include:$include_var
done

if [ "$REPO__SKY_BUILD_VAR" ] ; then
    export_string="$REPO__SKY_BUILD_VAR=$PWD${REPO__SKY_BUILD_VAR_EXTEND:+/}$REPO__SKY_BUILD_VAR_EXTEND"
    echo "export $export_string"
    echo >&2 "exported: $export_string"

    build_coloned_path  SKY_EXE     PATH
    build_coloned_path  SKY_INCLUDE CPATH
    build_coloned_path  SKY_LIB     LIBRARY_PATH
fi

