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

static void mock_destroy(struct emb_object *obj)
{
	mock(obj);
}

static const struct emb_type mock_object_type = {
	.release = mock_destroy,
};

Ensure(emb_viewport_create_returns_error_on_invalid_instance)
{
	VkSurfaceKHR surface = ((VkSurfaceKHR)1);
	emb_viewport viewport = NULL;

	enum emb_result result = emb_viewport_create(NULL, surface, &viewport);
	assert_that(result, is_equal_to(EMB_ERROR_INITIALIZATION_FAILED));
}

Ensure(emb_viewport_create_returns_error_on_invalid_surface)
{
	struct emb_instance emb = { 0 };
	emb_object_init(&emb.obj, &mock_object_type);
	emb_viewport viewport = NULL;

	enum emb_result result = emb_viewport_create(&emb, NULL, &viewport);
	assert_that(result, is_equal_to(EMB_ERROR_INVALID_EXTERNAL_HANDLE));
}

Ensure(emb_viewport_create_returns_error_on_invalid_pointer)
{
	struct emb_instance emb = { 0 };
	emb_object_init(&emb.obj, &mock_object_type);
	VkSurfaceKHR surface = ((VkSurfaceKHR)1);

	enum emb_result result = emb_viewport_create(&emb, surface, NULL);
	assert_that(result, is_equal_to(EMB_ERROR_INITIALIZATION_FAILED));
}

Ensure(emb_viewport_create_allocates_new_viewport)
{
	struct emb_instance emb = { 0 };
	emb_object_init(&emb.obj, &mock_object_type);
	VkSurfaceKHR surface = ((VkSurfaceKHR)1);
	emb_viewport viewport = NULL;

	enum emb_result result = emb_viewport_create(&emb, surface, &viewport);
	assert_that(result, is_equal_to(EMB_SUCCESS));
	assert_that(viewport, is_not_null);
	assert_that(viewport->obj.parent, is_equal_to(&emb.obj));
	assert_that(emb.obj.ref.count, is_equal_to(2));
	assert_that(viewport->surface, is_equal_to(surface));
	assert_that(viewport->emb, is_equal_to(&emb));

	free(viewport);
}

Ensure(emb_viewport_release_frees_memory)
{
	struct emb_instance emb = { 0 };
	emb_object_init(&emb.obj, &mock_object_type);
	VkSurfaceKHR surface = ((VkSurfaceKHR)1);
	emb_viewport viewport = NULL;
	emb_viewport_create(&emb, surface, &viewport);

	emb_viewport_release(viewport);
}

Ensure(emb_viewport_release_does_nothing_for_null_handle)
{
	emb_viewport_release(NULL);
}

Ensure(emb_viewport_release_releases_parent_too)
{
	struct emb_instance emb = { 0 };
	emb_object_init(&emb.obj, &mock_object_type);
	VkSurfaceKHR surface = ((VkSurfaceKHR)1);
	emb_viewport viewport = NULL;
	emb_viewport_create(&emb, surface, &viewport);
	emb_object_put(&emb.obj);

	expect(mock_destroy, when(obj, is_equal_to(&emb.obj)));
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
	add_test(suite, emb_viewport_create_returns_error_on_invalid_instance);
	add_test(suite, emb_viewport_create_returns_error_on_invalid_surface);
	add_test(suite, emb_viewport_create_returns_error_on_invalid_pointer);
	add_test(suite, emb_viewport_create_allocates_new_viewport);
	add_test(suite, emb_viewport_release_frees_memory);
	add_test(suite, emb_viewport_release_does_nothing_for_null_handle);
	add_test(suite, emb_viewport_release_releases_parent_too);
	add_test(suite, emb_viewport_render_always_returns_success);
	TestReporter *reporter = create_text_reporter();
	int exit_code = run_test_suite(suite, reporter);
	destroy_reporter(reporter);
	destroy_test_suite(suite);
	return exit_code;
}
