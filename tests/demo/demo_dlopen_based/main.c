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
 * @file main.c
 * @brief Demonstrates the usage of weak symbols and dynamic loading with `dlopen` and `dlsym`.
 *
 * This program attempts to load strong implementations of functions from a shared library.
 * If a strong implementation is not found, the weak implementations in this file are used.
 */

/**
 * @file main.c
 * @brief Main program to demonstrate dynamic loading of a plugin.
 *
 * This program attempts to dynamically load a shared library plugin and
 * execute its functions if available. If the plugin is not found, it runs without the plugin.
 */

#include <stdio.h>
#include <dlfcn.h>
#include "plugin.h"

/**
 * @brief Entry point for the program.
 *
 * Attempts to dynamically load a plugin shared library and execute its
 * interface functions (`initialize`, `perform_action`, `cleanup`). If the
 * plugin is unavailable, the program proceeds without it.
 *
 * @returns 0 on successful execution.
 */
int main(void)
{
    void *handle; /**< Handle for the dynamically loaded shared library. */
    PluginInterface *plugin = NULL; /**< Pointer to the plugin interface. */

    // Attempt to load the plugin dynamically
    handle = dlopen("./libplugin.so", RTLD_LAZY);
    if (handle)
    {
        plugin = (PluginInterface *)dlsym(handle, "plugin");
        if (plugin)
        {
            plugin->initialize();
            plugin->perform_action();
            plugin->cleanup();
        }
        dlclose(handle);
    }
    else
    {
        printf("Plugin not found. Running without plugin.\n");
    }

    return 0;
}