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

#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

Ensure(emb_viewport_create_always_returns_success)
{
	emb_instance embaland = NULL;
	VkSurfaceKHR surface = VK_NULL_HANDLE;
	emb_viewport viewport = NULL;
	assert_that(emb_viewport_create(embaland, surface, &viewport),
		    is_equal_to(EMB_SUCCESS));
}

Ensure(emb_viewport_release_does_not_do_anything)
{
	emb_viewport viewport = NULL;
	emb_viewport_release(viewport);
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
	add_test(suite, emb_viewport_create_always_returns_success);
	add_test(suite, emb_viewport_release_does_not_do_anything);
	add_test(suite, emb_viewport_render_always_returns_success);
	TestReporter *reporter = create_text_reporter();
	int exit_code = run_test_suite(suite, reporter);
	destroy_reporter(reporter);
	destroy_test_suite(suite);
	return exit_code;
}