#include "vulkan.hpp"

#include <array>
#include <iostream>

#include <imgui/backends/imgui_impl_sdl2.h>
#include <imgui/backends/imgui_impl_vulkan.h>

#include <sdl2/SDL_vulkan.h>

#include "utils.hpp"
#include "widgets/fps_counter.hpp"
#include "windows/sdl2.hpp"

using namespace GraphicLibraries::Engines;
using namespace GraphicLibraries::Widgets;
using namespace GraphicLibraries::Windows;

Vulkan::Vulkan()
    : m_window{ nullptr },
      m_fpsCounter{ nullptr },
      m_graphicsQueueIndex{ 0u },
      m_swapchainExtent { 0, 0 }
{
    m_isInit = false;
}

Vulkan::~Vulkan()
{
    std::cout << "Vulkan Destructor" << std::endl;

    if (m_isInit)
        release();
}

void Vulkan::init()
{
    std::cout << "Initializing Vulkan" << std::endl;

    m_window = new SDL2Window;

    if (!m_window)
        throw std::exception("VULKAN: Can't create sdl2 window");

    m_window->initForVulkan();
    m_window->setTitle("Vulkan");

    throwIfFailed(createInstance(), "Can't create instance");

    if (!SDL_Vulkan_CreateSurface(m_window->getWindow(), m_instance, &m_surface))
        throw std::exception("VULKAN: Can't create surface");

    throwIfFailed(createPhysicalDevice(), "Can't create physical device");

    throwIfFailed(createDevice(), "Can't create device");

    vkGetDeviceQueue(m_device, m_graphicsQueueIndex, 0, &m_queue);

    if (m_queue == VK_NULL_HANDLE)
        throw std::exception("VULKAN: Can't get device queue");

    throwIfFailed(createSwapchain(), "Can't create swap chain");
    throwIfFailed(createRenderPass(), "Can't create render pass");
    throwIfFailed(createPipeline(), "Can't create pipeline");
    throwIfFailed(createBuffer(), "Can't create buffer");
    throwIfFailed(createCommandPool(), "Can't create command pool");
    throwIfFailed(createVertexBuffer(), "Can't create vertex buffer");
    throwIfFailed(createCommandBuffers(), "Can't create command buffers");
    throwIfFailed(createSemaphore(), "Can't create semaphore");
    throwIfFailed(createFence(), "Can't create fence");
    throwIfFailed(initImgui(), "Can't init Imgui");

    m_fpsCounter = new FpsCounter;

    if (!m_fpsCounter)
        throw std::exception("OPENGL: Can't initialize FPS Counter widget");

    m_isInit = true;
}

void Vulkan::release()
{
    std::cout << "Releasing Vulkan" << std::endl;

    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    if (m_imguiDescriptorPool != VK_NULL_HANDLE)
        vkDestroyDescriptorPool(m_device, m_imguiDescriptorPool, nullptr);

    if (m_imgAvailSemaphores.size() > 0)
    {
        for (auto& sem : m_imgAvailSemaphores)
        {
            if (sem != VK_NULL_HANDLE)
                vkDestroySemaphore(m_device, sem, nullptr);
        }
    }

    if (m_renderFinishedSemaphores.size() > 0)
    {
        for (auto& sem : m_renderFinishedSemaphores)
        {
            if (sem != VK_NULL_HANDLE)
                vkDestroySemaphore(m_device, sem, nullptr);
        }
    }

    if (m_fences.size() > 0)
    {
        for (auto& fence : m_fences)
        {
            if (fence != VK_NULL_HANDLE)
                vkDestroyFence(m_device, fence, nullptr);
        }
    }

    vkFreeCommandBuffers(m_device, m_commandPool, static_cast<uint32_t>(m_commandBuffers.size()),
        m_commandBuffers.data());

    if (m_commandPool != VK_NULL_HANDLE)
        vkDestroyCommandPool(m_device, m_commandPool, nullptr);

    if (m_vertexBuffer != VK_NULL_HANDLE)
        vkDestroyBuffer(m_device, m_vertexBuffer, nullptr);

    if (m_vertexBufferMemory != VK_NULL_HANDLE)
        vkFreeMemory(m_device, m_vertexBufferMemory, nullptr);

    if (m_frameBuffers.size() > 0)
    {
        for (auto& fb : m_frameBuffers)
        {
            if (fb != VK_NULL_HANDLE)
                vkDestroyFramebuffer(m_device, fb, nullptr);
        }
    }

    if (m_pipeline != VK_NULL_HANDLE)
        vkDestroyPipeline(m_device, m_pipeline, nullptr);

    if (m_renderPass != VK_NULL_HANDLE)
        vkDestroyRenderPass(m_device, m_renderPass, nullptr);

    if (m_pipelineLayout != VK_NULL_HANDLE)
        vkDestroyPipelineLayout(m_device, m_pipelineLayout, nullptr);

    if (m_swapchainImageViews.size() > 0)
    {
        for (auto& v : m_swapchainImageViews)
        {
            if (v != VK_NULL_HANDLE)
                vkDestroyImageView(m_device, v, nullptr);
        }
    }

    if (m_swapchain != VK_NULL_HANDLE)
        vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);

    if (m_device != VK_NULL_HANDLE)
        vkDestroyDevice(m_device, nullptr);

    if (m_surface != VK_NULL_HANDLE)
        vkDestroySurfaceKHR(m_instance, m_surface, nullptr);

    if (m_instance != VK_NULL_HANDLE)
        vkDestroyInstance(m_instance, nullptr);

    if (m_fpsCounter)
    {
        delete m_fpsCounter;
        m_fpsCounter = nullptr;
    }

    if (m_window)
    {
        m_window->release();
        delete m_window;
        m_window = nullptr;
    }

    m_isInit = false;
}

void Vulkan::newFrame(float dt)
{
    //imgui new frame
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplSDL2_NewFrame(m_window->getWindow());

    ImGui::NewFrame();
    m_fpsCounter->draw();

    ImGui::Render();

    ImDrawData* drawData = ImGui::GetDrawData();
    if (drawData->DisplaySize.x <= 0.0f || drawData->DisplaySize.y <= 0.0f)
        return;

    static uint32_t currentFrame = 0;
    uint32_t imgIndex = 0;

    throwIfFailed(vkWaitForFences(m_device, 1, &m_fences[currentFrame], true, (std::numeric_limits<uint64_t>::max)()),
        "Can't wait for fence");

    throwIfFailed(vkAcquireNextImageKHR(m_device, m_swapchain, (std::numeric_limits<uint64_t>::max)(),
        m_imgAvailSemaphores[currentFrame], VK_NULL_HANDLE, &imgIndex), "Can't  aquire next image");

    const std::vector<VkSemaphore> waitSemaphore = { m_imgAvailSemaphores[currentFrame] };
    const std::vector<VkSemaphore> signalSemaphore = { m_renderFinishedSemaphores[currentFrame] };
    const std::vector<VkPipelineStageFlags> waitStages = { VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT };

    throwIfFailed(vkResetFences(m_device, 1, &m_fences[currentFrame]), "Can't reset fence");

    throwIfFailed(vkResetCommandBuffer(m_commandBuffers[currentFrame], 0),
        "Can't reset command buffer");

    throwIfFailed(recordCommandBuffer(m_commandBuffers[currentFrame], imgIndex),
        "Can't record command buffer");

    std::vector<VkCommandBuffer> cmdBuffers = { m_commandBuffers[imgIndex] };

    VkSubmitInfo submitInfo;
    ZeroMemory(&submitInfo, sizeof(submitInfo));

    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphore.size());
    submitInfo.pWaitSemaphores = waitSemaphore.data();
    submitInfo.pWaitDstStageMask = waitStages.data();
    submitInfo.commandBufferCount = static_cast<uint32_t>(cmdBuffers.size());
    submitInfo.pCommandBuffers = cmdBuffers.data();
    submitInfo.signalSemaphoreCount = static_cast<uint32_t>(signalSemaphore.size());
    submitInfo.pSignalSemaphores = signalSemaphore.data();

    throwIfFailed(vkQueueSubmit(m_queue, 1, &submitInfo, m_fences[currentFrame]), "Can't submit queue");

    std::array<VkSwapchainKHR, 1> presentChain = { m_swapchain };
    VkPresentInfoKHR presentInfo;
    ZeroMemory(&presentInfo, sizeof(presentInfo));

    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = static_cast<uint32_t>(signalSemaphore.size());
    presentInfo.pWaitSemaphores = signalSemaphore.data();
    presentInfo.swapchainCount = static_cast<uint32_t>(presentChain.size());
    presentInfo.pSwapchains = presentChain.data();
    presentInfo.pImageIndices = &imgIndex;
    throwIfFailed(vkQueuePresentKHR(m_queue, &presentInfo), "Can't present queue");

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

VkResult Vulkan::createInstance()
{
    const std::vector<const char*> validationLayers = { };

    unsigned int extensionCount = 0;
    SDL_Vulkan_GetInstanceExtensions(m_window->getWindow(), &extensionCount, nullptr);
    std::vector<const char*> extensionNames(extensionCount);
    SDL_Vulkan_GetInstanceExtensions(m_window->getWindow(), &extensionCount, extensionNames.data());

    VkApplicationInfo appInfo;
    ZeroMemory(&appInfo, sizeof(appInfo));

    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "GraphicLibraries";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Vulkan";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    VkInstanceCreateInfo instanceInfo;
    ZeroMemory(&instanceInfo, sizeof(instanceInfo));

    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pApplicationInfo = &appInfo;
    instanceInfo.enabledExtensionCount = static_cast<uint32_t>(extensionNames.size());
    instanceInfo.ppEnabledExtensionNames = extensionNames.data();
    instanceInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    instanceInfo.ppEnabledLayerNames = validationLayers.data();

    return vkCreateInstance(&instanceInfo, nullptr, &m_instance);
}

VkResult Vulkan::createPhysicalDevice()
{
    uint32_t deviceCount = 0;

    VkResult result = vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);

    if (result != VK_SUCCESS)
        return result;

    std::vector<VkPhysicalDevice> devices(deviceCount, VkPhysicalDevice{});
    result = vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

    if (result != VK_SUCCESS)
        return result;

    const bool hasDiscreteGpu = std::find_if(devices.begin(), devices.end(),
        [](const VkPhysicalDevice& d)
        {
            VkPhysicalDeviceProperties properties;
            vkGetPhysicalDeviceProperties(d, &properties);
            return properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
        }) != devices.end();


    for (const auto& device : devices)
    {
        VkPhysicalDeviceProperties properties;
        VkPhysicalDeviceFeatures features;
        vkGetPhysicalDeviceProperties(device, &properties);
        vkGetPhysicalDeviceFeatures(device, &features);

        if (hasDiscreteGpu && properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            m_physicalDevice = device;
            break;
        }
        else if (!hasDiscreteGpu && properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
        {
            m_physicalDevice = device;
            break;
        }
    }

        return result;
}

VkResult Vulkan::createDevice()
{
    uint32_t numQueueFamilies = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &numQueueFamilies, nullptr);
    std::vector<VkQueueFamilyProperties> familyProps(numQueueFamilies, VkQueueFamilyProperties{});
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &numQueueFamilies, familyProps.data());

    for (uint32_t i = 0; i < numQueueFamilies; ++i)
    {
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice, i, m_surface, &presentSupport);
        if (presentSupport && (familyProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT))
            m_graphicsQueueIndex = i;
    }

    const float queuePriority = 1.0f;

    VkDeviceQueueCreateInfo deviceQueueInfo;
    ZeroMemory(&deviceQueueInfo, sizeof(deviceQueueInfo));

    deviceQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    deviceQueueInfo.queueFamilyIndex = m_graphicsQueueIndex;
    deviceQueueInfo.queueCount = 1;
    deviceQueueInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures;
    ZeroMemory(&deviceFeatures, sizeof(deviceFeatures));

    const std::array<const char*, 1> deviceExtensions =
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    const std::array<const char*, 1> validationLayers =
    {
        "VK_LAYER_KHRONOS_validation"
    };

    VkDeviceCreateInfo deviceInfo;
    ZeroMemory(&deviceInfo, sizeof(deviceInfo));

    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.queueCreateInfoCount = 1;
    deviceInfo.pQueueCreateInfos = &deviceQueueInfo;
    deviceInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    deviceInfo.ppEnabledLayerNames = validationLayers.data();
    deviceInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    deviceInfo.ppEnabledExtensionNames = deviceExtensions.data();
    deviceInfo.pEnabledFeatures = &deviceFeatures;

    return vkCreateDevice(m_physicalDevice, &deviceInfo, nullptr, &m_device);
}

VkResult Vulkan::createSwapchain()
{
    ZeroMemory(&m_swapchainExtent, sizeof(m_swapchainExtent));

    m_swapchainExtent.width = m_window->getWidth();
    m_swapchainExtent.height = m_window->getHeight();

    VkSwapchainCreateInfoKHR swapchainInfo;
    ZeroMemory(&swapchainInfo, sizeof(swapchainInfo));
    swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainInfo.surface = m_surface;
    swapchainInfo.minImageCount = 2;
    swapchainInfo.imageFormat = m_swapchainImgFormat;
    swapchainInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    swapchainInfo.imageExtent = m_swapchainExtent;
    swapchainInfo.imageArrayLayers = 1;
    swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchainInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;
    swapchainInfo.clipped = true;
    swapchainInfo.oldSwapchain = VK_NULL_HANDLE;

    VkResult result = vkCreateSwapchainKHR(m_device, &swapchainInfo, nullptr, &m_swapchain);

    if (result != VK_SUCCESS)
        return result;

    uint32_t numSwapChainImgs = 0;
    vkGetSwapchainImagesKHR(m_device, m_swapchain, &numSwapChainImgs, nullptr);
    m_swapchainImageViews.resize(numSwapChainImgs);

    std::vector<VkImage> swapchainImages(numSwapChainImgs);
    vkGetSwapchainImagesKHR(m_device, m_swapchain, &numSwapChainImgs, swapchainImages.data());

    for (uint32_t i = 0; i < numSwapChainImgs; ++i)
    {
        VkImageViewCreateInfo viewCreateInfo;
        ZeroMemory(&viewCreateInfo, sizeof(viewCreateInfo));

        viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewCreateInfo.image = swapchainImages[i];
        viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewCreateInfo.format = m_swapchainImgFormat;

        viewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        viewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewCreateInfo.subresourceRange.baseMipLevel = 0;
        viewCreateInfo.subresourceRange.levelCount = 1;
        viewCreateInfo.subresourceRange.baseArrayLayer = 0;
        viewCreateInfo.subresourceRange.layerCount = 1;

        result = vkCreateImageView(m_device, &viewCreateInfo, nullptr, &m_swapchainImageViews[i]);

        if (result != VK_SUCCESS)
            return result;
    }

    return result;
}

VkResult Vulkan::createRenderPass()
{
    VkAttachmentDescription colorAttachment;
    ZeroMemory(&colorAttachment, sizeof(colorAttachment));

    colorAttachment.format = m_swapchainImgFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef;
    ZeroMemory(&colorAttachmentRef, sizeof(colorAttachmentRef));

    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass;
    ZeroMemory(&subpass, sizeof(subpass));

    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkSubpassDependency dependency;
    ZeroMemory(&dependency, sizeof(dependency));

    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassInfo;
    ZeroMemory(&renderPassInfo, sizeof(renderPassInfo));

    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    return vkCreateRenderPass(m_device, &renderPassInfo, nullptr, &m_renderPass);
}

VkResult Vulkan::createPipeline()
{
    VkShaderModule vertexShaderModule = VK_NULL_HANDLE;

    std::vector<char> vertex = readFile(L"vulkan/shaders/vertex.spv");

    VkShaderModuleCreateInfo shadeModuleInfo;
    ZeroMemory(&shadeModuleInfo, sizeof(shadeModuleInfo));

    shadeModuleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shadeModuleInfo.codeSize = vertex.size();
    shadeModuleInfo.pCode = reinterpret_cast<const uint32_t*>(vertex.data());
    VkResult result = vkCreateShaderModule(m_device, &shadeModuleInfo, nullptr, &vertexShaderModule);

    if (result != VK_SUCCESS)
        return result;

    VkPipelineShaderStageCreateInfo vertexStageInfo;
    ZeroMemory(&vertexStageInfo, sizeof(vertexStageInfo));

    vertexStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertexStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertexStageInfo.module = vertexShaderModule;
    vertexStageInfo.pName = "main";

    std::vector<char> fragment = readFile(L"vulkan/shaders/fragment.spv");

    VkShaderModule fragmentShaderModule = VK_NULL_HANDLE;
    ZeroMemory(&shadeModuleInfo, sizeof(shadeModuleInfo));

    shadeModuleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shadeModuleInfo.codeSize = fragment.size();
    shadeModuleInfo.pCode = reinterpret_cast<const uint32_t*>(fragment.data());
    result = vkCreateShaderModule(m_device, &shadeModuleInfo, nullptr, &fragmentShaderModule);

    VkPipelineShaderStageCreateInfo fragmentStageInfo;
    ZeroMemory(&fragmentStageInfo, sizeof(fragmentStageInfo));
    fragmentStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragmentStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragmentStageInfo.module = fragmentShaderModule;
    fragmentStageInfo.pName = "main";

    std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages =
    {
        vertexStageInfo,
        fragmentStageInfo
    };

    VkVertexInputBindingDescription bindingDescription;
    ZeroMemory(&bindingDescription, sizeof(bindingDescription));

    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(Vertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions;
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, position);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, color);

    // Vertex data hardcoded in vertex shader
    VkPipelineVertexInputStateCreateInfo vertexInputInfo;
    ZeroMemory(&vertexInputInfo, sizeof(vertexInputInfo));

    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    // Primitive type
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    ZeroMemory(&inputAssemblyInfo, sizeof(inputAssemblyInfo));

    inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

    VkPipelineViewportStateCreateInfo viewportStateInfo;
    ZeroMemory(&viewportStateInfo, sizeof(viewportStateInfo));

    viewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportStateInfo.viewportCount = 1;
    viewportStateInfo.scissorCount = 1;

    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    ZeroMemory(&rasterizationInfo, sizeof(rasterizationInfo));

    rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizationInfo.depthClampEnable = VK_FALSE;
    rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
    rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizationInfo.lineWidth = 1.0f;
    rasterizationInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizationInfo.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisamplingInfo;
    ZeroMemory(&multisamplingInfo, sizeof(multisamplingInfo));

    multisamplingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisamplingInfo.sampleShadingEnable = VK_FALSE;
    multisamplingInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState blendMode;
    ZeroMemory(&blendMode, sizeof(blendMode));

    blendMode.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    blendMode.blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo blendInfo;
    ZeroMemory(&blendInfo, sizeof(blendInfo));

    blendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    blendInfo.logicOpEnable = VK_FALSE;
    blendInfo.logicOp = VK_LOGIC_OP_COPY;
    blendInfo.attachmentCount = 1;
    blendInfo.pAttachments = &blendMode;
    blendInfo.blendConstants[0] = 0.0f;
    blendInfo.blendConstants[1] = 0.0f;
    blendInfo.blendConstants[2] = 0.0f;
    blendInfo.blendConstants[3] = 0.0f;

    std::vector<VkDynamicState> dynamicStates =
    {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamicStateInfo;
    ZeroMemory(&dynamicStateInfo, sizeof(dynamicStateInfo));

    dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicStateInfo.pDynamicStates = dynamicStates.data();

    VkPipelineLayoutCreateInfo pipelineInfo;
    ZeroMemory(&pipelineInfo, sizeof(pipelineInfo));

    pipelineInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineInfo.setLayoutCount = 0;
    pipelineInfo.pushConstantRangeCount = 0;

    result = vkCreatePipelineLayout(m_device, &pipelineInfo, nullptr, &m_pipelineLayout);

    if (result != VK_SUCCESS)
        return result;

    VkGraphicsPipelineCreateInfo graphicsPipelineInfo;
    ZeroMemory(&graphicsPipelineInfo, sizeof(graphicsPipelineInfo));

    graphicsPipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    graphicsPipelineInfo.stageCount = static_cast<uint32_t>(attributeDescriptions.size());
    graphicsPipelineInfo.pStages = shaderStages.data();
    graphicsPipelineInfo.pVertexInputState = &vertexInputInfo;
    graphicsPipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
    graphicsPipelineInfo.pViewportState = &viewportStateInfo;
    graphicsPipelineInfo.pRasterizationState = &rasterizationInfo;
    graphicsPipelineInfo.pMultisampleState = &multisamplingInfo;
    graphicsPipelineInfo.pColorBlendState = &blendInfo;
    graphicsPipelineInfo.pDynamicState = &dynamicStateInfo;
    graphicsPipelineInfo.layout = m_pipelineLayout;
    graphicsPipelineInfo.renderPass = m_renderPass;
    graphicsPipelineInfo.subpass = 0;
    graphicsPipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    result = vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &graphicsPipelineInfo, nullptr, &m_pipeline);

    if (result != VK_SUCCESS)
        return result;

    vkDestroyShaderModule(m_device, vertexShaderModule, nullptr);
    vkDestroyShaderModule(m_device, fragmentShaderModule, nullptr);

    return result;
}

VkResult Vulkan::createBuffer()
{
    VkResult result = VK_SUCCESS;

    m_frameBuffers.resize(m_swapchainImageViews.size());

    for (size_t i = 0; i < m_swapchainImageViews.size(); ++i)
    {
        std::vector<VkImageView> attachments = { m_swapchainImageViews[i] };

        VkFramebufferCreateInfo framebufferInfo;
        ZeroMemory(&framebufferInfo, sizeof(framebufferInfo));

        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = m_renderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = m_swapchainExtent.width;
        framebufferInfo.height = m_swapchainExtent.height;
        framebufferInfo.layers = 1;

        result = vkCreateFramebuffer(m_device, &framebufferInfo, nullptr, &m_frameBuffers[i]);

        if (result != VK_SUCCESS)
            return result;
    }

    return result;
}

VkResult Vulkan::createCommandPool()
{
    VkCommandPoolCreateInfo commandPoolInfo;
    ZeroMemory(&commandPoolInfo, sizeof(commandPoolInfo));

    commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    commandPoolInfo.queueFamilyIndex = m_graphicsQueueIndex;

    return vkCreateCommandPool(m_device, &commandPoolInfo, nullptr, &m_commandPool);
}

VkResult Vulkan::createVertexBuffer()
{
    Vertex triangle[] =
    {
        { {  0.0f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
        { {  0.5f,  0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
        { { -0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
    };

    VkBufferCreateInfo bufferInfo;
    ZeroMemory(&bufferInfo, sizeof(bufferInfo));

    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = sizeof(triangle);
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkResult result = vkCreateBuffer(m_device, &bufferInfo, nullptr, &m_vertexBuffer);

    if (result != VK_SUCCESS)
        return result;

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(m_device, m_vertexBuffer, &memRequirements);

    VkMemoryAllocateInfo allocateInfo;
    ZeroMemory(&allocateInfo, sizeof(allocateInfo));

    allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocateInfo.allocationSize = memRequirements.size;
    allocateInfo.memoryTypeIndex = findMemoryType(m_physicalDevice,
                                                  memRequirements.memoryTypeBits,
                                                  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                                  VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    result = vkAllocateMemory(m_device, &allocateInfo, nullptr, &m_vertexBufferMemory);

    if (result != VK_SUCCESS)
        return result;

    vkBindBufferMemory(m_device, m_vertexBuffer, m_vertexBufferMemory, 0);

    void* data;
    vkMapMemory(m_device, m_vertexBufferMemory, 0, bufferInfo.size, 0, &data);
    memcpy(data, triangle, sizeof(triangle));
    vkUnmapMemory(m_device, m_vertexBufferMemory);

    return result;
}

VkResult Vulkan::createCommandBuffers()
{
    m_commandBuffers = std::vector<VkCommandBuffer>(MAX_FRAMES_IN_FLIGHT, VkCommandBuffer{});
    VkCommandBufferAllocateInfo commandBufferAllocateInfo;
    ZeroMemory(&commandBufferAllocateInfo, sizeof(commandBufferAllocateInfo));

    commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocateInfo.commandPool = m_commandPool;
    commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferAllocateInfo.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size());

    return vkAllocateCommandBuffers(m_device, &commandBufferAllocateInfo, m_commandBuffers.data());
}

VkResult Vulkan::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
{
    VkCommandBuffer& cmdBuf = m_commandBuffers[imageIndex];

    VkCommandBufferBeginInfo beginInfo;
    ZeroMemory(&beginInfo, sizeof(beginInfo));

    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    VkResult result = vkBeginCommandBuffer(cmdBuf, &beginInfo);

    if (result != VK_SUCCESS)
        return result;

    VkRenderPassBeginInfo renderPassInfo;
    ZeroMemory(&renderPassInfo, sizeof(renderPassInfo));

    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = m_renderPass;
    renderPassInfo.framebuffer = m_frameBuffers[imageIndex];
    renderPassInfo.renderArea.offset.x = 0;
    renderPassInfo.renderArea.offset.y = 0;
    renderPassInfo.renderArea.extent = m_swapchainExtent;

    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &m_clearColor;

    vkCmdBeginRenderPass(cmdBuf, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    // Grab and record the draw data for Dear Imgui
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), m_commandBuffers[imageIndex]);

    vkCmdBindPipeline(cmdBuf, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);

    // Viewport config
    VkViewport viewport;
    ZeroMemory(&viewport, sizeof(viewport));

    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(m_swapchainExtent.width);
    viewport.height = static_cast<float>(m_swapchainExtent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    // Scissor rect config
    VkRect2D scissor;
    ZeroMemory(&scissor, sizeof(scissor));

    scissor.offset.x = 0;
    scissor.offset.y = 0;
    scissor.extent = m_swapchainExtent;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    VkBuffer vertexBuffers[] = { m_vertexBuffer };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(cmdBuf, 0, 1, vertexBuffers, offsets);

    vkCmdDraw(cmdBuf, 3, 1, 0, 0);

    vkCmdEndRenderPass(cmdBuf);

    return vkEndCommandBuffer(cmdBuf);
}

VkResult Vulkan::createSemaphore()
{
    VkResult result = VK_SUCCESS;

    m_imgAvailSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    m_renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo symaphoreInfo;
    ZeroMemory(&symaphoreInfo, sizeof(symaphoreInfo));

    symaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
    {
        result = vkCreateSemaphore(m_device, &symaphoreInfo, nullptr, &m_imgAvailSemaphores[i]);

        if (result != VK_SUCCESS)
            return result;

        result = vkCreateSemaphore(m_device, &symaphoreInfo, nullptr, &m_renderFinishedSemaphores[i]);

        if (result != VK_SUCCESS)
            return result;
    }

    return result;
}

VkResult Vulkan::createFence()
{
    VkResult result = VK_SUCCESS;

    m_fences.resize(MAX_FRAMES_IN_FLIGHT);
    VkFenceCreateInfo fenceInfo;
    ZeroMemory(&fenceInfo, sizeof(fenceInfo));

    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
    {
        result = vkCreateFence(m_device, &fenceInfo, nullptr, &m_fences[i]);

        if (result != VK_SUCCESS)
            return result;
    }

    return result;
}

std::vector<char> Vulkan::readFile(const std::wstring& filename)
{
    std::ifstream file(getFileFullPath(filename.c_str()), std::ios::ate | std::ios::binary);

    if (!file.is_open())
        throw std::exception(std::string("VULKAN: ").append("Can't open file").c_str());

    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();
    return buffer;
}

uint32_t Vulkan::findMemoryType(VkPhysicalDevice& physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i)
    {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
            return i;
    }

    throw std::exception("Can't find suitable memory type!");
}

VkResult Vulkan::createImguiDescriptorPool()
{
    std::vector<VkDescriptorPoolSize> poolSizes =
    {
        { VK_DESCRIPTOR_TYPE_SAMPLER,                1000 },
        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
        { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,          1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,          1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,   1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,   1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,         1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
        { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,       1000 }
    };

    VkDescriptorPoolCreateInfo poolInfo;
    ZeroMemory(&poolInfo, sizeof(poolInfo));

    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    poolInfo.maxSets = static_cast<uint32_t>(1000 * poolSizes.size());
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();

    return vkCreateDescriptorPool(m_device, &poolInfo, nullptr, &m_imguiDescriptorPool);
}

VkResult Vulkan::initImgui()
{
    VkResult result = VK_SUCCESS;

    // Initialize imgui library
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    // Initialize some DearImgui specific resources
    result = createImguiDescriptorPool();

    if (result != VK_SUCCESS)
        return result;

    // Provide bind points from Vulkan API
    if (!ImGui_ImplSDL2_InitForVulkan(m_window->getWindow()))
        return VK_ERROR_UNKNOWN;

    ImGui_ImplVulkan_InitInfo initInfo;
    ZeroMemory(&initInfo, sizeof(initInfo));

    initInfo.Instance = m_instance;
    initInfo.PhysicalDevice = m_physicalDevice;
    initInfo.Device = m_device;
    initInfo.QueueFamily = m_graphicsQueueIndex;
    initInfo.Queue = m_queue;
    initInfo.DescriptorPool = m_imguiDescriptorPool;
    initInfo.MinImageCount = MAX_FRAMES_IN_FLIGHT;
    initInfo.ImageCount = MAX_FRAMES_IN_FLIGHT;

    if (!ImGui_ImplVulkan_Init(&initInfo, m_renderPass))
        return VK_ERROR_UNKNOWN;

    // Upload the fonts for DearImgui
    VkCommandBufferAllocateInfo allocInfo;
    ZeroMemory(&allocInfo, sizeof(allocInfo));

    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = m_commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    ZeroMemory(&commandBuffer, sizeof(commandBuffer));

    result = vkAllocateCommandBuffers(m_device, &allocInfo, &commandBuffer);

    if (result != VK_SUCCESS)
        return result;

    VkCommandBufferBeginInfo beginInfo;
    ZeroMemory(&beginInfo, sizeof(beginInfo));

    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    result = vkBeginCommandBuffer(commandBuffer, &beginInfo);

    if (result != VK_SUCCESS)
        return result;

    if (!ImGui_ImplVulkan_CreateFontsTexture(commandBuffer))
        return VK_ERROR_UNKNOWN;

    result = vkEndCommandBuffer(commandBuffer);

    if (result != VK_SUCCESS)
        return result;

    VkSubmitInfo submitInfo;
    ZeroMemory(&submitInfo, sizeof(submitInfo));

    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    result = vkQueueSubmit(m_queue, 1, &submitInfo, VK_NULL_HANDLE);

    if (result != VK_SUCCESS)
        return result;

    result = vkQueueWaitIdle(m_queue);

    if (result != VK_SUCCESS)
        return result;

    vkFreeCommandBuffers(m_device, m_commandPool, 1, &commandBuffer);

    ImGui_ImplVulkan_DestroyFontUploadObjects();

    return VK_SUCCESS;
}
