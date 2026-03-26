#ifndef EMBALAND_BUFFER_H
#define EMBALAND_BUFFER_H 1

#include "platform.h"

#include <vulkan/vulkan_core.h>

struct emb_instance;
struct emb_buffer {
	size_t capacity;
	struct emb_instance *emb;
	VkBuffer vulkan;
	VkDeviceMemory memory;
};

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif
#endif
