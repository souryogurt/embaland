#ifndef EMBALAND_VIEWPORT_H
#define EMBALAND_VIEWPORT_H 1

#include "platform.h"

#include <vulkan/vulkan_core.h>

struct emb_instance;
struct emb_viewport {
	VkSurfaceKHR surface;
	struct emb_instance *emb;
};

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif
#endif
