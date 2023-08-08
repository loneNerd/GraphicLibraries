#include "directx_11.hpp"

#include <iostream>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_impl_dx11.h>

#include "widgets/fps_counter.hpp"
#include "windows/win32.hpp"

#include "utils.hpp"

using namespace GraphicLibraries::Engines;
using namespace GraphicLibraries::Widgets;
using namespace GraphicLibraries::Windows;

DirectX11::DirectX11()
    : m_window { nullptr },
      m_fpsCounter { nullptr }
{
    m_isInit = false;
}

DirectX11::~DirectX11()
{
    std::cout << "DirectX11 Destructor" << std::endl;

    if (m_isInit)
        release();
}

void DirectX11::init()
{
    std::cout << "Initializing DirectX 11" << std::endl;

    m_window = new Win32Window;

    if (!m_window)
        throw std::exception("DIRECTX11: Can't create win32 window");

    m_window->init();
    m_window->setTitle(L"DirectX 11");

    throwIfFailed(createSwapChain(), "Can't create device and swap chain");
    throwIfFailed(createBackBuffer(), "Can't allocate back buffer");

    // set the render target as the back buffer
    m_context->OMSetRenderTargets(1, m_targetView.GetAddressOf(), nullptr);

    // set the viewport
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.Width = static_cast<float>(m_window->getWidth());
    viewport.Height = static_cast<float>(m_window->getHeight());
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;

    m_context->RSSetViewports(1, &viewport);

    // Init pipeline
    initPipeline();

    // Init graphics
    throwIfFailed(createVertexBuffer(), "Can't create vertex buffer");

    if (!m_vertexBuffer)
        throw std::exception("Vertex buffer is empty");

    throwIfFailed(mapSubresources(), "Can't map subresources");

    m_window->showWindow();

    m_fpsCounter = new FpsCounter();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(m_window->getHwnd());
    ImGui_ImplDX11_Init(m_device.Get(), m_context.Get());

    m_isInit = true;
}

void DirectX11::release()
{
    std::cout << "Releasing DirectX12" << std::endl;

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

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

void DirectX11::newFrame(float dt)
{
    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    m_fpsCounter->draw();

    // Rendering
    ImGui::Render();

    // clear the back buffer to a deep blue
    m_context->ClearRenderTargetView(m_targetView.Get(), m_backColor.data());

    // select which vertex buffer to display
    unsigned stride = sizeof(Vertex);
    unsigned offset = 0;
    m_context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);

    // select which primitive type we are using
    m_context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // draw the vertex buffer to the back buffer
    m_context->Draw(3, 0);

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    //switch the back buffer and the front buffer
    throwIfFailed(m_swapChain->Present(1, 0), "Can't present swap chain");
}

HRESULT DirectX11::createSwapChain()
{
    // create a struct to hold information about the swap chain
    DXGI_SWAP_CHAIN_DESC desc;
    ZeroMemory(&desc, sizeof(desc));

    // fill the swap chain description struct
    desc.BufferDesc.Width = m_window->getWidth();
    desc.BufferDesc.Height = m_window->getHeight();
    desc.OutputWindow = m_window->getHwnd();             // the window to be used
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // use 32-bit color
    desc.SampleDesc.Count = 1;                           // how many multisamples
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  // how swap chain is to be used
    desc.BufferCount = BUFFERS_NUM;                      // one back buffer
    desc.Windowed = TRUE;                                // windowed/full-screen mode
    desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    // create a device, device context and swap chain using the information in the scd struct
    return D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
                                         nullptr, 0, nullptr, 0,
                                         D3D11_SDK_VERSION,
                                         &desc, &m_swapChain, &m_device,
                                         nullptr, &m_context);
}

HRESULT DirectX11::createBackBuffer()
{
    // get the address of the back buffer
    ComPtr<ID3D11Texture2D> backBuffer;
    m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));

    if (!backBuffer)
        return E_FAIL;

    // use the back buffer address to create the render target
    HRESULT result = m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, m_targetView.GetAddressOf());
    backBuffer->Release();
    return result;
}

void DirectX11::initPipeline()
{
    // load and compile the two shaders
    ComPtr<ID3D10Blob> vertexBlob;
    ComPtr<ID3D10Blob> pixelBlob;

    ID3D11VertexShader* vertexShader = nullptr;
    ID3D11PixelShader* pixelShader   = nullptr;

    // encapsulate both shaders into shader object
    throwIfFailed(D3DCompileFromFile(getFileFullPath(L"shaders\\vertex.hlsl").c_str(),
                                     nullptr, nullptr,
                                     "main", "vs_4_0", 0, 0,
                                     vertexBlob.GetAddressOf(), nullptr),
                  "Can't compile vertex shader from file");

    throwIfFailed(m_device->CreateVertexShader(vertexBlob->GetBufferPointer(),
                                               vertexBlob->GetBufferSize(),
                                               nullptr,
                                               &vertexShader),
                  "Can't create vertex shader");

    throwIfFailed(D3DCompileFromFile(getFileFullPath(L"shaders\\pixel.hlsl").c_str(),
                                     nullptr, nullptr,
                                     "main", "ps_4_0", 0, 0, pixelBlob.GetAddressOf(), nullptr),
                  "Can't compile pixel shader from file");

    throwIfFailed(m_device->CreatePixelShader(pixelBlob->GetBufferPointer(),
                                              pixelBlob->GetBufferSize(),
                                              nullptr,
                                              &pixelShader),
                  "Can't create pixel shader");

    //set the shader objects
    m_context->VSSetShader(vertexShader, 0, 0);
    m_context->PSSetShader(pixelShader, 0, 0);

    D3D11_INPUT_ELEMENT_DESC ied[]
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    ComPtr<ID3D11InputLayout> layout = nullptr;

    throwIfFailed(m_device->CreateInputLayout(ied, 2,
                                              vertexBlob->GetBufferPointer(),
                                              vertexBlob->GetBufferSize(),
                                              layout.GetAddressOf()),
        "Can't create input layout");
    m_context->IASetInputLayout(layout.Get());

    if (layout)
        layout->Release();

    if (vertexBlob)
        vertexBlob->Release();

    if (pixelBlob)
        pixelBlob->Release();

    if (vertexShader)
        vertexShader->Release();

    if (pixelShader)
        pixelShader->Release();
}

HRESULT DirectX11::createVertexBuffer()
{
    // create the vertex buffer
    D3D11_BUFFER_DESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(bufferDesc));

    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;             // write access by CPU and GPU
    bufferDesc.ByteWidth = sizeof(Vertex) * 3;          // size is the Vertex struct * 3
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;    // use as a vertex buffer
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // allow CPU to write in buffer

    return m_device->CreateBuffer(&bufferDesc, nullptr, m_vertexBuffer.GetAddressOf());
}

HRESULT DirectX11::mapSubresources()
{
    // create a triangle using the Vertex struct
    Vertex triangle[] =
    {
        { {  0.0f,  0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
        { {  0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
        { { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
    };

    // copy the vertices into the buffer
    D3D11_MAPPED_SUBRESOURCE mappedSubres;
    ZeroMemory(&mappedSubres, sizeof(mappedSubres));
    // map the buffer
    HRESULT result = m_context->Map(m_vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubres);
    // copy the data
    memcpy(mappedSubres.pData, triangle, sizeof(triangle));
    // unmap the buffer
    m_context->Unmap(m_vertexBuffer.Get(), 0);

    return result;
}
