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

EMB_API enum emb_result EMB_CALL emb_init(emb_instance embaland,
					  VkInstance vulkan)
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
	embaland->vulkan = vulkan;
err:
	return result;
}

EMB_API void EMB_CALL emb_destroy(emb_instance embaland)
{
	(void)embaland;
}
