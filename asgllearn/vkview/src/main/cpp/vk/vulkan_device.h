#ifndef __BAR_VULKAN_DEVICE_H__
#define __BAR_VULKAN_DEVICE_H__

#include <android/log.h>
#include <android/asset_manager.h>
#include <glm/glm.hpp>
#include <vulkan_wrapper.h>
#include "vulkan_data.h"


class VulkanDevice {
public:
    VulkanDevice();
    ~VulkanDevice();

    void destroy();

public:
    void createInstance();
    void setUpDebugCallback();
    void pickPhysicalDevice();
    void createLogicalDevice();
    void createSurface(ANativeWindow *window);

    bool isDeviceSuitable(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    SwapchainSupportDetails querySwapchainSupport(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    void createCommandPool();
    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);

public:
    VkDebugReportCallbackEXT callback;
    VkInstance instance;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice logicalDevice;
    VkSurfaceKHR surface;
    VkCommandPool mCommandPool;

    VkQueue graphicsQueue;
    VkQueue presentQueue;

};


#endif //__BAR_VULKAN_DEVICE_H__
