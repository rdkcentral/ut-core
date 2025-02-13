#!/usr/bin/env python3
# /*
#  * If not stated otherwise in this file or this component's LICENSE file the
#  * following copyright and licenses apply:
#  *
#  * Copyright 2023 RDK Management
#  *
#  * Licensed under the Apache License, Version 2.0 (the "License");
#  * you may not use this file except in compliance with the License.
#  * You may obtain a copy of the License at
#  *
#  * http://www.apache.org/licenses/LICENSE-2.0
#  *
#  * Unless required by applicable law or agreed to in writing, software
#  * distributed under the License is distributed on an "AS IS" BASIS,
#  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  * See the License for the specific language governing permissions and
#  * limitations under the License.
#  */

import re
import argparse

def check_macro_usage(cpp_file, macros):
    """Check if the given macros exist in the provided C++ file."""
    try:
        with open(cpp_file, "r", encoding="utf-8") as file:
            cpp_content = file.read()
    except FileNotFoundError:
        print("Error: File '{}' not found.".format(cpp_file))
        return None

    results = {macro: False for macro in macros}

    for macro in macros:
        if re.search(r"\b{}\b".format(macro), cpp_content):
            results[macro] = True

    return results

def extract_macros_from_header(header_file):
    """Extract assertion macros from the header file, ignoring header guards."""
    macro_pattern = re.compile(r"^\s*#define\s+(UT_[A-Z_]+)", re.MULTILINE)  # Match all `UT_` macros

    try:
        with open(header_file, "r", encoding="utf-8") as file:
            lines = file.readlines()
    except FileNotFoundError:
        print("Error: File '{}' not found.".format(header_file))
        return None

    macros = []
    for line in lines:
        match = macro_pattern.match(line)
        if match:
            macros.append(match.group(1))

    return macros

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Check macro usage in C++ files.")
    parser.add_argument("header_file", help="Path to the header file (.h/.hpp)")
    parser.add_argument("cpp_file", help="Path to the C++/C source file (.c/.cpp)")
    
    args = parser.parse_args()

    # Extract macros from the header file
    macros = extract_macros_from_header(args.header_file)
    if not macros:
        print("No macros found in header file!")
        exit(1)

    total_macros = len(macros)
    print(f"\nTotal macros extracted from header file: {total_macros}")
    
    # Check for macro usage in the C++ file
    macro_usage = check_macro_usage(args.cpp_file, macros)
    if macro_usage is None:
        exit(1)

    found_macros = sum(macro_usage.values())  # Count macros found in the C++ file

    # Print results
    print("\nMacro Usage Report:")
    for macro, found in macro_usage.items():
        status = "Found ✅" if found else "Not Found ❌"
        print(f"{macro}: {status}")

    # Print summary
    print("\n------------------------------------------")
    print(f"Total macros checked: {total_macros}")
    print(f"Macros found in C++ file: {found_macros} / {total_macros}")
    print("------------------------------------------")
