#include "compute_shader.h"
#include "d3dcompiler.h"
#include "debug.h"

ComputeShader::ComputeShader(LPCWSTR filePath)
{
    // vars 
    const UINT flags = D3DCOMPILE_DEBUG;
    const LPCSTR functionName = "Compute";
    const LPCSTR featureLevel = "cs_5_0";

    // blobs
    ID3DBlob* errorBlob = nullptr;
    ID3DBlob* computeBlob = nullptr;
    
    // compile shader
    HRESULT HR = D3DCompileFromFile(filePath,
        nullptr, // defines (using none for now)
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        functionName,
        featureLevel,
        flags,
        0,
        &computeBlob,
        &errorBlob);
    Debug(errorBlob, "failed to compile compute shader from file");

    // create shader
    HR = Graphics::device->CreateComputeShader(computeBlob->GetBufferPointer(), computeBlob->GetBufferSize(), nullptr, &shaderPtr);
    Debug(HR, "failed to create compute shader");

    // set private data
    shaderPtr->SetPrivateData(WKPDID_D3DDebugObjectName, lstrlenA(functionName), functionName);
};

void ComputeShader::Dispatch(UINT x, UINT y, UINT z)
{
    Graphics::context->CSSetShader(shaderPtr, nullptr, 0);
    Graphics::context->Dispatch(x, y, z);
    Graphics::context->CSSetShader(nullptr, nullptr, 0);
}
