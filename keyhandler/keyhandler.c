/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2023 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <fcntl.h>   // For open(), O_WRONLY, O_NONBLOCK
#include <string.h>
#include <assert.h>

#include <linux/uinput.h>
#include <ut_kvp.h>
#include <ut_log.h>

static ut_kvp_instance_t *gKVP_Instance = NULL;
#define KEYHANDLER_PROFILE_YAML_FILE "input-event-codes-5.15.yaml"

ut_kvp_status_t keyhandler_profile_open(char *fileName)
{
    ut_kvp_status_t result;

    if (gKVP_Instance == NULL)
    {
        gKVP_Instance = ut_kvp_createInstance();
        assert(gKVP_Instance != NULL);
    }

    result = ut_kvp_open(gKVP_Instance, fileName);
    assert( result == UT_KVP_STATUS_SUCCESS );

    return result;
}

void keyhandler_profile_close(void)
{
    if ( gKVP_Instance != NULL )
    {
        ut_kvp_destroyInstance(gKVP_Instance);
        gKVP_Instance = NULL;
    }
}

ut_kvp_instance_t *keyhandler_getInstance( void )
{
    return gKVP_Instance;
}

void print_usage(const char *program_name)
{
    printf("\nUsage: %s -p <profileFile> -t <event_type> -c <event_code> -v <value>\n", program_name);
    printf("More Info : https://github.com/rdkcentral/ut-core/discussions/148\n");
}

void keyhandler_emit(int fd, int type, int code, int val)
{
    struct input_event ie;

    ie.type = type;
    ie.code = code;
    ie.value = val;
    /* timestamp values below are ignored */
    ie.time.tv_sec = 0;
    ie.time.tv_usec = 0;

    write(fd, &ie, sizeof(ie)); //FIXME : For every write error check is needed

    ie.type = EV_SYN;          // for every KEY operation, need to report the event
    ie.code = SYN_REPORT;
    ie.value = 0;

    write(fd, &ie, sizeof(ie)); //FIXME : For every write error check is needed
}

int keyhandler_get_code(ut_kvp_instance_t *pInstance, char *type, char *code)
{
    char key_code_str[256];
    int key_code;

    snprintf(key_code_str, sizeof(key_code_str), "%s%s%s", type, "/", code);
    UT_LOG( "key_code_str=[%s]", key_code_str );
    key_code = ut_kvp_getUInt32Field( pInstance, key_code_str );
    if (key_code == 0)
    {
        UT_LOG_ERROR("Keycode not found");
        return 0;
    }

    UT_LOG( "key_code=[%d]", key_code );
    return key_code;
}

int keyhandler_get_type(ut_kvp_instance_t *pInstance, char *type)
{
    char event_type_str[256];
    int event_type_code;

    snprintf(event_type_str, sizeof(event_type_str), "%s%s", "EVENT_TYPE/", type);
    UT_LOG("event_type_str=[%s]", event_type_str);
    event_type_code = ut_kvp_getUInt32Field(pInstance, event_type_str);
    if (event_type_code == 0)
    {
        UT_LOG_ERROR("event type code not found");
        return 0;
    }

    UT_LOG("event_type_code=[%d]", event_type_code);
    return event_type_code;
}

void keyhandler_print_yaml(ut_kvp_instance_t *instance)
{
    char* kvpData;

    kvpData = ut_kvp_getData(instance);

    if(kvpData != NULL)
    {
        // Print the emitted KVP string
        printf("%s\n", kvpData);

        // Free the emitted KVP string
        free(kvpData);
    }
}

int main(int argc, char *argv[])
{
    int opt;
    char *profileFile = NULL;
    char *typeString = NULL;
    char *codeString = NULL;
    int type, code;
    int value;
    ut_kvp_status_t status;
    ut_kvp_instance_t *pInstance;
    struct uinput_setup usetup;

    // Parse command-line arguments
    /*
    •	-p <profileFile>
        Path to the profile YAML file defining event types and key mappings. Optional.
        Default : KEYHANDLER_PROFILE_YAML_FILE
    •	-t <event_type>
        Type of the event (e.g., EV_KEY). Required.
    •	-c <event_code>
        Event code to emit (e.g., KEY_A). Required.
    •	-v <value>
        Event value to emit. Required.
        •	1, 0 etc
    */
    while ((opt = getopt(argc, argv, "p:t:c:v:")) != -1)
    {
        switch (opt)
        {
        case 'p':
            profileFile = optarg;
            status = keyhandler_profile_open(optarg);
            if (status != UT_KVP_STATUS_SUCCESS)
            {
                UT_LOG_ERROR("Failed to Load profileFile [%s]", optarg);
            }
            UT_LOG_DEBUG("Profile File used: [%s]\n", profileFile);
            break;
        case 't':
            typeString = optarg;
            break;
        case 'c':
            codeString = optarg;
            break;
        case 'v':
            value = atoi(optarg);
            break;
        default:
            print_usage(argv[0]);
            return 1;
        }
    }

    // Validate required arguments
    if (profileFile == NULL)
    {
        profileFile = KEYHANDLER_PROFILE_YAML_FILE;
        status = keyhandler_profile_open(KEYHANDLER_PROFILE_YAML_FILE);
        if (status != UT_KVP_STATUS_SUCCESS)
        {
            UT_LOG_ERROR("Failed to Load profileFile [%s]", profileFile);
            keyhandler_profile_close();
            return -1;
        }
        UT_LOG_DEBUG("Profile File used: [%s]\n", profileFile);
    }

    pInstance = keyhandler_getInstance();
    if(pInstance == NULL)
    {
        UT_LOG_ERROR("KVP instance is NULL");
        return -1;
    }

    //keyhandler_print_yaml(pInstance);
    type = keyhandler_get_type(pInstance, typeString);
    code = keyhandler_get_code(pInstance, typeString, codeString);

    if (code == 0 || type == 0)
    {
        UT_LOG_ERROR("Code = [%d] and Type = [%d]\n", code, type);
        keyhandler_profile_close();
        return -1;
    }

    int fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    //FIXME : error checks for ioctls and fds

    /*
     * Note : Taken reference from : https://kernel.org/doc/html/latest/input/uinput.html#keyboard-events
     * The ioctls below will enable the device that is about to be
     * created, to pass key events.
     */
    //FIXME : error checks for ioctls and fds
    ioctl(fd, UI_SET_EVBIT, type);
    ioctl(fd, UI_SET_KEYBIT, code);

    memset(&usetup, 0, sizeof(usetup));
    usetup.id.bustype = BUS_USB;
    usetup.id.vendor = 0xbeef;  /* from RDK Key Simulator */
    usetup.id.product = 0xfedc; /* from RDK Key Simulator */
    usetup.id.version = 1; /* from RDK Key Simulator */
    strcpy(usetup.name, "key-simulator"); /* from RDK Key Simulator */

    ioctl(fd, UI_DEV_SETUP, &usetup);
    ioctl(fd, UI_DEV_CREATE);


    // Print the parsed arguments
    //UT_LOG_DEBUG("Profile file: %s\n", profileFile);
    //UT_LOG_DEBUG("type: %s\n", type);
    //UT_LOG_DEBUG("code: %s\n", code);
    //UT_LOG_DEBUG("value: %d\n", value);

    /* Key press, report the event, send key release, and report again */
    keyhandler_emit(fd, keyhandler_get_type(pInstance, typeString), keyhandler_get_code(pInstance, typeString, codeString), value);

    ioctl(fd, UI_DEV_DESTROY);
    close(fd);
    keyhandler_profile_close();

    return 0;
}