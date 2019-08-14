#ifndef EMBALAND_INSTANCE_H
#define EMBALAND_INSTANCE_H 1

#include "platform.h"

#include <vulkan/vulkan_core.h>
#include "object.h"

struct emb_presenter;
struct emb_instance {
	/* TODO: mtx_t presenter_lock */
	struct emb_object obj;
	VkInstance vulkan;
	struct emb_presenter *presenter;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Get reference to presenter.
 * Gets new reference to presenter if it already exists. If not new presenter is
 * created.
 * @param emb the instance of embaland to get presenter from
 * @param presenter the pointer to returned presenter.
 * @retval EMB_SUCCESS on success
 * @retval EMB_ERROR_OUT_OF_HOST_MEMORY if there are no memory on host
 */
EMB_LOCAL enum emb_result emb_get_presenter(struct emb_instance *emb,
					    struct emb_presenter **presenter);

#ifdef __cplusplus
}
#endif
#endif
