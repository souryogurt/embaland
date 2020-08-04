#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <embaland/instance.h>
#include <embaland/viewport.h>
#include <GLFW/glfw3.h>

static const uint16_t WIN_WIDTH = 960;
static const uint16_t WIN_HEIGHT = 540;

static VkDebugUtilsMessengerEXT vk_debug_messenger;
VKAPI_ATTR VkBool32 VKAPI_CALL
vk_debug_print(VkDebugUtilsMessageSeverityFlagBitsEXT sev,
	       VkDebugUtilsMessageTypeFlagsEXT messageType,
	       const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
	       void *pUserData)
{
	(void)pUserData;
	(void)messageType;
	const char *prefix = "unknown: ";
	int is_verbose = sev & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
	int is_info = sev & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;
	int is_warning = sev & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
	int is_error = sev & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	if (is_verbose) {
		prefix = "verbose: ";
	} else if (is_info) {
		prefix = "info: ";
	} else if (is_warning) {
		prefix = "warning: ";
	} else if (is_error) {
		prefix = "error: ";
	}
	fprintf(stderr, "%s: %s\n", prefix, pCallbackData->pMessage);
	return VK_FALSE;
}

static const VkDebugUtilsMessengerCreateInfoEXT msgr_info = {
	.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
	.pNext = NULL,
	.flags = 0,
	.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
			   VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
			   VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			   VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
	.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		       VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		       VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
	.pfnUserCallback = &vk_debug_print,
	.pUserData = NULL
};

#define APP_VERSION VK_MAKE_VERSION(VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH)
static const VkApplicationInfo app_info = {
	.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
	.pApplicationName = PACKAGE,
	.applicationVersion = APP_VERSION,
	.pEngineName = PACKAGE,
	.engineVersion = APP_VERSION,
	.apiVersion = VK_API_VERSION_1_1
};

static VkResult create_vulkan_instance(const VkApplicationInfo *app_info,
				       VkInstance *vulkan)
{
	VkResult ret = VK_SUCCESS;
	const char *ext[100] = { VK_EXT_DEBUG_UTILS_EXTENSION_NAME };
	uint32_t ext_count = 0;
	const char **wsi_ext = glfwGetRequiredInstanceExtensions(&ext_count);
	memcpy(&ext[1], wsi_ext, sizeof(wsi_ext[0]) * ext_count);
	VkInstanceCreateInfo vulkan_info = {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pApplicationInfo = app_info,
		.enabledLayerCount = 0,
		.ppEnabledLayerNames = NULL,
		.enabledExtensionCount = 1 + ext_count,
		.ppEnabledExtensionNames = ext,
	};
	if ((ret = vkCreateInstance(&vulkan_info, NULL, vulkan)) !=
	    VK_SUCCESS) {
		return ret;
	}
	PFN_vkCreateDebugUtilsMessengerEXT create_debug_msgr;
	create_debug_msgr =
		(PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
			*vulkan, "vkCreateDebugUtilsMessengerEXT");
	return create_debug_msgr(*vulkan, &msgr_info, NULL,
				 &vk_debug_messenger);
}

static void destroy_vulkan_instance(VkInstance vulkan)
{
	PFN_vkDestroyDebugUtilsMessengerEXT destroy_debug_msgr;
	destroy_debug_msgr =
		(PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
			vulkan, "vkDestroyDebugUtilsMessengerEXT");
	destroy_debug_msgr(vulkan, vk_debug_messenger, NULL);
	vkDestroyInstance(vulkan, NULL);
}

int main(int argc, char **argv)
{
	int ret = EXIT_SUCCESS;
	if (!glfwInit()) {
		fprintf(stderr, "%s: %s\n", argv[0], "can not init GLFW");
		return EXIT_FAILURE;
	}
	VkInstance vulkan = VK_NULL_HANDLE;
	if (create_vulkan_instance(&app_info, &vulkan) != VK_SUCCESS) {
		fprintf(stderr, "%s: %s\n", argv[0], "can not create Vulkan");
		ret = EXIT_FAILURE;
		goto err_terminate_glfw;
	}
	struct emb_instance emb = { NULL };
	if (emb_init(vulkan, &emb) != VK_SUCCESS) {
		fprintf(stderr, "%s: %s\n", argv[0], "can not init Embaland");
		ret = EXIT_FAILURE;
		goto err_destroy_vulkan;
	}

	GLFWwindow *win = NULL;
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	win = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Triangle", NULL, NULL);
	if (win == NULL) {
		fprintf(stderr, "%s: %s\n", argv[0], "can not create window");
		ret = EXIT_FAILURE;
		goto err_destroy_emb;
	}

	VkSurfaceKHR srf = VK_NULL_HANDLE;
	if (glfwCreateWindowSurface(vulkan, win, NULL, &srf) != VK_SUCCESS) {
		fprintf(stderr, "%s: %s\n", argv[0], "can not create surface");
		ret = EXIT_FAILURE;
		goto err_destroy_window;
	}

	struct emb_viewport viewport = { NULL };
	if (emb_viewport_init(srf, &viewport) != VK_SUCCESS) {
		fprintf(stderr, "%s: %s\n", argv[0], "can not create viewport");
		ret = EXIT_FAILURE;
		goto err_destroy_surface;
	}

	while (!glfwWindowShouldClose(win)) {
		glfwPollEvents();
		emb_render_viewport(&emb, &viewport, UINT64_MAX);
	}

	emb_viewport_cleanup(&emb, &viewport);
err_destroy_surface:
	vkDestroySurfaceKHR(vulkan, srf, NULL);
err_destroy_window:
	glfwDestroyWindow(win);
err_destroy_emb:
	emb_cleanup(&emb);
err_destroy_vulkan:
	destroy_vulkan_instance(vulkan);
err_terminate_glfw:
	glfwTerminate();
	return ret;
}
