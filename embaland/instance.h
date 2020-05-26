#ifndef EMBALAND_INSTANCE_H
#define EMBALAND_INSTANCE_H 1
/**
 * @file
 * Embaland instance interface
 */

#include "embaland.h"

#include <vulkan/vulkan_core.h>

struct emb_instance {
	VkInstance vulkan;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize embaland instance.
 * @param embaland handle of instance to initialize
 * @retval EMB_SUCCESS embaland instance succefully initialized
 * @retval EMB_ERROR_INITIALIZATION_FAILED target pointer is null
 * @sa emb_cleanup()
 */
EMB_LOCAL emb_result emb_init(emb_instance embaland);

/**
 * Cleanup embaland resources.
 * @param embaland is the handle of the instance to cleanup
 */
EMB_LOCAL void emb_cleanup(emb_instance embaland);
#ifdef __cplusplus
}
#endif
#endif
