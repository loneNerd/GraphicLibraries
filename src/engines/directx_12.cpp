#include "directx_12.h"

using namespace GraphicLibraries::Engines;
using namespace GraphicLibraries::UI;

DirectX12::DirectX12()
{
    std::cout << "Initializing DirectX 12" << std::endl;

    WindowSDL2::getInstance().setTitle("DX12");

    ComPtr<ID3D12Device> device;
    throwIfFailed(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&device)),
        "Can't create device");

    // Describe and create the command queue
    throwIfFailed(createCommandQueue(device), "Can't create command queue");

    // Describe and creat the swap chain
    throwIfFailed(createSwapChain(), "Can't create swap chain");

    // Make a descriptor heap
    throwIfFailed(createDescriptorHeap(device), "Can't create descriptor heap");
    m_rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    // Create render target descriptor for the swap chain's render target
    throwIfFailed(createCommandAllocator(device), "Can't create command allocator");

    // Load Assets

    // Make an empty root signature
    throwIfFailed(createRootSignature(device), "Can't create root signature");

    // Setup the pipeline state and the shaders which will be used for it
    initPipeline(device);

    // Make the command list
    throwIfFailed(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
                                            m_cmdAllocator.Get(),
                                            m_pipelineState.Get(),
                                            IID_PPV_ARGS(&m_cmdList)),
                  "Can't create command list");
    throwIfFailed(m_cmdList->Close(), "Can't close command list");

    // Create the VBO containing the triangle data
    ComPtr<ID3D12Resource> upload;
    throwIfFailed(createVertexBuffer(device, upload), "Can't create vertex buffer");

    // Create the fence
    throwIfFailed(createFence(device, upload), "Can't create fence");

    // Init screenBounds;
    ZeroMemory(&m_screenBounds, sizeof(m_screenBounds));
    m_screenBounds.right = WindowSDL2::getInstance().getWidth();
    m_screenBounds.bottom = WindowSDL2::getInstance().getHeight();

    // Init viewport;
    ZeroMemory(&m_viewport, sizeof(m_viewport));
    m_viewport.Width = static_cast<float>(WindowSDL2::getInstance().getWidth());
    m_viewport.Height = static_cast<float>(WindowSDL2::getInstance().getHeight());
    m_viewport.MinDepth = D3D12_MIN_DEPTH;
    m_viewport.MaxDepth = D3D12_MAX_DEPTH;

    // First frame
    firstFrame(upload);

    std::cout << "Done" << std::endl;
}

DirectX12::~DirectX12()
{
    std::cout << "Releasing DirectX 12" << std::endl;

    std::cout << "Done" << std::endl;
}

HRESULT DirectX12::createCommandQueue(ComPtr<ID3D12Device>& device)
{
    D3D12_COMMAND_QUEUE_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    return device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_cmdQueue));
}

HRESULT DirectX12::createSwapChain()
{
    DXGI_SWAP_CHAIN_DESC1 desc;
    ZeroMemory(&desc, sizeof(desc));

    desc.BufferCount = 2;
    desc.Width = WindowSDL2::getInstance().getWidth();
    desc.Height = WindowSDL2::getInstance().getHeight();
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    desc.SampleDesc.Count = 1;

    ComPtr<IDXGISwapChain1> swapChain;

    ComPtr<IDXGIFactory2> factory;
    HRESULT result = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&factory));

    if (FAILED(result))
        return result;

    result = factory->CreateSwapChainForHwnd(m_cmdQueue.Get(),
                                             WindowSDL2::getInstance().getHWND(),
                                             &desc,
                                             nullptr,
                                             nullptr,
                                             &swapChain);

    if (FAILED(result))
        return result;

    if (factory)
        factory->Release();

    return swapChain.As(&m_swapChain);
}

HRESULT DirectX12::createDescriptorHeap(ComPtr<ID3D12Device>& device)
{
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
    ZeroMemory(&rtvHeapDesc, sizeof(rtvHeapDesc));

    rtvHeapDesc.NumDescriptors = 2;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    return device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap));
}

HRESULT DirectX12::createCommandAllocator(ComPtr<ID3D12Device>& device)
{
    HRESULT result = S_OK;
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();

    // Create RTV for each frame
    for (int i = 0; i < 2; ++i)
    {
        result = m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_renderTargets[i]));

        if (FAILED(result))
            return result;

        device->CreateRenderTargetView(m_renderTargets[i].Get(), nullptr, rtvHandle);
        rtvHandle.ptr += m_rtvDescriptorSize;
    }

    return device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_cmdAllocator));
}

HRESULT DirectX12::createRootSignature(ComPtr<ID3D12Device>& device)
{
    D3D12_ROOT_SIGNATURE_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    ComPtr<ID3DBlob> signature;
    ComPtr<ID3DBlob> error;

    HRESULT result = D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);

    if (!FAILED(result))
    {
        result = device->CreateRootSignature(0,
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

void DirectX12::initPipeline(ComPtr<ID3D12Device>& device)
{
    ComPtr<ID3DBlob> vertexBlob;
    ComPtr<ID3DBlob> pixelBlob;

    const uint32_t compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
    throwIfFailed(D3DCompileFromFile(getFileFullPath(L"shaders\\directx_12\\vertex.hlsl").c_str(), nullptr, nullptr,
        "main", "vs_5_0", compileFlags, 0, &vertexBlob, nullptr),
        "Can't compile vertex shader from file");

    throwIfFailed(D3DCompileFromFile(getFileFullPath(L"shaders\\directx_12\\pixel.hlsl").c_str(), nullptr, nullptr,
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

    throwIfFailed(device->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&m_pipelineState)),
        "Can't create graphics pipeline state");

    if (vertexBlob)
        vertexBlob->Release();

    if (pixelBlob)
        pixelBlob->Release();
}

HRESULT DirectX12::createVertexBuffer(ComPtr<ID3D12Device>& device, ComPtr<ID3D12Resource>& upload)
{
    HRESULT result = S_OK;
    Vertex triangle[] =
    {
        {  0.0f,  0.5f, 0.0f, { 1.0f, 0.0f, 0.0f, 1.0f } },
        {  0.5f, -0.5f, 0.0f, { 0.0f, 1.0f, 0.0f, 1.0f } },
        { -0.5f, -0.5f, 0.0f, { 0.0f, 0.0f, 1.0f, 1.0f } },
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
    result = device->CreateCommittedResource(&properties, D3D12_HEAP_FLAG_NONE,
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
    result = device->CreateCommittedResource(&properties, D3D12_HEAP_FLAG_NONE,
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

HRESULT DirectX12::createFence(ComPtr<ID3D12Device>& device, ComPtr<ID3D12Resource>& upload)
{
    HRESULT result = device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
    
    if (FAILED(result))
        return result;
    
    m_fenceValue = 1;

    m_fenceEvt = CreateEvent(nullptr, false, false, nullptr);
    if (!m_fenceEvt)
        throw std::runtime_error("Failed to make fence event");

    // Sync with the fence to wait for the assets to upload
    m_signalVal = m_fenceValue++;
    return syncFetch(m_signalVal);
}

HRESULT DirectX12::syncFetch(uint32_t signalVal)
{
    HRESULT result = m_cmdQueue->Signal(m_fence.Get(), signalVal);

    if (FAILED(result))
        return result;

    if (m_fence->GetCompletedValue() < m_signalVal)
    {
        result = m_fence->SetEventOnCompletion(m_signalVal, m_fenceEvt);

        if (FAILED(result))
            return result;

        WaitForSingleObject(m_fenceEvt, INFINITE);
    }

    return result;
}

void DirectX12::firstFrame(ComPtr<ID3D12Resource>& upload)
{
    // Build the command list to clear the frame color
    throwIfFailed(m_cmdAllocator->Reset(), "Can't reset command allocator");

    throwIfFailed(m_cmdList->Reset(m_cmdAllocator.Get(), m_pipelineState.Get()),
        "Can't reset command list");

    m_cmdList->SetGraphicsRootSignature(m_rootSignature.Get());
    m_cmdList->RSSetViewports(1, &m_viewport);
    m_cmdList->RSSetScissorRects(1, &m_screenBounds);

    D3D12_RESOURCE_BARRIER barrier;
    ZeroMemory(&barrier, sizeof(barrier));

    // On frame 0 we need to do the GPU-side copy of our vertex data over
    // Transition vbo buffer to a copy dest buffer
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = m_vbo.Get();
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;

    m_cmdList->ResourceBarrier(1, &barrier);

    // Now enqueue the copy
    m_cmdList->CopyResource(m_vbo.Get(), upload.Get());
    m_cmdList->ResourceBarrier(1, &barrier);

    // Back buffer will be used ad render target
    barrier.Transition.pResource = m_renderTargets[m_swapChain->GetCurrentBackBufferIndex()].Get();
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    m_cmdList->ResourceBarrier(1, &barrier);

    D3D12_CPU_DESCRIPTOR_HANDLE renderTarget = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
    renderTarget.ptr += m_rtvDescriptorSize * m_swapChain->GetCurrentBackBufferIndex();
    m_cmdList->OMSetRenderTargets(1, &renderTarget, false, nullptr);

    m_cmdList->ClearRenderTargetView(renderTarget, m_clearColor.data(), 0, nullptr);
    m_cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_cmdList->IASetVertexBuffers(0, 1, &m_vboView);
    m_cmdList->DrawInstanced(3, 1, 0, 0);

    // Back buffer will now be used to present
    barrier.Transition.pResource = m_renderTargets[m_swapChain->GetCurrentBackBufferIndex()].Get();
    m_cmdList->ResourceBarrier(1, &barrier);

    throwIfFailed(m_cmdList->Close(), "Can't close cmd list");

    // Execute the command list and present
    std::array<ID3D12CommandList*, 1> cmdLists = { m_cmdList.Get() };
    m_cmdQueue->ExecuteCommandLists(static_cast<UINT>(cmdLists.size()), cmdLists.data());
    throwIfFailed(m_swapChain->Present(1, 0), "Can't present swap chain");

    // Sync with the fence to wait for the frame to be presented
    uint32_t signalVal = m_fenceValue++;

    throwIfFailed(syncFetch(signalVal), "Can't sync fetch");

    upload = nullptr;
}

void DirectX12::render()
{
    // Build the command list to clear the frame color
    throwIfFailed(m_cmdAllocator->Reset(), "Can't reset command allocator");

    throwIfFailed(m_cmdList->Reset(m_cmdAllocator.Get(), m_pipelineState.Get()),
        "Can't reset command list");

    m_cmdList->SetGraphicsRootSignature(m_rootSignature.Get());
    m_cmdList->RSSetViewports(1, &m_viewport);
    m_cmdList->RSSetScissorRects(1, &m_screenBounds);

    // Back buffer will be used ad render target
    D3D12_RESOURCE_BARRIER barrier;
    ZeroMemory(&barrier, sizeof(barrier));
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = m_renderTargets[m_swapChain->GetCurrentBackBufferIndex()].Get();
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    m_cmdList->ResourceBarrier(1, &barrier);

    D3D12_CPU_DESCRIPTOR_HANDLE renderTarget = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
    renderTarget.ptr += m_rtvDescriptorSize * m_swapChain->GetCurrentBackBufferIndex();
    m_cmdList->OMSetRenderTargets(1, &renderTarget, false, nullptr);

    m_cmdList->ClearRenderTargetView(renderTarget, m_clearColor.data(), 0, nullptr);
    m_cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_cmdList->IASetVertexBuffers(0, 1, &m_vboView);
    m_cmdList->DrawInstanced(3, 1, 0, 0);

    // Back buffer will now be used to presen
    barrier.Transition.pResource = m_renderTargets[m_swapChain->GetCurrentBackBufferIndex()].Get();
    m_cmdList->ResourceBarrier(1, &barrier);

    throwIfFailed(m_cmdList->Close(), "Can't close cmd list");

    // Execute the command list and present
    std::array<ID3D12CommandList*, 1> cmdLists = { m_cmdList.Get() };
    m_cmdQueue->ExecuteCommandLists(static_cast<UINT>(cmdLists.size()), cmdLists.data());
    throwIfFailed(m_swapChain->Present(1, 0), "Can't present swap chain");

    // Sync with the fence to wait for the frame to be presented
    uint32_t signalVal = m_fenceValue++;
    throwIfFailed(syncFetch(signalVal), "Can't sync fetch");
}
