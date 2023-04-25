#pragma once

#include <d3d11.h>
#include <vector>

#include <wrl.h>

namespace WRL = Microsoft::WRL;

class VertexShader
{
    public:

    VertexShader(LPCWSTR filePath, std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc);

    void Bind();

    private:

    WRL::ComPtr<ID3D11InputLayout> inputLayoutPtr;

    WRL::ComPtr<ID3D11VertexShader> shaderPtr;
};