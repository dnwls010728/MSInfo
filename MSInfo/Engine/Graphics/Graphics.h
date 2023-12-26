#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

#include "../Singleton.h"

#include <d3d11.h>
#include <d2d1.h>
#include <dwrite.h>

class Graphics : public Singleton<Graphics>
{
public:
    Graphics();
    virtual ~Graphics() final;

    bool Init();
    
    void BeginRenderD3D();
    void EndRenderD3D();
    void DrawRectangle(RECT position);

    inline ID3D11Device* GetD3DDevice() const { return d3d_device_; }
    inline ID3D11DeviceContext* GetD3DDeviceContext() const { return d3d_device_context_; }
    inline void BeginRenderD2D() const { d2d_render_target_->BeginDraw(); }
    inline void EndRenderD2D() const { d2d_render_target_->EndDraw(); }

private:
    bool InitDeviceD3D();
    bool InitRenderTargetD3D();
    bool InitFactoryD2D();
    bool InitRenderTargetD2D();
    
    ID3D11Device* d3d_device_;
    ID3D11DeviceContext* d3d_device_context_;
    ID3D11RenderTargetView* d3d_render_target_view_;
    D3D11_VIEWPORT d3d_viewport_;
    
    IDXGISwapChain* dxgi_swap_chain_;

    ID2D1Factory* d2d_factory_;
    ID2D1RenderTarget* d2d_render_target_;
    
};
