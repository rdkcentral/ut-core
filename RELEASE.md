# Release Notes

| Date   | Comment |
|--------|---------|
| 02/01/24 | Added for release 2.0.0 |

## Version 2.0.0

This is a major change compared to Version 1.x.x, fundamentially previous to Version 2.0.0 all functions where fatal.

- All functions are no longer FATAL
    Now all functions fall through, and are only fatal upton request by caller with the `_FATAL` extension.
- Color logging is now enabled
- Prefixes have been added to all logging so that grep can be used to narrow down issues etc. `cat /xxxx.log| grep ASSERT`
- Log files are always present in all modes
- Log file location is displayed in the log at output
- Adding Testing Suites to test all conditions of ut-core macro's, assumption lower framework (e.g. CUNIT etc) are tested outside of scope.
  - Tests are under `./tests/` and can be ran with `make` from the directory.
- Upgrades framework build scripts to support Major version handling and informing the users of upgrades possible.
  - Minor upgrades are automatically taken if the scripts are used.
  - **Recommended** that all users take the new template versions of `./template/api_definition_template/build_ut.sh` - for their header file directory
  - **Recommended** that all users take the new template for testing suite `template/ut_template/build.sh`, where minor changes will automatically be taken
- Log output for all modes is now controlled by ut-core.
- added option `-f - <filename>` - set the output filename for automated mode
- seperated lower framework from upper, but core re-structure to allow introduction of further frameworks.

### Change on behaviour for ASSERTS

There is a major change in behaviour to allow test code to continue without failure:-

- All `UT_ASSERT` - functions are now NOT fatal.
- All `_FATAL` - all ASSERTS with `_FATAL` extensions are now fatal.

```bash
# Version 1.7.0
UT_ASSERT(TRUE)       # is fatal and the test will quit

#Version 2.0.0
UT_ASSERT(TRUE)       # is NOT fatal and the test will continue
UT_ASSERT_FATAL(TRUE) # is fatal and the test will quit
```

### Testing Suites

Testing suites have been added, and are under `./tests` directory these can be ran by calling `make`, then executing the binary under `./obj/ut-test -a`
