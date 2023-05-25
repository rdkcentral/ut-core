# Snag list / Upgrade for ut-core
 
|Version|Comment|
|--------|-------|
|1.0|Initial release|
 

# Table Of Contents
 
- [Building Skeleton Library](Building-Skeleton-Library)
- [Testing Requriements](Testing-Requriements)
- [Key Value Pair](Key-Value-Pair)
- [Doxygen](Doxygen())
- [UT-Log](UT-Log)
- [C++ Support](C++-Support)
 
 See also [featureRequirements](ut-core-featureRequirements.md)

## Building Skeleton Library
 
Change the makefile, to that SKELETON_SRC is build into a library in `UT-CORE` makefiles, so no other dependencies area required.
 
It should detect if the image is already there, and not build it again, so to rebuild the library you could have to perform a clean.
 
It's up-to the caller, to replace the library with the target one in question as required, this can be done by copying the library in the lib directory, or by not copying it to the final target ,and using the one from rootfs or `LD_LIBRARY_PATH`

## Testing Requirements
 
We need to define a specification for requirements for testing, the do's and don't define what documentation / doxygen comments are required, and what are the expectations for test.
 
- Ensure we copy the specifications from and define the layers

## Key Value Pair
 
- In UT-Core we're going to support the reading and decoding of a YML / JSON file.
- This will be read into a internal KVP database, which can be accessed by calling the UT-Core
 
E.g run a specific platform test.
 
The configuration file is passed into ut-core via the '--config' parameter
 
`./run_test --config xione.de.wifi_configuration.yml`
 
The testing code would look like this
 
```c
                test_radio_count( void )
                {
                                int count = wifi_hal_getNumberOfRadios(handle);
                                UT_EXPECT_INT_KVP( count, "WifiRadioConfig.NumberOfRadios" );
                }
```
 
### Function defination
 
- ut_config.h
                - Doxygen Comments required
 
```c
typedef enum
{
                UT_CONFIG_STATUS_OK=0,
                UT_CONFIG_STATUS_FAILURE,
                ... <more as required>
}ut_config_status_t;
 
typedef ut_config_instance_t (void *);
 
ut_config_instance_t ut_config_open( char *configFile );
ut_config_status_t ut_config_close( ut_config_instance_t *pInstance );
 
uint64_t ut_config_get_uint64_kvp( ut_config_instance_t *pInstance, const char *key);
uint32_t ut_config_get_uint32_kvp( ut_config_instance_t *pInstance, const char *key);
uint8_t ut_config_get_uint8_kvp( ut_config_instance_t *pInstance, const char *key);
uint8_t *ut_config_get_string_kvp( ut_config_instance_t *pInstance, const char *key);
uint8_t *ut_config_get_float_kvp( ut_config_instance_t *pInstance, const char *key);
float *ut_config_get_float_kvp( ut_config_instance_t *pInstance, const char *key);
double *ut_config_get_double_kvp( ut_config_instance_t *pInstance, const char *key);
long double *ut_config_get_longDouble_kvp( ut_config_instance_t *pInstance, const char *key);
```
 
```c
                ut_config_instance_t *pInstance;
                pConfigInstance = ut_config_open( char *configFile );
...
                char myInt;
                float myFloat;
                const char *pMyString;
 
                myInt = ut_config_get_int_kvp( "WifiRadioConfig.NumberOfRadios" );
                myFloat = ut_config_get_string_kvp( "WifiRadioConfig.RadioName" );
                pMyString = ut_config_get_float_kvp( "WifiRadioConfig.RadioName" );
 
```
 
### Example configuration file
 
e..g -> xione.de.json
```json
{
                "WifiRadioConfig": [{
                                                "RadioIndex": 0,
                                                "RadioName": "radio1",
                                                "Enabled": true,
                                                "FreqBand": 1,
                                                "AutoChannelEnabled": true,
                                                "Channel": 1,
                                                "NumSecondaryChannels": 0,
                                                "SecondaryChannelsList": " ",
                                                "ChannelWidth": 2,
                                                "HwMode": 12,
                                                "CsaBeaconCount": 100,
                                                "Country": "US",
                                                "DcsEnabled": false,
                                                "DtimPeriod": 1,
                                                "OperatingClass": 0,
                                                "BasicDataTransmitRates": 656,
                                                "OperationalDataTransmitRates": 8112,
                                                "FragmentationThreshold": 2346,
                                                "GuardInterval": 16,
                                                "TransmitPower": 100,
                                                "BeaconInterval": 100,
                                                "RtsThreshold": 2347,
                                                "FactoryResetSsid": false,
                                                "RadioStatsMeasuringRate": 0,
                                                "RadioStatsMeasuringInterval": 0,
                                                "CtsProtection": false,
                                                "ObssCoex": false,
                                                "StbcEnable": false,
                                                "GreenFieldEnable": false,
                                                "UserControl": 0,
                                                "AdminControl": 0,
                                                "ChanUtilThreshold": 90,
                                                "ChanUtilSelfHealEnable": false
                                },
                                {
                                                "RadioIndex": 1,
                                                "RadioName": "radio2",
                                                "Enabled": true,
                                                "FreqBand": 2,
                                                "AutoChannelEnabled": true,
                                                "Channel": 40,
                                                "NumSecondaryChannels": 0,
                                                "SecondaryChannelsList": " ",
                                                "ChannelWidth": 1,
                                                "HwMode": 169,
                                                "CsaBeaconCount": 100,
                                                "Country": "US",
                                                "DcsEnabled": false,
                                                "DtimPeriod": 1,
                                                "OperatingClass": 0,
                                                "BasicDataTransmitRates": 656,
                                                "OperationalDataTransmitRates": 8112,
                                                "FragmentationThreshold": 2346,
                                                "GuardInterval": 16,
                                                "TransmitPower": 100,
                                                "BeaconInterval": 100,
                                                "RtsThreshold": 2347,
                                                "FactoryResetSsid": false,
                                                "RadioStatsMeasuringRate": 0,
                                                "RadioStatsMeasuringInterval": 0,
                                                "CtsProtection": false,
                                                "ObssCoex": false,
                                                "StbcEnable": false,
                                                "GreenFieldEnable": false,
                                                "UserControl": 0,
                                                "AdminControl": 0,
                                                "ChanUtilThreshold": 90,
                                                "ChanUtilSelfHealEnable": false
                                }
                ]
}
```
## Doxygen

- Upgrade doxygen to support mermaid from .md files into the HTML output, needs to be corrected.
                - It currently works in markdown displayed images, but not in the output for the HTML
- Check the latest version of doxygen, and check the patch level, does it support the above correctly?

## UT-Log

Enhance the support for UT-Log by under pinning the system with xfw_log. UT-Log will still exist just that the lower .c file will be a wrapper layer for xfw_log.

- Modules must have consistent logging, and a global system for ut-core to support module logging as required.
- All configuration should be driven by either switches to the command line, or incoming configuration files.

## C++ Support

Need to upgrade UT to support C++ Test

Compile Switch should switch the whole header file and makefile to download and compile the CPP Test Suite, rather than CUnit




