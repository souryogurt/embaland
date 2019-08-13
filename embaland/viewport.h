#ifndef EMBALAND_VIEWPORT_H
#define EMBALAND_VIEWPORT_H 1

#include "platform.h"

#include <vulkan/vulkan_core.h>
#include "object.h"

struct emb_instance;
struct emb_viewport {
	VkSurfaceKHR surface;
	struct emb_instance *emb;
	struct emb_object obj;
};

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif
#endif
