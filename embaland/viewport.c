#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <assert.h>
#include <stdlib.h>

#include "viewport.h"

EMB_API emb_result EMB_CALL emb_create_vulkan_viewport(emb_instance embaland,
						       VkSurfaceKHR surface,
						       emb_viewport *viewport)
{
	assert(embaland != NULL);
	emb_viewport vprt = malloc(sizeof(struct emb_viewport));
	if (vprt == NULL) {
		return EMB_ERROR_OUT_OF_HOST_MEMORY;
	}
	emb_result ret = emb_viewport_init(embaland, surface, vprt);
	if (ret == EMB_SUCCESS) {
		*viewport = vprt;
		return ret;
	}
	free(vprt);
	return ret;
}

EMB_LOCAL emb_result emb_viewport_init(const emb_instance embaland,
				       VkSurfaceKHR surface,
				       emb_viewport viewport)
{
	(void)embaland;
	viewport->surface = surface;
	return EMB_SUCCESS;
}

EMB_LOCAL void emb_viewport_cleanup(const emb_instance embaland,
				    const emb_viewport viewport)
{
	(void)embaland;
	(void)viewport;
}

EMB_API void EMB_CALL emb_destroy_viewport(emb_instance embaland,
					   emb_viewport viewport)
{
	if (viewport != NULL) {
		emb_viewport_cleanup(embaland, viewport);
		free(viewport);
	}
}

EMB_API void EMB_CALL emb_destroy_surface(VkInstance vulkan,
					  VkSurfaceKHR surface,
					  const VkAllocationCallbacks *cb)
{
	vkDestroySurfaceKHR(vulkan, surface, cb);
}

EMB_API emb_result EMB_CALL emb_render_viewport(emb_instance embaland,
						emb_viewport viewport,
						uint64_t timeout)
{
	(void)embaland;
	(void)viewport;
	(void)timeout;
	return EMB_SUCCESS;
}
