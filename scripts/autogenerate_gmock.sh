#!/usr/bin/env bash

# *
# * If not stated otherwise in this file or this component's LICENSE file the
# * following copyright and licenses apply:
# *
# * Copyright 2026 RDK Management
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

#* ************************************************************************
#*
#*   ** Project      : Unit Test Script
#*   ** @addtogroup  : ut
#*   ** @file        : autogenerate_gmock.sh
#*   ** @date        : 2026
#*   **
#*   ** @brief : Generate a GoogleMock mock class and a gtest test skeleton
#*   **          from a C++ interface header, using the ut-core UT_ wrappers
#*   **          (ut_gmock.h / ut_gtest.h). CPP/gtest analogue of the C test
#*   **          autogeneration.
#*   **
#*   ** @note  : Parses standard single-line pure-virtual declarations of the
#*   **          form:  virtual <ret> <name>(<params>) [const] = 0;
#*   **          Multi-line declarations and comma-bearing template return
#*   **          types are out of scope (wrap those return types in a typedef).
#*   **
#* ************************************************************************

set -euo pipefail

AGT_SCRIPTS_HOME="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
AGT_COPYRGT_TEMPLATE="${AGT_SCRIPTS_HOME}/templates/AGT_copywrite_template.txt"

function AGT_gmock_usage()
{
    echo "Usage: ./autogenerate_gmock.sh -f <interface_header.h> [-c <ClassName>] [-o <out_dir>]"
    echo "  -f  Path to the C++ interface header to mock (required)"
    echo "  -c  Interface class name to mock (default: first class in the header)"
    echo "  -o  Output directory (default: ./generated_mocks)"
    echo "  -h  Show this usage"
}

# Emit the shared Apache copyright header into $1
function AGT_gmock_copyright()
{
    if [ -f "${AGT_COPYRGT_TEMPLATE}" ]; then
        cat "${AGT_COPYRGT_TEMPLATE}" > "$1"
        echo "" >> "$1"
    fi
}

# Extract the body (between the outermost { }) of class $2 from header file $1.
# Prints the class body lines to stdout.
function AGT_gmock_class_body()
{
    local header="$1" cls="$2"
    awk -v cls="$cls" '
        # Detect the start of the target class: "class <cls>" possibly with bases.
        !inClass && $0 ~ ("(^|[^[:alnum:]_])class[[:space:]]+" cls "([^[:alnum:]_]|$)") {
            inClass = 1
        }
        inClass {
            # Track brace depth so we stop at the matching closing brace.
            n = gsub(/{/, "{"); m = gsub(/}/, "}");
            if (started) print
            depth += n - m
            if (!started && n > 0) { started = 1 }
            if (started && depth <= 0) { exit }
        }
    ' "$header"
}

# Print the first class name declared in header $1 (best-effort).
function AGT_gmock_first_class()
{
    grep -oP '(^|[^[:alnum:]_])class\s+\K\w+' "$1" | head -n1
}

# Transform one normalised pure-virtual declaration into a UT_MOCK_METHOD line.
# Input (stdin): a single declaration line, e.g.
#   virtual int read(const char *name) const = 0;
# Output (stdout): UT_MOCK_METHOD(int, read, (const char *name), (const, override));
function AGT_gmock_method_line()
{
    local line="$1" specs="(override)"
    # Strip comments, leading 'virtual', trailing '= 0;' and surrounding space.
    line="$(echo "$line" | sed -E 's://.*$::; s:/\*.*\*/::g')"
    line="$(echo "$line" | sed -E 's/^[[:space:]]*virtual[[:space:]]+//; s/[[:space:]]*=[[:space:]]*0[[:space:]]*;[[:space:]]*$//')"
    line="$(echo "$line" | sed -E 's/^[[:space:]]+//; s/[[:space:]]+$//')"

    # Params: between the first '(' and the last ')'.
    local params="${line#*(}"; params="${params%)*}"
    # Everything before the first '(' = return type + method name.
    local head="${line%%(*}"
    # Trailing qualifiers after the last ')'.
    local tail="${line##*)}"
    if echo "$tail" | grep -qw const; then
        specs="(const, override)"
    fi

    head="$(echo "$head" | sed -E 's/[[:space:]]+$//')"
    local name="${head##* }"     # last whitespace-delimited token
    local ret="${head% *}"       # everything before it
    ret="$(echo "$ret" | sed -E 's/[[:space:]]+$//; s/^[[:space:]]+//')"

    # A pointer/reference return type may bind to the name (e.g. "char *name");
    # migrate any leading '*'/'&' off the name and onto the return type.
    local ptr=""
    while [ -n "$name" ] && { [ "${name:0:1}" = "*" ] || [ "${name:0:1}" = "&" ]; }; do
        ptr="${ptr}${name:0:1}"
        name="${name:1}"
    done
    if [ -n "$ptr" ]; then
        ret="${ret} ${ptr}"
    fi

    echo "    UT_MOCK_METHOD(${ret}, ${name}, (${params}), ${specs});"
}

AGT_HEADER=""; AGT_CLASS=""; AGT_OUTDIR="./generated_mocks"
while getopts ":f:c:o:h" opt; do
    case "$opt" in
        f) AGT_HEADER="$OPTARG" ;;
        c) AGT_CLASS="$OPTARG" ;;
        o) AGT_OUTDIR="$OPTARG" ;;
        h) AGT_gmock_usage; exit 0 ;;
        *) AGT_gmock_usage; exit 1 ;;
    esac
done

if [ -z "${AGT_HEADER}" ] || [ ! -f "${AGT_HEADER}" ]; then
    echo "Error: a valid interface header (-f) is required." >&2
    AGT_gmock_usage
    exit 1
fi

if [ -z "${AGT_CLASS}" ]; then
    AGT_CLASS="$(AGT_gmock_first_class "${AGT_HEADER}")"
fi
if [ -z "${AGT_CLASS}" ]; then
    echo "Error: no class found in ${AGT_HEADER}; specify one with -c." >&2
    exit 1
fi

# Validate the class name is a plain C++ identifier before using it to form
# file paths and generated code (rejects path components like '/..').
if ! echo "${AGT_CLASS}" | grep -qE '^[A-Za-z_][A-Za-z0-9_]*$'; then
    echo "Error: class name '${AGT_CLASS}' is not a valid identifier." >&2
    exit 1
fi

mkdir -p "${AGT_OUTDIR}"
HEADER_BASENAME="$(basename "${AGT_HEADER}")"
# Copy the interface header alongside the generated files so the emitted mock
# (which includes it by basename) compiles regardless of where -o points.
if [ "$(cd "$(dirname "${AGT_HEADER}")" && pwd)" != "$(cd "${AGT_OUTDIR}" && pwd)" ]; then
    cp "${AGT_HEADER}" "${AGT_OUTDIR}/${HEADER_BASENAME}"
fi
CLASS_LOWER="$(echo "${AGT_CLASS}" | tr '[:upper:]' '[:lower:]')"
MOCK_CLASS="Mock${AGT_CLASS}"
MOCK_HEADER="${AGT_OUTDIR}/mock_${CLASS_LOWER}.h"
TEST_FILE="${AGT_OUTDIR}/test_${CLASS_LOWER}.cpp"

# Collect the pure-virtual declarations of the target class.
mapfile -t PURE_VIRTUALS < <(AGT_gmock_class_body "${AGT_HEADER}" "${AGT_CLASS}" \
    | grep -E 'virtual' | grep -E '=[[:space:]]*0[[:space:]]*;')

if [ "${#PURE_VIRTUALS[@]}" -eq 0 ]; then
    echo "Warning: no pure-virtual methods found in class ${AGT_CLASS}." >&2
fi

# ---- Emit the mock header ------------------------------------------------
AGT_gmock_copyright "${MOCK_HEADER}"
{
    echo "/* Auto-generated GoogleMock for ${AGT_CLASS} (from ${HEADER_BASENAME}). */"
    echo "#ifndef MOCK_${CLASS_LOWER^^}_H_"
    echo "#define MOCK_${CLASS_LOWER^^}_H_"
    echo ""
    echo "#include <ut.h>"
    echo "#include \"${HEADER_BASENAME}\""
    echo ""
    echo "class ${MOCK_CLASS} : public ${AGT_CLASS}"
    echo "{"
    echo "public:"
    for decl in "${PURE_VIRTUALS[@]}"; do
        AGT_gmock_method_line "${decl}"
    done
    echo "};"
    echo ""
    echo "#endif /* MOCK_${CLASS_LOWER^^}_H_ */"
} >> "${MOCK_HEADER}"

# ---- Emit the gtest test skeleton ---------------------------------------
AGT_gmock_copyright "${TEST_FILE}"
{
    echo "/* Auto-generated gtest skeleton for ${AGT_CLASS} using ${MOCK_CLASS}. */"
    echo "#include <ut.h>"
    echo "#include \"mock_${CLASS_LOWER}.h\""
    echo ""
    echo "class Test${AGT_CLASS} : public UTCore"
    echo "{"
    echo "public:"
    echo "    Test${AGT_CLASS}() : UTCore() {}"
    echo "    ~Test${AGT_CLASS}() override = default;"
    echo "};"
    echo ""
    echo "UT_ADD_TEST_TO_GROUP(Test${AGT_CLASS}, UT_TESTS_L1)"
    echo ""
    for decl in "${PURE_VIRTUALS[@]}"; do
        # Recover the method name for a per-method test stub.
        mline="$(AGT_gmock_method_line "${decl}")"       # UT_MOCK_METHOD(ret, name, ...)
        mname="$(echo "$mline" | sed -E 's/.*UT_MOCK_METHOD\([^,]*,[[:space:]]*([A-Za-z_][A-Za-z0-9_]*).*/\1/')"
        echo "UT_ADD_TEST(Test${AGT_CLASS}, ${mname}_L1)"
        echo "{"
        echo "    ${MOCK_CLASS} mock;"
        echo "    // TODO: set expectations, e.g."
        echo "    // UT_MOCK_EXPECT_CALL(mock, ${mname}(UT_MOCK_ANY)).WillOnce(UT_MOCK_RETURN(/* value */));"
        echo "    // TODO: drive the code under test with 'mock' and assert the result."
        echo "    (void)mock;"
        echo "}"
        echo ""
    done
} >> "${TEST_FILE}"

echo "Generated:"
echo "  mock     : ${MOCK_HEADER}"
echo "  test stub: ${TEST_FILE}"
echo "  methods  : ${#PURE_VIRTUALS[@]} pure-virtual(s) from class ${AGT_CLASS}"
