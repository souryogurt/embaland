#ifndef EMBALAND_BUFFER_VIEW_H
#define EMBALAND_BUFFER_VIEW_H 1

#include "platform.h"

#include <vulkan/vulkan_core.h>
#include "object.h"

struct emb_buffer_view {
	struct emb_object obj;
	struct emb_buffer *buffer;
	size_t offset;
	size_t nbytes;
};

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif
#endif
