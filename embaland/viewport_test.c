#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>

#include "instance.h"
#include "viewport.h"

#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

Ensure(emb_viewport_init_successes)
{
	VkSurfaceKHR surface = ((VkSurfaceKHR)1);
	struct emb_viewport viewport = { NULL };

	VkResult ret = emb_viewport_init(surface, &viewport);

	assert_that(ret, is_equal_to(VK_SUCCESS));
}

Ensure(emb_viewport_cleanup_releases_resources)
{
	struct emb_instance embaland = { NULL };
	struct emb_viewport viewport = { NULL };

	emb_viewport_cleanup(&embaland, &viewport);
}

int main(int argc, char **argv)
{
	(void)(argc);
	(void)(argv);
	TestSuite *suite = create_named_test_suite("viewport");
	add_test(suite, emb_viewport_init_successes);
	add_test(suite, emb_viewport_cleanup_releases_resources);
	TestReporter *reporter = create_text_reporter();
	int exit_code = run_test_suite(suite, reporter);
	destroy_reporter(reporter);
	destroy_test_suite(suite);
	return exit_code;
}
