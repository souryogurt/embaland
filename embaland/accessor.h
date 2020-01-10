#ifndef EMBALAND_ACCESSOR_H
#define EMBALAND_ACCESSOR_H 1

#include "platform.h"

#include <vulkan/vulkan_core.h>
#include "object.h"

struct emb_buffer_view;
struct emb_accessor {
	struct emb_object obj;
	struct emb_buffer_view *bview;
	uint32_t byte_offset;
	uint32_t comp_type;
	uint8_t is_normalized;
	uint32_t attrib_count;
	uint32_t attrib_type;
};

#ifdef __cplusplus
extern "C" {
#endif

EMB_LOCAL emb_accessor emb_accessor_get(emb_accessor acsr);

#ifdef __cplusplus
}
#endif
#endif
