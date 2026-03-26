/**
 * @file
 * Test suite for embaland buffer
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>

#include "embaland.h"
#include "buffer.h"
#include "instance.h"

#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>
#include <vulkan/vulkan.h>

static VkInstance create_vulkan_instance(void)
{
	static const VkApplicationInfo app_info = {
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = "embaland-buffer-test",
		.applicationVersion = VK_MAKE_VERSION(0, 1, 0),
		.pEngineName = "embaland-buffer-test",
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

Ensure(emb_buffer_init_returns_error_on_invalid_instance)
{
	struct emb_buffer buffer = { 0 };
	enum emb_result result = emb_buffer_init(&buffer, NULL, NULL, 0);
	assert_that(result, is_equal_to(EMB_ERROR_INITIALIZATION_FAILED));
}

Ensure(emb_buffer_init_returns_error_on_invalid_handle)
{
	struct emb_instance emb = { 0 };
	enum emb_result result = emb_buffer_init(NULL, &emb, NULL, 0);
	assert_that(result, is_equal_to(EMB_ERROR_INITIALIZATION_FAILED));
}

Ensure(emb_buffer_init_allocates_resources_for_buffer)
{
	VkInstance vulkan = create_vulkan_instance();
	struct emb_instance emb = { 0 };
	struct emb_buffer buffer = { 0 };

	assert_that(emb_init(&emb, vulkan), is_equal_to(EMB_SUCCESS));
	assert_that(emb_buffer_init(&buffer, &emb, NULL, 0),
		    is_equal_to(EMB_SUCCESS));
	assert_that(buffer.emb, is_equal_to(&emb));
	assert_that(buffer.vulkan, is_not_equal_to(VK_NULL_HANDLE));
	assert_that(buffer.memory, is_not_equal_to(VK_NULL_HANDLE));
	emb_buffer_destroy(&buffer);
	emb_destroy(&emb);
	vkDestroyInstance(vulkan, NULL);
}

Ensure(emb_buffer_destroy_accepts_zero_initialized_buffer)
{
	struct emb_buffer buffer = { 0 };
	emb_buffer_destroy(&buffer);
}

int main(int argc, char **argv)
{
	(void)(argc);
	(void)(argv);
	TestSuite *suite = create_named_test_suite("buffer");
	add_test(suite, emb_buffer_init_returns_error_on_invalid_instance);
	add_test(suite, emb_buffer_init_returns_error_on_invalid_handle);
	add_test(suite, emb_buffer_init_allocates_resources_for_buffer);
	add_test(suite, emb_buffer_destroy_accepts_zero_initialized_buffer);
	TestReporter *reporter = create_text_reporter();
	int exit_code = run_test_suite(suite, reporter);
	destroy_reporter(reporter);
	destroy_test_suite(suite);
	return exit_code;
}
