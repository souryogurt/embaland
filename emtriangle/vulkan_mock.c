/**
 * @file
 * Mock of libvulkan library
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <vulkan/vulkan.h>

#include <cgreen/mocks.h>

VKAPI_ATTR VkResult VKAPI_CALL
vkCreateInstance(const VkInstanceCreateInfo *pCreateInfo,
		 const VkAllocationCallbacks *pAllocator, VkInstance *pInstance)
{
	return (VkResult)mock(pCreateInfo, pAllocator, pInstance);
}

VKAPI_ATTR void VKAPI_CALL
vkDestroyInstance(VkInstance instance, const VkAllocationCallbacks *pAllocator)
{
	mock(instance, pAllocator);
}

VKAPI_ATTR void VKAPI_CALL
vkDestroySurfaceKHR(VkInstance instance, VkSurfaceKHR surface,
		    const VkAllocationCallbacks *pAllocator)
{
	mock(instance, surface, pAllocator);
}
