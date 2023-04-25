#pragma once

#include "SimpleMath.h"
#include "Camera.h"
#include "input.h"
#include "settings.h"
#include "const_buffer.h"

class CameraController
{
    public:

    static void Init();

    static void Update(float dt);

    static inline OrthographicCamera cam = OrthographicCamera();

    static inline ConstBuffer<float4x4> mvpBuffer = ConstBuffer<float4x4>();

    static inline float movementSpeed = 2.0f;

    static inline float zoomLevel = .1f;
    
    static inline float3 camStartingPos = float3(0, 0, -1.0f);

    static inline float3 camStartingDir = float3(0, 0, 0);
};