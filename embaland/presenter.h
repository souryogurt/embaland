#ifndef EMBALAND_PRESENTER_H
#define EMBALAND_PRESENTER_H 1

#include "platform.h"
#include "embaland.h"

#include <vulkan/vulkan_core.h>
#include "object.h"

struct emb_presenter {
	struct emb_set set;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Create new presenter.
 * @param emb the instance of embaland to create presenter on
 * @param presenter the pointer to returned presenter
 * @retval EMB_SUCCESS on success
 * @retval EMB_ERROR_OUT_OF_HOST_MEMORY if there are no memory on host
 */
EMB_LOCAL enum emb_result
emb_presenter_create(struct emb_instance *emb,
		     struct emb_presenter **presenter);

#ifdef __cplusplus
}
#endif
#endif
