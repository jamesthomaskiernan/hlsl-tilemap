#include "vertex_shader.h"
#include "graphics.h"
#include "d3dcompiler.h"
#include "debug.h"
#include "window.h"

VertexShader::VertexShader(LPCWSTR filePath, std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc)
{
    // define blobs
    ID3DBlob *errorBlob = NULL;
    ID3DBlob *vertexBlob = NULL;

    // define flags
    UINT flags = D3DCOMPILE_DEBUG;
    // UINT flags = D3DCOMPILE_ENABLE_STRICTNESS; // use for release 

    // compile vertex shader
    HRESULT HR = D3DCompileFromFile(
        filePath,
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "Vertex",
        "vs_5_0",
        flags,
        0,
        &vertexBlob,
        &errorBlob
    );

    Debug(errorBlob, "failed to compile vertex shader from file");

    // create vertex shader
    HR = Graphics::device->CreateVertexShader(
    vertexBlob->GetBufferPointer(),
    vertexBlob->GetBufferSize(),
    NULL,
    shaderPtr.GetAddressOf());
    Debug(HR, "failed to create vertex shader");
    
    // create input layout
    HR = Graphics::device->CreateInputLayout(
        &inputDesc[0],
        inputDesc.size(),
        vertexBlob->GetBufferPointer(),
        vertexBlob->GetBufferSize(),
        inputLayoutPtr.GetAddressOf());
    Debug(HR, "failed to create input layout for vertex shader");
}

void VertexShader::Bind()
{
    Graphics::context->VSSetShader(shaderPtr.Get(), NULL, 0);
    Graphics::context->IASetInputLayout(inputLayoutPtr.Get());
}