#ifndef STRUCT_BUFFER_CPP
#define STRUCT_BUFFER_CPP

#include "struct_buffer.h"
#include "graphics.h"
#include "debug.h"

template <typename T>
StructBuffer<T>::StructBuffer(StructBufferType type, uint32_t count, T* data)
{
    uint32_t stride = sizeof(T);

    // create description for buffer
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.BindFlags = (type == StructBufferType::Read) ? D3D11_BIND_SHADER_RESOURCE : D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
    bufferDesc.MiscFlags = (type == StructBufferType::IndirectArgs) ? D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS : D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    bufferDesc.ByteWidth = stride * count;
    bufferDesc.StructureByteStride = stride;

    // if data passed in
    if (data)
    {
        D3D11_SUBRESOURCE_DATA initData;
        initData.pSysMem = data;
        HRESULT HR = Graphics::device->CreateBuffer(&bufferDesc, &initData, buffer.GetAddressOf());
        Debug(HR, "failed to create struct buffer with provided data");
    }
    // if no data
    else
    {
        HRESULT HR = Graphics::device->CreateBuffer(&bufferDesc, nullptr, buffer.GetAddressOf());
        Debug(HR, "failed to create struct buffer");
    }
    
    // create SRV
    if (type != StructBufferType::IndirectArgs)
    {
        D3D11_SHADER_RESOURCE_VIEW_DESC standardViewDesc = {};
        standardViewDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
        standardViewDesc.Format = DXGI_FORMAT_UNKNOWN;
        standardViewDesc.Buffer.NumElements = count;
        
        HRESULT HR = Graphics::device->CreateShaderResourceView(buffer.Get(), &standardViewDesc, srv.GetAddressOf());
        Debug(HR, "failed to create SRV for struct buffer");
    }

    // create UAV
    if (type == StructBufferType::Append || type == StructBufferType::ReadWrite)
    {
        D3D11_UNORDERED_ACCESS_VIEW_DESC unorderedViewDesc = {};
        unorderedViewDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
        unorderedViewDesc.Format = DXGI_FORMAT_UNKNOWN;
        unorderedViewDesc.Buffer.NumElements = count;
        if (type == StructBufferType::Append) {unorderedViewDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_APPEND;};
        
        HRESULT HR = Graphics::device->CreateUnorderedAccessView(buffer.Get(), &unorderedViewDesc, uav.GetAddressOf());
        Debug(HR, "failed to create UAV for struct buffer");
    }
};

// template <typename T>
// T* StructBuffer<T>::GetData()
// {    
//     ID3D11Buffer* copyBuffer = nullptr;

//     // create copyBuffer description
//     D3D11_BUFFER_DESC desc = {};
//     buffer->GetDesc(&desc);
//     desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
//     desc.Usage = D3D11_USAGE_STAGING;
//     desc.BindFlags = 0;
//     desc.MiscFlags = 0;

//     // create copyBuffer and copy data to it
//     Graphics::devicePtr->CreateBuffer(&desc, nullptr, &copyBuffer);
//     Graphics::contextPtr->CopyResource(copyBuffer, buffer->Get());
        
//     // map buffer to mappedResource
//     D3D11_MAPPED_SUBRESOURCE mappedResource; 
//     Graphics::contextPtr->Map(copyBuffer, 0, D3D11_MAP_READ, 0, &mappedResource);
//     return (T*)mappedResource.pData;
// };

#endif