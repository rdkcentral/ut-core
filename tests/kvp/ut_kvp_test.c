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

#include <ut_log.h>
#include <ut_kvp.h>

int main(int argc, char *argv[])
{
	struct fy_document *fyd = NULL;
	int rv, index1, index2;
	
	if (argc > 1)
	{
		fyd = fy_document_build_from_file(NULL, argv[1]);
	}
	else
	{
    		UT_LOG_STEP("Please pass a yaml file as argument\n");
		return -1;
	}
	if (!fyd) {
    		UT_LOG_STEP("Unable to build YAML file\n");
		fy_document_destroy(fyd);
		return -1;
	}
	fy_document_scanf(fyd,"/WifiRadioConfig/0/RadioIndex %d ",&index1);
	fy_document_scanf(fyd,"/WifiRadioConfig/1/RadioIndex %d ",&index2);
    	
	UT_LOG_STEP("/WifiRadioConfig/0/RadioIndex = %d", index1);
	UT_LOG_STEP("/WifiRadioConfig/1/RadioIndex = %d\n", index2);


	#if 0	
	rv = fy_emit_document_to_fp(fyd, FYECF_DEFAULT | FYECF_SORT_KEYS, stdout);
	if (rv) {
		fprintf(stderr, "failed to emit document to stdout");
		UT_LOG_STEP("failed to emit document");
		fy_document_destroy(fyd);
		return -1;
	}
	#endif
		
	fy_document_destroy(fyd);
	return 0;
}
