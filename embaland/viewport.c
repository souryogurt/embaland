/**
 * @file
 * Viewport implementation
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>

#include "compiler.h"
#include "embaland.h"
#include "viewport.h"

EMB_API enum emb_result EMB_CALL emb_viewport_init(emb_viewport viewport,
						   emb_instance embaland,
						   VkSurfaceKHR surface)
{
	enum emb_result result = EMB_SUCCESS;
	if (embaland == NULL) {
		result = EMB_ERROR_INITIALIZATION_FAILED;
		goto err;
	}
	if (surface == VK_NULL_HANDLE) {
		result = EMB_ERROR_INVALID_EXTERNAL_HANDLE;
		goto err;
	}
	if (viewport == NULL) {
		result = EMB_ERROR_INITIALIZATION_FAILED;
		goto err;
	}

	viewport->emb = embaland;
	viewport->surface = surface;
err:
	return result;
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
