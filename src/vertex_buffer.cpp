#ifndef VERTEX_BUFFER_CPP
#define VERTEX_BUFFER_CPP

#include "vertex_buffer.h"
#include "graphics.h"
#include "debug.h"

template <typename T>
VertexBuffer<T>::VertexBuffer(std::vector<T> &data)
{
	uint32_t bufferSize = data.size();

    // create buffer description
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(T) * bufferSize; // entire length of buffer in memory
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

    // create vertex data resource
	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = &data[0];

    // create buffer
	HRESULT HR = Graphics::device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, buffer.GetAddressOf());
	Debug(HR, "failed to create vertex buffer");
};

template <typename T>
void VertexBuffer<T>::Bind()
{
	const UINT offset = 0;
	UINT stride = sizeof(T);
    Graphics::context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    Graphics::context->IASetVertexBuffers(0, 1, buffer.GetAddressOf(), &stride, &offset);
};

#endif