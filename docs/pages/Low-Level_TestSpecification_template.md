# L2/L3 Low Level Test Specification and Procedure Documentation template

## Table of Contents

- [L2/L3 Low Level Test Specification and Procedure Documentation template](#l2l3-low-level-test-specification-and-procedure-documentation-template)
  - [Table of Contents](#table-of-contents)
  - [Overview](#overview)
    - [Acronyms, Terms and Abbreviations](#acronyms-terms-and-abbreviations)
    - [Definitions](#definitions)
    - [References](#references)
  - [Level 2 Test Procedure](#level-2-test-procedure)

## Overview

This document describes the level 2 testing suite for the <component> module.

### Acronyms, Terms and Abbreviations

This section is intended to provide definitions for the acronyms and abbreviations used specifically in this module's

example:

- `HAL` \- Hardware Abstraction Layer, may include some common components
- `UT`  \- Unit Test(s)
- `OEM`  \- Original Equipment Manufacture
- `SoC`  \- System on a Chip

### Definitions

This sections should provide the definitions of all the significant terms used within this module

example:

- `ut-core` \- Common Testing Framework <https://github.com/rdkcentral/ut-core>, which wraps a open-source framework that can be expanded to the requirements for future framework.

### References

This sections should provide the High Level Test Specification document link

## Level 2 Test Procedure

The following functions are expecting to test the module operates correctly.

### Test 1

|Title|Details|
|--|--|
|Function Name|`test_l2_<filename>_<testName>`|
|Description|TODO: Add the description of what is tested and why in this test|
|Test Group|TODO: Basic (for L1): 01 / Module (L2): 02 / Stress (L2): 03)|
|Test Case ID|TODO: Add the ID of the test case so that it can be logically tracked in the logs|

**Pre-Conditions :**
TODO: Add pre-conditions, if any

**Dependencies :** TODO: Add dependencies for this test, if any

**User Interaction :** TODO: Add any user interactions required during this test, if any

#### Test Procedure :

TODO: Add the steps to run this test and add a line in the below table for each input variation tried in this function.

| Variation / Steps | Description | Test Data | Expected Result | Notes|
| -- | --------- | ---------- | -------------- | ----- |
| 01 | First set of conditions | What is input data to be tested | How to gauge success, is it a success variable? | Should be successful |

### Test 2

|Title|Details|
|--|--|
|Function Name|`test_l2_<filename>_<testName>`|
|Description|TODO: Add the description of what is tested and why in this test|
|Test Group|TODO: Basic (for L1): 01 / Module (L2): 02 / Stress (L2): 03)|
|Test Case ID|TODO: Add the ID of the test case so that it can be logically tracked in the logs|

**Pre-Conditions :**
TODO: Add pre-conditions, if any

**Dependencies :** TODO: Add dependencies for this test, if any

**User Interaction :** TODO: Add any user interactions required during this test, if any

#### Test Procedure :

TODO: Add the steps to run this test and add a line in the below table for each input variation tried in this function.

| Variation / Steps | Description | Test Data | Expected Result | Notes|
| -- | --------- | ---------- | -------------- | ----- |
| 01 | First set of conditions | What is input data to be tested | How to gauge success, is it a success variable? | Should be successful |