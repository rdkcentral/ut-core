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
 * @file plugin.h
 * @brief Defines the interface for the plugin.
 *
 * This header file declares the `PluginInterface` structure, which contains
 * function pointers representing the plugin's operations: initialization,
 * performing an action, and cleanup.
 */

#ifndef PLUGIN_H
#define PLUGIN_H

/**
 * @brief Structure representing the plugin interface.
 *
 * This structure contains pointers to functions that define the behavior
 * of a plugin. These functions are expected to be implemented by any plugin
 * conforming to this interface.
 */
typedef struct
{
    /**
     * @brief Initializes the plugin.
     *
     * This function pointer should point to a function that sets up the plugin
     * before it is used.
     */
    void (*initialize)(void);

    /**
     * @brief Performs the primary action of the plugin.
     *
     * This function pointer should point to a function that executes the main
     * functionality of the plugin.
     */
    void (*perform_action)(void);

    /**
     * @brief Cleans up the plugin.
     *
     * This function pointer should point to a function that releases any
     * resources or performs finalization tasks for the plugin.
     */
    void (*cleanup)(void);
} PluginInterface;

#endif // PLUGIN_H