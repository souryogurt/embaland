/**
 * @file
 * Viewport implementation
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "compiler.h"
#include "accessor.h"
#include "buffer.h"
#include "buffer_view.h"
#include "embaland.h"
#include "instance.h"
#include "mesh.h"
#include "viewport.h"

#include <vulkan/vulkan.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

static const uint32_t triangle_vert_spv[] = {
	0x07230203, 0x00010000, 0x0008000b, 0x0000001b, 0x00000000, 0x00020011,
	0x00000001, 0x0006000b, 0x00000001, 0x4c534c47, 0x6474732e, 0x3035342e,
	0x00000000, 0x0003000e, 0x00000000, 0x00000001, 0x0007000f, 0x00000000,
	0x00000004, 0x6e69616d, 0x00000000, 0x0000000d, 0x00000012, 0x00030003,
	0x00000002, 0x000001c2, 0x00040005, 0x00000004, 0x6e69616d, 0x00000000,
	0x00060005, 0x0000000b, 0x505f6c67, 0x65567265, 0x78657472, 0x00000000,
	0x00060006, 0x0000000b, 0x00000000, 0x505f6c67, 0x7469736f, 0x006e6f69,
	0x00070006, 0x0000000b, 0x00000001, 0x505f6c67, 0x746e696f, 0x657a6953,
	0x00000000, 0x00070006, 0x0000000b, 0x00000002, 0x435f6c67, 0x4470696c,
	0x61747369, 0x0065636e, 0x00070006, 0x0000000b, 0x00000003, 0x435f6c67,
	0x446c6c75, 0x61747369, 0x0065636e, 0x00030005, 0x0000000d, 0x00000000,
	0x00050005, 0x00000012, 0x705f6e69, 0x7469736f, 0x006e6f69, 0x00030047,
	0x0000000b, 0x00000002, 0x00050048, 0x0000000b, 0x00000000, 0x0000000b,
	0x00000000, 0x00050048, 0x0000000b, 0x00000001, 0x0000000b, 0x00000001,
	0x00050048, 0x0000000b, 0x00000002, 0x0000000b, 0x00000003, 0x00050048,
	0x0000000b, 0x00000003, 0x0000000b, 0x00000004, 0x00040047, 0x00000012,
	0x0000001e, 0x00000000, 0x00020013, 0x00000002, 0x00030021, 0x00000003,
	0x00000002, 0x00030016, 0x00000006, 0x00000020, 0x00040017, 0x00000007,
	0x00000006, 0x00000004, 0x00040015, 0x00000008, 0x00000020, 0x00000000,
	0x0004002b, 0x00000008, 0x00000009, 0x00000001, 0x0004001c, 0x0000000a,
	0x00000006, 0x00000009, 0x0006001e, 0x0000000b, 0x00000007, 0x00000006,
	0x0000000a, 0x0000000a, 0x00040020, 0x0000000c, 0x00000003, 0x0000000b,
	0x0004003b, 0x0000000c, 0x0000000d, 0x00000003, 0x00040015, 0x0000000e,
	0x00000020, 0x00000001, 0x0004002b, 0x0000000e, 0x0000000f, 0x00000000,
	0x00040017, 0x00000010, 0x00000006, 0x00000003, 0x00040020, 0x00000011,
	0x00000001, 0x00000010, 0x0004003b, 0x00000011, 0x00000012, 0x00000001,
	0x0004002b, 0x00000006, 0x00000014, 0x3f800000, 0x00040020, 0x00000019,
	0x00000003, 0x00000007, 0x00050036, 0x00000002, 0x00000004, 0x00000000,
	0x00000003, 0x000200f8, 0x00000005, 0x0004003d, 0x00000010, 0x00000013,
	0x00000012, 0x00050051, 0x00000006, 0x00000015, 0x00000013, 0x00000000,
	0x00050051, 0x00000006, 0x00000016, 0x00000013, 0x00000001, 0x00050051,
	0x00000006, 0x00000017, 0x00000013, 0x00000002, 0x00070050, 0x00000007,
	0x00000018, 0x00000015, 0x00000016, 0x00000017, 0x00000014, 0x00050041,
	0x00000019, 0x0000001a, 0x0000000d, 0x0000000f, 0x0003003e, 0x0000001a,
	0x00000018, 0x000100fd, 0x00010038,
};

static const uint32_t triangle_frag_spv[] = {
	0x07230203, 0x00010000, 0x0008000b, 0x0000000e, 0x00000000, 0x00020011,
	0x00000001, 0x0006000b, 0x00000001, 0x4c534c47, 0x6474732e, 0x3035342e,
	0x00000000, 0x0003000e, 0x00000000, 0x00000001, 0x0006000f, 0x00000004,
	0x00000004, 0x6e69616d, 0x00000000, 0x00000009, 0x00030010, 0x00000004,
	0x00000007, 0x00030003, 0x00000002, 0x000001c2, 0x00040005, 0x00000004,
	0x6e69616d, 0x00000000, 0x00050005, 0x00000009, 0x5f74756f, 0x6f6c6f63,
	0x00000072, 0x00040047, 0x00000009, 0x0000001e, 0x00000000, 0x00020013,
	0x00000002, 0x00030021, 0x00000003, 0x00000002, 0x00030016, 0x00000006,
	0x00000020, 0x00040017, 0x00000007, 0x00000006, 0x00000004, 0x00040020,
	0x00000008, 0x00000003, 0x00000007, 0x0004003b, 0x00000008, 0x00000009,
	0x00000003, 0x0004002b, 0x00000006, 0x0000000a, 0x3f800000, 0x0004002b,
	0x00000006, 0x0000000b, 0x3ecccccd, 0x0004002b, 0x00000006, 0x0000000c,
	0x3e4ccccd, 0x0007002c, 0x00000007, 0x0000000d, 0x0000000a, 0x0000000b,
	0x0000000c, 0x0000000a, 0x00050036, 0x00000002, 0x00000004, 0x00000000,
	0x00000003, 0x000200f8, 0x00000005, 0x0003003e, 0x00000009, 0x0000000d,
	0x000100fd, 0x00010038,
};

static VkDevice viewport_device(emb_viewport viewport)
{
	return viewport->emb->device;
}

static VkPhysicalDevice viewport_physical_device(emb_viewport viewport)
{
	return viewport->emb->physical_device;
}

static void destroy_sync_objects(emb_viewport viewport)
{
	if (viewport->emb != NULL && viewport->in_flight != VK_NULL_HANDLE) {
		vkDestroyFence(viewport_device(viewport), viewport->in_flight,
			       NULL);
		viewport->in_flight = VK_NULL_HANDLE;
	}
	if (viewport->emb != NULL &&
	    viewport->render_finished != VK_NULL_HANDLE) {
		vkDestroySemaphore(viewport_device(viewport),
				   viewport->render_finished, NULL);
		viewport->render_finished = VK_NULL_HANDLE;
	}
	if (viewport->emb != NULL &&
	    viewport->image_available != VK_NULL_HANDLE) {
		vkDestroySemaphore(viewport_device(viewport),
				   viewport->image_available, NULL);
		viewport->image_available = VK_NULL_HANDLE;
	}
}

static void destroy_command_pool(emb_viewport viewport)
{
	if (viewport->emb != NULL && viewport->command_pool != VK_NULL_HANDLE) {
		vkDestroyCommandPool(viewport_device(viewport),
				     viewport->command_pool, NULL);
		viewport->command_pool = VK_NULL_HANDLE;
	}
}

static void destroy_swapchain_objects(emb_viewport viewport)
{
	if (viewport->emb != NULL && viewport_device(viewport) != VK_NULL_HANDLE) {
		for (uint32_t i = 0; i < viewport->image_count; ++i) {
			if (viewport->framebuffers != NULL &&
			    viewport->framebuffers[i] != VK_NULL_HANDLE) {
				vkDestroyFramebuffer(viewport_device(viewport),
						     viewport->framebuffers[i],
						     NULL);
			}
			if (viewport->image_views != NULL &&
			    viewport->image_views[i] != VK_NULL_HANDLE) {
				vkDestroyImageView(viewport_device(viewport),
						   viewport->image_views[i],
						   NULL);
			}
		}
	}
	free(viewport->command_buffers);
	free(viewport->framebuffers);
	free(viewport->image_views);
	free(viewport->images);
	viewport->command_buffers = NULL;
	viewport->framebuffers = NULL;
	viewport->image_views = NULL;
	viewport->images = NULL;
	viewport->image_count = 0;
	if (viewport->emb != NULL && viewport->pipeline != VK_NULL_HANDLE) {
		vkDestroyPipeline(viewport_device(viewport), viewport->pipeline,
				  NULL);
		viewport->pipeline = VK_NULL_HANDLE;
	}
	if (viewport->emb != NULL &&
	    viewport->pipeline_layout != VK_NULL_HANDLE) {
		vkDestroyPipelineLayout(viewport_device(viewport),
					viewport->pipeline_layout, NULL);
		viewport->pipeline_layout = VK_NULL_HANDLE;
	}
	if (viewport->emb != NULL && viewport->render_pass != VK_NULL_HANDLE) {
		vkDestroyRenderPass(viewport_device(viewport),
				    viewport->render_pass, NULL);
		viewport->render_pass = VK_NULL_HANDLE;
	}
	if (viewport->emb != NULL && viewport->swapchain != VK_NULL_HANDLE) {
		vkDestroySwapchainKHR(viewport_device(viewport),
				      viewport->swapchain, NULL);
		viewport->swapchain = VK_NULL_HANDLE;
	}
}

static uint32_t clamp_uint32(uint32_t value, uint32_t min, uint32_t max)
{
	if (value < min) {
		return min;
	}
	if (value > max) {
		return max;
	}
	return value;
}

static VkExtent2D choose_swapchain_extent(
	const VkSurfaceCapabilitiesKHR *capabilities)
{
	VkExtent2D extent = capabilities->currentExtent;
	if (extent.width != UINT32_MAX) {
		return extent;
	}

	extent.width = clamp_uint32(960, capabilities->minImageExtent.width,
				    capabilities->maxImageExtent.width);
	extent.height = clamp_uint32(540, capabilities->minImageExtent.height,
				     capabilities->maxImageExtent.height);
	return extent;
}

static VkFormat choose_swapchain_format(VkColorSpaceKHR *color_space,
					const VkSurfaceFormatKHR *formats,
					uint32_t count)
{
	for (uint32_t i = 0; i < count; ++i) {
		if (formats[i].format == VK_FORMAT_B8G8R8A8_UNORM &&
		    formats[i].colorSpace ==
			    VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			*color_space = formats[i].colorSpace;
			return formats[i].format;
		}
	}

	*color_space = formats[0].colorSpace;
	return formats[0].format;
}

static uint32_t choose_image_count(
	const VkSurfaceCapabilitiesKHR *capabilities)
{
	uint32_t image_count = capabilities->minImageCount + 1;
	if (capabilities->maxImageCount != 0 &&
	    image_count > capabilities->maxImageCount) {
		image_count = capabilities->maxImageCount;
	}
	return image_count;
}

static VkBool32 queue_family_supports_present(VkPhysicalDevice device,
					      uint32_t index,
					      VkSurfaceKHR surface)
{
	VkBool32 is_supported = VK_FALSE;
	vkGetPhysicalDeviceSurfaceSupportKHR(device, index, surface,
					     &is_supported);
	return is_supported;
}

static VkResult create_swapchain(emb_viewport viewport)
{
	VkSurfaceCapabilitiesKHR capabilities;
	VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
		viewport_physical_device(viewport), viewport->surface,
		&capabilities);
	if (result != VK_SUCCESS) {
		return result;
	}

	uint32_t format_count = 0;
	result = vkGetPhysicalDeviceSurfaceFormatsKHR(
		viewport_physical_device(viewport), viewport->surface,
		&format_count, NULL);
	if (result != VK_SUCCESS || format_count == 0) {
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	VkSurfaceFormatKHR *formats =
		calloc(format_count, sizeof(VkSurfaceFormatKHR));
	if (formats == NULL) {
		return VK_ERROR_OUT_OF_HOST_MEMORY;
	}
	result = vkGetPhysicalDeviceSurfaceFormatsKHR(
		viewport_physical_device(viewport), viewport->surface,
		&format_count, formats);
	if (result != VK_SUCCESS) {
		goto err_free_formats;
	}

	uint32_t present_mode_count = 0;
	result = vkGetPhysicalDeviceSurfacePresentModesKHR(
		viewport_physical_device(viewport), viewport->surface,
		&present_mode_count, NULL);
	if (result != VK_SUCCESS || present_mode_count == 0) {
		result = VK_ERROR_INITIALIZATION_FAILED;
		goto err_free_formats;
	}

	VkColorSpaceKHR color_space = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	viewport->swapchain_format =
		choose_swapchain_format(&color_space, formats, format_count);
	viewport->swapchain_extent = choose_swapchain_extent(&capabilities);
	if (viewport->swapchain_extent.width == 0 ||
	    viewport->swapchain_extent.height == 0) {
		result = VK_ERROR_OUT_OF_DATE_KHR;
		goto err_free_formats;
	}

	VkSwapchainCreateInfoKHR swapchain_info = {
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.surface = viewport->surface,
		.minImageCount = choose_image_count(&capabilities),
		.imageFormat = viewport->swapchain_format,
		.imageColorSpace = color_space,
		.imageExtent = viewport->swapchain_extent,
		.imageArrayLayers = 1,
		.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.preTransform = capabilities.currentTransform,
		.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		.presentMode = VK_PRESENT_MODE_FIFO_KHR,
		.clipped = VK_TRUE,
		.oldSwapchain = VK_NULL_HANDLE,
	};
	result = vkCreateSwapchainKHR(viewport_device(viewport),
				      &swapchain_info, NULL,
				      &viewport->swapchain);
	if (result != VK_SUCCESS) {
		goto err_free_formats;
	}

	result = vkGetSwapchainImagesKHR(viewport_device(viewport),
					 viewport->swapchain,
					 &viewport->image_count, NULL);
	if (result != VK_SUCCESS || viewport->image_count == 0) {
		result = VK_ERROR_INITIALIZATION_FAILED;
		goto err_destroy_swapchain;
	}

	viewport->images = calloc(viewport->image_count, sizeof(VkImage));
	viewport->image_views =
		calloc(viewport->image_count, sizeof(VkImageView));
	viewport->framebuffers =
		calloc(viewport->image_count, sizeof(VkFramebuffer));
	viewport->command_buffers =
		calloc(viewport->image_count, sizeof(VkCommandBuffer));
	if (viewport->images == NULL || viewport->image_views == NULL ||
	    viewport->framebuffers == NULL ||
	    viewport->command_buffers == NULL) {
		result = VK_ERROR_OUT_OF_HOST_MEMORY;
		goto err_destroy_swapchain;
	}

	result = vkGetSwapchainImagesKHR(viewport_device(viewport),
					 viewport->swapchain,
					 &viewport->image_count,
					 viewport->images);
	if (result != VK_SUCCESS) {
		goto err_destroy_swapchain;
	}

	free(formats);
	return VK_SUCCESS;

err_destroy_swapchain:
	destroy_swapchain_objects(viewport);
err_free_formats:
	free(formats);
	return result;
}

static VkResult create_render_pass(emb_viewport viewport)
{
	VkAttachmentDescription color_attachment = {
		.format = viewport->swapchain_format,
		.samples = VK_SAMPLE_COUNT_1_BIT,
		.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
		.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
		.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
		.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
	};
	VkAttachmentReference color_ref = {
		.attachment = 0,
		.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
	};
	VkSubpassDescription subpass = {
		.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
		.colorAttachmentCount = 1,
		.pColorAttachments = &color_ref,
	};
	VkSubpassDependency dependency = {
		.srcSubpass = VK_SUBPASS_EXTERNAL,
		.dstSubpass = 0,
		.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
	};
	VkRenderPassCreateInfo render_pass_info = {
		.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
		.attachmentCount = 1,
		.pAttachments = &color_attachment,
		.subpassCount = 1,
		.pSubpasses = &subpass,
		.dependencyCount = 1,
		.pDependencies = &dependency,
	};
	return vkCreateRenderPass(viewport_device(viewport), &render_pass_info,
				  NULL, &viewport->render_pass);
}

static VkResult create_shader_module(VkShaderModule *module, VkDevice device,
				     const uint32_t *code, size_t size)
{
	VkShaderModuleCreateInfo info = {
		.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.codeSize = size,
		.pCode = code,
	};
	return vkCreateShaderModule(device, &info, NULL, module);
}

static VkResult create_pipeline(emb_viewport viewport)
{
	VkShaderModule vert_module = VK_NULL_HANDLE;
	VkShaderModule frag_module = VK_NULL_HANDLE;
	VkResult result = create_shader_module(
		&vert_module, viewport_device(viewport), triangle_vert_spv,
		sizeof(triangle_vert_spv));
	if (result != VK_SUCCESS) {
		return result;
	}

	result = create_shader_module(&frag_module, viewport_device(viewport),
				      triangle_frag_spv,
				      sizeof(triangle_frag_spv));
	if (result != VK_SUCCESS) {
		vkDestroyShaderModule(viewport_device(viewport), vert_module,
				      NULL);
		return result;
	}

	VkPipelineShaderStageCreateInfo shader_stages[] = {
		{
			.sType =
				VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			.stage = VK_SHADER_STAGE_VERTEX_BIT,
			.module = vert_module,
			.pName = "main",
		},
		{
			.sType =
				VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
			.module = frag_module,
			.pName = "main",
		},
	};
	VkVertexInputBindingDescription binding_description = {
		.binding = 0,
		.stride = sizeof(float) * 3U,
		.inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
	};
	VkVertexInputAttributeDescription attribute_description = {
		.location = 0,
		.binding = 0,
		.format = VK_FORMAT_R32G32B32_SFLOAT,
		.offset = 0,
	};
	VkPipelineVertexInputStateCreateInfo vertex_input = {
		.sType =
			VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
		.vertexBindingDescriptionCount = 1,
		.pVertexBindingDescriptions = &binding_description,
		.vertexAttributeDescriptionCount = 1,
		.pVertexAttributeDescriptions = &attribute_description,
	};
	VkPipelineInputAssemblyStateCreateInfo input_assembly = {
		.sType =
			VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
		.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
	};
	VkViewport vk_viewport = {
		.x = 0.0f,
		.y = 0.0f,
		.width = (float)viewport->swapchain_extent.width,
		.height = (float)viewport->swapchain_extent.height,
		.minDepth = 0.0f,
		.maxDepth = 1.0f,
	};
	VkRect2D scissor = {
		.offset = { 0, 0 },
		.extent = viewport->swapchain_extent,
	};
	VkPipelineViewportStateCreateInfo viewport_state = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
		.viewportCount = 1,
		.pViewports = &vk_viewport,
		.scissorCount = 1,
		.pScissors = &scissor,
	};
	VkPipelineRasterizationStateCreateInfo rasterization = {
		.sType =
			VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
		.polygonMode = VK_POLYGON_MODE_FILL,
		.cullMode = VK_CULL_MODE_NONE,
		.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
		.lineWidth = 1.0f,
	};
	VkPipelineMultisampleStateCreateInfo multisample = {
		.sType =
			VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
		.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
	};
	VkPipelineColorBlendAttachmentState blend_attachment = {
		.colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
				  VK_COLOR_COMPONENT_G_BIT |
				  VK_COLOR_COMPONENT_B_BIT |
				  VK_COLOR_COMPONENT_A_BIT,
	};
	VkPipelineColorBlendStateCreateInfo color_blend = {
		.sType =
			VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
		.attachmentCount = 1,
		.pAttachments = &blend_attachment,
	};
	VkPipelineLayoutCreateInfo pipeline_layout_info = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
	};
	result = vkCreatePipelineLayout(viewport_device(viewport),
					&pipeline_layout_info, NULL,
					&viewport->pipeline_layout);
	if (result != VK_SUCCESS) {
		goto err_destroy_modules;
	}

	VkGraphicsPipelineCreateInfo pipeline_info = {
		.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
		.stageCount = ARRAY_SIZE(shader_stages),
		.pStages = shader_stages,
		.pVertexInputState = &vertex_input,
		.pInputAssemblyState = &input_assembly,
		.pViewportState = &viewport_state,
		.pRasterizationState = &rasterization,
		.pMultisampleState = &multisample,
		.pColorBlendState = &color_blend,
		.layout = viewport->pipeline_layout,
		.renderPass = viewport->render_pass,
		.subpass = 0,
	};
	result = vkCreateGraphicsPipelines(viewport_device(viewport),
					   VK_NULL_HANDLE, 1, &pipeline_info,
					   NULL, &viewport->pipeline);
	if (result != VK_SUCCESS) {
		vkDestroyPipelineLayout(viewport_device(viewport),
					viewport->pipeline_layout, NULL);
		viewport->pipeline_layout = VK_NULL_HANDLE;
	}

err_destroy_modules:
	vkDestroyShaderModule(viewport_device(viewport), frag_module, NULL);
	vkDestroyShaderModule(viewport_device(viewport), vert_module, NULL);
	return result;
}

static VkResult create_image_views(emb_viewport viewport)
{
	for (uint32_t i = 0; i < viewport->image_count; ++i) {
		VkImageViewCreateInfo view_info = {
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.image = viewport->images[i],
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.format = viewport->swapchain_format,
			.components =
				{
					.r = VK_COMPONENT_SWIZZLE_IDENTITY,
					.g = VK_COMPONENT_SWIZZLE_IDENTITY,
					.b = VK_COMPONENT_SWIZZLE_IDENTITY,
					.a = VK_COMPONENT_SWIZZLE_IDENTITY,
				},
			.subresourceRange =
				{
					.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
					.baseMipLevel = 0,
					.levelCount = 1,
					.baseArrayLayer = 0,
					.layerCount = 1,
				},
		};
		VkResult result = vkCreateImageView(viewport_device(viewport),
						    &view_info, NULL,
						    &viewport->image_views[i]);
		if (result != VK_SUCCESS) {
			return result;
		}
	}
	return VK_SUCCESS;
}

static VkResult create_framebuffers(emb_viewport viewport)
{
	for (uint32_t i = 0; i < viewport->image_count; ++i) {
		VkImageView attachments[] = { viewport->image_views[i] };
		VkFramebufferCreateInfo framebuffer_info = {
			.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
			.renderPass = viewport->render_pass,
			.attachmentCount = ARRAY_SIZE(attachments),
			.pAttachments = attachments,
			.width = viewport->swapchain_extent.width,
			.height = viewport->swapchain_extent.height,
			.layers = 1,
		};
		VkResult result = vkCreateFramebuffer(
			viewport_device(viewport), &framebuffer_info, NULL,
			&viewport->framebuffers[i]);
		if (result != VK_SUCCESS) {
			return result;
		}
	}
	return VK_SUCCESS;
}

static VkResult create_command_buffers(emb_viewport viewport)
{
	VkCommandPoolCreateInfo command_pool_info = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
		.queueFamilyIndex = viewport->emb->queue_family_index,
	};
	VkResult result = vkCreateCommandPool(viewport_device(viewport),
					      &command_pool_info, NULL,
					      &viewport->command_pool);
	if (result != VK_SUCCESS) {
		return result;
	}

	VkCommandBufferAllocateInfo alloc_info = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool = viewport->command_pool,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = viewport->image_count,
	};
	return vkAllocateCommandBuffers(viewport_device(viewport), &alloc_info,
					viewport->command_buffers);
}

static VkResult create_sync_objects(emb_viewport viewport)
{
	VkSemaphoreCreateInfo semaphore_info = {
		.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
	};
	VkFenceCreateInfo fence_info = {
		.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
		.flags = VK_FENCE_CREATE_SIGNALED_BIT,
	};
	VkResult result = vkCreateSemaphore(viewport_device(viewport),
					    &semaphore_info, NULL,
					    &viewport->image_available);
	if (result != VK_SUCCESS) {
		return result;
	}
	result = vkCreateSemaphore(viewport_device(viewport),
				   &semaphore_info, NULL,
				   &viewport->render_finished);
	if (result != VK_SUCCESS) {
		return result;
	}
	return vkCreateFence(viewport_device(viewport), &fence_info, NULL,
			     &viewport->in_flight);
}

static VkResult create_viewport_runtime_objects(emb_viewport viewport)
{
	VkResult result = create_swapchain(viewport);
	if (result != VK_SUCCESS) {
		return result;
	}
	result = create_image_views(viewport);
	if (result != VK_SUCCESS) {
		return result;
	}
	result = create_render_pass(viewport);
	if (result != VK_SUCCESS) {
		return result;
	}
	result = create_pipeline(viewport);
	if (result != VK_SUCCESS) {
		return result;
	}
	result = create_framebuffers(viewport);
	if (result != VK_SUCCESS) {
		return result;
	}
	result = create_command_buffers(viewport);
	if (result != VK_SUCCESS) {
		return result;
	}
	return create_sync_objects(viewport);
}

static VkResult recreate_viewport_runtime_objects(emb_viewport viewport)
{
	VkResult result = vkDeviceWaitIdle(viewport_device(viewport));
	if (result != VK_SUCCESS) {
		return result;
	}

	destroy_sync_objects(viewport);
	destroy_command_pool(viewport);
	destroy_swapchain_objects(viewport);
	return create_viewport_runtime_objects(viewport);
}

static emb_accessor find_position_accessor(const struct emb_primitive *primitive)
{
	for (uint32_t i = 0; i < primitive->attributes_count; ++i) {
		if (primitive->attributes[i].type == EMB_ATTRIB_POSITION) {
			return primitive->attributes[i].data;
		}
	}

	return NULL;
}

static bool vertex_accessor_is_supported(emb_accessor accessor)
{
	if (accessor == NULL || accessor->bview == NULL ||
	    accessor->bview->buffer == NULL ||
	    accessor->bview->buffer->vulkan == VK_NULL_HANDLE) {
		return false;
	}
	if (accessor->bview->target != EMB_ARRAY_BUFFER) {
		return false;
	}
	if (accessor->comp_type != EMB_ATTRIB_COMPONENT_FLOAT ||
	    accessor->attrib_type != EMB_ATTRIB_VEC3 ||
	    accessor->is_normalized != 0) {
		return false;
	}
	if (accessor->attrib_count == 0) {
		return false;
	}

	return true;
}

static bool accessor_has_required_size(emb_accessor accessor, size_t elem_size)
{
	if (accessor == NULL || accessor->bview == NULL || elem_size == 0) {
		return false;
	}
	if ((size_t)accessor->byte_offset > accessor->bview->nbytes) {
		return false;
	}

	const size_t available = accessor->bview->nbytes - accessor->byte_offset;
	return (size_t)accessor->attrib_count <= (available / elem_size);
}

static bool index_accessor_component_info(emb_accessor accessor,
					  VkIndexType *index_type,
					  size_t *elem_size)
{
	if (accessor == NULL || index_type == NULL || elem_size == NULL) {
		return false;
	}

	switch (accessor->comp_type) {
	case EMB_ATTRIB_COMPONENT_USHORT:
		*index_type = VK_INDEX_TYPE_UINT16;
		*elem_size = sizeof(uint16_t);
		return true;
	case EMB_ATTRIB_COMPONENT_UINT:
		*index_type = VK_INDEX_TYPE_UINT32;
		*elem_size = sizeof(uint32_t);
		return true;
	default:
		return false;
	}
}

static bool index_accessor_is_supported(emb_accessor accessor,
					VkIndexType *index_type,
					size_t *elem_size)
{
	if (accessor == NULL || accessor->bview == NULL ||
	    accessor->bview->buffer == NULL ||
	    accessor->bview->buffer->vulkan == VK_NULL_HANDLE) {
		return false;
	}
	if (accessor->bview->target != EMB_ELEMENT_ARRAY_BUFFER) {
		return false;
	}
	if (accessor->attrib_type != EMB_ATTRIB_SCALAR ||
	    accessor->is_normalized != 0 || accessor->attrib_count == 0) {
		return false;
	}
	if (!index_accessor_component_info(accessor, index_type, elem_size)) {
		return false;
	}

	return true;
}

static enum emb_result record_scene_draws(emb_viewport viewport,
					  VkCommandBuffer command_buffer)
{
	for (struct list_head *item = viewport->emb->meshes.next;
	     item != &viewport->emb->meshes; item = item->next) {
		struct emb_mesh *mesh =
			container_of(item, struct emb_mesh, scene_node);

		for (size_t primitive_index = 0;
		     primitive_index < mesh->primitives_count;
		     ++primitive_index) {
			const struct emb_primitive *primitive =
				&mesh->primitives[primitive_index];
			emb_accessor accessor = find_position_accessor(primitive);
			const size_t packed_stride = sizeof(float) * 3U;
			const size_t stride = accessor == NULL ?
							 0 :
							 (accessor->bview->stride == 0 ?
								  packed_stride :
								  accessor->bview->stride);
			const bool has_indices = primitive->indeces != NULL;
			VkIndexType index_type = VK_INDEX_TYPE_MAX_ENUM;
			size_t index_size = 0;
			size_t index_stride = 0;
			bool has_valid_indices = false;

			if (has_indices) {
				has_valid_indices = index_accessor_is_supported(
					primitive->indeces, &index_type,
					&index_size);
				index_stride =
					(has_valid_indices &&
					 primitive->indeces->bview->stride != 0) ?
						primitive->indeces->bview->stride :
						index_size;
			}

			if (primitive->mode != EMB_MODE_TRIANGLES ||
			    !vertex_accessor_is_supported(accessor) ||
			    stride != packed_stride ||
			    !accessor_has_required_size(accessor,
							packed_stride) ||
			    (has_indices &&
			     (!has_valid_indices || index_stride != index_size ||
			      !accessor_has_required_size(primitive->indeces,
							  index_size)))) {
				return EMB_ERROR_INITIALIZATION_FAILED;
			}

			const VkBuffer buffer = accessor->bview->buffer->vulkan;
			const VkDeviceSize offset =
				(VkDeviceSize)(accessor->bview->offset +
					       accessor->byte_offset);
			vkCmdBindVertexBuffers(command_buffer, 0, 1, &buffer,
					       &offset);
			if (has_indices) {
				const VkBuffer index_buffer =
					primitive->indeces->bview->buffer->vulkan;
				const VkDeviceSize index_offset = (VkDeviceSize)(
					primitive->indeces->bview->offset +
					primitive->indeces->byte_offset);
				vkCmdBindIndexBuffer(command_buffer, index_buffer,
						     index_offset, index_type);
				vkCmdDrawIndexed(command_buffer,
						 primitive->indeces->attrib_count,
						 1, 0, 0, 0);
			} else {
				vkCmdDraw(command_buffer, accessor->attrib_count, 1,
					  0, 0);
			}
		}
	}

	return EMB_SUCCESS;
}

static enum emb_result record_command_buffer(emb_viewport viewport,
					     uint32_t image_index)
{
	VkResult result = vkResetCommandBuffer(
		viewport->command_buffers[image_index], 0);
	if (result != VK_SUCCESS) {
		return EMB_ERROR_INITIALIZATION_FAILED;
	}

	VkCommandBufferBeginInfo begin_info = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
	};
	result = vkBeginCommandBuffer(viewport->command_buffers[image_index],
				      &begin_info);
	if (result != VK_SUCCESS) {
		return EMB_ERROR_INITIALIZATION_FAILED;
	}

	VkClearValue clear_value = {
		.color = { .float32 = { 0.0f, 0.0f, 0.0f, 1.0f } },
	};
	VkRenderPassBeginInfo render_pass_info = {
		.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
		.renderPass = viewport->render_pass,
		.framebuffer = viewport->framebuffers[image_index],
		.renderArea =
			{
				.offset = { 0, 0 },
				.extent = viewport->swapchain_extent,
			},
		.clearValueCount = 1,
		.pClearValues = &clear_value,
	};
	vkCmdBeginRenderPass(viewport->command_buffers[image_index],
			     &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdBindPipeline(viewport->command_buffers[image_index],
			  VK_PIPELINE_BIND_POINT_GRAPHICS,
			  viewport->pipeline);
	enum emb_result emb_result = record_scene_draws(
		viewport, viewport->command_buffers[image_index]);
	vkCmdEndRenderPass(viewport->command_buffers[image_index]);
	result = vkEndCommandBuffer(viewport->command_buffers[image_index]);
	if (result != VK_SUCCESS || emb_result != EMB_SUCCESS) {
		return EMB_ERROR_INITIALIZATION_FAILED;
	}

	return EMB_SUCCESS;
}

EMB_API enum emb_result EMB_CALL emb_viewport_init(emb_viewport viewport,
						   emb_instance embaland,
						   VkSurfaceKHR surface)
{
	VkResult vk_result = VK_SUCCESS;
	if (embaland == NULL) {
		return EMB_ERROR_INITIALIZATION_FAILED;
	}
	if (viewport == NULL) {
		return EMB_ERROR_INITIALIZATION_FAILED;
	}
	if (embaland->vulkan == VK_NULL_HANDLE ||
	    embaland->device == VK_NULL_HANDLE ||
	    embaland->physical_device == VK_NULL_HANDLE ||
	    embaland->queue_family_index == UINT32_MAX) {
		return EMB_ERROR_INVALID_EXTERNAL_HANDLE;
	}
	if (surface == VK_NULL_HANDLE) {
		return EMB_ERROR_INVALID_EXTERNAL_HANDLE;
	}

	memset(viewport, 0, sizeof(*viewport));
	viewport->emb = embaland;
	viewport->surface = surface;

	if (!queue_family_supports_present(embaland->physical_device,
					   embaland->queue_family_index,
					   surface)) {
		goto err;
	}
	vk_result = create_viewport_runtime_objects(viewport);
	if (vk_result != VK_SUCCESS) {
		goto err;
	}

	return EMB_SUCCESS;

err:
	emb_viewport_destroy(viewport);
	return EMB_ERROR_INITIALIZATION_FAILED;
}

EMB_API void EMB_CALL emb_viewport_destroy(emb_viewport viewport)
{
	if (viewport == NULL) {
		return;
	}

	if (viewport->emb != NULL && viewport_device(viewport) != VK_NULL_HANDLE) {
		vkDeviceWaitIdle(viewport_device(viewport));
	}
	destroy_sync_objects(viewport);
	destroy_command_pool(viewport);
	destroy_swapchain_objects(viewport);
	viewport->surface = VK_NULL_HANDLE;
	viewport->emb = NULL;
}

EMB_API enum emb_result EMB_CALL emb_viewport_render(emb_viewport viewport,
						     uint64_t timeout)
{
	uint32_t image_index = 0;
	bool should_recreate_swapchain = false;
	if (viewport == NULL || viewport->emb == NULL ||
	    viewport_device(viewport) == VK_NULL_HANDLE ||
	    viewport->emb->graphics_queue == VK_NULL_HANDLE ||
	    viewport->swapchain == VK_NULL_HANDLE) {
		return EMB_ERROR_INITIALIZATION_FAILED;
	}

	VkResult result = vkWaitForFences(viewport_device(viewport), 1,
					  &viewport->in_flight, VK_TRUE,
					  timeout);
	if (result != VK_SUCCESS) {
		return EMB_ERROR_INITIALIZATION_FAILED;
	}

	result = vkAcquireNextImageKHR(viewport_device(viewport),
				       viewport->swapchain, timeout,
				       viewport->image_available,
				       VK_NULL_HANDLE, &image_index);
	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		if (recreate_viewport_runtime_objects(viewport) != VK_SUCCESS) {
			return EMB_ERROR_INITIALIZATION_FAILED;
		}
		return EMB_SUCCESS;
	}
	if (result == VK_SUBOPTIMAL_KHR) {
		should_recreate_swapchain = true;
	} else if (result != VK_SUCCESS) {
		return EMB_ERROR_INITIALIZATION_FAILED;
	}

	result = vkResetFences(viewport_device(viewport), 1,
			       &viewport->in_flight);
	if (result != VK_SUCCESS) {
		return EMB_ERROR_INITIALIZATION_FAILED;
	}
	if (record_command_buffer(viewport, image_index) != EMB_SUCCESS) {
		return EMB_ERROR_INITIALIZATION_FAILED;
	}

	VkPipelineStageFlags wait_stage =
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	VkSubmitInfo submit_info = {
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &viewport->image_available,
		.pWaitDstStageMask = &wait_stage,
		.commandBufferCount = 1,
		.pCommandBuffers = &viewport->command_buffers[image_index],
		.signalSemaphoreCount = 1,
		.pSignalSemaphores = &viewport->render_finished,
	};
	result = vkQueueSubmit(viewport->emb->graphics_queue, 1, &submit_info,
			       viewport->in_flight);
	if (result != VK_SUCCESS) {
		return EMB_ERROR_INITIALIZATION_FAILED;
	}

	VkPresentInfoKHR present_info = {
		.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &viewport->render_finished,
		.swapchainCount = 1,
		.pSwapchains = &viewport->swapchain,
		.pImageIndices = &image_index,
	};
	result = vkQueuePresentKHR(viewport->emb->graphics_queue, &present_info);
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
		should_recreate_swapchain = true;
	} else if (result != VK_SUCCESS) {
		return EMB_ERROR_INITIALIZATION_FAILED;
	}
	if (should_recreate_swapchain &&
	    recreate_viewport_runtime_objects(viewport) != VK_SUCCESS) {
		return EMB_ERROR_INITIALIZATION_FAILED;
	}

	return EMB_SUCCESS;
}
