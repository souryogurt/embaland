#ifndef EMBALAND_VIEWPORT_H
#define EMBALAND_VIEWPORT_H 1
/**
 * @file
 * Embaland viewport interface
 */

#include "platform.h"

#include <vulkan/vulkan_core.h>

struct emb_instance;
struct emb_viewport {
	VkSurfaceKHR surface;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize viewport.
 * @param surface is vulkan surface to init viewport on
 * @param viewport points to emb_viewport to initialize
 * @retval VK_SUCCESS viewport succefully initialized
 * @sa emb_viewport_cleanup()
 */
EMB_API VkResult EMB_CALL emb_viewport_init(VkSurfaceKHR surface,
					    struct emb_viewport *viewport);

/**
 * Cleanup viewport.
 * @param embaland is handle of embaland instance
 * @param viewport is the handle of the viewport to cleanup
 * @sa emb_viewport_init()
 */
EMB_API void EMB_CALL emb_viewport_cleanup(const struct emb_instance *embaland,
					   const struct emb_viewport *viewport);

#ifdef __cplusplus
}
#endif
#endif
