# Overview of release-test-script-ut-core.sh

## Purpose
This Bash script automates the process of cloning a Git repository, building code, and validating build artifacts across multiple environments. It is designed to handle different system architectures (like ARM and x86) and environments (like Ubuntu, VM-SYNC, Dunfell Linux), simplifying testing, building, and validation processes.

## Key Features

1. **Repository Cloning**: 
   - The script clones a specified branch from a Git repository.
   - A default repository (`git@github.com:rdkcentral/ut-core.git`) is used if none is provided.
   
2. **Environment-Specific Build Setup**:
   - Supports different environments such as Ubuntu, VM-SYNC, Dunfell Linux, and Dunfell ARM.
   - The script runs platform-specific build commands (e.g., `make` for Linux or ARM) in each environment.

3. **Logging Build Outputs**:
   - The script runs the `make` command for different targets (e.g., `linux`, `arm`) and variants (`CPP`), and logs all outputs to files such as `make_log.txt`, `make_cpp_log.txt`, etc.
   - Logs can be reviewed in case of build failure.

4. **Build Verification**:
   - After building, the script checks if key static libraries (e.g., CURL, OpenSSL, GTest) and binaries (e.g., CMake) exist.
   - Each environment has customized checks based on expected outputs (for instance, checking for the OpenSSL static library in some environments and not in others).

5. **Error Handling**:
   - If any part of the process (cloning, building, or checking) fails, the script provides detailed error messages and exits.

6. **Environment Support**:
    Supports testing on multiple environments for following packages:
  

|ENV|CMAKE(Host)|CURL(Target)|OPENSSL(Target)|GTEST(Host)
|----|--------|------|---------|----------|
|Ubuntu+build essentials|NO|YES|NO|YES
|VM-SYNC|YES|YES|YES|YES
|RDK-DUNFELL(arm)|NO|YES|YES|YES
|RDK-DUNFELL(linux)|NO|YES|NO|YES

7. **Parallel Execution**:
   - The script is designed to potentially run the builds and checks for different environments in parallel, although this feature is commented out in the script.

## Usage Example
```bash
./script.sh -u <REPO_URL> -t <BRANCH_NAME>