/**
 * @file
 * emtriangle application
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>

#include "emtriangle.h"

#include <embaland/embaland.h>

static emb_instance emb;

int application_main(int argc, char **argv)
{
	int retval = EXIT_SUCCESS;
	if (emb_create(&emb) != EMB_SUCCESS) {
		retval = EXIT_FAILURE;
		goto exit;
	}
	emb_destroy(emb);
exit:
	return retval;
}
