/**
 * @file
 * Embaland instance implementation
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "compiler.h"
#include "embaland.h"
#include "instance.h"
#include "mesh.h"

#include <vulkan/vulkan.h>

static const char *const device_extensions[] = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

static bool device_supports_extensions(VkPhysicalDevice device)
{
	uint32_t ext_count = 0;
	if (vkEnumerateDeviceExtensionProperties(device, NULL, &ext_count,
						 NULL) != VK_SUCCESS) {
		return false;
	}

	VkExtensionProperties *extensions =
		calloc(ext_count, sizeof(VkExtensionProperties));
	if (extensions == NULL) {
		return false;
	}

	bool has_swapchain = false;
	if (vkEnumerateDeviceExtensionProperties(device, NULL, &ext_count,
						 extensions) != VK_SUCCESS) {
		goto out;
	}

	for (uint32_t i = 0; i < ext_count; ++i) {
		if (strcmp(extensions[i].extensionName,
			   VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0) {
			has_swapchain = true;
			break;
		}
	}

out:
	free(extensions);
	return has_swapchain;
}

static VkResult select_physical_device(emb_instance embaland)
{
	uint32_t device_count = 0;
	VkResult result = vkEnumeratePhysicalDevices(embaland->vulkan,
						     &device_count, NULL);
	if (result != VK_SUCCESS || device_count == 0) {
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	VkPhysicalDevice *devices =
		calloc(device_count, sizeof(VkPhysicalDevice));
	if (devices == NULL) {
		return VK_ERROR_OUT_OF_HOST_MEMORY;
	}

	result = vkEnumeratePhysicalDevices(embaland->vulkan, &device_count,
					    devices);
	if (result != VK_SUCCESS) {
		goto err_free_devices;
	}

	for (uint32_t device_index = 0; device_index < device_count;
	     ++device_index) {
		uint32_t queue_family_count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(
			devices[device_index], &queue_family_count, NULL);
		if (queue_family_count == 0 ||
		    !device_supports_extensions(devices[device_index])) {
			continue;
		}

		VkQueueFamilyProperties *queue_families =
			calloc(queue_family_count,
			       sizeof(VkQueueFamilyProperties));
		if (queue_families == NULL) {
			result = VK_ERROR_OUT_OF_HOST_MEMORY;
			goto err_free_devices;
		}

		vkGetPhysicalDeviceQueueFamilyProperties(
			devices[device_index], &queue_family_count,
			queue_families);
		for (uint32_t family_index = 0; family_index < queue_family_count;
		     ++family_index) {
			if ((queue_families[family_index].queueFlags &
			     VK_QUEUE_GRAPHICS_BIT) == 0) {
				continue;
			}

			embaland->physical_device = devices[device_index];
			embaland->queue_family_index = family_index;
			vkGetPhysicalDeviceMemoryProperties(
				devices[device_index],
				&embaland->memory_properties);
			free(queue_families);
			free(devices);
			return VK_SUCCESS;
		}

		free(queue_families);
	}

	result = VK_ERROR_INITIALIZATION_FAILED;
err_free_devices:
	free(devices);
	return result;
}

static VkResult create_device(emb_instance embaland)
{
	const float queue_priority = 1.0f;
	VkDeviceQueueCreateInfo queue_info = {
		.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
		.queueFamilyIndex = embaland->queue_family_index,
		.queueCount = 1,
		.pQueuePriorities = &queue_priority,
	};
	VkDeviceCreateInfo device_info = {
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.queueCreateInfoCount = 1,
		.pQueueCreateInfos = &queue_info,
		.enabledExtensionCount = 1,
		.ppEnabledExtensionNames = device_extensions,
	};
	VkResult result = vkCreateDevice(embaland->physical_device, &device_info,
					 NULL, &embaland->device);
	if (result != VK_SUCCESS) {
		return result;
	}

	vkGetDeviceQueue(embaland->device, embaland->queue_family_index, 0,
			 &embaland->graphics_queue);
	return VK_SUCCESS;
}

EMB_API enum emb_result EMB_CALL emb_init(emb_instance embaland,
					  VkInstance vulkan)
{
	if (embaland == NULL) {
		return EMB_ERROR_INITIALIZATION_FAILED;
	}
	if (vulkan == VK_NULL_HANDLE) {
		return EMB_ERROR_INVALID_EXTERNAL_HANDLE;
	}

	memset(embaland, 0, sizeof(*embaland));
	embaland->vulkan = vulkan;
	embaland->queue_family_index = UINT32_MAX;
	list_init(&embaland->meshes);

	if (select_physical_device(embaland) != VK_SUCCESS) {
		emb_destroy(embaland);
		return EMB_ERROR_INITIALIZATION_FAILED;
	}
	if (create_device(embaland) != VK_SUCCESS) {
		emb_destroy(embaland);
		return EMB_ERROR_INITIALIZATION_FAILED;
	}

	return EMB_SUCCESS;
}

EMB_API void EMB_CALL emb_destroy(emb_instance embaland)
{
	if (embaland == NULL) {
		return;
	}

	if (embaland->device != VK_NULL_HANDLE) {
		vkDeviceWaitIdle(embaland->device);
	}
	if (embaland->meshes.next != NULL && embaland->meshes.prev != NULL) {
		while (embaland->meshes.next != &embaland->meshes) {
			struct emb_mesh *mesh = container_of(
				embaland->meshes.next, struct emb_mesh,
				scene_node);
			list_del_init(&mesh->scene_node);
			mesh->emb = NULL;
			mesh->primitives = NULL;
			mesh->primitives_count = 0;
			mesh->weights = NULL;
			mesh->weights_count = 0;
		}
		list_init(&embaland->meshes);
	}
	if (embaland->device != VK_NULL_HANDLE) {
		vkDestroyDevice(embaland->device, NULL);
	}

	memset(&embaland->memory_properties, 0,
	       sizeof(embaland->memory_properties));
	embaland->graphics_queue = VK_NULL_HANDLE;
	embaland->device = VK_NULL_HANDLE;
	embaland->physical_device = VK_NULL_HANDLE;
	embaland->queue_family_index = UINT32_MAX;
	embaland->vulkan = VK_NULL_HANDLE;
}
