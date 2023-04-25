#pragma once

#include <string>
#include "d3d11.h"
#include <wrl.h>
namespace WRL = Microsoft::WRL;

class Texture
{
    public:

    Texture(std::string filePath, DXGI_FORMAT texFormat = DXGI_FORMAT_R8G8B8A8_UNORM);

    WRL::ComPtr<ID3D11SamplerState> samplerState;

    WRL::ComPtr<ID3D11Texture2D> textureResource;

    WRL::ComPtr<ID3D11ShaderResourceView> resourceView;
};