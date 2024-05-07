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

/** @brief
 * UT Main application external functions
 * 
 */
/** @addtogroup UT
 * @{
 */

#ifndef __UT_MAIN_H
#define __UT_MAIN_H

#include <ut_kvp.h>

/**
 * @brief get the platform kvp instance
 * This instance is set by passing -p filename, into main()
 * 
 * @return ut_kvp_instance_t*  - instance handle.
 */
extern ut_kvp_instance_t *ut_getPlatformProfile(void);

/**
 * @brief Exit testing framework and shut down resources
 * 
 */
extern void UT_exit(void);

#endif  /*  __UT_MAIN_H  */
/** @} */
