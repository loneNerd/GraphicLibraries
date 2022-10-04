#include "directx_11.h"

using namespace GraphicLibraries::Engines;
using namespace GraphicLibraries::UI;

DirectX11::DirectX11(WindowSDL2* window)
{
    std::cout << "Initializing DirectX 11... ";

    if (!window)
        throw std::exception("DIRECTX 11: null window pointer");

    m_window = window;
    m_window->setTitle("DX11");

    // create a struct to hold information about the swap chain
    DXGI_SWAP_CHAIN_DESC scd {};

    // clear out the struct for use
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

    // fill the swap chain description struct
    scd.BufferDesc.Width = m_window->getWidth();
    scd.BufferDesc.Height = m_window->getHeight();
    scd.OutputWindow = m_window->getHWND();             // the window to be used
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // use 32-bit color
    scd.SampleDesc.Count = 4;                           // how many multisamples
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  // how swap chain is to be used
    scd.BufferCount = 1;                                // one back buffer
    scd.Windowed = TRUE;                                // windowed/full-screen mode
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    // create a device, device context and swap chain using the information in the scd struct
    D3D11CreateDeviceAndSwapChain(nullptr,
                                  D3D_DRIVER_TYPE_HARDWARE,
                                  nullptr,
                                  0,
                                  nullptr,
                                  0,
                                  D3D11_SDK_VERSION,
                                  &scd,
                                  &m_swapChain,
                                  &m_device,
                                  nullptr,
                                  &m_context);

    // get the address of the back buffer
    ID3D11Texture2D* backBuffer = nullptr;
    m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));

    if (!backBuffer)
        throw std::exception("DIRECTX 11: Can't allocate back buffer");

    // use the back buffer address to create the render target
    m_device->CreateRenderTargetView(backBuffer, nullptr, &m_targetView);
    backBuffer->Release();

    // set the render target as the back buffer
    m_context->OMSetRenderTargets(1, &m_targetView, nullptr);

    // set the viewport
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.Width = static_cast<float>(m_window->getWidth());
    viewport.Height = static_cast<float>(m_window->getHeight());
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;

    m_context->RSSetViewports(1, &viewport);

    initPipeline();
    initGraphics();

    std::cout << "Done" << std::endl;
}

DirectX11::~DirectX11()
{
    std::cout << "Releasing DirectX 11... ";

    // close and release all existing COM objects
    if (m_swapChain)
    {
        m_swapChain->SetFullscreenState(FALSE, nullptr);
        m_swapChain->Release();
    }

    if (m_targetView)
        m_targetView->Release();
    
    if (m_device)
        m_device->Release();
    
    if (m_context)
        m_context->Release();

    if (m_layout)
        m_layout->Release();

    if (m_vertexShader)
        m_vertexShader->Release();

    if (m_pixelShader)
        m_pixelShader->Release();

    if (m_vertexBuffer)
        m_vertexBuffer->Release();

    std::cout << "Done" << std::endl;
}

void DirectX11::render()
{
    // clear the back buffer to a deep blue
    std::array<float, 4> color{ 0.0f, 0.2f, 0.4f, 1.0f };
    m_context->ClearRenderTargetView(m_targetView, color.data());

    // select which vertex buffer to display
    unsigned stride = sizeof(Vertex);
    unsigned offset = 0;
    m_context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // select which primitive type we are using
    m_context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // draw the vertex buffer to the back buffer
    m_context->Draw(3, 0);

    //switch the back buffer and the front buffer
    m_swapChain->Present(0, 0);
}

void DirectX11::initGraphics()
{
    // create a triangle using the Vertex struct
    Vertex vertices[] =
    {
        {  0.0f,   0.5f, 0.0f, { 1.0f, 0.0f, 0.0f, 1.0f } },
        {  0.45f, -0.5f, 0.0f, { 0.0f, 1.0f, 0.0f, 1.0f } },
        { -0.45f, -0.5f, 0.0f, { 0.0f, 0.0f, 1.0f, 1.0f } },
    };

    // create the vertex buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DYNAMIC;             // write access by CPU and GPU
    bd.ByteWidth = sizeof(Vertex) * 3;          // size is the Vertex struct * 3
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;    // use as a vertex buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // allow CPU to write in buffer

    m_device->CreateBuffer(&bd, nullptr, &m_vertexBuffer);

    if (!m_vertexBuffer)
        throw std::exception("DIRECTX 11: Can't create vertex buffer");

    // copy the vertices into the buffer
    D3D11_MAPPED_SUBRESOURCE ms;
    m_context->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms); // map the buffer
    memcpy(ms.pData, vertices, sizeof(vertices));                       // copy the data
    m_context->Unmap(m_vertexBuffer, 0);                                // unmap the buffer
}

void DirectX11::initPipeline()
{
    // load and compile the two shaders
    ID3D10Blob* vertexShader;
    ID3D10Blob* pixelShader;

    wchar_t moduleName[_MAX_PATH] = {};
    if (!GetModuleFileNameW(nullptr, moduleName, _MAX_PATH))
        throw std::exception(std::error_code(static_cast<int>(GetLastError()), std::system_category()).message().c_str());

    wchar_t drive[_MAX_DRIVE];
    wchar_t path[_MAX_PATH];

    if (_wsplitpath_s(moduleName, drive, _MAX_DRIVE, path, _MAX_PATH, nullptr, 0, nullptr, 0))
        throw std::exception("DIRECTX 11: Can't split path");

    wchar_t vertexFilename[_MAX_PATH];
    if (_wmakepath_s(vertexFilename, _MAX_PATH, drive, path, L"shaders\\directx_11\\vertex.cso", nullptr))
        throw std::exception("DIRECTX 11: Can't find vertex shader filename");

    // encapsulate both shaders into shader objects
    D3DReadFileToBlob(vertexFilename, &vertexShader);

    m_device->CreateVertexShader(vertexShader->GetBufferPointer(),
                                 vertexShader->GetBufferSize(),
                                 nullptr,
                                 &m_vertexShader);

    wchar_t pixelFilename[_MAX_PATH];
    if (_wmakepath_s(pixelFilename, _MAX_PATH, drive, path, L"shaders\\directx_11\\pixel.cso", nullptr))
        throw std::exception("DIRECTX 11: Can't find pixel shader filename");

    D3DReadFileToBlob(pixelFilename, &pixelShader);
    m_device->CreatePixelShader(pixelShader->GetBufferPointer(),
                                pixelShader->GetBufferSize(),
                                nullptr,
                                &m_pixelShader);

    //set the shader objects
    m_context->VSSetShader(m_vertexShader, 0, 0);
    m_context->PSSetShader(m_pixelShader, 0, 0);

    D3D11_INPUT_ELEMENT_DESC ied[]
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    m_device->CreateInputLayout(ied, 2, vertexShader->GetBufferPointer(), vertexShader->GetBufferSize(), &m_layout);
    m_context->IASetInputLayout(m_layout);
}
