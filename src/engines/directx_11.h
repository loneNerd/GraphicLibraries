#pragma once
#ifndef GRAPHICLIBRARIES_ENGINES_DIRECTX_11_H_
#define GRAPHICLIBRARIES_ENGINES_DIRECTX_11_H_

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "D3DCompiler.lib")
//#pragma comment (lib, "d3dx11.lib")
//#pragma comment (lib, "d3dx10.lib")

#include <d3d11.h>
#include <D3Dcompiler.h>
//#include <d3dx11.h>
//#include <d3dx10.h>

#include <array>
#include <exception>
#include <fstream>
#include <memory>
#include <vector>

#include "ui/window_sdl2.h"

namespace GraphicLibraries
{
namespace Engines
{
    struct Vertex
    {
        float x;
        float y;
        float z;
        float color[4];
    };

    class DirectX11
    {
    public:
        explicit DirectX11(UI::WindowSDL2* window);
        ~DirectX11();

        DirectX11(const DirectX11& other)            = delete;
        DirectX11(DirectX11&& other)                 = delete;
        DirectX11& operator=(const DirectX11& other) = delete;
        DirectX11& operator=(DirectX11&& other)      = delete;

        void render();

    private:
        void initGraphics();
        void initPipeline();

        UI::WindowSDL2* m_window               = nullptr;

        IDXGISwapChain*         m_swapChain    = nullptr;
        ID3D11Device*           m_device       = nullptr;
        ID3D11DeviceContext*    m_context      = nullptr;
        ID3D11RenderTargetView* m_targetView   = nullptr;
        ID3D11InputLayout*      m_layout       = nullptr;
        ID3D11VertexShader*     m_vertexShader = nullptr;
        ID3D11PixelShader*      m_pixelShader  = nullptr;
        ID3D11Buffer*           m_vertexBuffer = nullptr;
    };
}
}

#endif // GRAPHICLIBRARIES_ENGINES_DIRECTX_11_H_
