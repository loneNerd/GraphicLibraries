#pragma once
#ifndef GRAPHICLIBRARIES_DIRECTX_12_DIRECTX_12_H_
#define GRAPHICLIBRARIES_DIRECTX_12_DIRECTX_12_H_

#include <array>
#include <exception>
#include <string>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>

#include <wrl.h>

#include "engines/render_interface.hpp"

namespace GraphicLibraries
{

namespace Windows { class Win32Window; class IWindow; }
namespace Widgets { class FpsCounter; }

namespace Engines
{
    using namespace Microsoft::WRL;

    class DirectX12 : public IRender
    {
    public:
        DirectX12();
        virtual ~DirectX12() override;

        DirectX12(const DirectX12& other)            = delete;
        DirectX12(DirectX12&& other)                 = delete;
        DirectX12& operator=(const DirectX12& other) = delete;
        DirectX12& operator=(DirectX12&& other)      = delete;

        virtual void init() override;
        virtual void release() override;
        virtual void newFrame(float dt) override;

        virtual Windows::IWindow* getWindow() const override { return reinterpret_cast<Windows::IWindow*>(m_window); }

    private:
        struct Vertex
        {
            float position[3];
            float color[4];
        };

        inline void throwIfFailed(HRESULT hr, const char* message)
        {
            if (FAILED(hr))
                throw std::exception(std::string("DIRECTX 12: ").append(message).c_str());
        }

        Windows::Win32Window* m_window;
        Widgets::FpsCounter* m_fpsCounter;

        HRESULT createRtvHeap();
        HRESULT createSrvHeap();
        HRESULT createCommandQueue();
        HRESULT createFence();
        HRESULT createSwapChain();
        HRESULT createRenderTarget();
        HRESULT createRootSignature();
        HRESULT createVertexBuffer(ComPtr<ID3D12Resource>& upload);

        void initPipeline();
        void uploadResources(ComPtr<ID3D12Resource>& upload);
        uint32_t waitForNextFrameResources();

        // Data
        static const int FRAMES_IN_FLIGHT_NUM = 3;
        static const int BUFFERS_NUM          = 3;

        ComPtr<ID3D12Device>              m_device        = nullptr;
        ComPtr<ID3D12DescriptorHeap>      m_rtvHeap       = nullptr;
        ComPtr<ID3D12DescriptorHeap>      m_srvHeap       = nullptr;
        ComPtr<ID3D12CommandQueue>        m_cmdQueue      = nullptr;
        ComPtr<ID3D12GraphicsCommandList> m_cmdList       = nullptr;
        ComPtr<ID3D12Fence>               m_fence         = nullptr;
        ComPtr<IDXGISwapChain3>           m_swapChain     = nullptr;
        ComPtr<ID3D12RootSignature>       m_rootSignature = nullptr;
        ComPtr<ID3D12PipelineState>       m_pipelineState = nullptr;
        ComPtr<ID3D12Resource>            m_vbo           = nullptr;
        ComPtr<ID3D12CommandAllocator>    m_cmdAllocators[FRAMES_IN_FLIGHT_NUM];
        ComPtr<ID3D12Resource>            m_renderTargetsResource[BUFFERS_NUM];

        HANDLE m_fenceEvent;
        HANDLE m_swapChainWaitableObject;
        uint64_t m_fenceValues[FRAMES_IN_FLIGHT_NUM] { 0 };
        uint32_t m_frameIndex = 0;
        uint64_t m_fenceLastSignaledValue = 0;

        const std::array<float, 4> m_clearColor = { 0.0f, 0.2f, 0.4f, 1.0f };

        D3D12_CPU_DESCRIPTOR_HANDLE m_renderTargetsDescriptor[BUFFERS_NUM];

        D3D12_VIEWPORT m_viewport;
        D3D12_RECT m_screenBounds;
        D3D12_VERTEX_BUFFER_VIEW m_vboView;
    };
}
}

#endif // GRAPHICLIBRARIES_DIRECTX_12_DIRECTX_12_H_
