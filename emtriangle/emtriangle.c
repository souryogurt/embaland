/**
 * @file
 * emtriangle application
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>
#include <stdbool.h>

#include "emtriangle.h"
#include "scene.h"

#include <vulkan/vulkan.h> /* must be included before glfw3.h */
#include <GLFW/glfw3.h>
#include <embaland/embaland.h>
#include <embaland/viewport.h>
#include <embaland/instance.h>

#define APP_VERSION VK_MAKE_VERSION(VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH)

static const VkApplicationInfo emtriangle_app_info = {
	.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
	.pApplicationName = PACKAGE,
	.applicationVersion = APP_VERSION,
	.pEngineName = PACKAGE,
	.engineVersion = APP_VERSION,
	.apiVersion = VK_API_VERSION_1_1
};

static VkResult create_vulkan_instance(VkInstance *vulkan)
{
	uint32_t ext_count = 0;
	const char **ext = glfwGetRequiredInstanceExtensions(&ext_count);
	VkInstanceCreateInfo vulkan_info = {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pApplicationInfo = &emtriangle_app_info,
		.enabledLayerCount = 0,
		.ppEnabledLayerNames = NULL,
		.enabledExtensionCount = ext_count,
		.ppEnabledExtensionNames = ext,
	};
	return vkCreateInstance(&vulkan_info, NULL, vulkan);
}

int application_main(int argc, char **argv)
{
	int retval = EXIT_SUCCESS;
	if (!glfwInit()) {
		return EXIT_FAILURE;
	}

	VkInstance vulkan = VK_NULL_HANDLE;
	if (create_vulkan_instance(&vulkan) != VK_SUCCESS) {
		retval = EXIT_FAILURE;
		goto err_terminate_glfw;
	}

	struct emb_instance emb = { NULL };
	if (emb_init(&emb, vulkan) != EMB_SUCCESS) {
		retval = EXIT_FAILURE;
		goto err_destroy_vulkan;
	}
	struct scene triangle_scene;
	if (scene_init(&emb, &triangle_scene) != EMB_SUCCESS) {
		retval = EXIT_FAILURE;
		goto err_destroy_emb;
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow *win = glfwCreateWindow(960, 540, "Triangle", NULL, NULL);
	if (win == NULL) {
		retval = EXIT_FAILURE;
		goto err_destroy_scene;
	}
	VkSurfaceKHR srf;
	if (glfwCreateWindowSurface(vulkan, win, NULL, &srf) != VK_SUCCESS) {
		retval = EXIT_FAILURE;
		goto err_destroy_window;
	}

	struct emb_viewport viewport = { NULL };
	if (emb_viewport_init(&viewport, &emb, srf) != EMB_SUCCESS) {
		retval = EXIT_FAILURE;
		goto err_destoy_surface;
	}

	while (!glfwWindowShouldClose(win)) {
		glfwPollEvents();
		emb_viewport_render(&viewport, UINT64_MAX);
	}

	emb_viewport_destroy(&viewport);
err_destoy_surface:
	vkDestroySurfaceKHR(vulkan, srf, NULL);
err_destroy_window:
	glfwDestroyWindow(win);
err_destroy_scene:
	scene_destroy(&triangle_scene);
err_destroy_emb:
	emb_destroy(&emb);
err_destroy_vulkan:
	vkDestroyInstance(vulkan, NULL);
err_terminate_glfw:
	glfwTerminate();
	return retval;
}
