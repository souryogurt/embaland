#ifndef EMBALAND_EMBALAND_H
#define EMBALAND_EMBALAND_H 1

#include "platform.h"

#include <vulkan/vulkan_core.h>

/**
 * The embaland API result.
 */
enum emb_result { EMB_SUCCESS = 0, EMB_ERROR_INITIALIZATION_FAILED = -1 };

/**
 * Opaque embaland instance handle.
 */
typedef struct emb_instance *emb_instance;

/**
 * Opaque viewport handle.
 */
typedef struct emb_viewport *emb_viewport;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Create embaland instance.
 * @param vulkan is the handle of the vulkan instance to use
 * @param[out] embaland points a emb_instance handle in which the resulting instance is returned
 * @returns EMB_SUCCESS on success, or emb_result error otherwise
 */
EMB_API enum emb_result EMB_CALL emb_create(VkInstance vulkan,
					    emb_instance *embaland);

/**
 * Destroy embaland instance.
 * @param embaland is the handle of the instance to destroy
 */
EMB_API void EMB_CALL emb_destroy(emb_instance embaland);

/**
 * Create embaland viewport.
 * @param embaland is handle of the embaland instance
 * @param surface is vulkan surface to create viewport on
 * @param[out] viewport points a emb_viewport handle in which the resulting
 *                      viewport is returned
 * @returns EMB_SUCCESS on success, or emb_result error otherwise
 */
EMB_API enum emb_result EMB_CALL emb_viewport_create(emb_instance embaland,
						     VkSurfaceKHR surface,
						     emb_viewport *viewport);

/**
 * Destroy embaland viewport.
 * @param viewport is the handle of the viewport to destroy
 */
EMB_API void EMB_CALL emb_viewport_destroy(emb_viewport viewport);

#ifdef __cplusplus
}
#endif
#endif
