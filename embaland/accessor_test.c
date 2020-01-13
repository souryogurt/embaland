/**
 * @file
 * Test suite for embaland accessor
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>

#include "embaland.h"
#include "buffer_view.h"
#include "accessor.h"

#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

Ensure(emb_accessor_init_returns_error_on_invalid_view)
{
	const float min_pos[] = { 0.0F, 0.0F, 0.0F };
	const float max_pos[] = { 1.0F, 1.0F, 0.0F };
	struct emb_accessor_info info = {
		.byte_offset = 0,
		.comp_type = EMB_ATTRIB_COMPONENT_FLOAT,
		.is_normalized = 0,
		.attrib_count = 3,
		.attrib_type = EMB_ATTRIB_VEC3,
		.min_values = min_pos,
		.max_values = max_pos,
	};
	struct emb_accessor acsr;
	enum emb_result result = emb_accessor_init(&acsr, NULL, &info);
	assert_that(result, is_equal_to(EMB_ERROR_INITIALIZATION_FAILED));
}

Ensure(emb_accessor_init_returns_error_on_invalid_info)
{
	struct emb_accessor acsr;
	struct emb_buffer_view bview;
	enum emb_result result = emb_accessor_init(&acsr, &bview, NULL);
	assert_that(result, is_equal_to(EMB_ERROR_INITIALIZATION_FAILED));
}

Ensure(emb_accessor_init_allocates_resources)
{
	const float min_pos[] = { 0.0F, 0.0F, 0.0F };
	const float max_pos[] = { 1.0F, 1.0F, 0.0F };
	struct emb_accessor_info info = {
		.byte_offset = 0,
		.comp_type = EMB_ATTRIB_COMPONENT_FLOAT,
		.is_normalized = 0,
		.attrib_count = 3,
		.attrib_type = EMB_ATTRIB_VEC3,
		.min_values = min_pos,
		.max_values = max_pos,
	};
	struct emb_buffer_view bview;
	struct emb_accessor acsr;
	enum emb_result result = emb_accessor_init(&acsr, &bview, &info);
	assert_that(result, is_equal_to(EMB_SUCCESS));
	assert_that(acsr.bview, is_equal_to(&bview));
	assert_that(acsr.byte_offset, is_equal_to(info.byte_offset));
	assert_that(acsr.comp_type, is_equal_to(info.comp_type));
	assert_that(acsr.is_normalized, is_equal_to(info.is_normalized));
	assert_that(acsr.attrib_count, is_equal_to(info.attrib_count));
	assert_that(acsr.attrib_type, is_equal_to(info.attrib_type));
}

Ensure(emb_accessor_destroy_does_nothing)
{
	struct emb_accessor acsr;
	emb_accessor_destroy(&acsr);
}

int main(int argc, char **argv)
{
	(void)(argc);
	(void)(argv);
	TestSuite *suite = create_named_test_suite("accessor");
	add_test(suite, emb_accessor_init_returns_error_on_invalid_view);
	add_test(suite, emb_accessor_init_returns_error_on_invalid_info);
	add_test(suite, emb_accessor_init_allocates_resources);
	add_test(suite, emb_accessor_destroy_does_nothing);
	TestReporter *reporter = create_text_reporter();
	int exit_code = run_test_suite(suite, reporter);
	destroy_reporter(reporter);
	destroy_test_suite(suite);
	return exit_code;
}
