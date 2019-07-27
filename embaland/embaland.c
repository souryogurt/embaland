/**
 * @file
 * embaland implementation
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "embaland.h"

EMB_API enum emb_result EMB_CALL emb_create(emb_instance *instance)
{
	*instance = NULL;
	return EMB_SUCCESS;
}

EMB_API void EMB_CALL emb_destroy(emb_instance instance)
{
	(void)instance;
}
