#ifndef EMBALAND_INSTANCE_H
#define EMBALAND_INSTANCE_H 1
/**
 * @file
 * Embaland instance interface
 */

#include "platform.h"

#include <vulkan/vulkan_core.h>

struct emb_viewport;
struct emb_instance {
	VkInstance vulkan;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize embaland instance.
 * @param vulkan handle of Vulkan instance to use
 * @param embaland points a emb_instance to initialize
 * @retval VK_SUCCESS embaland instance succefully initialized
 * @sa emb_cleanup()
 */
EMB_API VkResult EMB_CALL emb_init(VkInstance vulkan,
				   struct emb_instance *embaland);

/**
 * Cleanup embaland resources.
 * @param embaland is the handle of the instance to cleanup
 * @sa emb_init()
 */
EMB_API void EMB_CALL emb_cleanup(const struct emb_instance *embaland);

/**
 * Render viewport.
 * @param embaland is handle of embaland instance
 * @param viewport is handle of the viewport to render
 * @param timeout specifies how long the function can wait in nanoseconds
 * @retval VK_SUCCESS viewport is successfully rendered
 */
EMB_API VkResult EMB_CALL
emb_render_viewport(const struct emb_instance *embaland,
		    const struct emb_viewport *viewport, uint64_t timeout);

#ifdef __cplusplus
}
#endif
#endif
