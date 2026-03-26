#ifndef EMBALAND_INSTANCE_H
#define EMBALAND_INSTANCE_H 1

#include "platform.h"
#include "list.h"

#include <vulkan/vulkan_core.h>

struct emb_instance {
	VkInstance vulkan;
	VkPhysicalDevice physical_device;
	VkDevice device;
	VkQueue graphics_queue;
	uint32_t queue_family_index;
	VkPhysicalDeviceMemoryProperties memory_properties;
	struct list_head meshes;
};

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif
#endif
