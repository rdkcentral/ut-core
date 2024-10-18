# Overview of release-test-script-ut-core.sh

## Purpose
This Bash script automates the process of cloning the PR branch on ut-core Git repository, building code, and validating build artifacts across multiple environments. It is designed to handle different system architectures (like ARM and x86) and environments (like Ubuntu, VM-SYNC, Dunfell Linux, Dunfell ARM), simplifying testing, building, and validation processes.
where:
   VM-SYNC : is a docker simulating the RDK linux environment
   Dunfell Linux : is a docker with linux environment
   Dunfell ARM : is a docker simulating the arm environment for yocto version dunfell
   Kirkstone ARM : is a docker simulating the arm environment for yocto version kirkstone
It checks for the existence of various third party packages like openssl, curl, cmake, gtest etc for various environments listed above.
With these validations, it ensures if the PR is good for merge and has not broken the basic requirements.

Following table gives the overview:

|#|Target|Docker|Expectation
|---|-----|-----------|---------
|1|make TARGET=arm|rdk-dunfell|builds ut-core for target arm
|2|make TARGET=arm|rdk-kirkstone|builds ut-core for target arm
|3|make TARGET=linux|vm-sync|builds ut-core for target linux
|4|make TARGET=linux|none|builds ut-core for target linux
|5|make -C tests/ TARGET=arm|rdk-dunfell|builds ut-core tests for target arm
|6|make -C tests/  TARGET=arm|rdk-kirkstone|builds ut-core tests for target arm
|7|make -C tests/ TARGET=linux|vm-sync|builds ut-core tests for target linux
|8|make -C tests/ TARGET=linux|none|builds ut-core tests for target linux

## Key Features

1. **Repository Cloning**: 
   - The script clones a specified branch from ut-core  Git repository.
   - A default repository (`git@github.com:rdkcentral/ut-core.git`) is used if none is provided.

2. **Environment-Specific Build Setup**:
   - Supports different environments such as Ubuntu, VM-SYNC, Dunfell Linux, and Dunfell ARM.
   - The script runs platform-specific build commands (e.g., `make` for Linux or ARM) in each environment.

3. **Logging Build Outputs**:
   - The script runs the `make` command for different targets (e.g., `linux`, `arm`) and variants (`CPP`), and logs all outputs to files such as `make_log.txt`, `make_cpp_log.txt`, etc.
   - Logs can be reviewed in case of build failure.

4. **Build Verification**:
   - After building, the script checks if key artifacts :
   - [ ]  - static libraries - CURL, OpenSSL, GTest exist
   - [ ]    - binaries - CMake  exist.
   - Each environment has customized checks based on expected outputs (for instance, checking for the OpenSSL static library in some environments and not in others).

5. **Error Handling**:
   - If any part of the process (cloning, building, or checking) fails, the script provides detailed error messages and exits.

6. **Environment Support**:
    Supports testing on multiple environments for following packages:
  

|#|ENV/PACKAGES|CMAKE(Host)|CURL(Target)|OPENSSL(Target)|GTEST(Target)
|---|----|--------|------|---------|----------|
|1|Ubuntu+build essentials|NO|YES|NO|YES
|2|VM-SYNC|YES|YES|YES|YES
|3|RDK-DUNFELL(arm)|NO|YES|YES|YES
|4|RDK-DUNFELL(linux)|NO|YES|NO|YES
|5|RDK-KIRKSTONE(arm)|NO|YES|YES|YES
|6|RDK-KIRKSTONE(linux)|NO|YES|NO|YES

For ex:
On env 1,  the script will check for availability of CURL library, OpenSSL libraraies and Gtest libraries for Target.It would however would not look for CMAKE binary for host as this environment already provides cmake support which is provided by build essentials.
On env 2, on the other hand, none of the packages are present , hence the script will check for all of the packages listed above.



## Usage Example
```bash
./release-test-script-ut-core.sh -t <BRANCH_NAME_TO_BE_TESTED_ON_UT_CORE_REPO>