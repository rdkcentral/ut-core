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

/**
 * @file plugin.c
 * @brief Implementation of the plugin interface.
 *
 * This file defines the functions that form the plugin's interface,
 * including initialization, performing an action, and cleanup.
 */

#include "plugin.h"
#include <stdio.h>

/**
 * @brief Initializes the plugin.
 *
 * This function performs any necessary setup for the plugin.
 */
void plugin_initialize(void)
{
    printf("Plugin initialized.\n");
}

/**
 * @brief Executes the main action of the plugin.
 *
 * This function is called to perform the core functionality of the plugin.
 */
void plugin_perform_action(void)
{
    printf("Plugin action performed.\n");
}

/**
 * @brief Cleans up the plugin.
 *
 * This function performs any necessary cleanup before the plugin is unloaded.
 */
void plugin_cleanup(void)
{
    printf("Plugin cleaned up.\n");
}

/**
 * @brief Global instance of the plugin interface.
 *
 * This structure defines the plugin's interface, mapping its functions to the
 * corresponding implementation.
 */
PluginInterface plugin = {
    .initialize = plugin_initialize,
    .perform_action = plugin_perform_action,
    .cleanup = plugin_cleanup};