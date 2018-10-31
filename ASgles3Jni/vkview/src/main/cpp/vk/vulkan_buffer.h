#ifndef __VULKAN_BUFFER_H__
#define __VULKAN_BUFFER_H__

#include <vector>

#include <vulkan_wrapper.h>
#include "vulkan_device.h"

class HVkBuffer
{
public:
	VulkanDevice *mVkDevice;
	VkBuffer mBuffer;
	VkDeviceMemory mMemory;

	VkDescriptorBufferInfo mDescriptor;
	VkDeviceSize mSize = 0;
	VkDeviceSize alignment = 0;
	VkBufferUsageFlags usageFlags;
	VkMemoryPropertyFlags memoryPropertyFlags;
	void* mpData = nullptr;
public:
	HVkBuffer(VulkanDevice *device);

	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);

	VkResult map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

	void copyTo(void* data, VkDeviceSize size);

	void unmap();

	void copyBuffer(HVkBuffer &srcBuffer);

	VkResult bind(VkDeviceSize offset = 0);

	void setupDescriptor(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

	VkResult flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

	VkResult invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

	void destroy();

};

#endif
