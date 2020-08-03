#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>

#include <embaland/embaland.h>
#include <embaland/vulkan.h>
#include <GLFW/glfw3.h>

static const uint16_t WIN_WIDTH = 960;
static const uint16_t WIN_HEIGHT = 540;
static emb_instance emb;
static GLFWwindow *win;
static emb_viewport viewport;

int main(int argc, char **argv)
{
	int ret = EXIT_SUCCESS;
	if (!glfwInit()) {
		return EXIT_FAILURE;
	}
	if (emb_create(&emb) != EMB_SUCCESS) {
		ret = EXIT_FAILURE;
		goto err_terminate_glfw;
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	win = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Triangle", NULL, NULL);
	if (win == NULL) {
		ret = EXIT_FAILURE;
		goto err_destroy_emb;
	}

	VkInstance vulkan = VK_NULL_HANDLE;
	if (emb_get_vulkan(emb, &vulkan) != EMB_SUCCESS) {
		ret = EXIT_FAILURE;
		goto err_destroy_window;
	}
	VkSurfaceKHR srf = VK_NULL_HANDLE;
	if (glfwCreateWindowSurface(vulkan, win, NULL, &srf) != VK_SUCCESS) {
		ret = EXIT_FAILURE;
		goto err_destroy_window;
	}

	if (emb_create_vulkan_viewport(emb, srf, &viewport) != EMB_SUCCESS) {
		ret = EXIT_FAILURE;
		emb_destroy_surface(vulkan, srf, NULL);
		goto err_destroy_window;
	}

	while (!glfwWindowShouldClose(win)) {
		glfwPollEvents();
		emb_render_viewport(emb, viewport, UINT64_MAX);
	}

	emb_destroy_viewport(emb, viewport);
err_destroy_window:
	glfwDestroyWindow(win);
err_destroy_emb:
	emb_destroy(emb);
err_terminate_glfw:
	glfwTerminate();
	return ret;
}
