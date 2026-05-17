# ut-core AI Brief

> Single-file reference for the RDK ut-core unit testing framework.
> An AI reading ONLY this file should be able to write correct test code,
> integrate a new project, and understand all build/config options.

## 1. Purpose

ut-core is the standard unit-testing framework for RDK HAL (Hardware Abstraction Layer) components. It provides:

- A backend-agnostic C/C++ test API that abstracts over CUnit (C path) or Google Test (C++ path).
- A KVP (Key-Value Pair) profile system for loading YAML/JSON device configuration at runtime.
- Three execution modes: Console (interactive), Basic (stdout), and Automated (xUnit/JUnit XML).
- Integration with the **ut-control** support library (KVP engine, logging, control-plane helpers) and **ut-raft** (Python test orchestration).
- A template system for bootstrapping new HAL test projects.

Repository: `https://github.com/rdkcentral/ut-core`
License: Apache 2.0
Current version tag: **5.0.0.0** (latest tag on the develop branch; `5.0.0` is the immediately preceding tag)

---

## 2. Architecture Overview

```
+--------------------------+
|   HAL Test Binary        |  (downstream project, e.g. haltest-wifi)
|   main.c                 |
+-----------+--------------+
            |
            v
+-----------+--------------+
|        ut-core           |
|  include/ut.h            |  <-- unified public API
|  src/ut_main.c           |  <-- CLI, option parsing, init/exit
|  src/ut_kvp_profile.c    |  <-- KVP profile singleton
+-----------+--------------+
    |                  |
    v (UT_CUNIT)       v (VARIANT=CPP)
+----------+      +----------+
| CUnit    |      | GTest    |
| 2.1-3    |      | 1.15.2   |
+----------+      +----------+
            \      /
             v    v
+----------------------------+
|       ut-control           |  (KVP engine, logging, helpers)
|  framework/ut-control/     |  (cloned by build.sh)
+----------------------------+
```

### Compile-time backend selection

| VARIANT value | Define set      | Backend   | Source path                     |
|---------------|----------------|-----------|---------------------------------|
| `C` (default) | `-DUT_CUNIT`   | CUnit 2.1-3 | `src/c_source/ut_cunit.c`     |
| `CPP`         | (none extra)    | GTest 1.15.2 | `src/cpp_source/ut_gtest.cpp` |

When `UT_CUNIT` is defined, `ut.h` includes `ut_cunit.h`.
When `UT_CUNIT` is **not** defined (C++ path), `ut.h` includes `ut_gtest.h`.

---

## 3. Public API -- Common (ut.h)

### Types

```c
typedef enum {
    UT_STATUS_OK = 0,
    UT_STATUS_FAILURE
} UT_status_t;

typedef enum {
    UT_TESTS_L1 = 1,       // Level 1 basic tests
    UT_TESTS_L2,            // Level 2 advanced tests
    UT_TESTS_L3,            // Level 3 module tests
    UT_TESTS_L4,            // Level 4 module control (start/stop) -- not run as a test suite
    UT_TESTS_HUMAN_L2,      // L2 requiring human interaction
    UT_TESTS_HUMAN_L3,      // L3 requiring human interaction
    UT_TESTS_HUMAN_L4,      // L4 requiring human interaction
    UT_TESTS_VDEVICE,       // Setup-specific suite; cannot run on a real device
    UT_TESTS_UNKNOWN,       // Default for currently existing suites without explicit group
    UT_TESTS_MAX            // Out-of-range marker (not a valid value)
} UT_groupID_t;
```

### Functions

| Signature | Description |
|-----------|-------------|
| `UT_status_t UT_init(int argc, char** argv)` | Initialize framework; parse CLI flags and start up the system. Must be called first from `main()`. Also prints the `UT CORE Version`. |
| `void UT_exit(void)` | Release resources (closes the KVP profile via `ut_kvp_profile_close()`). Called automatically at the end of `UT_run_tests()` in both the CUnit and GTest paths. |
| `UT_status_t UT_run_tests(void)` | Execute all registered suites/tests in the selected mode. Returns `UT_STATUS_OK` if execution completed without framework error. |

---

## 4. C Path API (ut_cunit.h) -- When VARIANT=C / `-DUT_CUNIT`

### Additional types (C path only)

```c
typedef void UT_test_suite_t;          // Opaque suite handle
typedef void UT_test_t;                // Opaque test handle
typedef void (*UT_TestFunction_t)(void);
typedef int (*UT_TestCleanupFunction_t)(void);
typedef int (*UT_InitialiseFunction_t)(void);
typedef int (*UT_CleanupFunction_t)(void);
```

These suite/test typedefs and the registration prototypes below are declared in `ut.h` itself (inside the `#ifdef UT_CUNIT` block), not in `ut_cunit.h`. `ut_cunit.h` contains only the assertion macros.

### Suite/test registration functions (C path only)

| Signature | Description |
|-----------|-------------|
| `UT_test_suite_t *UT_add_suite(const char *pTitle, UT_InitialiseFunction_t pInit, UT_CleanupFunction_t pClean)` | Register a suite. `pInit`/`pClean` may be NULL (defaults provided). Returns handle or NULL on error. Implicitly uses `UT_TESTS_UNKNOWN` group. |
| `UT_test_suite_t *UT_add_suite_withGroupID(const char *pTitle, UT_InitialiseFunction_t pInit, UT_CleanupFunction_t pClean, UT_groupID_t groupId)` | Same as above but assigns a group ID for selective enable/disable via `-d`/`-e` CLI flags. |
| `UT_test_t *UT_add_test(UT_test_suite_t *pSuite, const char *pTitle, UT_TestFunction_t pFunction)` | Add a test case to a suite. |
| `const char *UT_getTestSuiteTitle(UT_test_suite_t *pSuite)` | Get suite title string. |
| `void UT_regsiter_test_cleanup_function(UT_test_suite_t *pSuite, UT_TestCleanupFunction_t pFunction)` | Register post-suite cleanup. Note: name has typo "regsiter" -- this is the actual API name. |

### Assertion macros (C path -- CUnit backend)

All non-`_FATAL` macros record failure but **continue** execution.
All `_FATAL` macros record failure and **abort the current test**.

| Macro | Parameters | Description |
|-------|-----------|-------------|
| `UT_PASS(msg)` | `msg` (string literal) | Force pass, continue |
| `UT_FAIL(msg)` | `msg` | Record failure, continue |
| `UT_FAIL_FATAL(msg)` | `msg` | Record failure, abort test |
| `UT_ASSERT(value)` | expression | Assert truthy |
| `UT_ASSERT_FATAL(value)` | expression | Assert truthy, abort on fail |
| `UT_ASSERT_TRUE(value)` | expression | Same as UT_ASSERT |
| `UT_ASSERT_TRUE_FATAL(value)` | expression | Same as UT_ASSERT_FATAL |
| `UT_ASSERT_FALSE(value)` | expression | Assert falsy |
| `UT_ASSERT_FALSE_FATAL(value)` | expression | Assert falsy, abort |
| `UT_ASSERT_EQUAL(actual, expected)` | int, int | Assert equal (cast to int) |
| `UT_ASSERT_EQUAL_FATAL(actual, expected)` | int, int | Assert equal, abort |
| `UT_ASSERT_NOT_EQUAL(actual, expected)` | int, int | Assert not equal |
| `UT_ASSERT_NOT_EQUAL_FATAL(actual, expected)` | int, int | Assert not equal, abort |
| `UT_ASSERT_STRING_EQUAL(expected, actual)` | string, string | strcmp == 0 |
| `UT_ASSERT_STRING_EQUAL_FATAL(expected, actual)` | string, string | strcmp == 0, abort |
| `UT_ASSERT_STRING_NOT_EQUAL(expected, actual)` | string, string | strcmp != 0 |
| `UT_ASSERT_STRING_NOT_EQUAL_FATAL(expected, actual)` | string, string | strcmp != 0, abort |
| `UT_ASSERT_PTR_EQUAL(actual, expected)` | pointer, pointer | Assert pointers equal |
| `UT_ASSERT_PTR_EQUAL_FATAL(actual, expected)` | pointer, pointer | Assert pointers equal, abort |
| `UT_ASSERT_PTR_NOT_EQUAL(actual, expected)` | pointer, pointer | Assert pointers differ |
| `UT_ASSERT_PTR_NOT_EQUAL_FATAL(actual, expected)` | pointer, pointer | Assert pointers differ, abort |
| `UT_ASSERT_PTR_NULL(value)` | pointer | Assert NULL |
| `UT_ASSERT_PTR_NULL_FATAL(value)` | pointer | Assert NULL, abort |
| `UT_ASSERT_PTR_NOT_NULL(value)` | pointer | Assert not NULL |
| `UT_ASSERT_PTR_NOT_NULL_FATAL(value)` | pointer | Assert not NULL, abort |
| `UT_ASSERT_MSG(value, message)` | expression, string | Assert truthy with custom message |
| `UT_ASSERT_MSG_FATAL(value, message)` | expression, string | Assert truthy with message, abort |
| `UT_ASSERT_TRUE_MSG(value, message)` | expression, string | Assert true with message |
| `UT_ASSERT_TRUE_MSG_FATAL(value, message)` | expression, string | Assert true with message, abort |
| `UT_ASSERT_FALSE_MSG(value, message)` | expression, string | Assert false with message |
| `UT_ASSERT_FALSE_MSG_FATAL(value, message)` | expression, string | Assert false with message, abort |
| `UT_ASSERT_LOG(value, message)` | expression, string | Assert truthy; always logs message regardless of pass/fail |
| `UT_ASSERT_LOG_FATAL(value, message)` | expression, string | Assert truthy; always logs, abort on fail |

> Note: there is no `UT_PASS_FATAL` macro in the C path -- that macro exists only in the C++ (GTest) path.

---

## 5. C++ Path API (ut_gtest.h) -- When VARIANT=CPP

### Test fixture class

```cpp
class UTCore : public ::testing::Test {
protected:
    UTCore() = default;
    ~UTCore() override = default;
    void SetUp() override;
    void TearDown() override;
public:
    static std::string UT_get_test_filter();
    static bool UT_add_suite_withGroupID(const std::string& testSuiteName, UT_groupID_t group);
private:
    static std::unordered_map<std::string, UT_groupID_t> suiteToGroup;
};
```

### Test registration macros (C++ path)

| Macro | Parameters | Description |
|-------|-----------|-------------|
| `UT_ADD_TEST(test_suite_name, test_name)` | fixture class, test name | Expands to `TEST_F(test_suite_name, test_name)`. The suite class must derive from `UTCore`. |
| `UT_ADD_TEST_TO_GROUP(groupName, groupID)` | identifier, `UT_groupID_t` | Registers a test suite under a group for selective filtering. `groupName` must match the fixture/suite name and be a valid identifier (not a string literal). |
| `UT_IGNORE_TEST()` | (none) | Skip current test (`GTEST_SKIP()`). |

### Assertion macros (C++ path -- GTest backend)

Non-`_FATAL` use GTest `EXPECT_*` (continue on failure).
`_FATAL` use GTest `ASSERT_*` (abort test on failure).

| Macro | Parameters | GTest equivalent |
|-------|-----------|------------------|
| `UT_ASSERT_TRUE(condition)` | expression | `EXPECT_TRUE` |
| `UT_ASSERT_TRUE_FATAL(condition)` | expression | `ASSERT_TRUE` |
| `UT_ASSERT_FALSE(condition)` | expression | `EXPECT_FALSE` |
| `UT_ASSERT_FALSE_FATAL(condition)` | expression | `ASSERT_FALSE` |
| `UT_ASSERT_EQUAL(actual, expected)` | any, any | `EXPECT_EQ` |
| `UT_ASSERT_EQUAL_FATAL(actual, expected)` | any, any | `ASSERT_EQ` |
| `UT_ASSERT_NOT_EQUAL(actual, expected)` | any, any | `EXPECT_NE` |
| `UT_ASSERT_NOT_EQUAL_FATAL(actual, expected)` | any, any | `ASSERT_NE` |
| `UT_ASSERT_LESS(actual, expected)` | any, any | `EXPECT_LT` |
| `UT_ASSERT_LESS_EQUAL(actual, expected)` | any, any | `EXPECT_LE` |
| `UT_ASSERT_GREATER(actual, expected)` | any, any | `EXPECT_GT` |
| `UT_ASSERT_GREATER_EQUAL(actual, expected)` | any, any | `EXPECT_GE` |
| `UT_ASSERT_NULL(ptr)` | pointer | `EXPECT_EQ(nullptr, ptr)` |
| `UT_ASSERT_NULL_FATAL(ptr)` | pointer | `ASSERT_EQ(nullptr, ptr)` |
| `UT_ASSERT_NOT_NULL(ptr)` | pointer | `EXPECT_NE(nullptr, ptr)` |
| `UT_ASSERT_NOT_NULL_FATAL(ptr)` | pointer | `ASSERT_NE(nullptr, ptr)` |
| `UT_ASSERT_STRING_EQUAL(actual, expected)` | c-string, c-string | `EXPECT_STREQ` |
| `UT_ASSERT_STRING_NOT_EQUAL(actual, expected)` | c-string, c-string | `EXPECT_STRNE` |
| `UT_ASSERT_STRING_EQUAL_IGNORE_CASE(actual, expected)` | c-string, c-string | `EXPECT_STRCASEEQ` |
| `UT_ASSERT_STRING_NOT_EQUAL_IGNORE_CASE(actual, expected)` | c-string, c-string | `EXPECT_STRCASENE` |
| `UT_ASSERT_DOUBLE_EQUAL(actual, expected, tolerance)` | double, double, double | `EXPECT_NEAR` |
| `UT_ASSERT_FLOAT_EQUAL(actual, expected, tolerance)` | float, float, float | `EXPECT_NEAR` |
| `UT_ASSERT_THROW(statement, exception_type)` | statement, type | `EXPECT_THROW` |
| `UT_ASSERT_ANY_THROW(statement)` | statement | `EXPECT_ANY_THROW` |
| `UT_ASSERT_NO_THROW(statement)` | statement | `EXPECT_NO_THROW` |
| `UT_ASSERT_MESSAGE(condition, message)` | expression, string | `EXPECT_TRUE(cond) << msg` |
| `UT_FAIL(message)` | string | `FAIL() << message` |
| `UT_FAIL_FATAL(message)` | string | `FAIL() << message` |
| `UT_PASS(message)` | string | `SUCCEED() << message` |
| `UT_PASS_FATAL(message)` | string | `SUCCEED() << message` |

> Note: the C++ path does **not** define the `_MSG`/`_LOG` macro family or the `UT_ASSERT_PTR_*` macros from the C path, and has no plain `UT_ASSERT` (use `UT_ASSERT_TRUE`). Conversely, `UT_ASSERT_LESS`, `UT_ASSERT_GREATER`, the throw macros, and the floating-point and ignore-case string macros exist only in the C++ path.

---

## 6. KVP Profile System (ut_kvp_profile.h)

The KVP profile system allows tests to load device-specific configuration from YAML or JSON files at runtime. This is used to parameterize tests with platform-specific expected values (e.g., number of audio ports, supported video formats). `ut_kvp_profile.h` includes `ut.h` and `ut_kvp.h` (the latter from ut-control).

### Core functions

```c
ut_kvp_status_t ut_kvp_profile_open(char *fileName);    // Load YAML/JSON profile
void ut_kvp_profile_close(void);                         // Close and free profile
ut_kvp_instance_t *ut_kvp_profile_getInstance(void);     // Get singleton KVP handle
```

The profile is loaded via the `-p <filename>` CLI flag, which calls `ut_kvp_profile_open()` internally.

### Profile accessor macros

| Macro | Returns |
|-------|---------|
| `UT_KVP_PROFILE_GET_BOOL(key)` | `bool` |
| `UT_KVP_PROFILE_GET_UINT8(key)` | `uint8_t` |
| `UT_KVP_PROFILE_GET_UINT16(key)` | `uint16_t` |
| `UT_KVP_PROFILE_GET_UINT32(key)` | `uint32_t` |
| `UT_KVP_PROFILE_GET_UINT64(key)` | `uint64_t` |
| `UT_KVP_PROFILE_GET_INT8(key)` | `int8_t` |
| `UT_KVP_PROFILE_GET_INT16(key)` | `int16_t` |
| `UT_KVP_PROFILE_GET_INT32(key)` | `int32_t` |
| `UT_KVP_PROFILE_GET_INT64(key)` | `int64_t` |
| `UT_KVP_PROFILE_GET_LIST_COUNT(key)` | `uint32_t` |
| `UT_KVP_PROFILE_GET_STRING(key, buffer)` | Writes to `buffer` (char array sized `UT_KVP_MAX_ELEMENT_SIZE`) |

Keys use slash-separated paths into the YAML/JSON document, e.g. `"dsAudio/ports/0/name"`.

### Profile assertion macros

These combine profile lookup with assertion in one call:

| Macro | Parameters |
|-------|-----------|
| `UT_ASSERT_KVP_EQUAL_PROFILE_BOOL(checkValue, key)` | expected bool, key string |
| `UT_ASSERT_KVP_EQUAL_PROFILE_UINT8(checkValue, key)` | expected uint8, key string |
| `UT_ASSERT_KVP_EQUAL_PROFILE_UINT16(checkValue, key)` | expected uint16, key string |
| `UT_ASSERT_KVP_EQUAL_PROFILE_UINT32(checkValue, key)` | expected uint32, key string |
| `UT_ASSERT_KVP_EQUAL_PROFILE_UINT64(checkValue, key)` | expected uint64, key string |
| `UT_ASSERT_KVP_EQUAL_PROFILE_INT8(checkValue, key)` | expected int8, key string |
| `UT_ASSERT_KVP_EQUAL_PROFILE_INT16(checkValue, key)` | expected int16, key string |
| `UT_ASSERT_KVP_EQUAL_PROFILE_INT32(checkValue, key)` | expected int32, key string |
| `UT_ASSERT_KVP_EQUAL_PROFILE_INT64(checkValue, key)` | expected int64, key string |
| `UT_ASSERT_KVP_EQUAL_PROFILE_LIST_COUNT(checkValue, key)` | expected count, key string |
| `UT_ASSERT_KVP_EQUAL_PROFILE_STRING(checkValue, key)` | expected string, key string |
| `UT_ASSERT_KVP_NOT_EQUAL_PROFILE_STRING(checkValue, key)` | unexpected string, key string |

Deprecated aliases (still functional): `UT_ASSERT_EQUAL_KVP_PROFILE_BOOL`, `UT_ASSERT_EQUAL_KVP_PROFILE_UINT64`, `UT_ASSERT_EQUAL_KVP_PROFILE_STRING`.

---

## 7. CLI Options

The test binary (built as `hal_test` by default) accepts these flags. Options are parsed with `getopt_long` using the option string `cabhf:l:tp:d:e:`:

| Flag | Description |
|------|-------------|
| `-c` | Console mode -- interactive menu. Console is the default mode, so `-c` is accepted but has no dedicated handler (no-op). |
| `-b` | Basic mode -- run all tests, output to stdout |
| `-a` | Automated mode -- generate xUnit/JUnit XML report |
| `-t` | Automated mode + list all tests run to a file |
| `-l <path>` | Set log file output path (default: `/tmp/`) |
| `-p <filename>` | Load a KVP profile (YAML or JSON); also used by the `kvp_assert` macros |
| `-d <group_id>` | Disable test group (1-9, see `UT_groupID_t`) |
| `-e <group_id>` | Enable test group |
| `-h` | Show help and exit |
| `-f <arg>` | Accepts an argument in the option string but has no handler; effectively reserved/unused. |
| `--gtest_output=<...>` | Long option recognized as a placeholder (GTest output passthrough). |

Group IDs correspond to `UT_groupID_t` values (1 = L1, 2 = L2, 3 = L3, 4 = L4, 5 = HUMAN_L2, 6 = HUMAN_L3, 7 = HUMAN_L4, 8 = VDEVICE, 9 = UNKNOWN). A group value `>= UT_TESTS_MAX` is rejected as invalid.

---

## 8. Build System

### Key files

| File | Role |
|------|------|
| `Makefile` | Core build logic; included/invoked by downstream projects |
| `build.sh` | Downloads CUnit/GTest + clones ut-control; invoked by the `framework` target via `download_and_build` |

### Build commands

```bash
# C path (CUnit) -- default
make                          # linux target
make TARGET=arm               # ARM cross-compile

# C++ path (GTest)
make VARIANT=CPP              # linux target
make VARIANT=CPP TARGET=arm   # ARM cross-compile

# Clean
make clean                    # Clean build objects, libs and the .variant file
make cleanall                 # Clean everything including framework downloads
```

The Makefile records the active VARIANT in a `.variant` file and aborts the build if VARIANT changes without a clean (`checkvariantchange` rule).

### Makefile variables for downstream integration

Downstream projects (e.g. `haltest-xxx`) set these variables **before** invoking ut-core's Makefile:

| Variable | Purpose | Example |
|----------|---------|---------|
| `TOP_DIR` | Root of the downstream project | `$(ROOT_DIR)` |
| `SRC_DIRS` | Space-separated list of source directories to compile | `$(ROOT_DIR)/src` |
| `INC_DIRS` | Additional include directories | `$(ROOT_DIR)/../include` |
| `TARGET_EXEC` | Output binary name | `hal_test` (default) |
| `TARGET` | Build target: `linux` or `arm` | `linux` (default) |
| `VARIANT` | `C` (default) or `CPP` | `C` |
| `YLDFLAGS` | Extra linker flags (e.g., for HAL libs) | `-L$(HAL_LIB_DIR) -lhal_xxx` |
| `HAL_LIB_DIR` | Path to vendor HAL shared libraries | `$(ROOT_DIR)/libs` |
| `XCFLAGS` / `KCFLAGS` | Extra compiler flags (`XCFLAGS` is seeded from `KCFLAGS`) | `-DNDEBUG` |
| `BIN_DIR` / `LIB_DIR` / `BUILD_DIR` | Override output locations (default under `$(TOP_DIR)/build/`) | -- |
| `BUILD_WEAK_STUBS_SRC` | Path to weak-stub source files (compiled into `libweak_stubs_libs.so`) | `/path/to/stubs` |

### Output directories

| Directory | Contents |
|-----------|----------|
| `build/bin/` | Final test binary + copied `.so` files (ut-control libs, weak-stubs, HAL libs) |
| `build/<TARGET>/lib/` | Built libraries (copied `libut_control.*`, `libweak_stubs_libs.so`) |
| `build/<TARGET>/obj/` | Object files |
| `build/<TARGET>/cpp_libs/` | GTest libraries built for the C++ path |

### Downstream Makefile pattern

A typical downstream `Makefile` (see `template/ut_template/Makefile`):

```makefile
ROOT_DIR := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
BIN_DIR := $(ROOT_DIR)/bin
TOP_DIR := $(ROOT_DIR)

SRC_DIRS = $(ROOT_DIR)/src
INC_DIRS := $(ROOT_DIR)/../include
XCFLAGS := -DNDEBUG

# For linux builds (default), include skeleton stubs
ifeq ($(TARGET),linux)
SRC_DIRS += $(ROOT_DIR)/skeletons/src
endif

# For ARM builds, link vendor HAL library
ifeq ($(TARGET),arm)
HAL_LIB_DIR := $(ROOT_DIR)/libs
YLDFLAGS = -Wl,-rpath,$(HAL_LIB_DIR) -L$(HAL_LIB_DIR) -lhal_xxx
endif

export YLDFLAGS BIN_DIR SRC_DIRS INC_DIRS TARGET TOP_DIR HAL_LIB_DIR XCFLAGS

build:
	make -C ./ut-core
```

The `skeletons/src` directory is supplied by the downstream HAL test project (it is not committed inside `template/ut_template/`).

### Downstream build.sh pattern

The template `build.sh` (see `template/ut_template/build.sh`):

1. Sets `UT_PROJECT_MAJOR_VERSION` (the template default is `"3."`) to pin the major version.
2. Clones `rdkcentral/ut-core` as `./ut-core` if not present.
3. Checks out the highest tag matching the major version.
4. On subsequent runs, `check_next_revision` warns if a newer major version exists.
5. Delegates to `make -C . -f Makefile` (which in turn runs `./ut-core/build.sh` to download frameworks).

---

## 9. Test Registration Patterns

### C path pattern

```c
#include <ut.h>

// Forward-declare registration functions
extern int register_l1_tests(void);
extern int register_l2_tests(void);

int main(int argc, char** argv) {
    UT_init(argc, argv);

    register_l1_tests();
    register_l2_tests();

    UT_run_tests();
    return 0;
}
```

Registration function:

```c
#include <ut.h>

static UT_test_suite_t *pSuite = NULL;

void test_positive_case(void) {
    int result = myHalFunction(validParam);
    UT_ASSERT_EQUAL(result, 0);
}

void test_negative_case(void) {
    int result = myHalFunction(NULL);
    UT_ASSERT_NOT_EQUAL(result, 0);
}

int register_l1_tests(void) {
    pSuite = UT_add_suite_withGroupID("[L1 MyHAL]", NULL, NULL, UT_TESTS_L1);
    if (pSuite == NULL) return -1;

    UT_add_test(pSuite, "test_positive_case", test_positive_case);
    UT_add_test(pSuite, "test_negative_case", test_negative_case);
    return 0;
}
```

### C++ path pattern

```cpp
#include <ut.h>

class MyHalTests : public UTCore {
protected:
    void SetUp() override { /* per-test setup */ }
    void TearDown() override { /* per-test teardown */ }
};

UT_ADD_TEST_TO_GROUP(MyHalTests, UT_TESTS_L1);

UT_ADD_TEST(MyHalTests, PositiveCase) {
    int result = myHalFunction(validParam);
    UT_ASSERT_EQUAL(result, 0);
}

UT_ADD_TEST(MyHalTests, NegativeCase) {
    int result = myHalFunction(nullptr);
    UT_ASSERT_NOT_EQUAL(result, 0);
}
```

In the C++ path the Makefile links `libgtest_main`, which supplies the `main()` entry point. `ut_gtest.cpp` provides ut-core's own `UT_init()` and `UT_run_tests()` implementations (including the interactive console menu and `UT_exit()` cleanup); these are the same unified API entry points as the C path. The framework filtering and group-activation logic are driven through `UTCore` and the `UTTestRunner` class in `ut_gtest.cpp`.

---

## 10. Template Directory Structure

`template/ut_template/` provides a skeleton for new test projects. Actual committed layout:

```
ut_template/
  build.sh            # Clone ut-core + trigger build
  Makefile            # Set TOP_DIR, SRC_DIRS, INC_DIRS, delegate to ut-core
  README.md
  CONTRIBUTING.md  LICENSE  NOTICE  COPYING
  src/
    main.c                      # UT_init -> register tests -> UT_run_tests
    test_l1_test_example.c
    test_l2_test_example.c
  bin/
    run.sh            # Helper to set LD_LIBRARY_PATH and execute
  docs/
    pages/
      README.md
      CONTRIBUTING.md
      LICENSE.md
      NOTICE.md
      l2_module_test_specification_template.md
  tools/              # Placeholder directory for project tooling
```

`template/api_definition_template/` provides a `build_ut.sh` script for the top-level HAL API repository to clone and trigger the test suite build.

---

## 11. Logging

ut-core provides logging via `ut_log.h` (from ut-control). Key macros used by ut-core's headers and sources:

| Macro | Usage |
|-------|-------|
| `UT_LOG(fmt, ...)` | General log output |
| `UT_LOG_ERROR(fmt, ...)` | Error-level log |
| `UT_LOG_PREFIX(prefix, msg)` | Log with colored prefix |
| `UT_LOG_ASSERT(macro_name, ...)` | Assertion failure log (called by the C-path assertion macros) |

Color constants: `UT_LOG_ASCII_GREEN`, `UT_LOG_ASCII_RED`, `UT_LOG_ASCII_YELLOW`, `UT_LOG_ASCII_NC` (no color).

Log files default to `/tmp/` and the path can be changed with `-l <path>`.

---

## 12. Integration with ut-control

**ut-control** (`framework/ut-control/`) is the RDK unit-testing support library, cloned and built by `build.sh` (via `git clone git@github.com:rdkcentral/ut-control.git`). It provides:

- **KVP engine**: Parses YAML/JSON configuration files. The `ut_kvp_*` functions used by the profile system come from ut-control.
- **Logging**: `ut_log.h` and the `UT_LOG*` macros.
- **Control-plane helpers**: ut-control additionally ships control-plane functionality used for remote interaction and result handling; the test binary links against `libut_control.so`.
- **Key types from ut-control**: `ut_kvp_instance_t`, `ut_kvp_status_t` (including `UT_KVP_STATUS_SUCCESS`), `UT_KVP_MAX_ELEMENT_SIZE`.
- **KVP accessor functions**: `ut_kvp_getBoolField()`, `ut_kvp_getUInt8Field()`, `ut_kvp_getUInt16Field()`, `ut_kvp_getUInt32Field()`, `ut_kvp_getUInt64Field()`, `ut_kvp_getInt8Field()`, `ut_kvp_getInt16Field()`, `ut_kvp_getInt32Field()`, `ut_kvp_getInt64Field()`, `ut_kvp_getStringField()`, `ut_kvp_getListCount()`, `ut_kvp_createInstance()`, `ut_kvp_destroyInstance()`, `ut_kvp_open()`.

`build.sh` pins ut-control via `UT_CONTROL_PROJECT_VERSION="2.0.0"` and checks out that tag. If a newer ut-control tag exists, the build prints an upgrade recommendation but does not auto-upgrade across major versions.

---

## 13. Integration with ut-raft

**ut-raft** is a Python-based test automation framework that drives ut-core test binaries on target devices. It:

1. Deploys the test binary to the target device.
2. Launches the binary in automated mode (`-a`) with a profile (`-p`).
3. Collects the xUnit/JUnit XML results.
4. Can selectively enable/disable test groups via `-d`/`-e` flags.

ut-raft orchestrates the test binary over a device session (e.g. serial/SSH) and consumes the produced XML results and logs.

---

## 14. Running Tests on Target

```bash
# Copy bin/* to target device
scp bin/* root@target:/home/root/

# On target
export LD_LIBRARY_PATH=/usr/lib:/lib:/home/root:./
./hal_test -b                          # Basic mode, all tests
./hal_test -a                          # Automated, xUnit/JUnit XML output
./hal_test -b -p platform_profile.yaml # With device profile
./hal_test -b -d 4                     # Disable L4 group
./hal_test -b -e 1 -e 2               # Enable only L1 and L2
```

---

## 15. Execution Modes Detail

| Mode | Flag | Output | Use case |
|------|------|--------|----------|
| Console | (default; `-c` accepted) | Interactive menu in terminal | Manual testing, debugging |
| Basic | `-b` | Results to stdout + log file | CI/CD, quick runs |
| Automated | `-a` | xUnit/JUnit XML file + log file | Integration with ut-raft, Jenkins |

The internal mode enum (`TestMode_t` in `src/ut_internal.h`) is `UT_MODE_BASIC`, `UT_MODE_AUTOMATED`, `UT_MODE_CONSOLE`.

---

## 16. Version Scheme

Tags use a dotted numeric form (e.g. `5.0.0`, `5.0.0.0`).

- Major version changes indicate ABI-breaking changes requiring test code updates.
- Minor/patch updates are backwards-compatible within a major version.
- Downstream projects pin to a major version via `UT_PROJECT_MAJOR_VERSION` (ut-core templates) / `UT_PROJECT_VERSION` (api_definition template) in their build scripts.
- The build embeds the version from `git describe --tags` into the binary as the `UT_VERSION` define, printed by `UT_init()`.

---

## 17. Dependencies

| Dependency | Version | How acquired |
|-----------|---------|-------------|
| CUnit | 2.1-3 | Downloaded by `build.sh` from SourceForge (C path), then patched |
| Google Test | 1.15.2 | Downloaded by `build.sh` from GitHub (C++ path) |
| ut-control | 2.0.0 | Cloned by `build.sh` from `rdkcentral/ut-control` (`git@github.com` SSH remote) |
| CMake | > 3.12 | Required for building GTest (system cmake preferred, else one bundled with ut-control) |
| GCC/G++ | System | For linux target |
| ARM toolchain | Vendor/SDK | For arm target (sourced via environment) |

---

## 18. File Map

```
ut-core/
  include/
    ut.h                    # Unified public API header (also declares C-path suite/test API)
    ut_cunit.h              # CUnit assertion macros (C path)
    ut_gtest.h              # GTest assertion macros + UTCore class (C++ path)
    ut_kvp_profile.h        # KVP profile API + accessor/assertion macros
  src/
    ut_main.c               # CLI parsing, UT_init(), UT_exit()
    ut_kvp_profile.c        # KVP profile singleton implementation
    ut_internal.h           # Internal types (TestMode_t, optionFlags_t, groupFlag_t) + internal prototypes
    c_source/
      ut_cunit.c            # CUnit backend: suite/test registration, run modes
      ut_cunit_internal.h   # CUnit internal function declarations
      cunit_lgpl/           # CUnit run-mode sources (ut_basic.c, ut_console.c, ut_automated.c) + patches/
    cpp_source/
      ut_gtest.cpp          # GTest backend: UTCore class, UTTestRunner, console/filtering UI
  framework/                # Downloaded at build time (not committed)
    CUnit-2.1-3/            # CUnit source (C path)
    gtest/<TARGET>/         # GTest source (C++ path)
    ut-control/             # ut-control subproject
  template/
    ut_template/            # Skeleton for new HAL test projects
    api_definition_template/ # Skeleton for HAL API repos (build_ut.sh)
  scripts/                  # Autogenerate + release-test helper scripts and templates
  tests/                    # Self-tests for ut-core itself (incl. demo/ examples)
  build.sh                  # Framework download script
  Makefile                  # Core build rules
  compare-functions-in-headers-testsuite.sh  # Header/binary symbol comparison helper
```
