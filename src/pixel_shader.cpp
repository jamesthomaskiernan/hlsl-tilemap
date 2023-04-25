#include "pixel_shader.h"
#include "graphics.h"
#include "d3dcompiler.h"
#include "debug.h"
#include "window.h"

PixelShader::PixelShader(LPCWSTR filePath)
{
    // define blobs
    ID3DBlob *errorBlob = NULL;
    ID3DBlob *pixelBlob = NULL;

    // define flags
    UINT flags = D3DCOMPILE_DEBUG;
    
    // use this flag for release
    // UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
    
    // compile pixel shader
    HRESULT HR = D3DCompileFromFile(
        filePath,
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "Pixel",
        "ps_5_0",
        flags,
        0,
        &pixelBlob,
        &errorBlob
    );
    Debug(errorBlob, "failed to compile pixel shader from file");

    // create pixel shader
    HR = Graphics::device->CreatePixelShader(
    pixelBlob->GetBufferPointer(),
    pixelBlob->GetBufferSize(),
    NULL,
    shaderPtr.GetAddressOf());
    Debug(HR, "failed to create pixel shader");
}

void PixelShader::Bind()
{
    Graphics::context->PSSetShader(shaderPtr.Get(), NULL, 0);
}