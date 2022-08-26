# Unit Testing - Hal Testing Suite

| Date   | Author       | Comment |
|--------|--------------|---------|
| 26/08/22 | G. Weatherup | Initial release|
| 09/07/22 | G. Weatherup | Initial Revision|


# TODO: THIS DOCUMENT IS DRAFT AND REQUIRES REWORK

## Scope of the issue

To develop a L1, L2 testing suite to support vendor deliverables. This combines API Documentation, Specifications and Tests, delivered without infrastructure and RDK framework requirements.

## HAL Scope

Each of the HALS will use the hal `UT-Core` framework, it will provide all the configuration required to support building and running a common testing environment.

Each HAL component definition, will have individual cadence, specific documentation, and tagged testing suites to support them. All code is shared in the `comcast-sky` git hub, currently for internal consumption.

- [HAL Test Framework URL - Comcast-Sky GitHub](#https://github.com/orgs/comcast-sky/repositories?q=rdk%2Fcomponents%2Fhal%2F&type=all&language=&sort=name)

The naming convention chosen will allow for future convergence of the RDK-B/RDK-V/RDK-X stack into a single component based definition.

Naming convention as follows:-

```
- rdk-components-hal-<componentName>
- rdk-components-haltest-<componentName>
```

## Build Environment Requirements

Cloning the test code is available from here:

```bash
git@skyswteam.com:gweatherup/wifi_hal_test.git
```

```bash
.
├── cunit
│   ├── arm-rdk-linux-gnueabi -> Arm prebuild version of cunit.so
│   └── i686-pc-linux-gnu     -> Linux prebuild version of cunit.so
├── include -> (Link to HAL Headers)
├── libs -> (Vendor .so)
├── Makefile
├── README.md
├── src
│   ├── app -> (Test Control Application)
│   ├── stubs -> (Wifi_Hal Stubs)
│   └── test -> (Wifi Hal Test Cases)
├── tools
└── sysroot -> ( Link to Vendor Toolchain sysroot )

```

In order to build the `vendor`, or the `developer` will need to provide the following build libraries, and include HAL header files

```bash
├── include -> (Link to HAL Headers)
├── libs -> (Vendor .so)
```

`includes`, should be linked to a prebuild `skysr300` platform directory, e.g.

```build-skysr300/tmp/work/armv7at2-neon-rdkmllib32-linux-gnueabi/lib32-hal-wifi-sky/1.99+git999-r0/lib32-recipe-sysroot/usr/include/ccsp/```

`libs`, should be linked to a prebuilt hal_wifi specifically for sky e.g.

```build-skysr300/tmp/work/armv7at2-neon-rdkmllib32-linux-gnueabi/lib32-hal-wifi-sky/1.99+git999-r0/hal-wifi-sky-1.99+git999/.libs/```

`ln -s <source> <desintation>` can be used to setup these directories


### Toolchain

Toolchain is provided by the vendor, or via an SDK build in the Yocto build system for the given platform.

Recommand to install the toolchain into `./tools/2.0` directory, so that `sysroot` link will work correctly from that location

The SDK installer script copied into shared location [http://hydra.bskyb.com/shared/Projects/Internal/Yocto-RDK-SDK/](http://hydra.bskyb.com/shared/Projects/Internal/Yocto-RDK-SDK/)

**How to use SDK Toolchain**

1. Copy SDK installer script to Linux Machine (**rdk-glibc-x86_64-arm-toolchain-2.0.sh**)

2. `chmod +x rdk-glibc-x86_64-arm-toolchain-2.0.sh`

3. `./rdk-glibc-x86_64-arm-toolchain-2.0`

This will install the toolchain, sysroots etc into /opt/rdk/2.0 folder in your linux machine.

```bash
./rdk-glibc-x86_64-arm-toolchain-2.0.sh
```

```bash
RDK (A Yocto Project based Distro) SDK installer version 2.0

============================================================
Enter target directory for SDK (default: /opt/rdk/2.0):
You are about to install the SDK to "/opt/rdk/2.0". Proceed \[Y/n\]? Y
`\[sudo\] password for xxxx:`
Extracting SDK..............................................................................done
Setting it up...done
SDK has been successfully set up and is ready to be used.
Each time you wish to use the SDK in a new shell session, you need to source the environment setup script e.g.

. /opt/rdk/2.0/environment-setup-cortexa9t2-vfp-rdk-linux-gnueabi
```

4. Once to use cross development toolchain in the current shell, you need to source the environment setup script.

```bash
chmod +x  /opt/rdk/2.0/environment-setup-cortexa9t2-vfp-rdk-linux-gnueabi`
source /opt/rdk/2.0/environment-setup-cortexa9t2-vfp-rdk-linux-gnueabi
echo $CC
```

```bash
arm-rdk-linux-gnueabi-gcc -mthumb -mfpu=vfp -mcpu=cortex-a9 -mfloat-abi=soft -mabi=aapcs-linux -mno-thumb-interwork -ffixed-r8 -fomit-frame-pointer --sysroot=/opt/rdk/2.0/sysroots/cortexa9t2-vfp-rdk-linux-gnueabi
```

## Testing Environment

Cunit testing framework is used to deploy the HAL tests : [http://cunit.sourceforge.net/doc/index.html](http://cunit.sourceforge.net/doc/index.html)

## Making the code

There are two targets for the platform

1. linux - (default) will build all the tests, the test_app, and the stubs
2. arm - TARGET=arm, will build all the tests, and the test_app for the target

```bash
make clean
```
is not required unless you swap between targets or wish to clean

### Build the linux environment

```bash
make
```

This will build the following directories `src/test/*.c`, `src/app/*.c`, `src/stubs/*.c`

### Build the target arm environment

The toolchain must be sourced as above, once sources though swapping between linux & arm is possible as required.

```bash
make TARGET=arm
```

This will build the following directories `src/test/*.c`, `src/app/*.c`, and link against `libs/libhal_wifi.so`

The final output binary is build as `wifi_hal_test` and resides in the `obj` directory, the `cunit` .so files will be copied to the same directory.

## Running on the target

Copy files from `bin/*` to the target.

Log into the target 

Assuming the files that are copied are in the `/home/root` directory, then the following export is required.

```
export LD_LIBRARY_PATH=/usr/lib:/lib:/home/root
```

or use the `run.sh`, which is in the same directory

Now the hal test can be executed, `-h` for help is supported.

```bash
./hal_test  -h
Help
-c - Console Mode (Default)
-a - Automated Mode
-b - Basic Mode
-f - <filename> - set the output filename for automated mode
-l - List all tests run to a file
-h - Help
```

### Modes of operation

1. Console Mode - will open the interactive console
2. Automated Mode - will output in xUnit form as a .xml file
3. Basic Mode - All tests will be ran and the output redirected to the shell

## Source Tree

The tests are defined into the following structure

 ```bash
├── src
│   ├── app -> (Test Control Application)
│   ├── stubs -> (Wifi_Hal Stubs)
│   └── test -> (Wifi Hal Test Cases)
```

The main launch point test application, will configure the test system install the framework and register the tests

```bash
├── src
│   ├── app/main.c
```

The main test app will register all the tests via the following function

```c
static int register_hal_tests( void );
```

The wifi test cases are defined in the following directory

```bash
├── src
│   └── test -> (Wifi Hal Test Cases)
```

Example of registering test functions with the framework is

```c
 pSuite   =   UT_add_suite("[L1 test_wifi_hal_generic]", &test_generic_init, &test_generic_clean);
```

Each module has a optional `init` and `clean` function, and it should be putting all the requirements to run this module and clean itself up in these functions.

## Testing

The current file by file, and function by function, may not be the ideal way to support all L1 testing, since calling all functions without the wifi_init being called, is likely a requirement.

Other files maybe required, that's no issue, and maybe subdirectories would also be required in reality to make it more clear which level of testing is going on.

Repeating functionality should be minimised, it's ok for tests to call each other e.g.

No hal init, test all functions without calling init.

```c
test_all_functions_without_init( void )
{
    test\_generic\_wifi_init();
    test\_generic\_wifi_getHalCapability();
    test\_generic\_wifi_reset()
    ...
}
```

## Level 1 Testing

All about function testing, does each function operate as expected.

Requirements for test

- Independent test application that will run without the RDK
- Start the HAL interface if required, pull it down if already up, so the test can control it.
- Application to start up the HAL and perform Black Box Testing

- Negative testing
        - Make each interface fail as per the documentation
- Postive testing
        - Make each interface pass as per the documentation

## Level 2 Testing

The purpose of the test level is to test the module functionality from an operational point of view with positive testing.

- Independent test application that will run without the RDK
- Start the HAL interface if required, pull it down if already up, so the test can control it.
- Application to start up the HAL and perform Black Box Testing

Actual tests idea's are below, more can be defined as required, update of document should be perform as required.

## Level 3 Testing

External Stimuli testing.. Does the interface in the case of Wifi perform externally correctly..

Examples of which are:-

- is the SSID visible outside of the box
- does the wifi come up when the box is reset
- how much throughput is available on the interface, peer to peer
  - Control of the peer to peer devices
