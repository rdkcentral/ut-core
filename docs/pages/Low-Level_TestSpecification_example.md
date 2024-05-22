# HDMI CEC L2 Low Level Test Specification and Procedure Documentation

## Table of Contents

- [HDMI CEC L2 Low Level Test Specification and Procedure Documentation](#hdmi-cec-l2-low-level-test-specification-and-procedure-documentation)
  - [Table of Contents](#table-of-contents)
  - [Overview](#overview)
    - [Acronyms, Terms and Abbreviations](#acronyms-terms-and-abbreviations)
    - [Definitions](#definitions)
    - [References](#references)
  - [Level 2 Test Procedure](#level-2-test-procedure)
    - [Module Level Function to check the logical address](#module-level-function-to-check-the-logical-address)

## Overview

This document describes the level 2 testing suite for the HDMICEC module.

### Acronyms, Terms and Abbreviations

- `HAL` \- Hardware Abstraction Layer, may include some common components
- `UT`  \- Unit Test(s)
- `DUT` \- Device Under Test

### Definitions

- `ut-core` \- Common Testing Framework <https://github.com/rdkcentral/ut-core>, which wraps a open-source framework that can be expanded to the requirements for future framework.

### References

- `High Level Test Specification` - link

## Level 2 Test Procedure

The following functions are expecting to test the module operates correctly.

### Module Level Function to check the logical address

|Title|Details|
|--|--|
|Function Name|test_l2_hdmi_cec_driver_checkLogicalAddress()|
|Description|This module test will check if the logicalAddress added is what is received for the devType specified|
|Test Group|02 (Module)|
|Priority|High|

**Pre-Conditions :** None

**Dependencies :** None

**User Interaction :** None

#### Test Procedure:

|  Variation / Step | Description | Test Data | Expected Result | Notes|
| :--: | --------- | ---------- | -------------- | ----- |
| 01 | call HdmiCecOpen(&hdmiHandle) - open interface | handle | HDMI_CEC_IO_SUCCESS| Should Pass |
| 02 | call HdmiCecAddLogicalAddress() - Add one Logical Address to be used by host device  | handle=hdmiHandle, logicalAddress=CONFIG  | HDMI_CEC_IO_SUCCESS| Should Pass |
| 03 | call HdmiCecGetLogicalAddress() - Get the Logical Address obtained by the driver  | handle=hdmiHandle, devType=CONFIG logicalAddress=&returnedLogicalAddress  | HDMI_CEC_IO_SUCCESS| Should Pass |
| 04 | check if logicalAddress == returnedLogicalAddress | N/A | Values should match | Should Pass |
| 05 | call HdmiCecRemoveLogicalAddress  | handle=hdmiHandle logicalAddress=CONFIG  | HDMI_CEC_IO_SUCCESS| Should Pass |
| 06 |call HdmiCecClose() - close interface | handle=hdmiHandle | HDMI_CEC_IO_SUCCESS| Should Pass |