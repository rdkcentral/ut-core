# L2 Test Specification template

## History

| Version | Date(YY-MM-DD) | Comments |
| --------| -------------- |  ----- |
| 1.0.0 | 22/02/23 | Inital Document |

## Table of Contents

- [L2 Test Specification template](#l2-test-specification-template)
  - [History](#history)
  - [Table of Contents](#table-of-contents)
  - [Overview](#overview)
    - [Acronyms, Terms and Abbreviations](#acronyms-terms-and-abbreviations)
    - [Definitions](#definitions)
    - [References](#references)
  - [Level 2 Test Suite](#level-2-test-suite)
    - [Test 1](#test-1)
      - [Test Procedure 1](#test-procedure-1)
    - [Test 2](#test-2)
      - [Test Procedure 2](#test-procedure-2)

## Overview

This document describes the level 2 testing suite for the `<component>` module.

### Acronyms, Terms and Abbreviations

- `HAL` \- Hardware Abstraction Layer, may include some common components
- `HAL.h`  \- Abstracted defined API to control the hardware
- `HAL.c`  \- Implementation wrapper layer created by the `OEM` or `SoC` Vendor.
- `RDK`  \- Reference Design Kit for All Devices
- `RDK-B`  \- Reference Design Kit for Broadband Devices
- `RDK-V`  \- Reference Design Kit for Video Devices
- `UT`  \- Unit Test(s)
- `OEM`  \- Original Equipment Manufacture
- `SoC`  \- System on a Chip

### Definitions

- `Broadcom` \- `SoC` manufacturer <https://www.broadcom.com/>
- `Amlogic` \- `SoC` manufacturer <https://en.wikipedia.org/wiki/Amlogic>
- `Soc Vendor` \- Definition to encompass multiple vendors
- `Unit Tests` \- C Function tests that run on the target hardware
- `Common Testing Framework` \- Off the shelf 3rd Party Testing Framework, or framework that does not require infrastructure to control it. That's not to say it cannot be controlled via infrastructure if required. Examples of which are.
  - `GTest` \- Google Test Suit <https://google.github.io/googletest>
  - `CUnit` \- C Testing Suit <http://cunit.sourceforge.net/>
  - `Unity` \- C Embedded Testing Suit <http://www.throwtheswitch.org/unity>
  - `ut-core` \- Common Testing Framework <https://github.com/rdkcentral/ut-core>, which wraps a open-source framework that can be expanded to the requirements for future framework.

### References

- `Feedback Loops` \- <https://www.softwaretestingnews.co.uk/4-methods-to-improve-your-feedback-loops-and-supercharge-your-testing-process/>
- `Doxygen` \- SourceCode documentation tool - <https://www.doxygen.nl/index.html>
- `Black Box Testing` \- <https://en.wikipedia.org/wiki/Black-box_testing>

## Level 2 Test Suite

The following functions are expecting to test the module operates correctly.

### Test 1

|Title|Details|
|--|--|
|Function Name|`test_l2_<filename>_<testName>`|
|Description|TODO: Add the description of what is tested and why in this test|
|Test Group|TODO: Basic (for L1): 01 / Module (L2): 02 / Stress (L2): 03)|
|Test Case ID|TODO: Add the ID of the test case so that it can be logically tracked in the logs|
|Priority|TODO: (Low/Med/High) Add the priority for the level of test, how important is the test to overall functionality|

**Pre-Conditions :**
TODO: Add pre-conditions, if any

**Dependencies :** TODO: Add dependencies for this test, if any

**User Interaction :** TODO: Add any user interactions required during this test, if any

#### Test Procedure 1

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
|Priority|TODO: (Low/Med/High) Add the priority for the level of test, how important is the test to overall functionality|

**Pre-Conditions :**
TODO: Add pre-conditions, if any

**Dependencies :** TODO: Add dependencies for this test, if any

**User Interaction :** TODO: Add any user interactions required during this test, if any

#### Test Procedure 2

TODO: Add the steps to run this test and add a line in the below table for each input variation tried in this function.

| Variation / Steps | Description | Test Data | Expected Result | Notes|
| -- | --------- | ---------- | -------------- | ----- |
| 01 | First set of conditions | What is input data to be tested | How to gauge success, is it a success variable? | Should be successful |
