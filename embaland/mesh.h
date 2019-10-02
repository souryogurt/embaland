#ifndef EMBALAND_MESH_H
#define EMBALAND_MESH_H 1

#include "platform.h"

#include <vulkan/vulkan_core.h>
#include "object.h"

struct emb_mesh {
	struct emb_object obj;
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
