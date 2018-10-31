#pragma once

#include <vector>

#include <vulkan_wrapper.h>
#include "vulkan_device.h"

class HVkTexture
{
public:
	VulkanDevice *mVkDevice;
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	VkImageView textureImageView;
	VkSampler textureSampler;

	uint32_t mWidth;
	uint32_t mHeight;
	uint32_t mipLevels;
	uint32_t layerCount;

	HVkTexture(VulkanDevice *device);

public:
    void createImage(uint32_t width, uint32_t height, VkFormat format,
                                  VkImageTiling tiling, VkImageUsageFlags usage,
                                  VkMemoryPropertyFlags properties, VkImage &image,
                                  VkDeviceMemory &imageMemory);

	void createTextureImage(AAssetManager *assetManager);
    void createTextureImage(const char *imgName);

	void transitionImageLayout(VkImage image, VkFormat format,
											VkImageLayout oldLayout,
											VkImageLayout newLayout);

	void copyBufferToImage(VkBuffer buffer, VkImage image,
                           uint32_t width, uint32_t height);

    void createTextureImageView();

    void createTextureSampler();

};
