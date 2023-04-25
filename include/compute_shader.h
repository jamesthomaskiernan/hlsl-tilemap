#pragma once

#include "graphics.h"

class ComputeShader
{
    public:

    ComputeShader(LPCWSTR filePath);

    void Dispatch(UINT x, UINT y, UINT z);

    private:

    ID3D11ComputeShader* shaderPtr = nullptr;
};

// TODO: make a way to set defines, like so:
// const D3D_SHADER_MACRO defines[] = 
// {
//     "USE_STRUCTURED_BUFFERS", "1",
//     "TEST_DOUBLE", "1",
//     nullptr, nullptr
// };

// TODO: use error blob to help compiler errors