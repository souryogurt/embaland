#ifndef EMBALAND_EMBALAND_H
#define EMBALAND_EMBALAND_H 1

#include "platform.h"

#include <vulkan/vulkan_core.h>

/**
 * The embaland API result.
 */
enum emb_result {
	EMB_SUCCESS = 0,
	EMB_ERROR_OUT_OF_HOST_MEMORY = -1,
	EMB_ERROR_INITIALIZATION_FAILED = -2,
	EMB_ERROR_INVALID_EXTERNAL_HANDLE = -3,
};

/**
 * Opaque embaland instance handle.
 */
typedef struct emb_instance *emb_instance;

/**
 * Opaque viewport handle.
 */
typedef struct emb_viewport *emb_viewport;

/**
 * Opaque buffer handle.
 */
typedef struct emb_buffer *emb_buffer;

/**
 * Opaque buffer view handle.
 */
typedef struct emb_buffer_view *emb_buffer_view;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Create embaland instance.
 * @param vulkan is the handle of the vulkan instance to use
 * @param[out] embaland points a emb_instance handle in which the resulting instance is returned
 * @retval EMB_SUCCESS embaland instance succefully created
 * @retval EMB_ERROR_INITIALIZATION_FAILED target pointer is null
 * @retval EMB_ERROR_INVALID_EXTERNAL_HANDLE vulkan handle is invalid
 * @retval EMB_ERROR_OUT_OF_HOST_MEMORY can't allocate memory
 * @sa emb_destroy()
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
 * @retval EMB_SUCCESS viewport succefully created
 * @retval EMB_ERROR_INITIALIZATION_FAILED instance handle or target are null
 * @retval EMB_ERROR_INVALID_EXTERNAL_HANDLE surface handle is invalid
 * @retval EMB_ERROR_OUT_OF_HOST_MEMORY can't allocate memory
 * @sa emb_viewport_release()
 */
EMB_API enum emb_result EMB_CALL emb_viewport_create(emb_instance embaland,
						     VkSurfaceKHR surface,
						     emb_viewport *viewport);

/**
 * Release reference to embaland viewport.
 * @param viewport is the handle of the viewport to release
 */
EMB_API void EMB_CALL emb_viewport_release(emb_viewport viewport);

/** Render viewport.
 * @param viewport is handle of the viewport to render
 * @param timeout specifies how long the function can wait in nanoseconds
 * @retval EMB_SUCCESS viewport is successfully rendered
 */
EMB_API enum emb_result EMB_CALL emb_viewport_render(emb_viewport viewport,
						     uint64_t timeout);

/**
 * Create embaland buffer.
 * @param embaland is handle of the embaland instance
 * @param bytes is a pointer to initial buffer's data
 * @param nbytes is the size in bytes of @data
 * @param[out] buffer points a emb_buffer handle in which the resulting
 *                    buffer is returned
 * @retval EMB_SUCCESS buffer succefully created
 * @retval EMB_ERROR_INITIALIZATION_FAILED instance handle or target are null
 * @retval EMB_ERROR_OUT_OF_HOST_MEMORY can't allocate memory
 * @sa emb_buffer_release()
 */
EMB_API enum emb_result EMB_CALL emb_buffer_create(emb_instance embaland,
						   const void *bytes,
						   size_t nbytes,
						   emb_buffer *buffer);
/**
 * Release embaland buffer.
 * @param buffer is the handle of buffer to release
 */
EMB_API void EMB_CALL emb_buffer_release(emb_buffer buffer);

/**
 * Create embaland buffer view.
 * @param buffer is handle of the embaland buffer to create view for
 * @param offset is offset in bytes in buffer
 * @param nbytes is the size in bytes of the view
 * @param[out] bview points a emb_buffer_view handle in which the resulting
 *                   buffer view is returned
 * @retval EMB_SUCCESS buffer view succefully created
 * @retval EMB_ERROR_INITIALIZATION_FAILED instance handle or target are null
 * @retval EMB_ERROR_OUT_OF_HOST_MEMORY can't allocate memory
 * @sa emb_buffer_view_release()
 */
EMB_API enum emb_result EMB_CALL emb_buffer_view_create(emb_buffer buffer,
							size_t offset,
							size_t nbytes,
							emb_buffer_view *bview);
/**
 * Release embaland buffer view.
 * @param bview is the handle of buffer view to release
 */
EMB_API void EMB_CALL emb_buffer_view_release(emb_buffer_view bview);

#ifdef __cplusplus
}
#endif
#endif
