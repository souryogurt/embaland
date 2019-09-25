#ifndef EMBALAND_INSTANCE_H
#define EMBALAND_INSTANCE_H 1

#include "platform.h"

#include <vulkan/vulkan_core.h>
#include "object.h"

struct emb_instance {
	struct emb_object obj;
	VkInstance vulkan;
};

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif
#endif
