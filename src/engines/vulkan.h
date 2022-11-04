#pragma once
#ifndef GRAPHICLIBRARIES_ENGINES_VULKAN_H_
#define GRAPHICLIBRARIES_ENGINES_VULKAN_H_

#include <windows.h>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h>

#include <array>
#include <iostream>
#include <limits>
#include <vector>

#include "utils.h"
#include "ui/window_sdl2.h"

namespace GraphicLibraries
{
namespace Engines
{
    class Vulkan
    {
    public:
        Vulkan();
        ~Vulkan();

        Vulkan(const Vulkan& other)            = delete;
        Vulkan(Vulkan&& other)                 = delete;
        Vulkan& operator=(const Vulkan& other) = delete;
        Vulkan& operator=(Vulkan&& other)      = delete;

        void render();

    private:
        VkResult createInstance();
        VkResult createSurface();
        VkResult createPhysicalDevice(VkPhysicalDevice& physicalDevice);
        VkResult createQueue(VkPhysicalDevice& physicalDevice, uint32_t& graphicsQueueIndex);
        VkResult createSwapchain();
        VkResult createRenderPass();
        VkResult createPipeline();
        VkResult createBuffer();
        VkResult createCommandPool(uint32_t& graphicsQueueIndex);
        VkResult createVertexBuffer(VkPhysicalDevice& physicalDevice);
        VkResult createCommandBuffers();
        VkResult recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
        VkResult createSemaphore();
        VkResult createFence();

        inline void throwIfFailed(VkResult rs, const char* message)
        {
            if (rs != VK_SUCCESS)
                throw std::exception(std::string("VULKAN: ").append(message).c_str());
        }

        std::vector<char> readFile(const std::wstring& filename);
        uint32_t findMemoryType(VkPhysicalDevice& physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);

        VkInstance m_instance                 = VK_NULL_HANDLE;
        VkSurfaceKHR m_surface                = VK_NULL_HANDLE;
        VkDevice m_device                     = VK_NULL_HANDLE;
        VkQueue m_queue                       = VK_NULL_HANDLE;
        VkSwapchainKHR m_swapchain            = VK_NULL_HANDLE;
        VkBuffer m_vertexBuffer               = VK_NULL_HANDLE;
        VkDeviceMemory m_vertexBufferMemory   = VK_NULL_HANDLE;

        VkExtent2D m_swapchainExtent;
        VkRenderPass m_renderPass;
        VkPipeline m_pipeline;
        VkPipelineLayout m_pipelineLayout;
        VkCommandPool m_commandPool;
        std::vector<VkImageView> m_swapchainImageViews;
        std::vector<VkFramebuffer> m_framebuffers;
        std::vector<VkCommandBuffer> m_commandBuffers;
        std::vector<VkSemaphore> m_imgAvailSemaphores;
        std::vector<VkSemaphore> m_renderFinishedSemaphores;
        std::vector<VkFence> m_fences;

        VkClearValue m_clearColor = { 0.0f, 0.2f, 0.4f, 1.0f };

        const VkFormat m_swapchainImgFormat = VK_FORMAT_B8G8R8A8_UNORM;
        const int MAX_FRAMES_IN_FLIGHT = 2;
    };
}
}

#endif // GRAPHICLIBRARIES_ENGINES_VULKAN_H_
