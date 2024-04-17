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

/* Application Includes */
#include <ut_log.h>
#include <ut_kvp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include <libfyaml.h>

ut_kvp_handle_t *pHandle = NULL;
ut_kvp_value_t kvpValue={0};

ut_kvp_handle_t *ut_kvp_createInstance_from_file_ptr(FILE *inputfileptr)
{
	inputfileptr = fopen("xione.de.yaml", "r");
	if (NULL == inputfileptr)
	{
		UT_LOG_STEP("Unable to open YAML file\n");
		return NULL;
	}
	pHandle = fy_document_build_from_fp(NULL, inputfileptr);
	if (!pHandle)
	{
		UT_LOG_STEP("Unable to build YAML file\n");
		ut_kvp_pHandledestoryInstance_from_file_ptr(pHandle, inputfileptr);
		return NULL;
	}
	fclose(inputfileptr);

	return pHandle;
}

ut_kvp_handle_t *ut_kvp_createInstance_from_file(char *file)
{
	ut_kvp_handle_t *pHandle = NULL;
	pHandle = fy_document_build_from_file(NULL, file);
	return pHandle;
}

ut_kvp_instance_t ut_kvp_pHandledestoryInstance_from_file_ptr( ut_kvp_handle_t *pHandle)
{
	fy_document_destroy(pHandle);
}

ut_kvp_instance_t ut_kvp_pHandleestoryInstance_from_file( ut_kvp_handle_t *pHandle)
{
	fy_document_destroy(pHandle);
}

ut_kvp_value_t ut_kvp_document_get_int(ut_kvp_handle_t *pHandle, const char *string)
{
	fy_document_scanf(pHandle, string, &kvpValue.u.intValue);
	kvpValue.type = UT_KVP_INT;
	UT_LOG_STEP("%s = %d", string, kvpValue.u.intValue);
	return kvpValue;
}

ut_kvp_value_t ut_kvp_document_get_string(ut_kvp_handle_t *pHandle, const char *string)
{
	fy_document_scanf(pHandle, string, kvpValue.u.szValue);
	kvpValue.type = UT_KVP_zSTRING;
	UT_LOG_STEP("%s = %s", string, kvpValue.u.szValue);
	return kvpValue;
}

ut_kvp_value_t ut_kvp_document_get_bool(ut_kvp_handle_t *pHandle, const char *string)
{
	char bool_string[10];
	fy_document_scanf(pHandle, string, bool_string);
	if (!strcmp("true", bool_string))
	{
		kvpValue.u.bValue = true;
	}
	else
	{
		kvpValue.u.bValue = false;
	}
	UT_LOG_STEP("%s = %d", string, kvpValue.u.bValue);
	return kvpValue;
}
