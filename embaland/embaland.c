/**
 * @file
 * embaland implementation
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "embaland.h"

EMB_API enum emb_result EMB_CALL emb_create(VkInstance vulkan,
					    emb_instance *embaland)
{
	(void)vulkan;
	*embaland = NULL;
	return EMB_SUCCESS;
}

EMB_API void EMB_CALL emb_destroy(emb_instance embaland)
{
	(void)embaland;
}
