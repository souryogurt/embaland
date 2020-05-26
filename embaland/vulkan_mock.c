#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <vulkan/vulkan.h>

#include <cgreen/mocks.h>

VKAPI_ATTR void VKAPI_CALL
vkDestroySurfaceKHR(VkInstance instance, VkSurfaceKHR surface,
		    const VkAllocationCallbacks *pAllocator)
{
	mock(instance, surface, pAllocator);
}
