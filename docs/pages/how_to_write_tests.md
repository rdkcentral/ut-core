# Guidelines to Authoring Tests

# History

| Date | Author | Comment | Version |
| --- | --- | --- | --- |
| 17/02/23 | G. Weatherup & Anjali thampi | Initial Version | 1.1.0 |

# Contents

+ [Overview](#overview)
+ [Acronyms, Terms and Abbreviations](#acronyms-terms-and-abbreviations)
+ [Related Documents](#related-documents)
+ [Testing Specification Level 1 (L1)](#testing-specification-level-1-l1)
+ [Testing Specification Level 2 (L2)](#testing-specification-level-2-l2)
+ [Implementation Guidelines](#implementation-guidelines)
	- [Linux Environment](#linux-environment)
	- [Test Naming Convention](#test-naming-convention)
	- [Git Branching Stategy](#git-branching-stategy)
+ [File header for .c files](#file-header-for-c-files)
	- [Template](#template)
	- [Example of the .c file template](#example-of-the-c-file-template)
+ [Function brief template L1 functions](#function-brief-template-l1-functions)
	- [Template](#template-1)
	- [Example - Positive Test](#example---positive-test)
	- [Example - Negative Test](#example---negative-test)
+ [Function brief template L2 module functions](#function-brief-template-l2-module-functions)
	- [Template](#template-2)
	- [Example](#example)

## Overview

This document defines the guidelines and requirements for the engineering teams on how to write Level 1 and Level 2 testing suites.

For more information on the Levels of Tests refer to [halUnitTesting_requirements.md](halUnitTesting_requirements.md)

## Acronyms, Terms and Abbreviations

The following list explains acronyms, terms and abbreviations used in this document.

- `HAL` \- Hardware Abstraction Layer, may include some common components
- `L1`  \- Level 1 Testing Suite
- `L2`  \- Level 2 Testing Suite
- `UT`  \- Unit Testing Core Functionality
- `Vendor`  \- 3rd Party `Soc` or `Software provider` who will be required to pass all the tests
- `Component Owner`  \- Owner of a component who will approve and oversee code reviews
- `Component Architect`  \- Architect who will oversee and review the design

## Related Documents

The following is the list of reference materials whioch may be helpful in understanding the topics in this document.

|Reference / Title| Link|
|---------|------|
| `Feedback Loops` | [Link to Feedback loops](https://www.softwaretestingnews.co.uk/4-methods-to-improve-your-feedback-loops-and-supercharge-your-testing-process/) |
| `Doxygen` | [Link to SourceCode documentation tool](https://www.doxygen.nl/index.html) |
| `Black Box Testing` | [Link to Wikipedia page on Black Box Testing](https://en.wikipedia.org/wiki/Black-box_testing) |
| `Coding Guidelines` | [Link to RDK Central Coding Guidelines](https://developer.rdkcentral.com/source/source-code/source-code/coding_guideline/) |
| `UT Core` (Unit Testing Core) | [Link to UT Core Repo](https://github.com/comcast-sky/rdk-components-ut-core)
| `hal_unit_testing_requirements.md` | [Github Reference Documentation](https://github.com/comcast-sky/rdk-components-ut-core/tree/master/docs/pages)|
| `l2_test_specification_template.md` | [Github Reference Documentation](https://github.com/comcast-sky/rdk-components-ut-core/tree/master/docs/pages) |
| `example_l2_module_test_specification.md` | [Github Reference Documentation](https://github.com/comcast-sky/rdk-components-ut-core/tree/master/docs/pages) |
| `git_branching_strategy.md` | [Github Reference Documentation](https://github.com/comcast-sky/rdk-components-ut-core/tree/master/docs/pages) |
| `how_to_run_autogen_script.md` | [Github Reference Documentation](https://github.com/comcast-sky/rdk-components-ut-core/tree/master/docs/pages) |
| `Mermaid Graphics in Markdown` | [Link to Mermaid](https://mermaid.js.org/#/)

## Testing Specification Level 1 (L1)

The Level 1 testing functions are auto-generated in a basic form; these are generated from the `API` interface, examples of which are available in this document. Please refer [Function brief template L1 functions](#function-brief-template-l1-functions) section.

It is expected that the `Component Owner` / `Component Architect` will work together to define the requirements by editing the auto-generated doxygen comments.

Test implementation can then proceed with clear implementation scope based on the testing specification.

Basic L1 automated code generation will likely align with the requirements, but if it does not, it's upto the `Component Owner` and `Component Architect` to upgrade the functions required.

## Testing Specification Level 2 (L2)

The Level 2 basic functions are auto-generated from the header files, based on an assumption that each file is a functional group.

The expectation is that the `Component Owner` and `Component Architect` will create a L2 Specification markdown document.

Modification of the source files will be required to aid the implementation.

A template document for the requirements for Level 2 testing can be found in [L2 Testing Specification Template](l2_test_specification_template.md). This can form the basis of the expectations.

## Implementation Guidelines

In order to support consistent interface design, and cross platform support the following guidelines are considered :

- Any gaps either not yet completed or require further work will be marked in the code with:
	- TODO: Some expansion of the coding / documentation.
	- BUG: There's a bug to fix.
	- FIXME: Something is broken, and will require further investigate.
- All suites are `Platform independent` and should follow these rules:-
	- **All tests** are **independent of platform**, and all are driven only by configurations.
	- **All tests** should be **enabled**, the **code flow** should be **dynamically controlled by configurations**.
	- **#ifdef should not** be used to enable / disable parts of the testing suites.
	- **No hardcoding values** should exist in the code, to reduce the number of cross platform issues.
- All field values, where applicable should be checkd by configurations.
- Each test should log it's "Test Group ID", "Test Case ID" and "Variation Step".
- Each test should log each of the variation steps.
- Common logging should be used from ut-core.

### Linux Environment :

There is a linux environment to aid rapid development, to generate the skeleton files and the framework and is expected to be run in an IDE. This will greatly aid debugging the testing environnement, before running on the target platform.

It's optional for the test developer to make the skeletons pass, if this is useful or helpful to aid debugging of the tests cases.

Engineers should be using the linux environment for building, and walking through their test code to ensure it's functionally correct. It's expected this can be setup and run in an IDE e.g. Visual Studio Code.

### Test Naming Convention :

Tests in the framework are named with a three field number convention as follows:-

| Field | Digits |
|-----|------|
| Test Group | 2, (Range [00-03]) |
| Test ID | 3, (Range [000-999]) |
| Variation / Step | 2, (Range [00-99]) |

|Test Group|Group Name|
|----------|-----------|
| Basic Tests | 01 (Level 1 tests are expected to be in this group) |
| Advanced Tests | 02 (Level 2 tests are expected to be in this group) |
| Stress Tests | 03 (Level 2 tests are expected to be in this group) |

*For example*: Test *`0300105`* means, Stress Test [`03`], Test ID test [`001`], Test Variation [`05`]

### Git Branching Stategy :

For information on the suggested branching strategy refer to the following document :- [gitBranchingStrategy.md](gitBranchingStrategy.md)

## File header for .c files

The following doxygen template should be placed at the top of any .c file and populated as required.

### Template :

```c
/**
* @file TODO: the filename of the file
* @page module_name TODO: Required field, name of the main module
* @subpage sub_page_name TODO: Add a function group if relevant
*
* ## Module's Role
* TODO: Explain the module's role in the system in general
* This is to ensure that the API meets the operational requirements of the module across all vendors.
*
* **Pre-Conditions:**  TODO: Add pre-conditions if any@n
* **Dependencies:** TODO: Add dependencies if any@n
*
* Ref to API Definition specification documentation : [halSpec.md](../../../docs/halSpec.md)
*/
```
### Example of the .c file template :

The following examples a filled out example a header file 

```c
/**
* @file test_L1_hdmi_cec_driver.c
* @page HDMI_CEC_L1_Tests HDMI CEC Level 1 Tests
*
* ## Module's Role
* This module includes Level 1 functional tests (success and failure scenarios).
* This is to ensure that the API meets the operational requirements of the HDMI CEC across all vendors.
*
* **Pre-Conditions:**  None@n
* **Dependencies:** None@n
*
* Ref to API Definition specification documentation : [halSpec.md](../../../docs/halSpec.md)
*/
```

## Function brief template L1 functions

The following doxygen template should be placed at the top of all .c functions that are used for L1 testing and populated as required.

### Template :

```c
/**
* @brief TODO: Describe the objective of the test
*
* TODO: Add the description of what is tested and why in this test@n
*
* **Test Group ID:** TODO: Basic (for L1): 01 / Module (L2): 02 / Stress (L2): 03)@n
* **Test Case ID:** TODO: Add the ID of the test case so that it can be logically tracked in the logs@n
* **Priority:** (Low/Med/High) TODO: Add the priority for the level of test, how important is the test to overall functionality@n
* @n
* **Pre-Conditions:** TODO: Add pre-conditions, if any@n
* **Dependencies:** TODO: Add dependencies for this test, if any@n
* **User Interaction:** TODO: Add any user interactions required during this test, if any
* @n
* **Test Procedure:**@n
* TODO: Add the steps to run this test and add a line in the below table for each input variation tried in this function.@n
* | Variation / Step | Description | Test Data |Expected Result |Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | First set of conditions | What is input data to be tested | How to gauge success, is it a success variable? | Should be successful |
*/
void test_l1_<filename>_<testName>( void );
```

### Example - Positive Test :

```c
/**
* @brief This function checks that HdmiCecSetLogicalAddress() operates correctly
*
* Calls the header function HdmiCecSetLogicalAddress() with correct params
*
* **Test Group ID:** Basic: 01@n
* **Test Case ID:** 001@n
* **Priority:** Medium@n
*
* **Pre-Conditions:**@n
* - Config: "testcase:01" - logicalAddresses[] - (valid param driven by configuration - CONFIG)
* - Config: "testcase:01" - num - (valid param be driven by configuration - CONFIG)
* **Dependencies:** Module must be installed for use@n
* **User Interaction:** N/A
*
* **Test Procedure:**@n
*
* |Variation / Step|Description|Test Data|Expected Result|Notes|
* |:--:|---------|----------|--------------|-----|
* |01|call HdmiCecOpen(&hdmiHandle) - open interface | handle | HDMI_CEC_IO_SUCCESS| Should Pass |
* |02|call HdmiCecSetLogicalAddress() - call with valid set logical addresses| handle=hdmiHandle, logicalAddresses=CONFIG, num=CONFIG  | HDMI_CEC_IO_SUCCESS| Should Pass |
* |03|call HdmiCecSetLogicalAddress() - repeat call with valid set logical addresses | handle=hdmiHandle, logicalAddresses=CONFIG, num=CONFIG  | HDMI_CEC_IO_SUCCESS| Should Pass |
* |04|call HdmiCecClose() - close interface | handle=hdmiHandle | HDMI_CEC_IO_SUCCESS| Should Pass |
*/
void test_l1_hdmi_cec_driver_positive_HdmiCecSetLogicalAddress( void )
```

### Example - Negative Test :

```c
/**
* @brief This function checks HdmiCecSetLogicalAddress with negative logical addresses
*
* Calls the header function HdmiCecSetLogicalAddress() with incorrect params@n
*
* **Test Group ID:** Basic: 01@n
* **Test Case ID:** 002@n
* **Priority:** Medium@n
*
* **Pre-Conditions:**@n
* - Config: "testcase:01002"   - logicalAddresses[] - (valid params driven by configuration - CONFIG)
* - Config: "testcase:01002" - logicalAddresses[] (valid params with one out of bounds value - CONFIG)
* - num (will be driven by configuration - CONFIG)
* **Dependencies:** Module must be installed for use@n
* **User Interaction:** N/A@n
*
* **Test Procedure:**@n
*
* |Variation / Step|Description|Test Data|Expected Result|Notes|
* |:--:|---------|----------|--------------|-----|
* |01|call HdmiCecSetLogicalAddress() - call with invalid handle | handle=NULL | HDMI_CEC_IO_INVALID_STATE| Should Fail |@n
* |02|call HdmiCecOpen(&hdmiHandle) - open interface | handle | HDMI_CEC_IO_SUCCESS| Should Pass |@n
* |03|call HdmiCecSetLogicalAddress() - with no logical addresses| logicalAddresses=NULL, num=CONFIG | HDMI_CEC_IO_INVALID_ARGUMENT| Should Fail |@n
* |04|call HdmiCecSetLogicalAddress() - with a wrong num for logical addresses sent |logicalAddresses=CONFIG, num=0 | HDMI_CEC_IO_INVALID_ARGUMENT | Should Fail |@n
* |05|call HdmiCecSetLogicalAddress() - with a one of the logical addresses greater than max:(CONFIG) | logicalAddresses=CONFIG, num=CONFIG | HDMI_CEC_IO_SENT_FAILED| Should Fail |@n
* |06|call HdmiCecClose() - close interface | handle=hdmiHandle | HDMI_CEC_IO_SUCCESS| Should Pass |@n
* |07|call HdmiCecSetLogicalAddress() after HdmiCecClose() | handle=NULL | HDMI_CEC_IO_INVALID_STATE| Should Fail |@n
*/
void test_l1_hdmi_cec_driver_negative_HdmiCecSetLogicalAddress( void )
```

## Function brief template L2 module functions

### Template :

```c
/**
* @brief TODO: Describe the object of the test
*
* TODO: Add the description of what is tested and why in this test
*
* **Test Group ID:** TODO: Add the group this test belongs to - Basic (for L1): 01 / Module (L2): 02 / Stress (L2): 03)@n
* **Test Case ID:** TODO: Add the ID of the test case so that it can be logically tracked in the logs@n
* **Priority:** (Low/Med/High) TODO: Add the priority for the level of test, how important is the test to overall functionality@n
*
* **Test Procedure:**
* Refer to UT specification documentation [l2_module_test_specification.md](l2_module_test_specification.md)
*/
void test_l2_<filename>_<testName>( void );
 ```

 ### Example

```c
/**
* @brief This module test will check if the logicalAddress added is what is received for the devType specified@n
*
* **Test Group ID:** 02@n
* **Test Case ID:** 001@n
* **Priority:** Low@n
*
* **Test Procedure:**
* Refer to UT specification documentation [l2_module_test_specification.md](l2_module_test_specification.md)
*/
void test_l2_hdmi_cec_driver_checkLogicalAddress( void )
```

### Level 2 Markdown Documentation

Level 2 requires a more detailed informational document to understand the nuances and interactions of the module. The document should include diagrams and detailed information. It is recommended that markdown be used, along side diagrams via mermaid, which `SCM`'s like github support in their markdown renderers. https://mermaid.js.org/#/

There is a basic template provided [l2_test_specification_template.md](https://github.com/comcast-sky/rdk-components-ut-core/tree/master/docs/pages)

There is a simple example of how to fill out the document [example_l2_module_test_specification.md](https://github.com/comcast-sky/rdk-components-ut-core/tree/master/docs/pages) |