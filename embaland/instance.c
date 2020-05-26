#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <assert.h>
#include <stdlib.h>

#include "instance.h"

EMB_API emb_result EMB_CALL emb_get_vulkan(const emb_instance embaland,
					   VkInstance *vulkan)
{
	assert(embaland != NULL);
	*vulkan = embaland->vulkan;
	return EMB_SUCCESS;
}

EMB_API emb_result EMB_CALL emb_create(emb_instance *embaland)
{
	assert(embaland != NULL);
	emb_instance emb = malloc(sizeof(struct emb_instance));
	if (emb == NULL) {
		return EMB_ERROR_OUT_OF_HOST_MEMORY;
	}
	emb_result ret = emb_init(emb);
	if (ret == EMB_SUCCESS) {
		*embaland = emb;
		return EMB_SUCCESS;
	}
	free(emb);
	return ret;
}

EMB_LOCAL emb_result emb_init(emb_instance embaland)
{
	(void)embaland;
	emb_result ret = EMB_ERROR_INITIALIZATION_FAILED;
	return ret;
}

EMB_LOCAL void emb_cleanup(emb_instance embaland)
{
	(void)embaland;
}

EMB_API void EMB_CALL emb_destroy(emb_instance embaland)
{
	if (embaland != NULL) {
		emb_cleanup(embaland);
		free(embaland);
	}
}
