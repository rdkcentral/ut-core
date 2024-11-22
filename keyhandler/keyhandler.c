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
#include <unistd.h>  // For write(), close(), and sleep()
#include <fcntl.h>   // For open(), O_WRONLY, O_NONBLOCK
#include <string.h>

#include <ut_kvp_profile.h>
#include <linux/uinput.h>
#include <ut_log.h>
#include <ut_kvp.h>

void print_usage(const char *program_name)
{
    printf("Usage: %s -p <profile_file> -t <event_type> -c <event_code> -v <value>\n", program_name);
    printf("OR if keys are included in the profile yaml file then\n");
    printf("Usage: %s -p <profile_file>\n", program_name);
}

void emit(int fd, int type, int code, int val)
{
    struct input_event ie;

    ie.type = type;
    ie.code = code;
    ie.value = val;
    /* timestamp values below are ignored */
    ie.time.tv_sec = 0;
    ie.time.tv_usec = 0;

    write(fd, &ie, sizeof(ie));

    ie.type = EV_SYN;
    ie.code = SYN_REPORT;
    ie.value = 0;

    write(fd, &ie, sizeof(ie));
}

int get_code(ut_kvp_instance_t *pInstance, char *code)
{
    char key_code_str[256];
    int key_code;

    snprintf(key_code_str, sizeof(key_code_str), "%s%s", "key_codes/", code);
    UT_LOG( "key_code_str=[%s]", key_code_str );
    key_code = ut_kvp_getUInt32Field( pInstance, key_code_str );

    UT_LOG( "key_code=[%d]", key_code );
    return key_code;
}

int get_type(ut_kvp_instance_t *pInstance, char *type)
{
    char event_type_str[256];
    int event_type_code;

    snprintf(event_type_str, sizeof(event_type_str), "%s%s", "event_types/", type);
    UT_LOG("event_type_str=[%s]", event_type_str);
    event_type_code = ut_kvp_getUInt32Field(pInstance, event_type_str);

    UT_LOG("event_type_code=[%d]", event_type_code);
    return event_type_code;
}

void print_yaml(ut_kvp_instance_t *instance)
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
    char *profile_file = NULL;
    char *type = NULL;
    char *code = NULL;
    int value;
    int list_count;
    ut_kvp_status_t status;
    ut_kvp_instance_t *pInstance;
    //char result_code[UT_KVP_MAX_ELEMENT_SIZE]={-1};
    struct uinput_setup usetup;
    
    //char* kvpData;
    

    // Parse command-line arguments
    while ((opt = getopt(argc, argv, "p:t:c:v:")) != -1)
    {
        switch (opt)
        {
        case 'p':
            profile_file = optarg;
            status = ut_kvp_profile_open(optarg);
            if (status != UT_KVP_STATUS_SUCCESS)
            {
                UT_LOG_ERROR("Failed to Load profile_file [%s]", optarg);
            }
            break;
        case 't':
            type = optarg;
            break;
        case 'c':
            code = optarg;
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
    if (profile_file == NULL ) {
        fprintf(stderr, "Error: Missing required arguments.\n");
        print_usage(argv[0]);
        return 1;
    }

    pInstance = ut_kvp_profile_getInstance();
    ///UT_ASSERT( pInstance != NULL );
    if(pInstance == NULL)
    {
        UT_LOG_ERROR("perhaps platform profile was not passed using -p switch");
        return -1;
    }

    list_count = ut_kvp_getListCount(pInstance, "remote/events");
    UT_LOG_DEBUG("listcount = [%d]", list_count);

    //print_yaml(pInstance);

    int fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);


    /*
     * The ioctls below will enable the device that is about to be
     * created, to pass key events, in this case the space key.
     */
    //ioctl(fd, UI_SET_EVBIT, EV_KEY);
    //ioctl(fd, UI_SET_KEYBIT, result_kvp);

    memset(&usetup, 0, sizeof(usetup));
    usetup.id.bustype = BUS_USB;
    usetup.id.vendor = 0x1234;  /* sample vendor */
    usetup.id.product = 0x5678; /* sample product */
    strcpy(usetup.name, "Example device");

    ioctl(fd, UI_DEV_SETUP, &usetup);
    ioctl(fd, UI_DEV_CREATE);

    /*
     * On UI_DEV_CREATE the kernel will create the device node for this
     * device. We are inserting a pause here so that userspace has time
     * to detect, initialize the new device, and can start listening to
     * the event, otherwise it will not notice the event we are about
     * to send. This pause is only needed in our example code!
     */
    sleep(1);

    // for(int i = 0; i < list_count; i++)
    // {
        if (type == NULL || code == NULL || value == 0)
        {
            ut_kvp_status_t status;
            char result_type[UT_KVP_MAX_ELEMENT_SIZE] = {0xff};
            char result_code[UT_KVP_MAX_ELEMENT_SIZE] = {0xff};
            status = ut_kvp_getStringField(pInstance, "remote/events/0/event/type", result_type, UT_KVP_MAX_ELEMENT_SIZE);
            if (status == UT_KVP_STATUS_SUCCESS)
            {
                UT_LOG_DEBUG("result_kvp for type = [%s]", result_type);
            }
            type = result_type;

            status = ut_kvp_getStringField(pInstance, "remote/events/0/event/code", result_code, UT_KVP_MAX_ELEMENT_SIZE);
            if (status == UT_KVP_STATUS_SUCCESS)
            {
                UT_LOG_DEBUG("result_kvp for code = [%s]", result_code);
            }
            code = result_code;

            value = ut_kvp_getUInt32Field(pInstance, "remote/events/0/event/value");
            UT_LOG_DEBUG("result for value = [%d]", value);
        }

        // Print the parsed arguments
        UT_LOG_DEBUG("Profile file: %s\n", profile_file);
        UT_LOG_DEBUG("type: %s\n", type);
        UT_LOG_DEBUG("code: %s\n", code);
        UT_LOG_DEBUG("value: %d\n", value);

        /* Key press, report the event, send key release, and report again */
        emit(fd, get_type(pInstance, type), get_code(pInstance, code), value);
   // }

    
    emit(fd, get_type(pInstance, type), get_code(pInstance, code), 0);
    /*
     * Give userspace some time to read the events before we destroy the
     * device with UI_DEV_DESTROY.
     */
    sleep(1);

    ioctl(fd, UI_DEV_DESTROY);
    close(fd);
    ut_kvp_profile_close();

    return 0;
}