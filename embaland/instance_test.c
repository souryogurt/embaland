/**
 * @file
 * Test suite for embaland instance
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>

#include "embaland.h"
#include "instance.h"

#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>
#include <vulkan/vulkan.h>

static VkInstance create_vulkan_instance(void)
{
	static const VkApplicationInfo app_info = {
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = "embaland-instance-test",
		.applicationVersion = VK_MAKE_VERSION(0, 1, 0),
		.pEngineName = "embaland-instance-test",
		.engineVersion = VK_MAKE_VERSION(0, 1, 0),
		.apiVersion = VK_API_VERSION_1_0,
	};
	VkInstance instance = VK_NULL_HANDLE;
	VkInstanceCreateInfo create_info = {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pApplicationInfo = &app_info,
	};
	assert_that(vkCreateInstance(&create_info, NULL, &instance),
		    is_equal_to(VK_SUCCESS));
	return instance;
}

Ensure(emb_init_returns_error_on_invalid_vulkan_handle)
{
	struct emb_instance embaland = { 0 };
	enum emb_result result = emb_init(&embaland, VK_NULL_HANDLE);
	assert_that(result, is_equal_to(EMB_ERROR_INVALID_EXTERNAL_HANDLE));
}

Ensure(emb_init_returns_error_on_invalid_handle_pointer)
{
	VkInstance vulkan = ((VkInstance)1);

	enum emb_result result = emb_init(NULL, vulkan);
	assert_that(result, is_equal_to(EMB_ERROR_INITIALIZATION_FAILED));
}

Ensure(emb_init_allocates_new_embaland_instance)
{
	VkInstance vulkan = create_vulkan_instance();
	struct emb_instance embaland = { 0 };

	assert_that(emb_init(&embaland, vulkan), is_equal_to(EMB_SUCCESS));
	assert_that(embaland.vulkan, is_equal_to(vulkan));
	assert_that(embaland.device, is_not_equal_to(VK_NULL_HANDLE));
	assert_that(embaland.graphics_queue, is_not_equal_to(VK_NULL_HANDLE));
	emb_destroy(&embaland);
	vkDestroyInstance(vulkan, NULL);
}

Ensure(emb_destroy_releases_instance_resources)
{
	VkInstance vulkan = create_vulkan_instance();
	struct emb_instance embaland = { 0 };

	assert_that(emb_init(&embaland, vulkan), is_equal_to(EMB_SUCCESS));
	emb_destroy(&embaland);
	assert_that(embaland.device, is_equal_to(VK_NULL_HANDLE));
	assert_that(embaland.vulkan, is_equal_to(VK_NULL_HANDLE));
	vkDestroyInstance(vulkan, NULL);
}

int main(int argc, char **argv)
{
	(void)(argc);
	(void)(argv);
	TestSuite *suite = create_named_test_suite("instance");
	add_test(suite, emb_init_returns_error_on_invalid_vulkan_handle);
	add_test(suite, emb_init_returns_error_on_invalid_handle_pointer);
	add_test(suite, emb_init_allocates_new_embaland_instance);
	add_test(suite, emb_destroy_releases_instance_resources);
	TestReporter *reporter = create_text_reporter();
	int exit_code = run_test_suite(suite, reporter);
	destroy_reporter(reporter);
	destroy_test_suite(suite);
	return exit_code;
}
