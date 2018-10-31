#include <set>
#include <algorithm>
#include <stdexcept>
#include <chrono>
#include "mylog.h"

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>
#include <unistd.h>
#include <android/native_window.h>
#include <cstring>

#include "string"
#include "vulkan_utils.h"
#include "shaderc/shaderc.hpp"


const int WIDTH = 800;
const int HEIGHT = 448;

const std::vector<Vertex> vertices = {
        {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
        {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
};
const std::vector<uint16_t> indices = {
        0, 1, 2, 2, 3, 0
};


struct shader_type_mapping {
    VkShaderStageFlagBits vkshader_type;
    shaderc_shader_kind   shaderc_type;
};
static const shader_type_mapping shader_map_table[] = {
        {
                VK_SHADER_STAGE_VERTEX_BIT,
                shaderc_glsl_vertex_shader
        },
        {
                VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT,
                shaderc_glsl_tess_control_shader
        },
        {
                VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT,
                shaderc_glsl_tess_evaluation_shader
        },
        {
                VK_SHADER_STAGE_GEOMETRY_BIT,
                shaderc_glsl_geometry_shader},
        {
                VK_SHADER_STAGE_FRAGMENT_BIT,
                shaderc_glsl_fragment_shader
        },
        {
                VK_SHADER_STAGE_COMPUTE_BIT,
                shaderc_glsl_compute_shader
        },
};

shaderc_shader_kind MapShadercType(VkShaderStageFlagBits vkShader) {
    for (auto shader : shader_map_table) {
        if (shader.vkshader_type == vkShader) {
            return shader.shaderc_type;
        }
    }
    assert(false);
    return shaderc_glsl_infer_from_source;
}


VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats)
{
    if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
        return {VK_FORMAT_B8G8R8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
    }

    for (const auto &format : availableFormats) {
        if (format.format == VK_FORMAT_B8G8R8_UNORM
            && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return format;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes)
{
    VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

    for (const auto &mode : availablePresentModes) {
        if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return mode;
        } else if (mode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
            bestMode = mode;
        }
    }

    return bestMode;
}

VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    }

    VkExtent2D actualExtent = {WIDTH, HEIGHT};
    actualExtent.width = std::max(
            capabilities.minImageExtent.width,
            std::min(capabilities.maxImageExtent.width, actualExtent.width)
    );
    actualExtent.height = std::max(
            capabilities.minImageExtent.height,
            std::min(capabilities.maxImageExtent.height, actualExtent.height)
    );

    return actualExtent;
}


bool GLSLtoSPV(const VkShaderStageFlagBits shader_type, const char *pshader,
               std::vector<unsigned int> &spirv)
{
    // On Android, use shaderc instead.
    shaderc::Compiler compiler;
    shaderc::SpvCompilationResult module =
            compiler.CompileGlslToSpv(pshader, strlen(pshader),
                                      MapShadercType(shader_type),
                                      "shader");
    if (module.GetCompilationStatus() !=
        shaderc_compilation_status_success) {
        LOGE("Error: Id=%d, Msg=%s",
             module.GetCompilationStatus(),
             module.GetErrorMessage().c_str());
        return false;
    }
    spirv.assign(module.cbegin(), module.cend());

    return true;
}


VulkanUtils::VulkanUtils(AAssetManager *assetManager, const char *vertexShader, const char *fragmentShader) :
        mVertexBuffer(&mVKDevice),
        mIndexBuffer(&mVKDevice),
        mUniformBuffer(&mVKDevice),
        mTexImage(&mVKDevice),
    assetManager(assetManager),
    vertexShader(std::string(vertexShader)),
    fragmentShader(std::string(fragmentShader)),
    state(STATE_RUNNING)
{
}

VulkanUtils::VulkanUtils():
        mVertexBuffer(&mVKDevice),
        mIndexBuffer(&mVKDevice),
        mUniformBuffer(&mVKDevice),
        mTexImage(&mVKDevice)
{
    state = STATE_RUNNING;
}

void VulkanUtils::SetData(AAssetManager *assetMgr, const char *verShader, const char *fraShader)
{
    assetManager = assetMgr;
    vertexShader = std::string(verShader);
    fragmentShader = std::string(fraShader);
}

void VulkanUtils::pause() {
    state = STATE_PAUSED;
    vkDeviceWaitIdle(mVKDevice.logicalDevice);
}

void VulkanUtils::createSurfaceDevice()
{
    mVKDevice.createInstance();
    mVKDevice.setUpDebugCallback();
    mVKDevice.createSurface(window);
    mVKDevice.pickPhysicalDevice();
    mVKDevice.createLogicalDevice();
    mVKDevice.createCommandPool();
}


void VulkanUtils::OnSurfaceCreated()
{
    createSwapchain();
    createImageViews();
    createRenderPass();
    createDescriptorSetLayout();
    createGraphicsPipeline();
    createFramebuffers();

    mTexImage.createTextureImage(assetManager);
    mTexImage.createTextureImageView();
    mTexImage.createTextureSampler();

    createVertexBuffer();
    createIndexBuffer();
    createUniformBuffer();
    createDescriptorPool();
    createDescriptorSet();
    createCommandBuffers();

    createSemaphores();
}

void VulkanUtils::OnSurfaceChanged()
{
    state = STATE_PAUSED;
    recreateSwapchain();
    state = STATE_RUNNING;
}

void VulkanUtils::OnDrawFrame()
{
    updateUniformBuffer();
    drawFrame();
}

void VulkanUtils::start()
{
    if (!InitVulkan()) {
        throw std::runtime_error("InitVulkan fail!");
    }
    return;
}

void VulkanUtils::cleanUp() {
    cleanupSwapchain();

    vkDestroyDescriptorPool(mVKDevice.logicalDevice, descriptorPool, nullptr);
    vkDestroyDescriptorSetLayout(mVKDevice.logicalDevice, descriptorSetLayout, nullptr);

    mUniformBuffer.destroy();
    mIndexBuffer.destroy();
    mVertexBuffer.destroy();

    vkDestroySemaphore(mVKDevice.logicalDevice, renderFinishedSemaphore, nullptr);
    vkDestroySemaphore(mVKDevice.logicalDevice, imageAvailableSemaphore, nullptr);
//    mVKDevice.destroy();

    ANativeWindow_release(window);
}


void VulkanUtils::createSwapchain()
{
    SwapchainSupportDetails swapchainSupport = mVKDevice.querySwapchainSupport(mVKDevice.physicalDevice);
    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapchainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapchainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapchainSupport.capabilities);

    uint32_t imageCount = swapchainSupport.capabilities.minImageCount + 1;
    if (swapchainSupport.capabilities.maxImageCount > 0
        && imageCount > swapchainSupport.capabilities.maxImageCount)
    {
        imageCount = swapchainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = mVKDevice.surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = mVKDevice.findQueueFamilies(mVKDevice.physicalDevice);
    if (indices.graphicsFamily != indices.presentFamily) {
        uint32_t queueFamilyIndices[] = {
                (uint32_t) indices.graphicsFamily, (uint32_t) indices.presentFamily
        };
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    createInfo.preTransform = swapchainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(mVKDevice.logicalDevice, &createInfo, nullptr, &swapchain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }

    vkGetSwapchainImagesKHR(mVKDevice.logicalDevice, swapchain, &imageCount, nullptr);
    swapchainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(mVKDevice.logicalDevice, swapchain, &imageCount, swapchainImages.data());

    swapchainImageFormat = surfaceFormat.format;
    swapchainExtent = extent;
}

void VulkanUtils::createImageViews()
{
    swapchainImageViews.resize(swapchainImages.size());
    for (size_t i = 0; i < swapchainImages.size(); i++)
    {
        VkImageViewCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = swapchainImages[i];

        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = swapchainImageFormat;

        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(mVKDevice.logicalDevice, &createInfo, nullptr,
                              &swapchainImageViews[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create image views!");
        }
    }
}

void VulkanUtils::createRenderPass()
{
    VkAttachmentDescription colorAttachment = {};
    colorAttachment.format = swapchainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef = {};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    if (vkCreateRenderPass(mVKDevice.logicalDevice, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create render pass!");
    }
}

void VulkanUtils::createDescriptorSetLayout() {
    VkDescriptorSetLayoutBinding uboLayoutBinding = {
            .binding = 0,
            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .descriptorCount = 1,
            .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
            .pImmutableSamplers = nullptr,
    };

    VkDescriptorSetLayoutBinding samplerLayoutBinding = {
            .binding = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .descriptorCount = 1,
            .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
            .pImmutableSamplers = nullptr,
    };

    std::array<VkDescriptorSetLayoutBinding,2> bindings = {uboLayoutBinding,samplerLayoutBinding};

    VkDescriptorSetLayoutCreateInfo layoutInfo = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .bindingCount = static_cast<uint32_t>(bindings.size()),
            .pBindings = bindings.data(),
    };

    if (vkCreateDescriptorSetLayout(mVKDevice.logicalDevice, &layoutInfo, nullptr, &descriptorSetLayout)
        != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}

//#define USE_SPV

void VulkanUtils::createGraphicsPipeline() {

#ifdef USE_SPV
    vertexShader = "shaders/triangle.vert.spv";
    fragmentShader = "shaders/triangle.frag.spv";
    auto vertexShaderCode = readAsset(vertexShader);
    auto fragmentShaderCode = readAsset(fragmentShader);
    VkShaderModule vertexShaderModule = createShaderModule(vertexShaderCode);
    VkShaderModule fragmentShaderModule = createShaderModule(fragmentShaderCode);
#else
    auto vertexShaderCode = readAsset("shaders/triangle.vert");
    auto fragmentShaderCode = readAsset("shaders/triangle.frag");
    std::vector<unsigned int> vtx_spv;
    std::vector<unsigned int> frag_spv;
    GLSLtoSPV(VK_SHADER_STAGE_VERTEX_BIT, vertexShaderCode.data(), vtx_spv);
    GLSLtoSPV(VK_SHADER_STAGE_FRAGMENT_BIT, fragmentShaderCode.data(), frag_spv);
    VkShaderModule vertexShaderModule = createShaderModule(vtx_spv);
    VkShaderModule fragmentShaderModule = createShaderModule(frag_spv);
#endif

    VkPipelineShaderStageCreateInfo vertexShaderStageInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage = VK_SHADER_STAGE_VERTEX_BIT,
            .module = vertexShaderModule,
            .pName = "main",
    };
    VkPipelineShaderStageCreateInfo fragmentShaderStageInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
            .module = fragmentShaderModule,
            .pName = "main",
    };
    VkPipelineShaderStageCreateInfo shaderStages[] = {
            vertexShaderStageInfo, fragmentShaderStageInfo
    };

    auto bindingDesc = Vertex::getBindingDescription();
    auto attrDesc =    Vertex::getAttributeDescriptions();
    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            .vertexBindingDescriptionCount = 1,
            .pVertexBindingDescriptions = &bindingDesc,
            .vertexAttributeDescriptionCount = static_cast<uint32_t>(attrDesc.size()),
            .pVertexAttributeDescriptions = attrDesc.data(),
    };

    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
            .primitiveRestartEnable = VK_FALSE,
    };

    VkViewport viewport = {
            .x = 0.0f,
            .y = 0.0f,
            .width = (float) swapchainExtent.width,
            .height = (float) swapchainExtent.height,
            .minDepth = 0.0f,
            .maxDepth = 1.0f,
    };

    VkRect2D scissor = {
            .offset = {0, 0},
            .extent = swapchainExtent,
    };

    VkPipelineViewportStateCreateInfo viewportState = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
            .viewportCount = 1,
            .pViewports = &viewport,
            .scissorCount = 1,
            .pScissors = &scissor,
    };

    VkPipelineRasterizationStateCreateInfo rasterizer = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            .depthClampEnable = VK_FALSE,
            .rasterizerDiscardEnable = VK_FALSE,
            .polygonMode = VK_POLYGON_MODE_FILL,
            .lineWidth = 1.0f,
            .cullMode = VK_CULL_MODE_BACK_BIT,
            .frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
            .depthBiasEnable = VK_FALSE,
    };

    VkPipelineMultisampleStateCreateInfo multisampling = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            .sampleShadingEnable = VK_FALSE,
            .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
    };

    VkPipelineColorBlendAttachmentState colorBlendAttachment = {
            .colorWriteMask = VK_COLOR_COMPONENT_R_BIT
                              | VK_COLOR_COMPONENT_G_BIT
                              | VK_COLOR_COMPONENT_B_BIT
                              | VK_COLOR_COMPONENT_A_BIT,
            .blendEnable = VK_FALSE,
    };
    VkPipelineColorBlendStateCreateInfo colorBlending = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            .logicOpEnable = VK_FALSE,
            .logicOp = VK_LOGIC_OP_COPY,
            .attachmentCount = 1,
            .pAttachments = &colorBlendAttachment,
            .blendConstants[0] = 0.0f,
            .blendConstants[1] = 0.0f,
            .blendConstants[2] = 0.0f,
            .blendConstants[3] = 0.0f,
    };

    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .setLayoutCount = 1,
            .pSetLayouts = &descriptorSetLayout,
    };
    if (vkCreatePipelineLayout(mVKDevice.logicalDevice, &pipelineLayoutInfo, nullptr, &pipelineLayout)
        != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }

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
            .basePipelineHandle = VK_NULL_HANDLE,
    };

    if (vkCreateGraphicsPipelines(mVKDevice.logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr,
                                  &graphicsPipeline) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline!");
    }

    vkDestroyShaderModule(mVKDevice.logicalDevice, vertexShaderModule, nullptr);
    vkDestroyShaderModule(mVKDevice.logicalDevice, fragmentShaderModule, nullptr);
}

void VulkanUtils::createFramebuffers() {
    swapchainFramebuffers.resize(swapchainImageViews.size());

    for (size_t i = 0; i < swapchainImageViews.size(); i++) {
        VkImageView attachments[] = {
                swapchainImageViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = swapchainExtent.width;
        framebufferInfo.height = swapchainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(mVKDevice.logicalDevice, &framebufferInfo, nullptr, &swapchainFramebuffers[i])
            != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
}

void VulkanUtils::createVertexBuffer() {
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    HVkBuffer stagBuffer(&mVKDevice);
    stagBuffer.createBuffer(bufferSize,
                 VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    stagBuffer.map(bufferSize, 0 );
    stagBuffer.copyTo((void *)vertices.data(), (size_t) bufferSize);
    stagBuffer.unmap();

    mVertexBuffer.createBuffer(bufferSize,
                 VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    mVertexBuffer.copyBuffer(stagBuffer);
    return;
}

void VulkanUtils::createIndexBuffer() {
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    HVkBuffer stagBuffer(&mVKDevice);

    stagBuffer.createBuffer(bufferSize,
                 VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    stagBuffer.map(bufferSize, 0);
    stagBuffer.copyTo( (void*)indices.data(), (size_t) bufferSize);
    stagBuffer.unmap();

    mIndexBuffer.createBuffer(bufferSize,
                 VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
//    copyBuffer(stagBuffer.mBuffer, mIndexBuffer.mBuffer, bufferSize);
    mIndexBuffer.copyBuffer(stagBuffer);


}

void VulkanUtils::createUniformBuffer() {
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);
    mUniformBuffer.createBuffer(bufferSize,
                 VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

void VulkanUtils::createDescriptorPool() {
    std::array<VkDescriptorPoolSize, 2> poolSizes = {};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = 1;
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = 1;

    VkDescriptorPoolCreateInfo poolInfo = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .poolSizeCount = static_cast<uint32_t>(poolSizes.size()),
            .pPoolSizes = poolSizes.data(),
            .maxSets = 1,
    };
    if (vkCreateDescriptorPool(mVKDevice.logicalDevice, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

void VulkanUtils::createDescriptorSet() {
    VkDescriptorSetLayout layouts[] = {descriptorSetLayout};
    VkDescriptorSetAllocateInfo allocInfo = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .descriptorPool = descriptorPool,
            .descriptorSetCount = 1,
            .pSetLayouts = layouts,
    };
    if (vkAllocateDescriptorSets(mVKDevice.logicalDevice, &allocInfo, &descriptorSet) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor set!");
    }

    VkDescriptorBufferInfo bufferInfo = {
            .buffer = mUniformBuffer.mBuffer,
            .offset = 0,
            .range = sizeof(UniformBufferObject),
    };

    VkDescriptorImageInfo imageInfo = {
            .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            .imageView = mTexImage.textureImageView,
            .sampler = mTexImage.textureSampler,
    };

    std::array<VkWriteDescriptorSet, 2> descriptorWrites = {};

    descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[0].dstSet = descriptorSet;
    descriptorWrites[0].dstBinding = 0;
    descriptorWrites[0].dstArrayElement = 0;
    descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrites[0].descriptorCount = 1;
    descriptorWrites[0].pBufferInfo = &bufferInfo;

    descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[1].dstSet = descriptorSet;
    descriptorWrites[1].dstBinding = 1;
    descriptorWrites[1].dstArrayElement = 0;
    descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrites[1].descriptorCount = 1;
    descriptorWrites[1].pImageInfo = &imageInfo;

    vkUpdateDescriptorSets(mVKDevice.logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
}

void VulkanUtils::createCommandBuffers() {
    commandBuffers.resize(swapchainFramebuffers.size());

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = mVKDevice.mCommandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

    if (vkAllocateCommandBuffers(mVKDevice.logicalDevice, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }

    for (size_t i = 0; i < commandBuffers.size(); i++) {
        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
        beginInfo.pInheritanceInfo = nullptr; // Optional

        vkBeginCommandBuffer(commandBuffers[i], &beginInfo);

        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass;
        renderPassInfo.framebuffer = swapchainFramebuffers[i];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swapchainExtent;

        VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

        VkBuffer vertexBuffers[] = {mVertexBuffer.mBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);

        vkCmdBindIndexBuffer(commandBuffers[i], mIndexBuffer.mBuffer, 0, VK_INDEX_TYPE_UINT16);

        vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout,
                                0, 1, &descriptorSet, 0, nullptr);

        vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

        vkCmdEndRenderPass(commandBuffers[i]);

        if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
    }
}

void VulkanUtils::createSemaphores() {
    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    if (vkCreateSemaphore(mVKDevice.logicalDevice, &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS
        || vkCreateSemaphore(mVKDevice.logicalDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphore)
           != VK_SUCCESS) {
        throw std::runtime_error("failed to create semaphores!");
    }
}

void VulkanUtils::updateUniformBuffer() {
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count() / 1e3f;

    UniformBufferObject ubo = {
            .model = glm::rotate(glm::mat4(), time * glm::radians(90.0f),
                                 glm::vec3(0.0f, 0.0f, 1.0f)),
            .view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f),
                                glm::vec3(0.0f, 0.0f, 0.0f),
                                glm::vec3(0.0f, 0.0f, 1.0f)),
            .proj = glm::perspective(glm::radians(45.0f),
                                     swapchainExtent.width / (float) swapchainExtent.height, 0.1f, 10.0f),
    };
    ubo.proj[1][1] *= -1;

    mUniformBuffer.map(sizeof(ubo), 0);
    mUniformBuffer.copyTo(&ubo, sizeof(ubo));
    mUniformBuffer.unmap();
    return;
}

void VulkanUtils::drawFrame() {
    uint32_t imageIndex = 0;
    VkResult result = vkAcquireNextImageKHR(mVKDevice.logicalDevice, swapchain, std::numeric_limits<uint64_t>::max(),
                                            imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapchain();
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {imageAvailableSemaphore};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

    VkSemaphore signalSemaphores[] = {renderFinishedSemaphore};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(mVKDevice.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapchain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    result = vkQueuePresentKHR(mVKDevice.presentQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        recreateSwapchain();
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    vkQueueWaitIdle(mVKDevice.presentQueue);
}

void VulkanUtils::recreateSwapchain() {
    LOGI("recreateSwapchain");

    vkDeviceWaitIdle(mVKDevice.logicalDevice);

    cleanupSwapchain();

    createSwapchain();
    createImageViews();
    createRenderPass();
    createGraphicsPipeline();
    createFramebuffers();
    createCommandBuffers();
}

void VulkanUtils::cleanupSwapchain() {
    for (size_t i = 0; i < swapchainFramebuffers.size(); i++) {
        vkDestroyFramebuffer(mVKDevice.logicalDevice, swapchainFramebuffers[i], nullptr);
    }

    vkFreeCommandBuffers(mVKDevice.logicalDevice, mVKDevice.mCommandPool, static_cast<uint32_t>(commandBuffers.size()),
                         commandBuffers.data());

    vkDestroyPipeline(mVKDevice.logicalDevice, graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(mVKDevice.logicalDevice, pipelineLayout, nullptr);
    vkDestroyRenderPass(mVKDevice.logicalDevice, renderPass, nullptr);

    for (size_t i = 0; i < swapchainImageViews.size(); i++) {
        vkDestroyImageView(mVKDevice.logicalDevice, swapchainImageViews[i], nullptr);
    }

    vkDestroySwapchainKHR(mVKDevice.logicalDevice, swapchain, nullptr);
}

std::vector<char> VulkanUtils::readAsset(std::string name) {
    AAsset *file = AAssetManager_open(assetManager, name.c_str(), AASSET_MODE_BUFFER);
    size_t len = AAsset_getLength(file);
    std::vector<char> buffer(len);

    AAsset_read(file, buffer.data(), len);

    AAsset_close(file);

    LOGI("read asset %s, length %d", name.c_str(), (int)len);

    return buffer;
}

VkShaderModule VulkanUtils::createShaderModule(const std::vector<char> &code) {
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(mVKDevice.logicalDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }

    return shaderModule;
}


VkShaderModule VulkanUtils::createShaderModule(const std::vector<uint32_t> &code) {
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size() * sizeof(uint32_t);
    createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(mVKDevice.logicalDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }

    return shaderModule;
}