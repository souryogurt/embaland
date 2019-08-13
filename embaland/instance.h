#ifndef EMBALAND_INSTANCE_H
#define EMBALAND_INSTANCE_H 1

#include "platform.h"

#include <vulkan/vulkan_core.h>
#include "object.h"

struct emb_instance {
	VkInstance vulkan;
	struct emb_object obj;
};

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif
#endif
