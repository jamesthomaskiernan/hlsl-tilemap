#ifndef CONST_BUFFER_CPP
#define CONST_BUFFER_CPP

#include "const_buffer.h"
#include "graphics.h"
#include "debug.h"

template <typename T>
ConstBuffer<T>::ConstBuffer() {};

template <typename T>
void ConstBuffer<T>::SetData(T &data)
{
    this->data = &data;
}

template <typename T>
void ConstBuffer<T>::SetRegister(UINT reg)
{
    this->reg = reg;
}

template <typename T>
void ConstBuffer<T>::Create()
{
    // create buffer description
    D3D11_BUFFER_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    desc.MiscFlags = 0;
    desc.ByteWidth = static_cast<UINT>(sizeof(T) + (16 - (sizeof(T) % 16)));
    desc.StructureByteStride = 0;

    // create buffer
    HRESULT hr = Graphics::device->CreateBuffer(&desc, 0, buffer.GetAddressOf());
    Debug(hr, "failed to create const buffer");

    // update data
    Refresh();
};

template <typename T>
void ConstBuffer<T>::Refresh()
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HRESULT hr = Graphics::context->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    Debug(hr, "failed to map const buffer");
    CopyMemory(mappedResource.pData, data, sizeof(T));
    Graphics::context->Unmap(buffer.Get(), 0);
};

template <typename T>
void ConstBuffer<T>::Bind()
{
    Graphics::context->VSSetConstantBuffers(reg, 1, buffer.GetAddressOf());
}

#endif