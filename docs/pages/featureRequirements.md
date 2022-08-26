# Feature requirements for the UT-Core

## History

|Version|Date (YY-MM-DD)|Author|Comments|
|-------|-----|-----|----|
|1.0.0| 26/08/22 |Gerald Weatherup| First Release|

## Document Scope

This document states the future expansion requirements for the ut-core framework

## Future Core Requirements

### Configuration Validation

Note: This requirement needs further specification and is only defined here so as not to be forgotten

- All of the `components` require to define a getCapabilities() function, and this function and the parameters returned by it are specific to each platform. As defined by the product specification.

- The requirement therefore is two fold.

  - Add support for ut-core KeyValuePair value checking into the core framework, for STRINGS, UINTS, ARRAYS etc.
  - Add platform specific definitions configuration files, ideally .yml to support the platform, and each platform will require definition of this.