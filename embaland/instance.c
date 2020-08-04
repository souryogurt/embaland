#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "instance.h"

EMB_API VkResult EMB_CALL emb_init(VkInstance vulkan,
				   struct emb_instance *embaland)
{
	embaland->vulkan = vulkan;
	return VK_ERROR_INITIALIZATION_FAILED;
}

EMB_API void EMB_CALL emb_cleanup(struct emb_instance *embaland)
{
	(void)embaland;
}

EMB_API VkResult EMB_CALL emb_render_viewport(struct emb_instance *embaland,
					      struct emb_viewport *viewport,
					      uint64_t timeout)
{
	(void)embaland;
	(void)viewport;
	(void)timeout;
	return VK_SUCCESS;
}
