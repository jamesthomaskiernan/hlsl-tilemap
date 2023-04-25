#pragma once

#include "graphics.h"
#include "application.h"
#include "window.h"
#include "input.h"
#include "graphics.h"
#include "pixel_shader.h"
#include "vertex_shader.h"
#include <vector>
#include "struct_buffer.h"
#include "vertex_buffer.h"
#include "camera_controller.h"
#include "texture.h"
#include "compute_shader.h"
#include "stdint.h"
#include "compute_shader.h"
#include "struct_buffer.h"
#include "vertex_buffer.h"
#include <cstdio>

struct Vertex
{
    float3 pos;
    float2 uv;
};

class Tilemap
{
    public:

    // If changed don't forget to update in tilemap.hlsl and world_generator.hlsl
    static inline int width = 128; 

    // Runs on program start
    static void Init();

    // Runs once per frame
    static void Update();

    private:

    // Generates a world by running the world_generator.hlsl compute shader.
    // As it runs, fills up tileBuffer with integer values.
    static void GenerateWorld();

    // Creates a vertex buffer containing a single quad, which the tilemap
    // will be rendered on.
    static void CreateVertexBuffer();

    // Compiles and binds the vertex, pixel, and compute shaders necessary
    static void CreateShaders();

    // Loads the texture atlas and binds it to tilemap.hlsl
    static void LoadTexture();

    // This buffer holds all tile data, will be read by tilemap.hlsl and
    // written to by world_generator.hlsl
    static inline StructBuffer<int32_t>* tileBuffer = nullptr;

    // Holds the vertices for a simple quad, on which the tilemap will be rendered
    static inline VertexBuffer<Vertex>* vertexBuffer = nullptr;

    // tilemap.hlsl
    static inline VertexShader* vertexShader = nullptr;

    // tilemap.hlsl
    static inline PixelShader* pixelShader = nullptr;

    // world_generator.hlsl
    static inline ComputeShader* computeShader = nullptr;
};