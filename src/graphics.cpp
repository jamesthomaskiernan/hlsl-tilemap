#include "graphics.h"
#include "d3dcompiler.h"
#include "debug.h"
#include "window.h"
#include "settings.h"

void Graphics::CreateDevice()
{
    // create a struct to hold information about the swap chain
    DXGI_SWAP_CHAIN_DESC swapChainDesc;

    // clear out the struct for use
    ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

    // fill the swap chain description struct
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.BufferDesc.Width = WINDOW_WIDTH;
    swapChainDesc.BufferDesc.Height = WINDOW_HEIGHT;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
    swapChainDesc.SampleDesc.Count = 1;                               // antialiasing
    swapChainDesc.SampleDesc.Quality = 0;                             // antialiasing
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
    swapChainDesc.BufferCount = 1;                                    // one back buffer
    swapChainDesc.OutputWindow = Window::windowHandle;                // the window to be used
    swapChainDesc.Windowed = true;                                    // windowed/full-screen mode
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;              // default swap effect

    // create a device, device context and swap chain using the information in the scd struct
    HRESULT HR = D3D11CreateDeviceAndSwapChain(0,
                                D3D_DRIVER_TYPE_HARDWARE,
                                0,
                                0,
                                0,
                                0,
                                D3D11_SDK_VERSION,
                                &swapChainDesc,
                                swapchain.GetAddressOf(),
                                device.GetAddressOf(),
                                0,
                                context.GetAddressOf()
    );
    
    Debug(HR, "failed to create device and swapchain");
}

void Graphics::CreateBackBuffer()
{
    // find backbuffer and use it to make targetview
    ID3D11Texture2D* backBuffer = nullptr;

    // get the address of the back buffer
    HRESULT HR = swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
    Debug(HR, "failed to find back buffer");

    // use the back buffer address to create the render target
    HR = device->CreateRenderTargetView(backBuffer, NULL, targetView.GetAddressOf());
    Debug(HR, "failed to create back buffer render target");
    backBuffer->Release();
}

void Graphics::CreateDepthStencil()
{
    // create depth stencil and view
    D3D11_TEXTURE2D_DESC depthStencilDesc;
    depthStencilDesc.Width = WINDOW_WIDTH;
    depthStencilDesc.Height = WINDOW_HEIGHT;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;

    // create depth stencil buffer
    HRESULT HR = device->CreateTexture2D(&depthStencilDesc, NULL, depthStencilBuffer.GetAddressOf());
    Debug(HR, "failed to create 2D texture for depth stencil");

    // create depth stencil view from that buffer
    HR = device->CreateDepthStencilView(depthStencilBuffer.Get(), NULL, depthStencilView.GetAddressOf());
    Debug(HR, "failed to create depth stencil view from depth stencil buffer");
    
    // create depth stencil state description
    D3D11_DEPTH_STENCIL_DESC dsDesc;

    // Depth test parameters
    dsDesc.DepthEnable = true;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;

    device->CreateDepthStencilState(&dsDesc, depthStencilState.GetAddressOf());
    context->OMSetDepthStencilState(depthStencilState.Get(), 1);
}

void Graphics::CreateViewport()
{    
    D3D11_VIEWPORT viewport;
    viewport.Width = WINDOW_WIDTH;
    viewport.Height = WINDOW_HEIGHT;
    viewport.MinDepth = 0;
    viewport.MaxDepth = 1;
    context->RSSetViewports(1, &viewport);
}

void Graphics::Init()
{
    CreateDevice();
    CreateBackBuffer();
    CreateDepthStencil();
    CreateViewport();
    CreateRasterizerState();

    // set the render target as the back buffer
    context->OMSetRenderTargets(1, targetView.GetAddressOf(), depthStencilView.Get());
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Graphics::CreateRasterizerState()
{
    D3D11_RASTERIZER_DESC rasterizerDesc = {};
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_BACK;
    rasterizerDesc.FrontCounterClockwise = false;
    rasterizerDesc.DepthBias = .01f; // Set depth bias here
    rasterizerDesc.DepthBiasClamp = 0.1f;
    rasterizerDesc.SlopeScaledDepthBias = 1.0f;
    rasterizerDesc.DepthClipEnable = true;
    rasterizerDesc.ScissorEnable = false;
    rasterizerDesc.MultisampleEnable = false;
    rasterizerDesc.AntialiasedLineEnable = false;

    ID3D11RasterizerState* rasterizerState;
    Graphics::device->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
    Graphics::context->RSSetState(rasterizerState);
}

void Graphics::RenderFrame()
{
    // render frame
    swapchain->Present(VSYNC_ENABLED, 0);

    // clear back buffer and depth
    float background_colour[4] = {1, 1, 1, 1};
    context->ClearRenderTargetView(targetView.Get(), background_colour);
    context->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}