#pragma once
#ifndef GRAPHICLIBRARIES_ENGINES_DIRECTX_12_H_
#define GRAPHICLIBRARIES_ENGINES_DIRECTX_12_H_

#pragma comment (lib, "d3d12.lib")
#pragma comment (lib, "dxgi.lib")
#pragma comment (lib, "D3DCompiler.lib")

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>

#include <wrl.h>

#include <array>

#include "utils.h"
#include "ui/window_sdl2.h"

namespace GraphicLibraries
{
namespace Engines
{
    using namespace Microsoft::WRL;

    class DirectX12
    {
    public:
        DirectX12();
        ~DirectX12();

        DirectX12(const DirectX12& other)            = delete;
        DirectX12(DirectX12&& other)                 = delete;
        DirectX12& operator=(const DirectX12& other) = delete;
        DirectX12& operator=(DirectX12&& other)      = delete;

        void render();

    private:
        inline void throwIfFailed(HRESULT hr, const char* message)
        {
            if (FAILED(hr))
                throw std::exception(std::string("DIRECTX 12: ").append(message).c_str());
        }

        HRESULT createCommandQueue(ComPtr<ID3D12Device>& device);
        HRESULT createSwapChain();
        HRESULT createDescriptorHeap(ComPtr<ID3D12Device>& device);
        HRESULT createCommandAllocator(ComPtr<ID3D12Device>& device);
        HRESULT createRootSignature(ComPtr<ID3D12Device>& device);
        HRESULT createVertexBuffer(ComPtr<ID3D12Device>& device, ComPtr<ID3D12Resource>& upload);
        HRESULT createFence(ComPtr<ID3D12Device>& device, ComPtr<ID3D12Resource>& upload);
        HRESULT syncFetch(uint32_t signalVal);

        void initPipeline(ComPtr<ID3D12Device>& device);
        void firstFrame(ComPtr<ID3D12Resource>& upload);

        ComPtr<IDXGISwapChain3>           m_swapChain;
        ComPtr<ID3D12CommandAllocator>    m_cmdAllocator;
        ComPtr<ID3D12GraphicsCommandList> m_cmdList;
        ComPtr<ID3D12PipelineState>       m_pipelineState;
        ComPtr<ID3D12RootSignature>       m_rootSignature;
        ComPtr<ID3D12Resource>            m_vbo;
        ComPtr<ID3D12Resource>            m_renderTargets[2];
        ComPtr<ID3D12DescriptorHeap>      m_rtvHeap;
        ComPtr<ID3D12CommandQueue>        m_cmdQueue;
        ComPtr<ID3D12Fence>               m_fence;

        uint32_t m_rtvDescriptorSize;
        int m_fenceValue;
        uint32_t m_signalVal;
        HANDLE m_fenceEvt;

        const std::array<float, 4> m_clearColor = { 0.0f, 0.2f, 0.4f, 1.0f };

        D3D12_VIEWPORT m_viewport;
        D3D12_RECT m_screenBounds;
        D3D12_VERTEX_BUFFER_VIEW m_vboView;
    };
}
}

#endif // GRAPHICLIBRARIES_ENGINES_DIRECTX_12_H_
