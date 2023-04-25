#pragma once

#include <d3d11.h>
#include <vector>
#include <wrl.h>
namespace WRL = Microsoft::WRL;

class PixelShader
{
    public:

    PixelShader(LPCWSTR filePath);

    void Bind();
    
    private:

    WRL::ComPtr<ID3D11PixelShader> shaderPtr;
};