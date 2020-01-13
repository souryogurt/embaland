/**
 * @file
 * Test suite for embaland material
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>

#include "embaland.h"
#include "material.h"

#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

Ensure(emb_material_destroy_does_nothing)
{
	struct emb_material mat;
	emb_material_destroy(&mat);
}

int main(int argc, char **argv)
{
	(void)(argc);
	(void)(argv);
	TestSuite *suite = create_named_test_suite("material");
	add_test(suite, emb_material_destroy_does_nothing);
	TestReporter *reporter = create_text_reporter();
	int exit_code = run_test_suite(suite, reporter);
	destroy_reporter(reporter);
	destroy_test_suite(suite);
	return exit_code;
}
