#include <cstring>
#include "vulkan_buffer.h"
#include "vulkan_utils.h"


HVkBuffer::HVkBuffer(VulkanDevice *device)
{
	mVkDevice = device;
	mMemory = NULL;
	mBuffer = NULL;
	mSize = 0;
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

void HVkBuffer::copyBuffer(HVkBuffer &srcBuffer)
{
	VkCommandBufferAllocateInfo allocInfo = {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			.commandPool = mVkDevice->mCommandPool,
			.commandBufferCount = 1,
	};

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(mVkDevice->logicalDevice, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo = {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
	};
	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	VkBufferCopy copyRegion = {
			.size = srcBuffer.mSize
	};
	vkCmdCopyBuffer(commandBuffer, srcBuffer.mBuffer, mBuffer, 1, &copyRegion);

	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo = {
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
			.commandBufferCount = 1,
			.pCommandBuffers = &commandBuffer,
	};
	vkQueueSubmit(mVkDevice->graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(mVkDevice->graphicsQueue);

	vkFreeCommandBuffers(mVkDevice->logicalDevice, mVkDevice->mCommandPool, 1, &commandBuffer);
	return;
}


VkResult HVkBuffer::bind(VkDeviceSize offset)
{
	return vkBindBufferMemory(mVkDevice->logicalDevice, mBuffer, mMemory, offset);
}

void HVkBuffer::setupDescriptor(VkDeviceSize size, VkDeviceSize offset)
{
	mDescriptor.offset = offset;
	mDescriptor.buffer = mBuffer;
	mDescriptor.range = size;
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
		mBuffer = NULL;
	}
	if (mMemory)
	{
		vkFreeMemory(mVkDevice->logicalDevice, mMemory, nullptr);
		mMemory = NULL;
	}
}

