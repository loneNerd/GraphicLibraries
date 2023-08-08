#pragma once
#ifndef GRAPHICLIBRARIES_DIRECTX_11_DIRECTX_11_H_
#define GRAPHICLIBRARIES_DIRECTX_11_DIRECTX_11_H_

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "D3DCompiler.lib")

#include <array>
#include <exception>
#include <string>

#include <d3d11.h>
#include <D3Dcompiler.h>

#include <wrl.h>

#include "engines/render_interface.hpp"

namespace GraphicLibraries
{

namespace Windows { class Win32Window; class IWindow; }
namespace Widgets { class FpsCounter; }

namespace Engines
{
    using namespace Microsoft::WRL;

    class DirectX11 : public IRender
    {
    public:
        DirectX11();
        ~DirectX11();

        DirectX11(const DirectX11& other)            = delete;
        DirectX11(DirectX11&& other)                 = delete;
        DirectX11& operator=(const DirectX11& other) = delete;
        DirectX11& operator=(DirectX11&& other)      = delete;

        virtual void init()     override;
        virtual void release()  override;
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
                throw std::exception(std::string("DIRECTX 11: ").append(message).c_str());
        }

        Windows::Win32Window* m_window    = nullptr;
        Widgets::FpsCounter* m_fpsCounter = nullptr;

        HRESULT createSwapChain();
        HRESULT createBackBuffer();
        HRESULT createVertexBuffer();
        HRESULT mapSubresources();

        void initPipeline();

        static const int BUFFERS_NUM = 2;

        ComPtr<IDXGISwapChain>         m_swapChain    = nullptr;
        ComPtr<ID3D11Device>           m_device       = nullptr;
        ComPtr<ID3D11DeviceContext>    m_context      = nullptr;
        ComPtr<ID3D11RenderTargetView> m_targetView   = nullptr;
        ComPtr<ID3D11Buffer>           m_vertexBuffer = nullptr;

        const std::array<float, 4> m_backColor = { 0.0f, 0.2f, 0.4f, 1.0f };
    };
}
}

#endif // GRAPHICLIBRARIES_DIRECTX_11_DIRECTX_11_H_
