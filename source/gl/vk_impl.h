#ifndef VK_IMPL_H
# define VK_IMPL_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

# include <stddef.h>
# include <stdint.h>
# include <stdbool.h>
# include <stdlib.h>

typedef struct s_window {
	int height;
	int width;
	const char *title;
	VkInstance instance;
	VkPhysicalDevice phy_device;
	VkDevice device;
	VkSurfaceKHR surface;
} window_t;

typedef struct s_swapchainsupportdetails {
	VkSurfaceCapabilitiesKHR capabilities;
	uint32_t formatCount;
	VkSurfaceFormatKHR *formats;
	uint32_t presentModesCount;
	VkPresentModeKHR *presentModes;
} swapChainSupportDetails;

#endif