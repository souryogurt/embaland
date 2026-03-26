/**
 * @file
 * Buffer implementation
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>
#include <string.h>

#include "compiler.h"
#include "embaland.h"
#include "buffer.h"
#include "instance.h"

#include <vulkan/vulkan.h>

static uint32_t find_memory_type(emb_instance embaland, uint32_t type_filter,
				 VkMemoryPropertyFlags properties)
{
	for (uint32_t i = 0;
	     i < embaland->memory_properties.memoryTypeCount; ++i) {
		if ((type_filter & (1U << i)) == 0) {
			continue;
		}
		if ((embaland->memory_properties.memoryTypes[i].propertyFlags &
		     properties) == properties) {
			return i;
		}
	}

	return UINT32_MAX;
}

EMB_API enum emb_result EMB_CALL emb_buffer_init(emb_buffer buffer,
						 emb_instance embaland,
						 const void *bytes,
						 size_t nbytes)
{
	if (embaland == NULL) {
		return EMB_ERROR_INITIALIZATION_FAILED;
	}
	if (buffer == NULL) {
		return EMB_ERROR_INITIALIZATION_FAILED;
	}
	if (embaland->device == VK_NULL_HANDLE) {
		return EMB_ERROR_INVALID_EXTERNAL_HANDLE;
	}

	memset(buffer, 0, sizeof(*buffer));
	buffer->capacity = nbytes;
	buffer->emb = embaland;

	VkDeviceSize buffer_size = nbytes == 0 ? 1 : (VkDeviceSize)nbytes;
	VkBufferCreateInfo buffer_info = {
		.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		.size = buffer_size,
		.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT |
			 VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
	};
	VkResult result = vkCreateBuffer(embaland->device, &buffer_info, NULL,
					 &buffer->vulkan);
	if (result != VK_SUCCESS) {
		goto err;
	}

	VkMemoryRequirements memory_requirements;
	vkGetBufferMemoryRequirements(embaland->device, buffer->vulkan,
				      &memory_requirements);
	uint32_t memory_type = find_memory_type(
		embaland, memory_requirements.memoryTypeBits,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
			VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	if (memory_type == UINT32_MAX) {
		goto err;
	}

	VkMemoryAllocateInfo alloc_info = {
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.allocationSize = memory_requirements.size,
		.memoryTypeIndex = memory_type,
	};
	result = vkAllocateMemory(embaland->device, &alloc_info, NULL,
				  &buffer->memory);
	if (result != VK_SUCCESS) {
		goto err;
	}
	result = vkBindBufferMemory(embaland->device, buffer->vulkan,
				    buffer->memory, 0);
	if (result != VK_SUCCESS) {
		goto err;
	}

	if (nbytes > 0) {
		void *mapped = NULL;
		result = vkMapMemory(embaland->device, buffer->memory, 0,
				     buffer_size, 0, &mapped);
		if (result != VK_SUCCESS) {
			goto err;
		}
		if (bytes != NULL) {
			memcpy(mapped, bytes, nbytes);
		} else {
			memset(mapped, 0, nbytes);
		}
		vkUnmapMemory(embaland->device, buffer->memory);
	}

	return EMB_SUCCESS;

err:
	emb_buffer_destroy(buffer);
	return EMB_ERROR_INITIALIZATION_FAILED;
}

EMB_API void EMB_CALL emb_buffer_destroy(emb_buffer buffer)
{
	if (buffer == NULL) {
		return;
	}
	if (buffer->emb != NULL && buffer->emb->device != VK_NULL_HANDLE) {
		if (buffer->vulkan != VK_NULL_HANDLE) {
			vkDestroyBuffer(buffer->emb->device, buffer->vulkan,
					NULL);
		}
		if (buffer->memory != VK_NULL_HANDLE) {
			vkFreeMemory(buffer->emb->device, buffer->memory, NULL);
		}
	}
	buffer->memory = VK_NULL_HANDLE;
	buffer->vulkan = VK_NULL_HANDLE;
	buffer->capacity = 0;
	buffer->emb = NULL;
}
