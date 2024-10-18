# Overview for release-test-script-platform.sh

This Bash script is designed for automating the process of cloning a PR branch from platform test suite Git repository, building software, and validating build artifacts across multiple environments. It simplifies testing and development workflows by managing repository setup, environment-specific builds, and post-build checks for required binaries and libraries.
It also checks for the existence of various third party packages like openssl, curl, cmake, gtest etc for various environments listed below.
With these validations, it ensures if the PR is good for merge and has not broken the basic requirements.

Following table gives the overview:

|#|Target|Docker|Expectation
|---|-----|-----------|---------
|1|./build_ut.sh TARGET=arm|rdk-dunfell|builds hal for target arm
|2|./build_ut.sh TARGET=arm|rdk-kirkstone|builds hal for target arm
|3|./build_ut.sh TARGET=linux|vm-sync|builds hal for target linux
|4|./build_ut.sh TARGET=linux|none|builds hal for target linux
|5|./build_ut.sh -C tests/ TARGET=arm|rdk-dunfell|builds hal tests for target arm
|6|./build_ut.sh -C tests/ TARGET=arm|rdk-kirkstone|builds hal tests for target arm
|7|./build_ut.sh -C tests/ TARGET=linux|vm-sync|builds hal tests for target linux
|8|./build_ut.sh -C tests/ TARGET=linux|none|builds hal tests for target linux

## Key Objectives:
1. **Automated Repository Setup**:  
   Clones a given Git repository (or a default one if not specified) for different target environments (e.g., Ubuntu, VM-SYNC, Dunfell Linux, Dunfell ARM).

   where:
   VM-SYNC : is a docker simulating the RDK linux environment
   Dunfell Linux : is a docker with linux environment
   Dunfell ARM : is a docker simulating the arm environment for yocto version dunfell
   Kirkstone ARM : is a docker simulating the arm environment for yocto version kirkstone

2. **Environment-Specific Builds**:  
   Runs a customized build process based on the environment. It supports branching logic to switch and build from a specific branch (`UT_CORE_BRANCH_NAME`) when provided.

3. **Post-Build Checks**:  
  After compiling, the script verifies:
   - The current branch matches the target branch.
   - The existence of specific libraries such as:
   - [ ] -  **CURL static library** (`libcurl.a`)
   - [ ] -     **OpenSSL static library** (`libssl.a`)
   - [ ] -     **CMake binary**
   - [ ]  -     **HAL binary**
- Each check outputs either `PASS` or `FAIL`, depending on whether the conditions are met.
- The checks vary based on the environment, allowing for customized validation for each setup.

4. **Cross-Environment Testing**:  
   Supports testing on multiple environments for following packages:
  

|#|ENV/PACKAGES|CMAKE(Host)|CURL(Target)|OPENSSL(Target)|GTEST(Target)
|-----|----|--------|------|---------|----------|
|1|Ubuntu+build essentials|NO|YES|NO|YES
|2|VM-SYNC|YES|YES|YES|YES
|3|RDK-DUNFELL(arm)|NO|YES|YES|YES
|4|RDK-DUNFELL(linux)|NO|YES|NO|YES
|5|RDK-KIRKSTONE(arm)|NO|YES|YES|YES
|6|RDK-KIRKSTONE(linux)|NO|YES|NO|YES

For ex:
On env 1,  the script will check for availability of CURL library, OpenSSL libraraies and Gtest libraries for Target.It would however would not look for CMAKE binary for host as this environment already provides cmake support which is provided by build essentials.
On env 2, on the other hand, none of the packages are present , hence the script will check for all of the packages listed above.

5. **Error Handling**:  
   Provides error messaging and graceful exits if any step (cloning, building, or checking) fails, ensuring robust automation.

6. **Output and Logs**:  
   Generates detailed output, including build logs (`build_log.txt`), and provides real-time feedback about successes and failures for each environment.

## Execution Flow:
1. **Clone the repository** for each environment.
2. **Run environment-specific builds** using the provided or default branch.
3. **Perform checks** on the build output to ensure that necessary files are present.
4. **Print results** for all environments in a summary format for review.

This script is a powerful tool for managing multi-environment development workflows, allowing seamless switching between branches, running builds, and verifying output in a repeatable, automated fashion.

## Caveats:
- Observed that during some of the runs the build_log.txt have undefined reference error, however same is not observed while runing it on terminal (w/o using the script).
 In the rare case of the above occurence, user can run manually change to the respective directory, remove `ut/ut-core` and `ut/build/`, and trigger the command : `build_ut.sh TARGET=<TARGET>`.To print the results user can comment all the 4 commands at the end of the bash script except print_results and run the script as usual.
- This script doesnot handle the case of `build_ut.sh TARGET=<TARGET> VARIANT=CPP` as this change is not available from platform test-suit as of now. This script only checks that the PR changes doesnot break the platform test-suit as it exists today.