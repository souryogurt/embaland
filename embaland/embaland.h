#ifndef EMBALAND_EMBALAND_H
#define EMBALAND_EMBALAND_H 1

#include "platform.h"

/**
 * The embaland API result.
 */
enum emb_result { EMB_SUCCESS = 0, EMB_ERROR_INITIALIZATION_FAILED = -1 };

/**
 * Opaque embaland instance handle.
 */
typedef struct emb_instance_t *emb_instance;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Create embaland instance.
 * @param[out] instance points a emb_instance handle in which the resulting instance is returned
 * @returns EMB_SUCCESS on success, or emb_result error otherwise
 */
EMB_API enum emb_result EMB_CALL emb_create(emb_instance *instance);

/**
 * Destroy embaland instance.
 * @param instance is the handle of the instance to destroy
 */
EMB_API void EMB_CALL emb_destroy(emb_instance instance);

#ifdef __cplusplus
}
#endif
#endif
