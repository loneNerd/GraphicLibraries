#pragma once
#ifndef GRAPHICLIBRARIES_ENGINES_DIRECTX_11_H_
#define GRAPHICLIBRARIES_ENGINES_DIRECTX_11_H_

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "D3DCompiler.lib")

#include <d3d11.h>
#include <D3Dcompiler.h>

#include <array>
#include <exception>
#include <fstream>
#include <memory>
#include <vector>

#include <wrl.h>

#include "utils.h"
#include "ui/window_sdl2.h"

namespace GraphicLibraries
{
namespace Engines
{
    using namespace Microsoft::WRL;

    class DirectX11
    {
    public:
        DirectX11();
        ~DirectX11();

        DirectX11(const DirectX11& other)            = delete;
        DirectX11(DirectX11&& other)                 = delete;
        DirectX11& operator=(const DirectX11& other) = delete;
        DirectX11& operator=(DirectX11&& other)      = delete;

        void render();

    private:
        inline void throwIfFailed(HRESULT hr, const char* message)
        {
            if (FAILED(hr))
                throw std::exception(std::string("DIRECTX 11: ").append(message).c_str());
        }

        HRESULT createSwapChain(ComPtr<ID3D11Device>& device);
        HRESULT createBackBuffer(ComPtr<ID3D11Device>& device);
        HRESULT createVertexBuffer(ComPtr<ID3D11Device>& device);
        HRESULT mapSubresources();

        void initPipeline(ComPtr<ID3D11Device>& device);

        const std::array<float, 4> m_backColor { 0.0f, 0.2f, 0.4f, 1.0f };

        ComPtr<IDXGISwapChain>         m_swapChain    = nullptr;
        ComPtr<ID3D11DeviceContext>    m_context      = nullptr;
        ComPtr<ID3D11RenderTargetView> m_targetView   = nullptr;
        ComPtr<ID3D11Buffer>           m_vertexBuffer = nullptr;
    };
}
}

#endif // GRAPHICLIBRARIES_ENGINES_DIRECTX_11_H_
