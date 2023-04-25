#pragma once
#include "SimpleMath.h"

class Camera
{
    public:
	
    Camera();
	
    // set/get position

    void SetPosition(float x, float y, float z);
	
    void SetPosition(float3 &pos);
    
    const float3 & GetPosition() const;
    
    // adjust position

    void AdjustPosition(float x, float y, float z);

    void AdjustPosition(float3 &adjustment);
    
    // set/get rotation
    
    void SetRotation(float x, float y, float z);

    void SetRotation(float3 &rot);

    const float3 & GetRotation() const;

    // adjust rotation

    void AdjustRotation(float x, float y, float z);

    void AdjustRotation(float3 &adjustment);
    
    // set/get matrices

    const float4x4 & GetViewMatrix() const;
	
    const float4x4 & GetProjectionMatrix() const;

    void SetLookAtPos(float3 lookAtPos);

    // vectors
	
    const float4 & GetForwardVector();
	
    const float4 & GetRightVector();
	
    const float4 & GetBackwardVector();
	
    const float4 & GetLeftVector();

    protected:

	void UpdateViewMatrix();
	
    // transform

	float3 pos;
	
    float3 rot;

    // matrices

	float4x4 viewMatrix;
	
    float4x4 projectionMatrix;

    // vectors

	float4 vecForward;
	
    float4 vecLeft;
	
    float4 vecRight;
	
    float4 vecBackward;

    // default vectors

	const float4 DEFAULT_FORWARD_VECTOR = float4(0.0f, 0.0f, 1.0f, 0.0f);
	
    const float4 DEFAULT_UP_VECTOR = float4(0.0f, 1.0f, 0.0f, 0.0f);
	
    const float4 DEFAULT_BACKWARD_VECTOR = float4(0.0f, 0.0f, -1.0f, 0.0f);
	
    const float4 DEFAULT_LEFT_VECTOR = float4(-1.0f, 0.0f, 0.0f, 0.0f);
	
    const float4 DEFAULT_RIGHT_VECTOR = float4(1.0f, 0.0f, 0.0f, 0.0f);
};

class OrthographicCamera : public Camera
{
    public:

    void SetProjectionValues(float viewWidth, float viewHeight, float nearZ, float farZ);
};

class PerspectiveCamera : public Camera
{
    public:

    void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);
};

// world:
// scale
// rotation
// translate

// view matrix:
// eye
// look at
// up

// proj matrix:
// near
// far
// fov