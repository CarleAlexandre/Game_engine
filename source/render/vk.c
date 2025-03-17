#include "vk_impl.h"

static VkInstance	createInstance(const char *app_name) {
	VkInstance instance;

	// only one layer used for now maybe if later not changed
	const char *layers[1] = {
		"VK_LAYER_KHRONOS_validation"
	};

	const char *exts[4] = {
		"VK_KHR_surface",
		#if defined (_WIN32)
		"VK_KHR_win32_surface",
		#endif
		#if defined (__APPLE__)
		"VK_MVK_macos_surface",
		#elif defined (__linux__)
		"VK_KHR_xcb_surface",
		#endif
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
		VK_EXT_DEBUG_REPORT_EXTENSION_NAME
	};

	const VkApplicationInfo app_info = {
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pNext = 0x00,
		.pApplicationName = app_name,
		.applicationVersion = VK_MAKE_VERSION(1, 0, 0),
		.pEngineName = "NoHeavenEngine",
		.engineVersion = VK_MAKE_VERSION(1, 0, 0),
		.apiVersion = VK_API_VERSION_1_3
	};

	const VkInstanceCreateInfo create_info = {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pNext = 0x00,
		.flags = 0,
		.pApplicationInfo = &app_info,
		.enabledLayerCount = 1,
		.ppEnabledLayerNames = layers,
		.enabledExtensionCount = 4,
		.ppEnabledExtensionNames = exts,
	};

	if (vkCreateInstance(&create_info, NULL, &instance) != VK_SUCCESS) {
		return (0x00);
	}

	return (instance);
}

static VkResult	findSuitablePhysicalDevice(VkInstance instance, int (*selector)(VkPhysicalDevice), VkPhysicalDevice physicalDevice) {
	uint32_t deviceCount = 0;
	if (vkEnumeratePhysicalDevices(instance, &deviceCount, 0x00)) {
		return (VK_ERROR_INITIALIZATION_FAILED);
	}
	if (!deviceCount) {
		return(VK_ERROR_INITIALIZATION_FAILED);
	}
	VkPhysicalDevice devices[deviceCount];

	if (vkEnumeratePhysicalDevices(instance, &deviceCount, devices)) {
		return (VK_ERROR_INITIALIZATION_FAILED);
	}
	for (uint32_t i = 0; i < deviceCount; i++) {
		if (selector(devices[i])) {
			physicalDevice = devices[i];
			return (VK_SUCCESS);
		}
	}

	return (VK_ERROR_INITIALIZATION_FAILED);
}

static uint32_t	findQueueFamily(VkPhysicalDevice device, VkQueueFlags desiredFlags){
	uint32_t familyCount;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, 0x00);
	VkQueueFamilyProperties families[familyCount];
	vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, families);

	for (uint32_t i = 0; i < familyCount; i++) {
		if (families[i].queueCount && (families[i].queueFlags & desiredFlags)) {
			return i;
		}
	}
	return (0);
}

static VkResult	createDevice(VkPhysicalDevice physical_device, VkPhysicalDeviceFeatures device_features,
	uint32_t graphic_family, VkDevice device) {
	const char *extension = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME,
	};
	const float queuePriority = 1.0f;

	const VkDeviceQueueCreateInfo queue_create_info = {
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.pNext = 0x00,
		.flags = 0,
		.queueFamilyIndex = graphic_family,
		.queueCount = 1,
		.pQueuePriorities = &queuePriority,
	};

	const VkDeviceCreateInfo create_info = {
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.pNext = 0x00,
		.flags = 0,
		.queueCreateInfoCount = 1,
		.pQueueCreateInfos = &queue_create_info,
		.enabledLayerCount = 0,
		.ppEnabledLayerNames = 0x00,
		.enabledExtensionCount = 1,
		.ppEnabledExtensionNames = &extension,
		.pEnabledFeatures = &device_features,
	};

	if (vkCreateDevice(physical_device, &create_info, NULL, &device)) {
		return (VK_ERROR_OUT_OF_DEVICE_MEMORY);
	}

	return (VK_SUCCESS);
}

static int selectDevice(VkPhysicalDevice device) {
	if (device) {
		return (1);
	}
	return (0);
}

static swapChainSupportDetails	querySwapchainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
	swapChainSupportDetails details;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
		device, surface, &details.capabilities);

	vkGetPhysicalDeviceSurfaceFormatsKHR(
		device, surface, &details.formatCount, 0x00);
	if (details.formatCount) {
		details.formats = malloc(
			sizeof(VkSurfaceFormatKHR) * details.formatCount);//need to free size in win destroy
		vkGetPhysicalDeviceSurfaceFormatsKHR(
			device, surface, &details.formatCount, details.formats);
	} else {
		return (details);
	}

	vkGetPhysicalDeviceSurfacePresentModesKHR(
		device, surface, &details.presentModesCount, 0x00);
	if (details.presentModesCount) {
		details.presentModes = malloc(
			sizeof(VkPresentModeKHR) * details.presentModesCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(
			device, surface, &details.presentModesCount, details.presentModes);
	} else {
		free(details.formats);
		return (details);
	}
	return (details);
}

static VkSurfaceFormatKHR	chooseSwapSurfaceFormat(const VkSurfaceFormatKHR *availableFormats) {
	(void)availableFormats;
	return ((VkSurfaceFormatKHR){
		VK_FORMAT_B8G8R8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
	});
}

static VkPresentModeKHR	chooseSwapPresentMode(const VkPresentModeKHR *availablePresentModes,
	const uint32_t presentModeCount) {
	(void)availablePresentModes;
	for (uint32_t i = 0; i < presentModeCount; i++) {
		if (availablePresentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
			return (availablePresentModes[i]);
	}
	return (VK_PRESENT_MODE_FIFO_KHR);
}

window_t init_window(const int height, const int width, const char *title) {
	window_t window = {0};
	if (volkInitialize() != VK_SUCCESS) {
		return (window);
	};
	window.height = height;
	window.width = width;
	window.title = title;

	window.instance = createInstance(title);
	volkLoadInstance(window.instance);

	findSuitablePhysicalDevice(
		window.instance, selectDevice, window.phy_device);

	VkPhysicalDeviceFeatures deviceFeatures = {0};
	if (createDevice(window.phy_device, deviceFeatures, findQueueFamily(
			window.phy_device, VK_QUEUE_GRAPHICS_BIT), window.device)) {
		return (window);
	}
	return (window);
}

void	window_destroy(window_t *window) {
	vkDestroyDevice(window->device, 0x00);
	vkDestroySurfaceKHR(window->instance, window->surface, 0x00);
	vkDestroyInstance(window->instance, 0x00);
}

VkSwapchainKHR createSwapChain(window_t *window, VkSurfaceKHR surface) {
    VkSwapchainKHR swapChain;
    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(window->phy_device, surface, &capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(window->phy_device, surface, &formatCount, NULL);
    VkSurfaceFormatKHR formats[formatCount];
    vkGetPhysicalDeviceSurfaceFormatsKHR(window->phy_device, surface, &formatCount, formats);

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(window->phy_device, surface, &presentModeCount, NULL);
    VkPresentModeKHR presentModes[presentModeCount];
    vkGetPhysicalDeviceSurfacePresentModesKHR(window->phy_device, surface, &presentModeCount, presentModes);

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(presentModes, presentModeCount);
    VkExtent2D extent = {
        .width = window->width,
        .height = window->height
    };

    uint32_t imageCount = capabilities.minImageCount + 1;
    if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) {
        imageCount = capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = surface,
        .minImageCount = imageCount,
        .imageFormat = surfaceFormat.format,
        .imageColorSpace = surfaceFormat.colorSpace,
        .imageExtent = extent,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .preTransform = capabilities.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = presentMode,
        .clipped = VK_TRUE,
        .oldSwapchain = VK_NULL_HANDLE
    };

    if (vkCreateSwapchainKHR(window->device, &createInfo, NULL, &swapChain) != VK_SUCCESS) {
        return VK_NULL_HANDLE;
    }

    return swapChain;
}

VkRenderPass createRenderPass(VkDevice device, VkFormat swapChainImageFormat) {
    VkAttachmentDescription colorAttachment = {
        .format = swapChainImageFormat,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
    };

    VkAttachmentReference colorAttachmentRef = {
        .attachment = 0,
        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    };

    VkSubpassDescription subpass = {
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .colorAttachmentCount = 1,
        .pColorAttachments = &colorAttachmentRef
    };

    VkRenderPassCreateInfo renderPassInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .attachmentCount = 1,
        .pAttachments = &colorAttachment,
        .subpassCount = 1,
        .pSubpasses = &subpass
    };

    VkRenderPass renderPass;
    if (vkCreateRenderPass(device, &renderPassInfo, NULL, &renderPass) != VK_SUCCESS) {
        return VK_NULL_HANDLE;
    }

    return renderPass;
}

VkPipelineLayout createPipelineLayout(VkDevice device) {
    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount = 0,
        .pushConstantRangeCount = 0
    };

    VkPipelineLayout pipelineLayout;
    if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, NULL, &pipelineLayout) != VK_SUCCESS) {
        return VK_NULL_HANDLE;
    }

    return pipelineLayout;
}

VkShaderModule createShaderModule(VkDevice device, const char *code, size_t codeSize) {
    VkShaderModuleCreateInfo createInfo = {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = codeSize,
        .pCode = (const uint32_t*)code
    };

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(device, &createInfo, NULL, &shaderModule) != VK_SUCCESS) {
        return VK_NULL_HANDLE;
    }

    return shaderModule;
}

VkPipeline createGraphicsPipeline(VkDevice device, VkRenderPass renderPass, VkPipelineLayout pipelineLayout,
                                 VkShaderModule vertShaderModule, VkShaderModule fragShaderModule) {
    VkPipelineShaderStageCreateInfo vertShaderStageInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .stage = VK_SHADER_STAGE_VERTEX_BIT,
        .module = vertShaderModule,
        .pName = "main"
    };

    VkPipelineShaderStageCreateInfo fragShaderStageInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
        .module = fragShaderModule,
        .pName = "main"
    };

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .vertexBindingDescriptionCount = 0,
        .vertexAttributeDescriptionCount = 0
    };

    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = VK_FALSE
    };

    VkViewport viewport = {
        .x = 0.0f,
        .y = 0.0f,
        .width = 800.0f,
        .height = 600.0f,
        .minDepth = 0.0f,
        .maxDepth = 1.0f
    };

    VkRect2D scissor = {
        .offset = {0, 0},
        .extent = {800, 600}
    };

    VkPipelineViewportStateCreateInfo viewportState = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .viewportCount = 1,
        .pViewports = &viewport,
        .scissorCount = 1,
        .pScissors = &scissor
    };

    VkPipelineRasterizationStateCreateInfo rasterizer = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .depthClampEnable = VK_FALSE,
        .rasterizerDiscardEnable = VK_FALSE,
        .polygonMode = VK_POLYGON_MODE_FILL,
        .lineWidth = 1.0f,
        .cullMode = VK_CULL_MODE_BACK_BIT,
        .frontFace = VK_FRONT_FACE_CLOCKWISE,
        .depthBiasEnable = VK_FALSE
    };

    VkPipelineMultisampleStateCreateInfo multisampling = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .sampleShadingEnable = VK_FALSE,
        .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT
    };

    VkPipelineColorBlendAttachmentState colorBlendAttachment = {
        .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | 
                         VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
        .blendEnable = VK_FALSE
    };

    VkPipelineColorBlendStateCreateInfo colorBlending = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .logicOpEnable = VK_FALSE,
        .attachmentCount = 1,
        .pAttachments = &colorBlendAttachment
    };

    VkGraphicsPipelineCreateInfo pipelineInfo = {
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .stageCount = 2,
        .pStages = shaderStages,
        .pVertexInputState = &vertexInputInfo,
        .pInputAssemblyState = &inputAssembly,
        .pViewportState = &viewportState,
        .pRasterizationState = &rasterizer,
        .pMultisampleState = &multisampling,
        .pColorBlendState = &colorBlending,
        .layout = pipelineLayout,
        .renderPass = renderPass,
        .subpass = 0,
        .basePipelineHandle = VK_NULL_HANDLE
    };

    VkPipeline graphicsPipeline;
    if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, NULL, &graphicsPipeline) != VK_SUCCESS) {
        return VK_NULL_HANDLE;
    }

    return graphicsPipeline;
}

VkCommandPool createCommandPool(VkDevice device, uint32_t queueFamilyIndex) {
    VkCommandPoolCreateInfo poolInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = queueFamilyIndex
    };

    VkCommandPool commandPool;
    if (vkCreateCommandPool(device, &poolInfo, NULL, &commandPool) != VK_SUCCESS) {
        return VK_NULL_HANDLE;
    }

    return commandPool;
}

VkCommandBuffer beginSingleTimeCommands(VkDevice device, VkCommandPool commandPool) {
    VkCommandBufferAllocateInfo allocInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandPool = commandPool,
        .commandBufferCount = 1
    };

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
    };

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void endSingleTimeCommands(VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, VkCommandBuffer commandBuffer) {
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .commandBufferCount = 1,
        .pCommandBuffers = &commandBuffer
    };

    vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(graphicsQueue);

    vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
}

void vkMultiDrawElementsInstanced(VkCommandBuffer commandBuffer,
                                VkBuffer vertexBuffer,
                                VkBuffer indexBuffer,
                                uint32_t *firstIndex,
                                uint32_t *indexCount, 
                                uint32_t *instanceCount,
                                uint32_t drawCount) {
    VkDeviceSize offset = 0;
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer, &offset);
    vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);

    for (uint32_t i = 0; i < drawCount; i++) {
        vkCmdDrawIndexed(commandBuffer,
                        indexCount[i],
                        instanceCount[i], 
                        firstIndex[i],
                        0, // vertexOffset
                        0  // firstInstance
                        );
    }
}
