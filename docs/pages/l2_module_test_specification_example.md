# HDMI CEC Level 2 - Module Testing
## History

| Version | Date(YY-MM-DD) | Author |Comments |
| -------| ----- | ----- | ----- |
| 1.0.0 | 22/02/23 | Anjali Thampi| Inital Document |

## Table of Contents

- [Overview](#overview)
  - [Acronyms](#acronyms)
  - [Definitions](#definitions)
  - [References](#references)
- [Level 2 Test Suite](#level-2-test-suite)
  - [Module Level Function to check the logical address](#module-level-function-to-check-the-logical-address)
 
## Overview

This document describes the level 2 testing suite for the HDMICEC module.

### Acronyms, Terms and Abbreviations

- `HAL` \- Hardware Abstraction Layer, may include some common components
- `HAL.h`  \- Abstracted defined API to control the hardware
- `HAL.c`  \- Implementation wrapper layer created by the `OEM` or `SoC` Vendor.
- `RDK`  \- Reference Design Kit for All Devices
- `RDK-B`  \- Reference Design Kit for Broadband Devices
- `RDK-V`  \- Reference Design Kit for Video Devices
- `UT`  \- Unit Test(s)
- `OEM`  \- Original Equipment Manufacture (Sky is also an OEM)
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
  - `ut-core` \- Common Testing Framework <https://github.com/comcast-sky/rdk-components-ut-core>, which wraps a open-source framework that can be expanded to the requirements for future comcast-sky framework.

### References

- `Feedback Loops` \- <https://www.softwaretestingnews.co.uk/4-methods-to-improve-your-feedback-loops-and-supercharge-your-testing-process/>
- `Doxygen` \- SourceCode documentation tool - <https://www.doxygen.nl/index.html>
- `Black Box Testing` \- <https://en.wikipedia.org/wiki/Black-box_testing>

## Level 2 Test Suite

The following functions are expecting to test the module operates correctly.

### Module Level Function to check the logical address

|Title|Details|
|--|--|
|Function Name|test_l2_hdmi_cec_driver_checkLogicalAddress()|
|Description|This module test will check if the logicalAddress added is what is received for the devType specified|
|Test Group|02 (Module)|
|Test Case ID|001|

**Pre-Conditions :**
-  There should be a HDMICEC enabled device connected via HDMI
- Config: "testcase:02001" - devType (will be driven by configuration)
- Config: "testcase:02001" - logicalAddresses[] (valid param driven by configuration - CONFIG)

**Dependencies :** N/A

**User Interaction :** N/A

#### Test Procedure :

|  Variation / Step | Description | Test Data | Expected Result | Notes|
| :--: | --------- | ---------- | -------------- | ----- |
| 01 | call HdmiCecOpen(&hdmiHandle) - open interface | handle | HDMI_CEC_IO_SUCCESS| Should Pass |
| 02 | call HdmiCecAddLogicalAddress() - Add one Logical Address to be used by host device  | handle=hdmiHandle, logicalAddress=CONFIG  | HDMI_CEC_IO_SUCCESS| Should Pass |
| 03 | call HdmiCecGetLogicalAddress() - Get the Logical Address obtained by the driver  | handle=hdmiHandle, devType=CONFIG logicalAddress=&returnedLogicalAddress  | HDMI_CEC_IO_SUCCESS| Should Pass |
| 04 | check if logicalAddress == returnedLogicalAddress | N/A | Values should match | Should Pass |
| 05 | call HdmiCecRemoveLogicalAddress  | handle=hdmiHandle logicalAddress=CONFIG  | HDMI_CEC_IO_SUCCESS| Should Pass |
| 06 |call HdmiCecClose() - close interface | handle=hdmiHandle | HDMI_CEC_IO_SUCCESS| Should Pass |