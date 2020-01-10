#ifndef EMBALAND_BUFFER_VIEW_H
#define EMBALAND_BUFFER_VIEW_H 1

#include "platform.h"

#include <vulkan/vulkan_core.h>

struct emb_instance;
struct emb_buffer_view {
	struct emb_instance *emb;
	struct emb_buffer *buffer;
	size_t offset;
	size_t nbytes;
	uint8_t stride;
	enum emb_buffer_target target;
};

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif
#endif
