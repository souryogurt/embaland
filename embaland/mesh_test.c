/**
 * @file
 * Test suite for embaland mesh
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>

#include "embaland.h"
#include "buffer_view.h"
#include "accessor.h"
#include "mesh.h"
#include "instance.h"

#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>
#include <vulkan/vulkan.h>

static VkInstance create_vulkan_instance(void)
{
	static const VkApplicationInfo app_info = {
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = "embaland-mesh-test",
		.applicationVersion = VK_MAKE_VERSION(0, 1, 0),
		.pEngineName = "embaland-mesh-test",
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

Ensure(emb_mesh_init_returns_error_on_invalid_instance)
{
	struct emb_accessor acsr = { 0 };
	const struct emb_attribute tri_attribute = {
		.type = EMB_ATTRIB_POSITION,
		.data = &acsr,
	};
	const struct emb_primitive tri_primitive = {
		.attributes = &tri_attribute,
		.attributes_count = 1,
		.indeces = NULL,
		.material = NULL,
		.mode = EMB_MODE_TRIANGLES,
		.targets = NULL,
		.targets_count = 0,
	};
	const struct emb_mesh_info info = {
		.primitives = &tri_primitive,
		.primitives_count = 1,
		.weights = NULL,
		.weights_count = 0,
	};
	struct emb_mesh mesh = { 0 };
	enum emb_result result = emb_mesh_init(&mesh, NULL, &info);
	assert_that(result, is_equal_to(EMB_ERROR_INITIALIZATION_FAILED));
}

Ensure(emb_mesh_init_returns_error_on_invalid_info)
{
	struct emb_mesh mesh = { 0 };
	struct emb_instance emb = { 0 };
	enum emb_result result = emb_mesh_init(&mesh, &emb, NULL);
	assert_that(result, is_equal_to(EMB_ERROR_INITIALIZATION_FAILED));
}

Ensure(emb_mesh_init_returns_error_on_no_primitives)
{
	const struct emb_mesh_info info = {
		.primitives = NULL,
		.primitives_count = 0,
		.weights = NULL,
		.weights_count = 0,
	};
	struct emb_mesh mesh = { 0 };
	struct emb_instance emb = { 0 };
	enum emb_result result = emb_mesh_init(&mesh, &emb, &info);
	assert_that(result, is_equal_to(EMB_ERROR_INITIALIZATION_FAILED));
}

Ensure(emb_mesh_init_returns_error_on_invalid_handle)
{
	struct emb_accessor acsr = { 0 };
	const struct emb_attribute tri_attribute = {
		.type = EMB_ATTRIB_POSITION,
		.data = &acsr,
	};
	const struct emb_primitive tri_primitive = {
		.attributes = &tri_attribute,
		.attributes_count = 1,
		.indeces = NULL,
		.material = NULL,
		.mode = EMB_MODE_TRIANGLES,
		.targets = NULL,
		.targets_count = 0,
	};
	const struct emb_mesh_info info = {
		.primitives = &tri_primitive,
		.primitives_count = 1,
		.weights = NULL,
		.weights_count = 0,
	};
	struct emb_instance emb = { 0 };
	enum emb_result result = emb_mesh_init(NULL, &emb, &info);
	assert_that(result, is_equal_to(EMB_ERROR_INITIALIZATION_FAILED));
}

Ensure(emb_mesh_init_allocates_resources)
{
	struct emb_accessor acsr = { 0 };
	const struct emb_attribute tri_attribute = {
		.type = EMB_ATTRIB_POSITION,
		.data = &acsr,
	};
	const struct emb_primitive tri_primitive = {
		.attributes = &tri_attribute,
		.attributes_count = 1,
		.indeces = NULL,
		.material = NULL,
		.mode = EMB_MODE_TRIANGLES,
		.targets = NULL,
		.targets_count = 0,
	};
	const struct emb_mesh_info info = {
		.primitives = &tri_primitive,
		.primitives_count = 1,
		.weights = NULL,
		.weights_count = 0,
	};
	VkInstance vulkan = create_vulkan_instance();
	struct emb_instance emb = { 0 };
	struct emb_mesh mesh = { 0 };

	assert_that(emb_init(&emb, vulkan), is_equal_to(EMB_SUCCESS));
	assert_that(emb_mesh_init(&mesh, &emb, &info), is_equal_to(EMB_SUCCESS));
	assert_that(mesh.emb, is_equal_to(&emb));
	assert_that(emb.meshes.next, is_equal_to(&mesh.scene_node));
	assert_that(emb.meshes.prev, is_equal_to(&mesh.scene_node));
	emb_mesh_destroy(&mesh);
	assert_that(emb.meshes.next, is_equal_to(&emb.meshes));
	assert_that(emb.meshes.prev, is_equal_to(&emb.meshes));
	emb_destroy(&emb);
	vkDestroyInstance(vulkan, NULL);
}

Ensure(emb_mesh_destroy_accepts_zero_initialized_mesh)
{
	struct emb_mesh mesh = { 0 };
	emb_mesh_destroy(&mesh);
}

int main(int argc, char **argv)
{
	(void)(argc);
	(void)(argv);
	TestSuite *suite = create_named_test_suite("mesh");
	add_test(suite, emb_mesh_init_returns_error_on_invalid_instance);
	add_test(suite, emb_mesh_init_returns_error_on_invalid_info);
	add_test(suite, emb_mesh_init_returns_error_on_no_primitives);
	add_test(suite, emb_mesh_init_returns_error_on_invalid_handle);
	add_test(suite, emb_mesh_init_allocates_resources);
	add_test(suite, emb_mesh_destroy_accepts_zero_initialized_mesh);
	TestReporter *reporter = create_text_reporter();
	int exit_code = run_test_suite(suite, reporter);
	destroy_reporter(reporter);
	destroy_test_suite(suite);
	return exit_code;
}
