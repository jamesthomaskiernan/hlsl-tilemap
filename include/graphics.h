#pragma once

#include <d3d11.h>
#include <wrl/client.h>
namespace WRL = Microsoft::WRL;


class Graphics
{
    public:

    // devices

    inline static WRL::ComPtr<ID3D11Device> device = nullptr;

    inline static WRL::ComPtr<IDXGISwapChain> swapchain = nullptr;

    inline static WRL::ComPtr<ID3D11DeviceContext> context = nullptr;

    // depth buffer stuff;

    inline static WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer = nullptr;

    inline static WRL::ComPtr<ID3D11DepthStencilView> depthStencilView = nullptr;

    inline static WRL::ComPtr<ID3D11DepthStencilState> depthStencilState = nullptr;

    // other

    inline static WRL::ComPtr<ID3D11RasterizerState> rasterState = nullptr;

    inline static WRL::ComPtr<ID3D11RenderTargetView> targetView = nullptr;

    static void RenderFrame();

    static void Init();

    static void CreateDevice();

    static void CreateBackBuffer();

    static void CreateDepthStencil();

    static void CreateRasterizerState();

    static void CreateViewport();
};