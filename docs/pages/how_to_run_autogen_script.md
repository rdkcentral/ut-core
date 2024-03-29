# How to run autogen script
## History

| Date   | Comment | Doc Version |
|--------|---------|---------|
| 12/01/23 | Initial release| 1.0.0 |
| 19/03/23 | Added debug | 1.1.0 |

## Contents
- [How to run autogen script](#how-to-run-autogen-script)
  - [History](#history)
  - [Contents](#contents)
  - [Overview](#overview)
  - [Autogen Command](#autogen-command)
  - [Autogen options explained](#autogen-options-explained)
  - [Examples](#examples)
  - [Index](#index)

## Overview
This document defines the guidelines and requirements for engineering teams on how to auto generate Level 1 and Level 2 testing suite frameworks.

## Autogen Command

```console
./autogenerate.sh <api-def-repo-url/-c/-clean/-b/-branch/-h/-help>
```

## Autogen options explained

| Argument  | Optional?|What is this?|Values/Examples |
|--------|--------------|--------------|------------------|
|api-def-repo-url|Y|Provide the API definitions repo url to be cloned |[git@github.com:rdkcentral/hal-deepsleepmanager.git](git@github.com:rdkcentral/hal-deepsleepmanager.git)|
|-clean / -c|Y|Delete the workspace dir (including all API definitions repos)|-clean / -c|
|-branch / -b |Y|This switch needs to be used along with the API definition branch to be checked out |-branch / -b [branchname]|
|-help / -h|Y|Show the usage of this command|-help / -h|

__Please Note :__ To include debug statements, please export the following env variable before running the script.

```console
    export AGT_DEBUG=1
```


## Examples

1. The below command will generate L1 and L2 test framework for 'deepsleepmanager' (along with adding template directories and files in the API definition directory and ut directory and also, generating skeletons in the ut directory, if any of them don't exist) HAL:

```console
    ./autogenerate.sh git@github.com:rdkcentral/hal-deepsleepmanager.git
```
2. The below command will generate L1 and L2 test framework for 'deepsleepmanager' (along with adding template directories and files in the API definition directory and ut directory and also, generating skeletons in the ut directory, if any of them don't exist) HAL on a 'hal-review' branch on the API definition repo:

```console
    ./autogenerate.sh git@github.com:rdkcentral/hal-deepsleepmanager.git -b hal-review
```
3.  The below command will delete the workspace directory (if it exists)

```console
    ./autogenerate.sh git@github.com:rdkcentral/hal-powermanager.git -c
```
4. The below command will show the usage for the autogen script

```console
    ./autogenerate.sh -h
```

## Index
| Terminlogy | Defintion      | Relative Location created |
|--------|--------------|---------|
|Workspace|The directory which is created by the tests generation scripts to clone the required repos and build the test framework|[ut-core-dir]/workspace|
|API Definitions|The main repo where the actual header files reside (e.g.: a HAL repo)|[ut-core-dir]/workspace/[API-definitions-dir]|
|UT|The directory under the particular API definition directory in workspace where the Unit Test repo (e.g.: HALTest) of that particular API Definition (e.g.: HAL) is cloned|<[ut-core-dir]/workspace/[API-definitions-dir]/ut|
|Skeletons|The generated skeleton code for the header files present in the API Definiton|[ut-core-dir]/workspace/[API-definitions-dir]/ut/skeletons/src|
|Tests|The L1 and L2 tests generated by the scripts for the header files present in the API Definiton |[ut-core-dir]/workspace/[API-definitions-dir]/ut/src|
|L1 Tests|The Level 1 testing suit are functional tests (positive and negative results) for each and every function in the header files [Please refer: TestSpecification.md#Level 1 Testing](TestSpecification.md#level-1-testing---functional-testing)|[ut-core-dir]/workspace/[API-definitions-dir]/ut/src/test_L1_*|
|L2 Tests|The purpose of this test level is to test the module functionality as much as possible from an operational point of view [Please refer: TestSpecification.md#Level 2 Testing](TestSpecification.md#level-2-testing---module-testing)|[ut-core-dir]/workspace/[API-definitions-dir]/ut/src/test_L2_*|
