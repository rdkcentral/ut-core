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

"""Guard against mistyped compiler flags in the build files.

Two checks, both static enough to run before anything is compiled:

  scan    walk the build files looking for a lowercase `-o` immediately
          followed by an optimisation level (`-o0`, `-os`, `-og`, ...).
          Lowercase `-o` means "write output to this file", so `-o0` asks
          for a file named `0` rather than setting optimisation. gcc accepts
          it silently, and a later `-o $@` on the same command line wins, so
          nothing ever fails - see ut-core issue #256 / ut-control issue #130.

  expect  ask `make printenv` what a variable actually expands to and assert
          the flags we intend are present. This catches the flag being
          dropped or overwritten as well as mistyped.
"""

import argparse
import os
import re
import subprocess
import sys

# ANSI escape codes for colored output
RED = "\033[91m"
GREEN = "\033[92m"
YELLOW = "\033[93m"
RESET = "\033[0m"

# Build files worth scanning: makefiles and the shell scripts that drive them.
SCANNED_NAMES = ("Makefile", "makefile", "GNUmakefile")
SCANNED_SUFFIXES = (".mk", ".sh")

# Directories holding third party or generated content, not our build files.
SKIPPED_DIRS = {".git", "build", "framework", "sysroot", "cpp_libs", "node_modules"}

# A lowercase -o glued directly to an optimisation level. The trailing lookahead
# keeps `-o3rdparty.o` and similar output filenames out of the results.
OPT_TYPO_PATTERN = re.compile(r"(?<![\w-])-o(?=(?:[0-3]|s|z|g|fast)(?![\w./$-]))")

# Makefiles and shell scripts both comment with #, escaped as \#. Only the part
# of a line that survives the comment reaches the compiler, so only that part is
# worth checking - otherwise prose describing the typo trips the check.
COMMENT_PATTERN = re.compile(r"(?<!\\)#")


def strip_comment(line):
    """Return the part of the line that the shell or make would actually act on."""
    match = COMMENT_PATTERN.search(line)
    return line[: match.start()] if match else line


def scan_file(path):
    """Return [(line_number, line_text)] for each mistyped optimisation flag."""
    try:
        with open(path, "r", encoding="utf-8", errors="replace") as handle:
            lines = handle.readlines()
    except OSError as error:
        print(f"{RED}Error: cannot read '{path}': {error}{RESET}")
        return None

    return [
        (number, text.rstrip())
        for number, text in enumerate(lines, start=1)
        if OPT_TYPO_PATTERN.search(strip_comment(text))
    ]


def scan_tree(root):
    """Scan every build file under root. Returns a list of (path, line, text)."""
    findings = []
    scanned = 0

    for directory, subdirectories, filenames in os.walk(root):
        subdirectories[:] = [name for name in subdirectories if name not in SKIPPED_DIRS]

        for filename in filenames:
            if filename not in SCANNED_NAMES and not filename.endswith(SCANNED_SUFFIXES):
                continue

            path = os.path.join(directory, filename)
            hits = scan_file(path)
            if hits is None:
                continue

            scanned += 1
            relative = os.path.relpath(path, root)
            findings.extend((relative, number, text) for number, text in hits)

    return scanned, findings


def read_make_variables(make_dir, make_args):
    """Return {name: value} as reported by `make printenv` in make_dir."""
    command = ["make", "-C", make_dir, "printenv"] + list(make_args)

    try:
        completed = subprocess.run(
            command, capture_output=True, text=True, check=False
        )
    except OSError as error:
        print(f"{RED}Error: cannot run '{' '.join(command)}': {error}{RESET}")
        return None

    if completed.returncode != 0:
        print(f"{RED}Error: '{' '.join(command)}' failed:{RESET}")
        print(completed.stderr.strip())
        return None

    variables = {}
    for line in completed.stdout.splitlines():
        name, separator, value = line.partition(" = ")
        if separator and name and " " not in name:
            variables[name] = value.strip()

    return variables


def check_expectations(variables, expectations):
    """Return [(variable, missing_flags, value)] for each unmet expectation."""
    failures = []

    for expectation in expectations:
        name, separator, flag_list = expectation.partition("=")
        if not separator:
            print(f"{RED}Error: --expect needs VAR=flag,flag - got '{expectation}'{RESET}")
            return None

        flags = [flag for flag in flag_list.split(",") if flag]
        value = variables.get(name)

        if value is None:
            failures.append((name, flags, None))
            continue

        missing = [flag for flag in flags if flag not in value.split()]
        if missing:
            failures.append((name, missing, value))

    return failures


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Check the build files for mistyped or missing compiler flags."
    )
    parser.add_argument("root", help="Directory tree to scan for build files")
    parser.add_argument(
        "--expect",
        metavar="VAR=flag,flag",
        action="append",
        default=[],
        help="Assert `make printenv` reports VAR containing these flags",
    )
    parser.add_argument(
        "--make-arg",
        metavar="NAME=VALUE",
        action="append",
        default=[],
        help="Argument passed to `make printenv`, e.g. TARGET=linux",
    )

    args = parser.parse_args()
    root = os.path.abspath(args.root)

    scanned, findings = scan_tree(root)

    if findings:
        print(f"\n{RED}Mistyped optimisation flags:{RESET}")
        for path, number, text in findings:
            print(f"{path}:{number}: {RED}{text.strip()} ❌{RESET}")
        print(f"{YELLOW}Lowercase -o names an output file. Optimisation is -O.{RESET}")

    expectation_failures = []
    if args.expect:
        variables = read_make_variables(root, args.make_arg)
        if variables is None:
            sys.exit(1)

        expectation_failures = check_expectations(variables, args.expect)
        if expectation_failures is None:
            sys.exit(1)

        if expectation_failures:
            print(f"\n{RED}Expected flags missing:{RESET}")
            for name, missing, value in expectation_failures:
                if value is None:
                    print(f"{name}: {RED}not reported by make printenv ❌{RESET}")
                else:
                    print(f"{name}: {RED}missing {' '.join(missing)} ❌{RESET} - is [{value}]")

    print("\n------------------------------------------------------------------")
    print(f"Build files scanned under {os.path.relpath(root, os.getcwd())} :: {scanned}")
    flag_colour = RED if findings else GREEN
    print(f"Mistyped optimisation flags :: {flag_colour}{len(findings)}{RESET}")
    if args.expect:
        expect_colour = RED if expectation_failures else GREEN
        checked = len(args.expect)
        met = checked - len(expectation_failures)
        print(f"Expected flag sets met :: {expect_colour}{met} / {checked}{RESET}")
    print("--------------------------------------------------------------------")

    if findings or expectation_failures:
        sys.exit(1)
