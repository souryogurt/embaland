#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "viewport.h"

EMB_API VkResult EMB_CALL emb_viewport_init(VkSurfaceKHR surface,
					    struct emb_viewport *viewport)
{
	viewport->surface = surface;
	return VK_SUCCESS;
}

EMB_API void EMB_CALL emb_viewport_cleanup(const struct emb_instance *embaland,
					   const struct emb_viewport *viewport)
{
	(void)embaland;
	(void)viewport;
}
