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
import os

# ANSI escape codes for colored output
RED = "\033[91m"
GREEN = "\033[92m"
RESET = "\033[0m"

def check_macro_usage(cpp_file, macros):
    """Check if the given macros exist in the provided source file."""
    try:
        with open(cpp_file, "r", encoding="utf-8") as file:
            cpp_content = file.read()
    except FileNotFoundError:
        print(f"Error: File '{cpp_file}' not found.")
        return None

    results = {macro: False for macro in macros}

    for macro in macros:
        if re.search(rf"\b{macro}\b", cpp_content):
            results[macro] = True

    return results

def extract_macros_from_header(header_file):
    """Extract assertion macros from the header file, ignoring header guards."""
    macro_pattern = re.compile(r"^\s*#define\s+(UT_[A-Z_]+)", re.MULTILINE)  # Match all `UT_` macros

    try:
        with open(header_file, "r", encoding="utf-8") as file:
            lines = file.readlines()
    except FileNotFoundError:
        print(f"Error: File '{header_file}' not found.")
        return None

    macros = []
    for line in lines:
        match = macro_pattern.match(line)
        if match:
            macros.append(match.group(1))

    return macros

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Check macro usage in source files.")
    parser.add_argument("header_file", help="Path to the header file (.h/.hpp)")
    parser.add_argument("source_file", help="Path to the source file (.c/.cpp)")
    
    args = parser.parse_args()

    # Convert to relative paths
    header_relative = os.path.relpath(args.header_file, os.getcwd())
    source_relative = os.path.relpath(args.source_file, os.getcwd())

    # Extract macros from the header file
    macros = extract_macros_from_header(args.header_file)
    if not macros:
        print("No macros found in header file!")
        exit(1)

    total_macros = len(macros)
    
    # Check for macro usage in the source file
    macro_usage = check_macro_usage(args.source_file, macros)
    if macro_usage is None:
        exit(1)

    found_macros = sum(macro_usage.values())  # Count macros found in the C++ file
    missing_macros = [macro for macro, found in macro_usage.items() if not found]

    # Print only missing macros
    if missing_macros:
        print(f"\n{RED}Missing Macros Usage:{RESET}")
        for macro in missing_macros:
            print(f"{macro}: {RED}Not Found ❌{RESET}")

    # Print summary
    print("\n------------------------------------------------------------------")
    print(f"Macros extracted from header file :: {header_relative}")
    print(f"Macros found in {source_relative} :: {GREEN}{found_macros} / {total_macros}{RESET}")
    print("--------------------------------------------------------------------")

    # Exit with failure if any macro is missing
    if missing_macros:
        exit(1)
