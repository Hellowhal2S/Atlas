#include "GraphicDevice.h"
#include <wchar.h>
void FGraphicsDevice::Initialize(HWND hWindow) {
    CreateDeviceAndSwapChain(hWindow);
    CreateFrameBuffer();
    CreateDepthStencilBuffer(hWindow);
    CreateDepthStencilState();
    D3D11_RASTERIZER_DESC rasterizerdesc = {};
    rasterizerdesc.FillMode = D3D11_FILL_SOLID ;
    rasterizerdesc.CullMode = D3D11_CULL_BACK;
    Device->CreateRasterizerState(&rasterizerdesc, &RasterizerState);
}
void FGraphicsDevice::CreateDeviceAndSwapChain(HWND hWindow) {
    // �����ϴ� Direct3D ��� ������ ����
    D3D_FEATURE_LEVEL featurelevels[] = { D3D_FEATURE_LEVEL_11_0 };

    // ���� ü�� ���� ����ü �ʱ�ȭ
    DXGI_SWAP_CHAIN_DESC swapchaindesc = {};
    swapchaindesc.BufferDesc.Width = 0; // â ũ�⿡ �°� �ڵ����� ����
    swapchaindesc.BufferDesc.Height = 0; // â ũ�⿡ �°� �ڵ����� ����
    swapchaindesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // ���� ����
    swapchaindesc.SampleDesc.Count = 1; // ��Ƽ ���ø� ��Ȱ��ȭ
    swapchaindesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // ���� Ÿ������ ���
    swapchaindesc.BufferCount = 2; // ���� ���۸�
    swapchaindesc.OutputWindow = hWindow; // �������� â �ڵ�
    swapchaindesc.Windowed = TRUE; // â ���
    swapchaindesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // ���� ���

    // ����̽��� ���� ü�� ����
    HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
        D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG,
        featurelevels, ARRAYSIZE(featurelevels), D3D11_SDK_VERSION,
        &swapchaindesc, &SwapChain, &Device, nullptr, &DeviceContext);

    if (FAILED(hr)) {
        MessageBox(hWindow, L"CreateDeviceAndSwapChain failed!", L"Error", MB_ICONERROR | MB_OK);
        return;
    }

    // ���� ü�� ���� �������� (���Ŀ� ����� ����)
    SwapChain->GetDesc(&swapchaindesc);

    // ����Ʈ ���� ����
    ViewportInfo = { 0.0f, 0.0f, (float)swapchaindesc.BufferDesc.Width, (float)swapchaindesc.BufferDesc.Height, 0.0f, 1.0f };
}



void FGraphicsDevice::CreateDepthStencilBuffer(HWND hWindow) {


    RECT clientRect;
    GetClientRect(hWindow, &clientRect);
    UINT width = clientRect.right - clientRect.left;
    UINT height = clientRect.bottom - clientRect.top;

    // ����/���ٽ� �ؽ�ó ����
    D3D11_TEXTURE2D_DESC descDepth;
    ZeroMemory(&descDepth, sizeof(descDepth));
    descDepth.Width = width; // �ؽ�ó �ʺ� ����
    descDepth.Height = height; // �ؽ�ó ���� ����
    descDepth.MipLevels = 1; // �̸� ���� �� (1�� �����Ͽ� �̸� ����)
    descDepth.ArraySize = 1; // �ؽ�ó �迭�� ũ�� (1�� ���� �ؽ�ó)
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // 24��Ʈ ���̿� 8��Ʈ ���ٽ��� ���� ����
    descDepth.SampleDesc.Count = 1; // ��Ƽ���ø� ���� (1�� ���� ����)
    descDepth.SampleDesc.Quality = 0; // ���� ����Ƽ ����
    descDepth.Usage = D3D11_USAGE_DEFAULT; // �ؽ�ó ��� ���
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL; // ���� ���ٽ� ��� ���ε� ����
    descDepth.CPUAccessFlags = 0; // CPU ���� ��� ����
    descDepth.MiscFlags = 0; // ��Ÿ �÷��� ����

    HRESULT hr = Device->CreateTexture2D(&descDepth, NULL, &DepthStencilBuffer);

    if (FAILED(hr)) {
        MessageBox(hWindow, L"Failed to create depth stencilBuffer!", L"Error", MB_ICONERROR | MB_OK);
        return;
    }


    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
    ZeroMemory(&descDSV, sizeof(descDSV));
    descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // ���� ���ٽ� ����
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D; // �� Ÿ�� ���� (2D �ؽ�ó)
    descDSV.Texture2D.MipSlice = 0; // ����� �̸� �����̽� ����

    hr = Device->CreateDepthStencilView(DepthStencilBuffer, // Depth stencil texture
        &descDSV, // Depth stencil desc
        &DepthStencilView);  // [out] Depth stencil view

    if (FAILED(hr)) {
        wchar_t errorMsg[256];
        swprintf_s(errorMsg, L"Failed to create depth stencil view! HRESULT: 0x%08X", hr);
        MessageBox(hWindow, errorMsg, L"Error", MB_ICONERROR | MB_OK);
        return;
    }
}

void FGraphicsDevice::CreateDepthStencilState()
{
    // DepthStencil ���� ���� ����
    D3D11_DEPTH_STENCIL_DESC dsDesc;

    // Depth test parameters
    dsDesc.DepthEnable = true;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

    // Stencil test parameters
    dsDesc.StencilEnable = true;
    dsDesc.StencilReadMask = 0xFF;
    dsDesc.StencilWriteMask = 0xFF;

    // Stencil operations if pixel is front-facing
    dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Stencil operations if pixel is back-facing
    dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    //// DepthStencil ���� ����
    HRESULT hr = Device->CreateDepthStencilState(&dsDesc, &DepthStencilState);
    if (FAILED(hr)) {
        // ���� ó��
        return;
    }
}


void FGraphicsDevice::ReleaseDeviceAndSwapChain()
{
    if (DeviceContext)
    {
        DeviceContext->Flush(); // �����ִ� GPU ��� ����
    }

    if (SwapChain)
    {
        SwapChain->Release();
        SwapChain = nullptr;
    }

    if (Device)
    {
        Device->Release();
        Device = nullptr;
    }

    if (DeviceContext)
    {
        DeviceContext->Release();
        DeviceContext = nullptr;
    }
}

void FGraphicsDevice::CreateFrameBuffer()
{
    // ���� ü�����κ��� �� ���� �ؽ�ó ��������
    SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&FrameBuffer);

    // ���� Ÿ�� �� ����
    D3D11_RENDER_TARGET_VIEW_DESC framebufferRTVdesc = {};
    framebufferRTVdesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB; // ���� ����
    framebufferRTVdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D; // 2D �ؽ�ó

    Device->CreateRenderTargetView(FrameBuffer, &framebufferRTVdesc, &FrameBufferRTV);
}

void FGraphicsDevice::ReleaseFrameBuffer()
{
    if (FrameBuffer)
    {
        FrameBuffer->Release();
        FrameBuffer = nullptr;
    }

    if (FrameBufferRTV)
    {
        FrameBufferRTV->Release();
        FrameBufferRTV = nullptr;
    }
}

void FGraphicsDevice::ReleaseRasterizerState()
{
    if (RasterizerState)
    {
        RasterizerState->Release();
        RasterizerState = nullptr;
    }
}

void FGraphicsDevice::ReleaseDepthStencilResources()
{
    if (DepthStencilView) {
        DepthStencilView->Release();
        DepthStencilView = nullptr;
    }

    // ����/���ٽ� ���� ����
    if (DepthStencilBuffer) {
        DepthStencilBuffer->Release();
        DepthStencilBuffer = nullptr;
    }

    // ����/���ٽ� ���� ����
    if (DepthStencilState) {
        DepthStencilState->Release();
        DepthStencilState = nullptr;
    }
}

void FGraphicsDevice::Release() {
    RasterizerState->Release();

    DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);

    ReleaseFrameBuffer();
    ReleaseDepthStencilResources();
    ReleaseDeviceAndSwapChain();
}

void FGraphicsDevice::SwapBuffer() {
    SwapChain->Present(1, 0);
}
void FGraphicsDevice::Prepare()
{
    DeviceContext->ClearRenderTargetView(FrameBufferRTV, ClearColor); // ���� Ÿ�� �信 ����� ���� ������ �����͸� ����
    DeviceContext->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0); // ���� ���� �ʱ�ȭ �߰�

    DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // ���� ���� ��� ����

    DeviceContext->RSSetViewports(1, &ViewportInfo); // GPU�� ȭ���� �������� ���� ����
    DeviceContext->RSSetState(RasterizerState); //������ ������ ���� ����

    DeviceContext->OMSetDepthStencilState(DepthStencilState, 0);

    DeviceContext->OMSetRenderTargets(1, &FrameBufferRTV, DepthStencilView); // ���� Ÿ�� ����(����۸� ����Ŵ)
    DeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff); // ���� ���� ����, �⺻���� ������
}

void FGraphicsDevice::OnResize(HWND hWindow) {
    DeviceContext->OMSetRenderTargets(0, 0, 0);
    
    FrameBufferRTV->Release();
    FrameBufferRTV = nullptr;
    if (DepthStencilView) {
        DepthStencilView->Release();
        DepthStencilView = nullptr;
    }

    ReleaseFrameBuffer();

    // ������ ũ�� ��������
    RECT clientRect;
    GetClientRect(hWindow, &clientRect);
    UINT width = clientRect.right - clientRect.left;
    UINT height = clientRect.bottom - clientRect.top;

    if (width == 0 || height == 0) {
        MessageBox(hWindow, L"Invalid width or height for ResizeBuffers!", L"Error", MB_ICONERROR | MB_OK);
        return;
    }

    // SwapChain ũ�� ����
    HRESULT hr;
    hr = SwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_B8G8R8A8_UNORM, 0);  // DXGI_FORMAT_B8G8R8A8_UNORM���� �õ�
    if (FAILED(hr)) {
        // HRESULT �� �α׷� ���
        wchar_t errorMsg[256];
        //swprintf_s(errorMsg, L"ResizeBuffers failed with HRESULT: 0x%08X", hr);
        MessageBox(hWindow, L"failed", L"ResizeBuffers failed ", MB_ICONERROR | MB_OK);
        return;
    }
    
    CreateFrameBuffer();
    CreateDepthStencilBuffer(hWindow);


    DXGI_SWAP_CHAIN_DESC swapchaindesc = {};
    SwapChain->GetDesc(&swapchaindesc);
    ViewportInfo = { 0.0f, 0.0f, (float)swapchaindesc.BufferDesc.Width, (float)swapchaindesc.BufferDesc.Height, 0.0f, 1.0f };
}
