/**
 * @file
 * Mock of libembaland library
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <embaland/embaland.h>

#include <cgreen/mocks.h>

EMB_API enum emb_result EMB_CALL emb_create(VkInstance vulkan,
					    emb_instance *instance)
{
	return (enum emb_result)mock(vulkan, instance);
}

EMB_API void EMB_CALL emb_destroy(emb_instance instance)
{
	(void)mock(instance);
}

EMB_API enum emb_result EMB_CALL emb_viewport_create(emb_instance embaland,
						     VkSurfaceKHR surface,
						     emb_viewport *viewport)
{
	return (enum emb_result)mock(embaland, surface, viewport);
}

EMB_API void EMB_CALL emb_viewport_release(emb_viewport viewport)
{
	mock(viewport);
}

EMB_API enum emb_result EMB_CALL emb_viewport_render(emb_viewport viewport,
						     uint64_t timeout)
{
	return (enum emb_result)mock(viewport, timeout);
}
