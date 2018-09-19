#ifndef __BAR_VULKAN_UTILS_H__
#define __BAR_VULKAN_UTILS_H__

#include <android/log.h>
#include <android/asset_manager.h>

#include <vector>
#include <array>
#include <string>

#include <glm/glm.hpp>
#include <vulkan_wrapper.h>

#include "vulkan_data.h"
#include "vulkan_device.h"
#include "vulkan_buffer.h"

#define STATE_RUNNING 1
#define STATE_PAUSED 2
#define STATE_EXIT 3

class VulkanUtils {
public:

    VulkanUtils(AAssetManager *assetManager, const char *vertexShader, const char *fragmentShader);

    VulkanUtils();
    void SetData(AAssetManager *assetManager, const char *vertexShader, const char *fragmentShader);

public:
    void OnSurfaceCreated();
    void OnSurfaceChanged();
    void OnDrawFrame();

public:
    void start();
    void pause();

    inline void resume() { state = STATE_RUNNING; }

    inline void stop() { state = STATE_EXIT; }

    inline void initWindow(ANativeWindow *window) { this->window = window; }

    void createSurfaceDevice();

    void mainLoop();

    void cleanUp();

public:
    void createSwapchain();
    void recreateSwapchain();
    void cleanupSwapchain();

    void createImageViews();
    void createRenderPass();

    void createDescriptorSetLayout();

    void createGraphicsPipeline();

    void createFramebuffers();

public:
    //buffer
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
                      VkMemoryPropertyFlags properties,
                      VkBuffer &buffer, VkDeviceMemory &bufferMemory);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

    void createVertexBuffer();
    void createIndexBuffer();
    void createUniformBuffer();
    void updateUniformBuffer();

public:
    //texture
    void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
                     VkImageUsageFlags usage, VkMemoryPropertyFlags properties,
                     VkImage& image, VkDeviceMemory& imageMemory);
    void createTextureImage();

    void createTextureImageView();

    void createTextureSampler();

public:

    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

    VkCommandBuffer beginSingleTimeCommands();

    void endSingleTimeCommands(VkCommandBuffer commandBuffer);



    void createCommandPool();

    void createDescriptorPool();
    void createDescriptorSet();
    void createCommandBuffers();
    void createSemaphores();
    void drawFrame();

    std::vector<char> readAsset(std::string name);

    VkShaderModule createShaderModule(const std::vector<char> &code);
    VkShaderModule createShaderModule(const std::vector<uint32_t> &code);

    AAssetManager *assetManager;
    std::string vertexShader;
    std::string fragmentShader;
    int state;

    ANativeWindow *window;
    VulkanDevice mVKDevice;

    VkSwapchainKHR swapchain;
    VkRenderPass renderPass;
    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorPool descriptorPool;
    VkDescriptorSet descriptorSet;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
    VkCommandPool mCommandPool;

    VkFormat swapchainImageFormat;
    VkExtent2D swapchainExtent;
    std::vector<VkImage> swapchainImages;
    std::vector<VkImageView> swapchainImageViews;
    std::vector<VkFramebuffer> swapchainFramebuffers;
    std::vector<VkCommandBuffer> commandBuffers;

    HVkBuffer mVertexBuffer;
    HVkBuffer mIndexBuffer;
    HVkBuffer mUniformBuffer;

    VkImage textureImage;
    VkDeviceMemory textureImageMemory;
    VkImageView textureImageView;
    VkSampler textureSampler;

    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;
};

#endif //__BAR_VULKAN_UTILS_H__
