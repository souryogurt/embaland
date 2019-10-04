#ifndef EMBALAND_MATERIAL_H
#define EMBALAND_MATERIAL_H 1

#include "platform.h"

#include <vulkan/vulkan_core.h>
#include "object.h"

struct emb_material {
	struct emb_object obj;
};

#ifdef __cplusplus
extern "C" {
#endif

EMB_LOCAL emb_material emb_material_get(emb_material mat);

#ifdef __cplusplus
}
#endif
#endif
