#ifndef EMBALAND_EMBALAND_H
#define EMBALAND_EMBALAND_H 1
/**
 * @file
 * Embaland Application Programming Interface
 */

#include "platform.h"

/**
 * The embaland API result.
 */
typedef enum emb_result {
	EMB_SUCCESS = 0,
	EMB_ERROR_INITIALIZATION_FAILED = -1,
	EMB_ERROR_INVALID_EXTERNAL_HANDLE = -2,
	EMB_ERROR_OUT_OF_HOST_MEMORY = -3,
} emb_result;

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
 * @param embaland points a emb_instance to initialize
 * @retval EMB_SUCCESS embaland instance succefully created
 * @retval EMB_ERROR_OUT_OF_HOST_MEMORY memory allocation failed
 * @retval EMB_ERROR_INITIALIZATION_FAILED initialization failed
 * @sa emb_destroy()
 */
EMB_API emb_result EMB_CALL emb_create(emb_instance *embaland);

/**
 * Destroy embaland instance.
 * @param embaland is the handle of the instance to destroy
 */
EMB_API void EMB_CALL emb_destroy(emb_instance embaland);

/**
 * Destroy embaland viewport.
 * @param embaland is handle of embaland instance
 * @param viewport is the handle of the viewport to destroy
 */
EMB_API void EMB_CALL emb_destroy_viewport(emb_instance embaland,
					   emb_viewport viewport);

/**
 * Render viewport.
 * @param embaland is handle of embaland instance
 * @param viewport is handle of the viewport to render
 * @param timeout specifies how long the function can wait in nanoseconds
 * @retval EMB_SUCCESS viewport is successfully rendered
 */
EMB_API emb_result EMB_CALL emb_render_viewport(emb_instance embaland,
						emb_viewport viewport,
						uint64_t timeout);

#ifdef __cplusplus
}
#endif
#endif
