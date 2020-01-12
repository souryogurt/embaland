#ifndef EMBALAND_MESH_H
#define EMBALAND_MESH_H 1

#include "platform.h"

#include <vulkan/vulkan_core.h>

struct emb_mesh {
	struct emb_instance *emb;
	const struct emb_primitive *primitives;
	size_t primitives_count;
	const float *weights;
	size_t weights_count;
};

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif
#endif
