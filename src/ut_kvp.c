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
#include <assert.h>

#include <libfyaml.h>

ut_kvp_instance_t gKVP_Instance = NULL;

typedef struct
{
	uint32_t magic;
	FILE *inputFilePtr;
	struct fy_document *fy_handle;
} ut_kvp_instance_internal_t;

static ut_kvp_instance_internal_t *validateInstance(ut_kvp_instance_t *pInstance);

ut_kvp_instance_t *ut_kvp_createInstance(void)
{
	ut_kvp_instance_internal_t *pInstance = malloc(sizeof(ut_kvp_instance_internal_t));
	memset(pInstance, 0, sizeof(ut_kvp_instance_internal_t));

	pInstance->magic = 0xdeadbeef;
}

ut_kvp_instance_t ut_kvp_destoryInstance(ut_kvp_instance_t *pInstance)
{
	// ut_kvp_instance_t *pInstance = get_ut_kvp_instance;
	ut_kvp_instance_internal_t *pInternal = validateInstance(pInstance);

	assert(pInstance != NULL);
	assert(pInternal->fy_handle != NULL);
	assert(pInternal->inputFilePtr != NULL);

	if (pInternal->fy_handle != NULL)
	{
		fy_document_destroy(pInternal->fy_handle);
	}
	if (pInternal->inputFilePtr != NULL)
	{
		fclose(pInternal->inputFilePtr);
	}

	memset(pInternal, sizeof(ut_kvp_instance_internal_t), 0);

	free(pInstance);
}

ut_kvp_status_t ut_kvp_read(ut_kvp_instance_t *pInstance, char *fileName)
{
	ut_kvp_instance_internal_t *pInternal = validateInstance(pInstance);

	assert(pInternal != NULL);
	if (pInternal == NULL)
	{
		return UT_KVP_VALID_PARAM;
	}

	if (pInternal->inputFilePtr != NULL)
	{
		fclose(pInternal->inputFilePtr);
		pInternal->inputFilePtr = NULL;
	}

	pInternal->inputFilePtr = fopen(fileName, "r");
	if (NULL == pInternal->inputFilePtr)
	{
		UT_LOG_ERROR("Unable to open file\n");
		return UT_KVP_FILE_OPEN_ERROR;
	}
	pInternal->fy_handle = fy_document_build_from_fp(NULL, pInternal->inputFilePtr);
	if (NULL == pInternal->fy_handle)
	{
		UT_LOG_ERROR("Unable to build file\n");
		return UT_KVP_FILE_READ_ERROR;
	}

	return UT_KVP_STATUS_OK;
}

const char *ut_kvp_getField(ut_kvp_instance_t *pInstance, const char *string)
{
	const char *result;
	// ut_kvp_instance_t *pInstance = get_ut_kvp_instance();
	ut_kvp_instance_internal_t *pInternal = validateInstance(pInstance);

	assert(pInternal != NULL);
	if (pInternal == NULL)
	{
		return NULL;
	}
	fy_document_scanf(pInternal->fy_handle, string, result);

	return result;
}

static ut_kvp_instance_internal_t *validateInstance(ut_kvp_instance_t *pInstance)
{
	ut_kvp_instance_internal_t *pInternal = (ut_kvp_instance_internal_t *)pInstance;

	assert(pInternal != NULL);
	assert(pInternal->magic == 0xdeadbeef);

	if (pInternal->magic != 0xdeadbeef)
	{
		return NULL;
	}

	return pInternal;
}