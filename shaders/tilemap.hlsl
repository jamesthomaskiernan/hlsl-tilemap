#include "noise.hlsl"

// Spritesheet texture and sampler
Texture2D tex : register(t0);
SamplerState samplerState : register(s0)
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = WRAP;
    AddressV = WRAP;
    AddressW = WRAP;
};

// User specified info about size of tielmap and texture atlas
static uint2 textureAtlasDimensions = uint2(12, 21);
static uint2 tilemapDimensions = uint2(128, 128);

// Simple camera MVP cbuffer for transforming tilemap quad from world space to screen space
cbuffer mvpBuffer : register(b0)
{
    float4x4 mvp;
}; 

// Format used by quad vertices
struct VertexInput
{
    float3 positionWorld : POS;
    float2 uv : TEX;
};

// Sent from Vertex shader to Pixel shader
struct VertexOutput
{
    float4 positionClip : SV_POSITION;
    float2 uv : TEX;
};

// This buffer holds tile value for each position in the tilemap; length is tilemap width * height.
// A value of -1 means no tile, value of 0 means tile of type 0 present, value 1 is tile type 1, etc.
// These values are used to find the corresponding position on the texture atlas.
StructuredBuffer<int> tiles : register (t1);

// Given a tile's position, gets the index of that tile on the tiles buffer
uint PositionToIndex(uint2 position)
{
    return (position.y * tilemapDimensions.x) + position.x;
}

// Given a tile's type, finds its position on the texture atlas
uint2 GetSpritePosition(int tileType)
{
    return uint2(tileType % textureAtlasDimensions.x, tileType / textureAtlasDimensions.x);
}

// Could maybe shrink ratio of y to x by ratio of tilemap dimensions, to prevetn stretched tiles
VertexOutput Vertex(VertexInput input, uint vertexID : SV_VertexID)
{
    // simply transform vertex from world space to camera space with MVP
    VertexOutput output = (VertexOutput)0;
    output.positionClip = mul(float4(input.positionWorld, 1.0f), mvp);
    output.uv = float2(input.uv.x, input.uv.y);
    return output;
}

float4 Pixel(VertexOutput input) : SV_TARGET
{   
    // The position of the tile on the tilemap
    uint2 tilePos = uint2(input.uv * tilemapDimensions);

    // The type of tile at this position (-1 is no tile)
    int tileType = tiles[PositionToIndex(tilePos)]; 

    // If no tile present, discard this pixel
    if (tileType == -1)
    {
        discard;
    }

    // The position of this tile type's texture in the texture atlas
    // Right now, I'm using this function to map the tileType to the sprite's
    // position, but the tileType could also be used to index a buffer containing
    // tile sprite positions.
    uint2 texturePos = GetSpritePosition(tileType);

    // UV of the tile
    float2 tileUV = (tilemapDimensions * input.uv) % float2(1.0f, 1.0f);

    // Shrink UV by size of atlas
    tileUV /= textureAtlasDimensions;

    // Offset UV by position of tile
    tileUV += (float2)texturePos / (float2)textureAtlasDimensions;
    
    // Sample texture
    float4 col = tex.SampleLevel(samplerState, float2(tileUV.x, tileUV.y * -1), 0);

    // If pixel is blank, defer to grass texture
    // Alternatively, you could discard the pixel here if desired
    if (col.a == 0)
    {
        float2 grassTexturePosition = float2(0.0f, 20.0f);
        tileUV = (tilemapDimensions * input.uv) % float2(1.0f, 1.0f);
        tileUV /= textureAtlasDimensions;
        tileUV += grassTexturePosition / (float2)textureAtlasDimensions;
        col = tex.Sample(samplerState, float2(tileUV.x, tileUV.y * -1), 0);
    }

    return col;
}