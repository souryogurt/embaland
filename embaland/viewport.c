/**
 * @file
 * Viewport implementation
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>

#include "embaland.h"
#include "compiler.h"
#include "viewport.h"
#include "instance.h"

static void emb_viewport_destroy(struct emb_object *obj)
{
	struct emb_viewport *view = container_of(obj, struct emb_viewport, obj);
	free(view);
}

static const struct emb_type viewport_type = {
	.release = emb_viewport_destroy,
};

EMB_API enum emb_result EMB_CALL emb_viewport_create(emb_instance embaland,
						     VkSurfaceKHR surface,
						     emb_viewport *viewport)
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
	struct emb_viewport *new_viewport = malloc(sizeof(*new_viewport));
	if (new_viewport == NULL) {
		result = EMB_ERROR_OUT_OF_HOST_MEMORY;
		goto err;
	}
	emb_object_init(&new_viewport->obj, &viewport_type);
	emb_object_add(&new_viewport->obj, &embaland->obj);
	new_viewport->emb = embaland;
	new_viewport->surface = surface;

	*viewport = new_viewport;
err:
	return result;
}

EMB_API void EMB_CALL emb_viewport_release(emb_viewport viewport)
{
	if (viewport != NULL) {
		emb_object_put(&viewport->obj);
	}
}

EMB_API enum emb_result EMB_CALL emb_viewport_render(emb_viewport viewport,
						     uint64_t timeout)
{
	(void)viewport;
	(void)timeout;
	return EMB_SUCCESS;
}
