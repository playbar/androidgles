#include <cstring>
#include "vulkan_buffer.h"
#include "vulkan_utils.h"


HVkBuffer::HVkBuffer(VulkanDevice *device)
{
	mVkDevice = device;
}


void HVkBuffer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
{
	mSize = size;
	VkBufferCreateInfo bufferInfo = {
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.size = size,
			.usage = usage,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
	};
	if (vkCreateBuffer(mVkDevice->logicalDevice, &bufferInfo, nullptr, &mBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to create buffer!");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(mVkDevice->logicalDevice, mBuffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {
			.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
			.allocationSize = memRequirements.size,
			.memoryTypeIndex = mVkDevice->findMemoryType(memRequirements.memoryTypeBits, properties),
	};

	if (vkAllocateMemory(mVkDevice->logicalDevice, &allocInfo, nullptr, &mMemory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate buffer memory!");
	}

	vkBindBufferMemory(mVkDevice->logicalDevice, mBuffer, mMemory, 0);
	return;
}

VkResult HVkBuffer::map(VkDeviceSize size, VkDeviceSize offset)
{
	return vkMapMemory(mVkDevice->logicalDevice, mMemory, offset, size, 0, &mpData);
}

void HVkBuffer::copyTo(void* data, VkDeviceSize size)
{
    assert(mpData);
    memcpy(mpData, data, size);
}

void HVkBuffer::unmap()
{
	if (mpData)
	{
		vkUnmapMemory(mVkDevice->logicalDevice, mMemory);
        mpData = nullptr;
	}
}


VkResult HVkBuffer::bind(VkDeviceSize offset)
{
	return vkBindBufferMemory(mVkDevice->logicalDevice, mBuffer, mMemory, offset);
}

void HVkBuffer::setupDescriptor(VkDeviceSize size, VkDeviceSize offset)
{
	descriptor.offset = offset;
	descriptor.buffer = mBuffer;
	descriptor.range = size;
}



VkResult HVkBuffer::flush(VkDeviceSize size, VkDeviceSize offset)
{
	VkMappedMemoryRange mappedRange = {};
	mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
	mappedRange.memory = mMemory;
	mappedRange.offset = offset;
	mappedRange.size = size;
	return vkFlushMappedMemoryRanges(mVkDevice->logicalDevice, 1, &mappedRange);
}

VkResult HVkBuffer::invalidate(VkDeviceSize size, VkDeviceSize offset)
{
	VkMappedMemoryRange mappedRange = {};
	mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
	mappedRange.memory = mMemory;
	mappedRange.offset = offset;
	mappedRange.size = size;
	return vkInvalidateMappedMemoryRanges(mVkDevice->logicalDevice, 1, &mappedRange);
}


void HVkBuffer::destroy()
{
	if (mBuffer)
	{
		vkDestroyBuffer(mVkDevice->logicalDevice, mBuffer, nullptr);
	}
	if (mMemory)
	{
		vkFreeMemory(mVkDevice->logicalDevice, mMemory, nullptr);
	}
}

