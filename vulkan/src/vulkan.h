#pragma once
#ifndef GRAPHICLIBRARIES_ENGINES_VULKAN_VULKAN_H_
#define GRAPHICLIBRARIES_ENGINES_VULKAN_VULKAN_H_

#pragma comment(lib, "vulkan-1.lib")

#include <exception>
#include <string>
#include <vector>

#include <vulkan/vulkan.h>

#include "engines/render_interface.h"

namespace GraphicLibraries
{

namespace Windows { class SDL2Window; class IWindow; }
namespace Widgets { class FpsCounter; }

namespace Engines
{
    class Vulkan : public IRender
    {
    public:
        Vulkan();
        virtual ~Vulkan() override;

        Vulkan(const Vulkan& other)            = delete;
        Vulkan(Vulkan&& other)                 = delete;
        Vulkan& operator=(const Vulkan& other) = delete;
        Vulkan& operator=(Vulkan&& other)      = delete;

        virtual void init()     override;
        virtual void release()  override;
        virtual void newFrame() override;

        virtual Windows::IWindow* getWindow() const override { return reinterpret_cast<Windows::IWindow*>(m_window); }

    private:
        inline void throwIfFailed(VkResult rs, const char* message)
        {
            if (rs != VK_SUCCESS)
                throw std::exception(std::string("VULKAN: ").append(message).c_str());
        }

        Windows::SDL2Window* m_window;
        Widgets::FpsCounter* m_fpsCounter;

        VkResult createInstance();
        VkResult createPhysicalDevice();
        VkResult createDevice();
        VkResult createSwapchain();
        VkResult createRenderPass();
        VkResult createPipeline();
        VkResult createBuffer();
        VkResult createCommandPool();
        VkResult createVertexBuffer();
        VkResult createCommandBuffers();
        VkResult recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
        VkResult createSemaphore();
        VkResult createFence();
        VkResult createImguiDescriptorPool();
        VkResult initImgui();

        std::vector<char> readFile(const std::wstring& filename);
        uint32_t findMemoryType(VkPhysicalDevice& physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);

        VkInstance m_instance                  = VK_NULL_HANDLE;
        VkSurfaceKHR m_surface                 = VK_NULL_HANDLE;
        VkPhysicalDevice m_physicalDevice      = VK_NULL_HANDLE;
        VkDevice m_device                      = VK_NULL_HANDLE;
        VkQueue m_queue                        = VK_NULL_HANDLE;
        VkSwapchainKHR m_swapchain             = VK_NULL_HANDLE;
        VkBuffer m_vertexBuffer                = VK_NULL_HANDLE;
        VkDeviceMemory m_vertexBufferMemory    = VK_NULL_HANDLE;
        VkRenderPass m_renderPass              = VK_NULL_HANDLE;
        VkPipeline m_pipeline                  = VK_NULL_HANDLE;
        VkPipelineLayout m_pipelineLayout      = VK_NULL_HANDLE;
        VkCommandPool m_commandPool            = VK_NULL_HANDLE;
        VkDescriptorPool m_imguiDescriptorPool = VK_NULL_HANDLE;

        uint32_t m_graphicsQueueIndex;
        VkExtent2D m_swapchainExtent;
        std::vector<VkImageView> m_swapchainImageViews;
        std::vector<VkFramebuffer> m_frameBuffers;
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

#endif // GRAPHICLIBRARIES_ENGINES_VULKAN_VULKAN_H_
