/**
 * @file
 * Test suite for viewport
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>

#include "embaland.h"
#include "viewport.h"
#include "instance.h"

#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

Ensure(emb_viewport_init_returns_error_on_invalid_instance)
{
	VkSurfaceKHR surface = ((VkSurfaceKHR)1);
	struct emb_viewport viewport = { NULL };

	enum emb_result result = emb_viewport_init(&viewport, NULL, surface);
	assert_that(result, is_equal_to(EMB_ERROR_INITIALIZATION_FAILED));
}

Ensure(emb_viewport_init_returns_error_on_invalid_surface)
{
	struct emb_instance emb = { 0 };
	struct emb_viewport viewport = { NULL };

	enum emb_result result = emb_viewport_init(&viewport, &emb, NULL);
	assert_that(result, is_equal_to(EMB_ERROR_INVALID_EXTERNAL_HANDLE));
}

Ensure(emb_viewport_init_returns_error_on_invalid_pointer)
{
	struct emb_instance emb = { 0 };
	VkSurfaceKHR surface = ((VkSurfaceKHR)1);

	enum emb_result result = emb_viewport_init(NULL, &emb, surface);
	assert_that(result, is_equal_to(EMB_ERROR_INITIALIZATION_FAILED));
}

Ensure(emb_viewport_init_allocates_new_viewport)
{
	struct emb_instance emb = { 0 };
	VkSurfaceKHR surface = ((VkSurfaceKHR)1);
	struct emb_viewport viewport = { NULL };

	enum emb_result result = emb_viewport_init(&viewport, &emb, surface);
	assert_that(result, is_equal_to(EMB_SUCCESS));
	assert_that(viewport.surface, is_equal_to(surface));
	assert_that(viewport.emb, is_equal_to(&emb));
}

Ensure(emb_viewport_release_frees_resources)
{
	struct emb_instance emb = { 0 };
	VkSurfaceKHR surface = ((VkSurfaceKHR)1);
	struct emb_viewport viewport = { NULL };
	emb_viewport_init(&viewport, &emb, surface);

	emb_viewport_release(&viewport);
}

Ensure(emb_viewport_render_always_returns_success)
{
	emb_viewport viewport = NULL;
	assert_that(emb_viewport_render(viewport, 0), is_equal_to(EMB_SUCCESS));
}

int main(int argc, char **argv)
{
	(void)(argc);
	(void)(argv);
	TestSuite *suite = create_named_test_suite("viewport");
	add_test(suite, emb_viewport_init_returns_error_on_invalid_instance);
	add_test(suite, emb_viewport_init_returns_error_on_invalid_surface);
	add_test(suite, emb_viewport_init_returns_error_on_invalid_pointer);
	add_test(suite, emb_viewport_init_allocates_new_viewport);
	add_test(suite, emb_viewport_release_frees_resources);
	add_test(suite, emb_viewport_render_always_returns_success);
	TestReporter *reporter = create_text_reporter();
	int exit_code = run_test_suite(suite, reporter);
	destroy_reporter(reporter);
	destroy_test_suite(suite);
	return exit_code;
}
