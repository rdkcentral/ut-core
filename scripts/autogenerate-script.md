# Use cases supported by autogenerate.sh

## Use cases:
1. API Def url can be any git repo url for which user has access ex:
- ./autogenerate.sh git@github.com:rdkcentral/rdk-halif-device_settings.git
- ./autogenerate.sh https://github.com/comcast-sky/xfw-xsign
- ./autogenerate.sh https://github.com/comcast-sky/skysr213-platform-hal.git

2. Unit Test directory can be either cloned by the user provided url or the absolute directory path provided by the user ex:
- Please input the URL (or leave blank to skip):    https://github.com/comcast-sky/skysr213-platform-hal.git
- Please input the directory (absolute) path (or leave blank to skip):    /home/jpn323/workspace/ut-core-28Oct/scripts/skysr213-platform-hal

3. The header files for which tests needs to be created can be inside include folder or anywhere in the API Definition directory cloned by the autogenerate script.