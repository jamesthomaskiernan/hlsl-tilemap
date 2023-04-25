#include "tilemap.h"

void Tilemap::Init()
{    
    CreateShaders();
    CreateVertexBuffer();
    LoadTexture();

    tileBuffer = new StructBuffer<int32_t>(ReadWrite, width * width);

    GenerateWorld();
}

void Tilemap::Update()
{
    Graphics::context->Draw(6, 0);
}

void Tilemap::CreateVertexBuffer()
{
    // Vertices for tilemap (just a quad, made of 2 tris)
    std::vector<Vertex> verts = std::vector<Vertex>();
    
    // Push first triangle
    verts.push_back({float3(-0.5f, -0.5f, 0), float2(0, 0)}); // bottom left
    verts.push_back({float3(-0.5f, 0.5f, 0), float2(0, 1)}); // top left
    verts.push_back({float3(0.5f, -0.5f, 0), float2(1, 0)}); // bottom right

    // Push second triangle
    verts.push_back({float3(0.5f, -0.5f, 0), float2(1, 0)}); // bottom right
    verts.push_back({float3(-0.5f, 0.5f, 0), float2(0, 1)}); // top left
    verts.push_back({float3(0.5f, 0.5f, 0), float2(1, 1)}); // top right

    // Create and bind vertex buffer
    vertexBuffer = new VertexBuffer<Vertex>(verts);
    vertexBuffer->Bind();
}

void Tilemap::CreateShaders()
{
    // Input format for vertex shader
    std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc;
    inputDesc.push_back({ "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
    inputDesc.push_back({ "TEX", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });

    // Create and bind shaders
    vertexShader = new VertexShader(L"../shaders/tilemap.hlsl", inputDesc);
    vertexShader->Bind();
    pixelShader = new PixelShader(L"../shaders/tilemap.hlsl");
    pixelShader->Bind();
    computeShader = new ComputeShader(L"../shaders/world_generator.hlsl");
}

void Tilemap::LoadTexture()
{
    Texture tex = Texture( "../textures/tileset.png");
    Graphics::context->PSSetSamplers(0, 1, tex.samplerState.GetAddressOf());
    Graphics::context->PSSetShaderResources(0, 1, tex.resourceView.GetAddressOf());
}

void Tilemap::GenerateWorld()
{
    // Bind uav of buffer to compute shader prior to generation
    Graphics::context->CSSetUnorderedAccessViews(0, 1, tileBuffer->uav.GetAddressOf(), nullptr);
    computeShader->Dispatch(width / 8, width / 8, 1);
    
    // Unbind UAV 
    ID3D11UnorderedAccessView *empty = nullptr;
    Graphics::context->CSSetUnorderedAccessViews(0, 1, &empty, nullptr);
    
    // Set tiles on tilemap
    Graphics::context->PSSetShaderResources(1, 1, tileBuffer->srv.GetAddressOf());
}