/*
 * inprogram.c - libfyaml inprogram YAML example
 *
 * Copyright (c) 2019 Pantelis Antoniou <pantelis.antoniou@konsulko.com>
 *
 * SPDX-License-Identifier: MIT
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <libfyaml.h>

int main(int argc, char *argv[])
{
	static const char *yaml = 
		"invoice: 34843\n"
		"date   : !!str 2001-01-23\n"
		"bill-to: &id001\n"
		"    given  : Chris\n"
		"    family : Dumars\n"
		"    address:\n"
		"        lines: |\n"
		"            458 Walkman Dr.\n"
		"            Suite #292\n";
	struct fy_document *fyd = NULL;
	int rc, count, ret = EXIT_FAILURE;
	unsigned int invoice_nr;
	char given[256 + 1];
		if (argc == 1)
		fyd = fy_document_build_from_string(NULL, yaml, FY_NT);
	else
		fyd = fy_document_build_from_file(NULL, argv[1]);
	if (!fyd) {
		fprintf(stderr, "failed to build document");
		goto fail;
	}
		/* get the invoice number and the given name */
	count = fy_document_scanf(fyd,
			"/invoice %u "
			"/bill-to/given %256s",
			&invoice_nr, given);
	if (count != 2) {
		fprintf(stderr, "Failed to retreive the two items\n");
		goto fail;
	}

	/* print them as comments in the emitted YAML */
	printf("%s\n", fy_library_version());
	printf("# invoice number was %u\n", invoice_nr);
	printf("# given name is %s\n", given);
	
		rc =
		/* set increased invoice number (modify existing node) */
		fy_document_insert_at(fyd, "/invoice", FY_NT,
			fy_node_buildf(fyd, "%u", invoice_nr + 1)) ||
		/* add spouse (create new mapping pair) */
		fy_document_insert_at(fyd, "/bill-to", FY_NT,
			fy_node_buildf(fyd, "spouse: %s", "Doris")) ||
		/* add a second address */
		fy_document_insert_at(fyd, "/bill-to", FY_NT,
			fy_node_buildf(fyd, "delivery-address:\n"
				            "  lines: |\n"
					    "    1226 Windward Ave.\n"));
	if (rc) {
		fprintf(stderr, "failed to insert to document\n");
		goto fail;
	}
	
		/* emit the document to stdout (but sorted) */
	rc = fy_emit_document_to_fp(fyd, FYECF_DEFAULT | FYECF_SORT_KEYS, stdout);
	if (rc) {
		fprintf(stderr, "failed to emit document to stdout");
		goto fail;
	}
	
		ret = EXIT_SUCCESS;
fail:
	fy_document_destroy(fyd);	/* NULL is OK */

	return ret;
}
