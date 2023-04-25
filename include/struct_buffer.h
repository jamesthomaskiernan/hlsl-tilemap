#pragma once

#include <d3d11.h>
#include <cstdint>
#include <wrl.h>
namespace WRL = Microsoft::WRL;

enum StructBufferType
{
    Read,
    ReadWrite,
    Append,
    IndirectArgs,
};

template<typename T>
class StructBuffer
{
    public:
	
    StructBuffer(StructBufferType type, uint32_t count, T* data = nullptr);

    T* GetData();

    WRL::ComPtr<ID3D11Buffer> buffer;

    WRL::ComPtr<ID3D11UnorderedAccessView> uav;

    WRL::ComPtr<ID3D11ShaderResourceView> srv;
};

#include "../src/struct_buffer.cpp"
