#include "Graphics.h"

#include "../Core.h"

Graphics::Graphics()
{
}

Graphics::~Graphics()
{
    if (d2d_render_target_) d2d_render_target_->Release();
    if (d2d_factory_) d2d_factory_->Release();
    if (d3d_render_target_view_) d3d_render_target_view_->Release();
    if (dxgi_swap_chain_) dxgi_swap_chain_->Release();
    if (d3d_device_context_) d3d_device_context_->Release();
    if (d3d_device_) d3d_device_->Release();
}

bool Graphics::Init()
{
    return InitDeviceD3D() && InitFactoryD2D();
}

bool Graphics::InitDeviceD3D()
{
    DXGI_SWAP_CHAIN_DESC swap_chain_desc;
    ZeroMemory(&swap_chain_desc, sizeof(DXGI_SWAP_CHAIN_DESC));

    swap_chain_desc.BufferDesc.Width = Core::GetInstance()->GetResolution().x;
    swap_chain_desc.BufferDesc.Height = Core::GetInstance()->GetResolution().y;
    swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
    swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
    swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    swap_chain_desc.SampleDesc.Count = 1;
    swap_chain_desc.SampleDesc.Quality = 0;

    swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swap_chain_desc.BufferCount = 1;
    swap_chain_desc.OutputWindow = Core::GetInstance()->GetWindowHandle();
    swap_chain_desc.Windowed = TRUE;
    swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swap_chain_desc.Flags = 0;

    constexpr D3D_FEATURE_LEVEL feature_levels[] = {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1
    };

    HRESULT result = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        D3D11_CREATE_DEVICE_BGRA_SUPPORT,
        feature_levels,
        2,
        D3D11_SDK_VERSION,
        &swap_chain_desc,
        &dxgi_swap_chain_,
        &d3d_device_,
        nullptr,
        &d3d_device_context_
    );

    if (FAILED(result)) return false;
    return InitRenderTargetD3D();
}

bool Graphics::InitRenderTargetD3D()
{
    ID3D11Texture2D* back_buffer;
    
    HRESULT result = dxgi_swap_chain_->GetBuffer(0, IID_PPV_ARGS(&back_buffer));
    if (FAILED(result)) return false;

    result = d3d_device_->CreateRenderTargetView(back_buffer, nullptr, &d3d_render_target_view_);
    back_buffer->Release();

    d3d_viewport_.TopLeftX = 0;
    d3d_viewport_.TopLeftY = 0;
    d3d_viewport_.Width = static_cast<float>(Core::GetInstance()->GetResolution().x);
    d3d_viewport_.Height = static_cast<float>(Core::GetInstance()->GetResolution().y);
    d3d_viewport_.MinDepth = 0.0f;
    d3d_viewport_.MaxDepth = 1.0f;

    return SUCCEEDED(result);
}

bool Graphics::InitFactoryD2D()
{
    HRESULT result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2d_factory_);

    if (FAILED(result)) return false;
    return InitRenderTargetD2D();
}

bool Graphics::InitRenderTargetD2D()
{
    const float dpi = GetDpiForWindow(Core::GetInstance()->GetWindowHandle());
    const D2D1_RENDER_TARGET_PROPERTIES render_target_properties = D2D1::RenderTargetProperties(
        D2D1_RENDER_TARGET_TYPE_DEFAULT,
        D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
        dpi,
        dpi
    );

    IDXGISurface* back_buffer;
    
    HRESULT result = dxgi_swap_chain_->GetBuffer(0, IID_PPV_ARGS(&back_buffer));
    if (FAILED(result)) return false;

    result = d2d_factory_->CreateDxgiSurfaceRenderTarget(back_buffer, &render_target_properties, &d2d_render_target_);
    back_buffer->Release();

    return SUCCEEDED(result);
}

void Graphics::BeginRenderD3D()
{
    constexpr float clear_color[] = { 0.f, 0.f, 0.f, 1.f };

    d3d_device_context_->OMSetRenderTargets(1, &d3d_render_target_view_, nullptr);
    d3d_device_context_->ClearRenderTargetView(d3d_render_target_view_, clear_color);
    d3d_device_context_->RSSetViewports(1, &d3d_viewport_);
}

void Graphics::EndRenderD3D()
{
    if (dxgi_swap_chain_->Present(1, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED) return;
    dxgi_swap_chain_->Present(0, 0);
}

void Graphics::DrawRectangle(RECT position)
{
    const D2D1_RECT_F rect = D2D1::RectF(
        static_cast<float>(position.left),
        static_cast<float>(position.top),
        static_cast<float>(position.right),
        static_cast<float>(position.bottom)
    );

    ID2D1SolidColorBrush* brush;
    d2d_render_target_->CreateSolidColorBrush(
        D2D1::ColorF(
            1.f,
            0.f,
            0.f,
            1.f
        ),
        &brush
    );
    
    d2d_render_target_->DrawRectangle(&rect, brush, true);

    brush->Release();
}
