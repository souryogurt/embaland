/**
 * @file
 * embaland implementation
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "embaland/embaland.h"

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

EMB_API enum emb_result EMB_CALL emb_viewport_create(emb_instance embaland,
						     VkSurfaceKHR surface,
						     emb_viewport *viewport)
{
	(void)embaland;
	(void)surface;
	*viewport = NULL;
	return EMB_SUCCESS;
}

EMB_API void EMB_CALL emb_viewport_destroy(emb_viewport viewport)
{
	(void)viewport;
}

EMB_API enum emb_result EMB_CALL emb_viewport_render(emb_viewport viewport,
						     uint64_t timeout)
{
	(void)viewport;
	(void)timeout;
	return EMB_SUCCESS;
}
