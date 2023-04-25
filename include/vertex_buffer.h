#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include <memory>
#include <wrl.h>
namespace WRL = Microsoft::WRL;

template<typename T>
class VertexBuffer
{
    public:

	VertexBuffer(std::vector<T> &data);

    void Bind();

    private:
    
    WRL::ComPtr<ID3D11Buffer> buffer = nullptr;
};

#include "../src/vertex_buffer.cpp"