#ifndef EMBALAND_VIEWPORT_H
#define EMBALAND_VIEWPORT_H 1

#include "platform.h"

#include <vulkan/vulkan_core.h>

struct emb_instance;
struct emb_viewport {
	VkSurfaceKHR surface;
	struct emb_instance *emb;
	VkSwapchainKHR swapchain;
	VkFormat swapchain_format;
	VkExtent2D swapchain_extent;
	VkImage *images;
	VkImageView *image_views;
	VkFramebuffer *framebuffers;
	VkCommandBuffer *command_buffers;
	uint32_t image_count;
	VkRenderPass render_pass;
	VkPipelineLayout pipeline_layout;
	VkPipeline pipeline;
	VkCommandPool command_pool;
	VkSemaphore image_available;
	VkSemaphore render_finished;
	VkFence in_flight;
};

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif
#endif
