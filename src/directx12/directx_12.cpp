#include "directx_12.hpp"

#include <iostream>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_win32.h>
#include <imgui/backends/imgui_impl_dx12.h>

#include "widgets/fps_counter.hpp"
#include "windows/win32.hpp"

#include "utils.hpp"

using namespace GraphicLibraries::Engines;
using namespace GraphicLibraries::Widgets;
using namespace GraphicLibraries::Windows;

DirectX12::DirectX12()
    : m_window { nullptr },
      m_fpsCounter { nullptr }
{
    m_isInit = false;
}

DirectX12::~DirectX12()
{
    std::cout << "DirectX 12 Destructor" << std::endl;

    if (m_isInit)
        release();
}

void DirectX12::init()
{
    std::cout << "Initializing DirectX 12" << std::endl;

    m_window = new Win32Window;

    if (!m_window)
        throw std::exception("DIRECTX12: Can't create win32 window");

    m_window->init();
    m_window->setTitle("DirectX 12");

    throwIfFailed(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&m_device)),
        "Can't create device");

    // Make rtv and srv heap
    throwIfFailed(createRtvHeap(), "Can't create rtb heap");
    throwIfFailed(createSrvHeap(), "Can't create srv heap");

    // Describe and create the command queue
    throwIfFailed(createCommandQueue(), "Can't create command queue");

    for (UINT i = 0; i < FRAMES_IN_FLIGHT_NUM; ++i)
        throwIfFailed(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_cmdAllocators[i])),
            "Can't create command allocator");

        // Make the command list
    throwIfFailed(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
                                              m_cmdAllocators[0].Get(),
                                              nullptr,
                                              IID_PPV_ARGS(&m_cmdList)),
        "Can't create command list");
    throwIfFailed(m_cmdList->Close(), "Can't close command list");

    // Create the fence
    throwIfFailed(createFence(), "Can't create fence");

    // Describe and creat the swap chain
    throwIfFailed(createSwapChain(), "Can't create swap chain");

    m_swapChain->SetMaximumFrameLatency(BUFFERS_NUM);
    m_swapChainWaitableObject = m_swapChain->GetFrameLatencyWaitableObject();

    throwIfFailed(createRenderTarget(), "Can't create render target");

    // Load Assets
        // Make an empty root signature
    throwIfFailed(createRootSignature(), "Can't create root signature");

    // Setup the pipeline state and the shaders which will be used for it
    initPipeline();

    // Create the VBO containing the triangle data
    ComPtr<ID3D12Resource> upload;
    throwIfFailed(createVertexBuffer(upload), "Can't create vertex buffer");

    // Init screenBounds;
    ZeroMemory(&m_screenBounds, sizeof(m_screenBounds));
    m_screenBounds.right = m_window->getWidth();
    m_screenBounds.bottom = m_window->getHeight();

    // Init viewport;
    ZeroMemory(&m_viewport, sizeof(m_viewport));
    m_viewport.Width = static_cast<float>(m_window->getWidth());
    m_viewport.Height = static_cast<float>(m_window->getHeight());
    m_viewport.MinDepth = D3D12_MIN_DEPTH;
    m_viewport.MaxDepth = D3D12_MAX_DEPTH;

    // First frame
    uploadResources(upload);

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
    ImGui_ImplDX12_Init(m_device.Get(),
                        FRAMES_IN_FLIGHT_NUM,
                        DXGI_FORMAT_R8G8B8A8_UNORM, m_srvHeap.Get(),
                        m_srvHeap->GetCPUDescriptorHandleForHeapStart(),
                        m_srvHeap->GetGPUDescriptorHandleForHeapStart());

    m_isInit = true;
}

void DirectX12::release()
{
    std::cout << "Releasing DirectX 12" << std::endl;

    // Cleanup
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    int index = m_frameIndex % FRAMES_IN_FLIGHT_NUM;
    uint64_t fenceValue = m_fenceValues[index];

    if (!fenceValue)
    {
        m_fenceValues[index] = 0;
        if (m_fence->GetCompletedValue() < fenceValue)
        {
            m_fence->SetEventOnCompletion(fenceValue, m_fenceEvent);
            WaitForSingleObject(m_fenceEvent, INFINITE);
        }
    }

    if (m_swapChainWaitableObject != NULL)
    {
        CloseHandle(m_swapChainWaitableObject);
        m_swapChainWaitableObject = NULL;
    }

    if (m_fenceEvent)
    {
        CloseHandle(m_fenceEvent);
        m_fenceEvent = NULL;
    }

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

void DirectX12::newFrame(float dt)
{
    // Start the Dear ImGui frame
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    m_fpsCounter->draw();

    // Rendering
    ImGui::Render();

    uint32_t index = waitForNextFrameResources();

    UINT backBufferIdx = m_swapChain->GetCurrentBackBufferIndex();
    //frameCtx->CommandAllocator->Reset();
    m_cmdAllocators[index]->Reset();

    throwIfFailed(m_cmdList->Reset(m_cmdAllocators[index].Get(), m_pipelineState.Get()),
        "Can't reset command list");

    m_cmdList->SetGraphicsRootSignature(m_rootSignature.Get());
    m_cmdList->RSSetViewports(1, &m_viewport);
    m_cmdList->RSSetScissorRects(1, &m_screenBounds);

    D3D12_RESOURCE_BARRIER barrier;
    ZeroMemory(&barrier, sizeof(barrier));

    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = m_renderTargetsResource[backBufferIdx].Get();
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    m_cmdList->Reset(m_cmdAllocators[index].Get(), nullptr);
    m_cmdList->ResourceBarrier(1, &barrier);

    m_cmdList->OMSetRenderTargets(1, &m_renderTargetsDescriptor[backBufferIdx], false, nullptr);
    m_cmdList->ClearRenderTargetView(m_renderTargetsDescriptor[backBufferIdx], m_clearColor.data(), 0, nullptr);
    m_cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_cmdList->IASetVertexBuffers(0, 1, &m_vboView);
    m_cmdList->DrawInstanced(3, 1, 0, 0);

    // Render Dear ImGui graphics
    m_cmdList->SetDescriptorHeaps(1, m_srvHeap.GetAddressOf());
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), m_cmdList.Get());
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    m_cmdList->ResourceBarrier(1, &barrier);
    m_cmdList->Close();

    m_cmdQueue->ExecuteCommandLists(1, (ID3D12CommandList* const*)m_cmdList.GetAddressOf());

    m_swapChain->Present(1, 0); // Present with vsync

    UINT64 fenceValue = m_fenceLastSignaledValue + 1;
    m_cmdQueue->Signal(m_fence.Get(), fenceValue);
    m_fenceLastSignaledValue = fenceValue;
    m_fenceValues[index] = fenceValue;
}

uint32_t DirectX12::waitForNextFrameResources()
{
    uint32_t nextFrameIndex = m_frameIndex + 1;
    m_frameIndex = nextFrameIndex;

    HANDLE waitableObjects[] = { m_swapChainWaitableObject, NULL };
    DWORD numWaitableObjects = 1;

    uint32_t index = nextFrameIndex % FRAMES_IN_FLIGHT_NUM;

    uint64_t fenceValue = m_fenceValues[index];
    if (fenceValue != 0) // means no fence was signaled
    {
        m_fenceValues[index] = 0;
        m_fence->SetEventOnCompletion(fenceValue, m_fenceEvent);
        waitableObjects[1] = m_fenceEvent;
        numWaitableObjects = 2;
    }

    WaitForMultipleObjects(numWaitableObjects, waitableObjects, TRUE, INFINITE);

    return index;
}

HRESULT DirectX12::createRtvHeap()
{
    D3D12_DESCRIPTOR_HEAP_DESC desc;
    ZeroMemory(&desc, sizeof(desc));

    desc.NumDescriptors = BUFFERS_NUM;
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    desc.NodeMask = 1;

    if (FAILED(m_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_rtvHeap))))
        return E_FAIL;

    uint32_t rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();

    for (UINT i = 0; i < BUFFERS_NUM; ++i)
    {
        m_renderTargetsDescriptor[i] = rtvHandle;
        rtvHandle.ptr += rtvDescriptorSize;
    }

    return S_OK;
}

HRESULT DirectX12::createSrvHeap()
{
    D3D12_DESCRIPTOR_HEAP_DESC desc;
    ZeroMemory(&desc, sizeof(desc));

    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    desc.NumDescriptors = 1;

    return m_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_srvHeap));
}

HRESULT DirectX12::createCommandQueue()
{
    D3D12_COMMAND_QUEUE_DESC desc;
    ZeroMemory(&desc, sizeof(desc));

    desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    desc.NodeMask = 1;

    return m_device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_cmdQueue));
}

HRESULT DirectX12::createFence()
{
    if (FAILED(m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence))))
        return E_FAIL;

    m_fenceEvent = CreateEvent(nullptr, false, false, nullptr);

    if (!m_fenceEvent)
        return E_FAIL;

    return S_OK;
}

HRESULT DirectX12::createSwapChain()
{
    DXGI_SWAP_CHAIN_DESC1 desc;
    ZeroMemory(&desc, sizeof(desc));

    desc.BufferCount = BUFFERS_NUM;
    desc.Width = 0;
    desc.Height = 0;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.Flags = DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    desc.Scaling = DXGI_SCALING_STRETCH;;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Stereo = false;

    ComPtr<IDXGISwapChain1> swapChain;
    ComPtr<IDXGIFactory4> factory;

    if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory))))
        return E_FAIL;

    if (FAILED(factory->CreateSwapChainForHwnd(m_cmdQueue.Get(),
                                               m_window->getHwnd(),
                                               &desc,
                                               nullptr,
                                               nullptr,
                                               &swapChain)))
        return E_FAIL;

    if (factory)
        factory->Release();

    return swapChain->QueryInterface(IID_PPV_ARGS(&m_swapChain));
}

HRESULT DirectX12::createRenderTarget()
{
    for (UINT i = 0; i < BUFFERS_NUM; ++i)
    {
        if (FAILED(m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_renderTargetsResource[i]))))
            return E_FAIL;

        m_device->CreateRenderTargetView(m_renderTargetsResource[i].Get(), nullptr, m_renderTargetsDescriptor[i]);
    }

    return S_OK;
}

HRESULT DirectX12::createRootSignature()
{
    D3D12_ROOT_SIGNATURE_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    ComPtr<ID3DBlob> signature;
    ComPtr<ID3DBlob> error;

    HRESULT result = D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);

    if (!FAILED(result))
    {
        result = m_device->CreateRootSignature(0,
            signature->GetBufferPointer(),
            signature->GetBufferSize(),
            IID_PPV_ARGS(&m_rootSignature));
    }

    if (signature)
        signature->Release();

    if (error)
        error->Release();

    return result;
}

void DirectX12::initPipeline()
{
    ComPtr<ID3DBlob> vertexBlob;
    ComPtr<ID3DBlob> pixelBlob;

    const uint32_t compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
    throwIfFailed(D3DCompileFromFile(getFileFullPath(L"directx12\\shaders\\vertex.hlsl").c_str(), nullptr, nullptr,
        "main", "vs_5_0", compileFlags, 0, &vertexBlob, nullptr),
        "Can't compile vertex shader from file");

    throwIfFailed(D3DCompileFromFile(getFileFullPath(L"directx12\\shaders\\pixel.hlsl").c_str(), nullptr, nullptr,
        "main", "ps_5_0", compileFlags, 0, &pixelBlob, nullptr),
        "Can't compile pixel shader from file");

    // Specify the vertex data layout
    std::array<D3D12_INPUT_ELEMENT_DESC, 2> vertexLayout =
    {
        D3D12_INPUT_ELEMENT_DESC { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        D3D12_INPUT_ELEMENT_DESC { "COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    // Create the graphic pipeline state description
    D3D12_GRAPHICS_PIPELINE_STATE_DESC desc;
    ZeroMemory(&desc, sizeof(desc));

    desc.pRootSignature = m_rootSignature.Get();
    desc.VS.pShaderBytecode = vertexBlob->GetBufferPointer();
    desc.VS.BytecodeLength = vertexBlob->GetBufferSize();
    desc.PS.pShaderBytecode = pixelBlob->GetBufferPointer();
    desc.PS.BytecodeLength = pixelBlob->GetBufferSize();

    desc.BlendState.AlphaToCoverageEnable = FALSE;
    desc.BlendState.IndependentBlendEnable = FALSE;

    const D3D12_RENDER_TARGET_BLEND_DESC rtBlendDesc =
    {
        false, false,
        D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
        D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
        D3D12_LOGIC_OP_NOOP,
        D3D12_COLOR_WRITE_ENABLE_ALL
    };

    for (int i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
        desc.BlendState.RenderTarget[i] = rtBlendDesc;

    desc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
    desc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
    desc.RasterizerState.FrontCounterClockwise = FALSE;
    desc.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
    desc.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
    desc.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
    desc.RasterizerState.DepthClipEnable = TRUE;
    desc.RasterizerState.MultisampleEnable = FALSE;
    desc.RasterizerState.AntialiasedLineEnable = FALSE;
    desc.RasterizerState.ForcedSampleCount = 0;
    desc.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

    desc.SampleMask = UINT_MAX;
    desc.DepthStencilState.DepthEnable = false;
    desc.DepthStencilState.StencilEnable = false;

    desc.InputLayout.pInputElementDescs = vertexLayout.data();
    desc.InputLayout.NumElements = static_cast<UINT>(vertexLayout.size());
    desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    desc.NumRenderTargets = 1;
    desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;

    throwIfFailed(m_device->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&m_pipelineState)),
        "Can't create graphics pipeline state");

    if (vertexBlob)
        vertexBlob->Release();

    if (pixelBlob)
        pixelBlob->Release();
}

HRESULT DirectX12::createVertexBuffer(ComPtr<ID3D12Resource>& upload)
{
    HRESULT result = S_OK;
    Vertex triangle[] =
    {
        { {  0.0f,  0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
        { {  0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
        { { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
    };

    D3D12_HEAP_PROPERTIES properties;
    ZeroMemory(&properties, sizeof(properties));
    properties.Type = D3D12_HEAP_TYPE_UPLOAD;
    properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

    D3D12_RESOURCE_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    desc.Width = sizeof(triangle);
    desc.Height = 1;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = 1;
    desc.Format = DXGI_FORMAT_UNKNOWN;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    desc.Flags = D3D12_RESOURCE_FLAG_NONE;

    // Place the vertex data in an upload heap first, the do a GPU-side copy
    // into a default heap (resident in VRAM)
    result = m_device->CreateCommittedResource(&properties, D3D12_HEAP_FLAG_NONE,
        &desc, D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr, IID_PPV_ARGS(&upload));

    if (FAILED(result))
        return result;

    // Now copy the data into the upload heap
    uint8_t* mapping = nullptr;

    D3D12_RANGE readRange;
    ZeroMemory(&readRange, sizeof(readRange));

    result = upload->Map(0, &readRange, reinterpret_cast<void**>(&mapping));

    if (FAILED(result))
        return result;

    std::memcpy(mapping, triangle, sizeof(triangle));
    upload->Unmap(0, nullptr);

    // Now setup the GPU-side heap to hold the verts for rendering
    properties.Type = D3D12_HEAP_TYPE_DEFAULT;
    result = m_device->CreateCommittedResource(&properties, D3D12_HEAP_FLAG_NONE,
        &desc, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
        nullptr, IID_PPV_ARGS(&m_vbo));

    if (FAILED(result))
        return result;

    // Setup the vertex buffer view
    m_vboView.BufferLocation = m_vbo->GetGPUVirtualAddress();
    m_vboView.StrideInBytes = sizeof(Vertex);
    m_vboView.SizeInBytes = sizeof(triangle);

    return result;
}

void DirectX12::uploadResources(ComPtr<ID3D12Resource>& upload)
{
    uint32_t index = waitForNextFrameResources();

    m_cmdAllocators[index]->Reset();
    throwIfFailed(m_cmdList->Reset(m_cmdAllocators[index].Get(), m_pipelineState.Get()),
        "Can't reset command list");

    // Now enqueue the copy
    m_cmdList->CopyResource(m_vbo.Get(), upload.Get());
    throwIfFailed(m_cmdList->Close(), "Can't close cmd list");

    // Execute the command list and present
    std::array<ID3D12CommandList*, 1> cmdLists = { m_cmdList.Get() };
    m_cmdQueue->ExecuteCommandLists(static_cast<UINT>(cmdLists.size()), cmdLists.data());

    UINT64 fenceValue = m_fenceLastSignaledValue + 1;
    m_cmdQueue->Signal(m_fence.Get(), fenceValue);
    m_fenceLastSignaledValue = fenceValue;
    m_fenceValues[index] = fenceValue;

    upload = nullptr;
}
