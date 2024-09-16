# Overview for release-test-script-platform.sh

This Bash script is designed for automating the process of cloning a Git repository, building software, and validating build artifacts across multiple environments. It simplifies testing and development workflows by managing repository setup, environment-specific builds, and post-build checks for required binaries and libraries. Here’s a high-level breakdown:

## Key Objectives:
1. **Automated Repository Setup**:  
   Clones a given Git repository (or a default one if not specified) for different target environments (e.g., Ubuntu, VM-SYNC, Dunfell Linux, Dunfell ARM).

2. **Environment-Specific Builds**:  
   Runs a customized build process based on the environment. It supports branching logic to switch and build from a specific branch (`UT_CORE_BRANCH_NAME`) when provided.

3. **Post-Build Checks**:  
   Verifies the presence of key files (such as binaries and libraries) in the build output, ensuring that each environment’s build process produces the required results.

4. **Cross-Environment Testing**:  
   Supports testing on multiple environments for following packages:
  

|ENV|CMAKE(Host)|CURL(Target)|OPENSSL(Target)|GTEST(Host)
|----|--------|------|---------|----------|
|Ubuntu+build essentials|NO|YES|NO|YES
|VM-SYNC|YES|YES|YES|YES
|RDK-DUNFELL(arm)|NO|YES|YES|YES
|RDK-DUNFELL(linux)|NO|YES|NO|YES

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
