#ifndef EMBALAND_VIEWPORT_H
#define EMBALAND_VIEWPORT_H 1
/**
 * @file
 * Embaland viewport interface
 */

#include "embaland.h"

#include <vulkan/vulkan_core.h>

struct emb_viewport {
	VkSurfaceKHR surface;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize viewport.
 * @param embaland is handle of the embaland instance
 * @param surface is vulkan surface to init viewport on
 * @param viewport points to emb_viewport to initialize
 * @retval EMB_SUCCESS viewport succefully initialized
 * @retval EMB_ERROR_INITIALIZATION_FAILED initialization failed
 * @sa emb_viewport_cleanup()
 */
EMB_LOCAL emb_result emb_viewport_init(const emb_instance embaland,
				       VkSurfaceKHR surface,
				       emb_viewport viewport);

/**
 * Cleanup viewport.
 * @param embaland is handle of embaland instance
 * @param viewport is the handle of the viewport to cleanup
 */
EMB_LOCAL void emb_viewport_cleanup(const emb_instance embaland,
				    const emb_viewport viewport);

#ifdef __cplusplus
}
#endif
#endif
