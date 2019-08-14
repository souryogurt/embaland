/**
 * @file
 * Embaland instance implementation
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>

#include "compiler.h"
#include "embaland.h"
#include "instance.h"
#include "presenter.h"

static void emb_instance_destroy(struct emb_object *obj)
{
	struct emb_instance *emb = container_of(obj, struct emb_instance, obj);
	free(emb);
}

static const struct emb_type instance_type = {
	.release = emb_instance_destroy,
};

EMB_API enum emb_result EMB_CALL emb_create(VkInstance vulkan,
					    emb_instance *embaland)
{
	enum emb_result result = EMB_SUCCESS;
	if (vulkan == VK_NULL_HANDLE) {
		result = EMB_ERROR_INVALID_EXTERNAL_HANDLE;
		goto err;
	}
	if (embaland == NULL) {
		result = EMB_ERROR_INITIALIZATION_FAILED;
		goto err;
	}
	struct emb_instance *instance = malloc(sizeof(*instance));
	if (instance == NULL) {
		result = EMB_ERROR_OUT_OF_HOST_MEMORY;
		goto err;
	}
	emb_object_init(&instance->obj, &instance_type);
	instance->vulkan = vulkan;
	instance->presenter = NULL;
	*embaland = instance;
err:
	return result;
}

EMB_API void EMB_CALL emb_destroy(emb_instance embaland)
{
	if (embaland != NULL) {
		emb_object_put(&embaland->obj);
	}
}

EMB_LOCAL enum emb_result emb_get_presenter(struct emb_instance *emb,
					    struct emb_presenter **presenter)
{
	enum emb_result retval = EMB_SUCCESS;
	/* TODO: lock presenter_lock */
	if (emb->presenter != NULL) {
		emb_object_get(&emb->presenter->set.obj);
		*presenter = emb->presenter;
		goto ret_unlock;
	}
	struct emb_presenter *new_presenter = NULL;
	retval = emb_presenter_create(emb, &new_presenter);
	if (retval != EMB_SUCCESS) {
		goto ret_unlock;
	}
	emb->presenter = new_presenter;
	*presenter = new_presenter;
ret_unlock:
	/* TODO: unlock presenter_lock */
	return retval;
}
