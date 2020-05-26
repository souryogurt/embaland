#ifndef EMBALAND_VULKAN_H
#define EMBALAND_VULKAN_H 1
/**
 * @file
 * Embaland Vulkan Application Programming Interface
 */

#include "platform.h"
#include "embaland.h"
#include <vulkan/vulkan_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Get vulkan instance created by embaland.
 * @param[in] embaland initialized embaland instance
 * @param[out] vulkan pointer to a variable in which Vulkan will be stored
 * @retval EMB_SUCCESS Vulkan Instance succefully returned
 * @retval EMB_ERROR_INITIALIZATION_FAILED embaland instance not initialized
 * @sa emb_create()
 */
EMB_API emb_result EMB_CALL emb_get_vulkan(const emb_instance embaland,
					   VkInstance *vulkan);

EMB_API void EMB_CALL emb_destroy_surface(VkInstance vulkan,
					  VkSurfaceKHR surface,
					  const VkAllocationCallbacks *cb);

/**
 * Create embaland viewport from Vulkan surface.
 * @param[in] embaland initialized embaland instance
 * @param[in] surface Vulkan surface to create viewport from
 * @param[out] viewport pointer to a variable in which viewport will be stored
 * @retval EMB_SUCCESS viewport succefully created
 * @retval EMB_ERROR_OUT_OF_HOST_MEMORY memory allocation failed
 * @sa emb_destroy_viewport()
 */
EMB_API emb_result EMB_CALL emb_create_vulkan_viewport(emb_instance embaland,
						       VkSurfaceKHR surface,
						       emb_viewport *viewport);
#ifdef __cplusplus
}
#endif
#endif
