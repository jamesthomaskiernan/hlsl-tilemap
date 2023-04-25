#include "camera.h"

using namespace DirectX;

Camera::Camera()
{
	this->pos = float3(0.0f, 0.0f, 0.0f);
	this->rot = float3(0.0f, 0.0f, 0.0f);
	this->UpdateViewMatrix();
}

const float4x4 & Camera::GetViewMatrix() const
{
	return this->viewMatrix;
}

const float4x4 & Camera::GetProjectionMatrix() const
{
	return this->projectionMatrix;
}

const float3 & Camera::GetPosition() const
{
	return this->pos;
}

const float3 & Camera::GetRotation() const
{
	return this->rot;
}

void Camera::SetPosition(float x, float y, float z)
{
	this->pos = float3(x, y, z);
	this->UpdateViewMatrix();
}

void Camera::SetPosition(float3 &pos)
{
	this->pos += pos;
	this->UpdateViewMatrix();
}

void Camera::AdjustPosition(float x, float y, float z)
{
	this->pos.x += x;
	this->pos.y += y;
	this->pos.z += z;
	this->UpdateViewMatrix();
}

void Camera::AdjustPosition(float3 &adjustment)
{
	this->pos += adjustment;
	this->UpdateViewMatrix();
}

void Camera::SetRotation(float x, float y, float z)
{
	this->rot = float3(x, y, z);
	this->UpdateViewMatrix();
}

void Camera::SetRotation(float3 &rot)
{
	this->rot+= rot;
	this->UpdateViewMatrix();
}

void Camera::AdjustRotation(float x, float y, float z)
{
	this->rot.x += x;
	this->rot.y += y;
	this->rot.z += z;
	this->UpdateViewMatrix();
}

void Camera::AdjustRotation(float3 &adjustment)
{
	this->rot += adjustment;
	this->UpdateViewMatrix();
}

void Camera::SetLookAtPos(float3 lookAtPos)
{
	// verify that look at pos is not the same as cam pos; they cannot be the same as that wouldn't make sense and would result in undefined behavior.
	if (lookAtPos.x == this->pos.x && lookAtPos.y == this->pos.y && lookAtPos.z == this->pos.z)
		return;

	lookAtPos.x = this->pos.x - lookAtPos.x;
	lookAtPos.y = this->pos.y - lookAtPos.y;
	lookAtPos.z = this->pos.z - lookAtPos.z;

	float pitch = 0.0f;
	if (lookAtPos.y != 0.0f)
	{
		const float distance = sqrt(lookAtPos.x * lookAtPos.x + lookAtPos.z * lookAtPos.z);
		pitch = atan(lookAtPos.y / distance);
	}

	float yaw = 0.0f;
	if (lookAtPos.x != 0.0f)
	{
		yaw = atan(lookAtPos.x / lookAtPos.z);
	}
	if (lookAtPos.z > 0)
		yaw += DirectX::XM_PI;

	this->SetRotation(pitch, yaw, 0.0f);
}

const float4 & Camera::GetForwardVector()
{
	return this->vecForward;
}

const float4 & Camera::GetRightVector()
{
	return this->vecRight;
}

const float4 & Camera::GetBackwardVector()
{
	return this->vecBackward;
}

const float4 & Camera::GetLeftVector()
{
	return this->vecLeft;
}

void Camera::UpdateViewMatrix() //Updates view matrix and also updates the movement vectors
{
	// calculate camera rotation matrix
	float4x4 camRotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z);
	
	// calculate unit vector of cam target based off camera forward value transformed by cam rotation matrix
	float4 camTarget = DirectX::XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, camRotationMatrix);
	
	// adjust cam target to be offset by the camera's current position
	camTarget += float4(this->pos.x, this->pos.y, this->pos.z, 1);
	
	// calculate up direction based on current rotation
	float4 upDir = DirectX::XMVector3TransformCoord(this->DEFAULT_UP_VECTOR, camRotationMatrix);
	
	// rebuild view matrix
	this->viewMatrix = DirectX::XMMatrixLookAtLH(this->pos, camTarget, upDir);

	float4x4 vecRotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(0.0f, this->rot.y, 0.0f);
	this->vecForward = DirectX::XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
	this->vecBackward = DirectX::XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
	this->vecLeft = DirectX::XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR, vecRotationMatrix);
	this->vecRight = DirectX::XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
}

void PerspectiveCamera::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
	float fovRadians = (fovDegrees / 360.0f) * DirectX::XM_2PI;
	this->projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);
}

void OrthographicCamera::SetProjectionValues(float viewWidth, float viewHeight, float nearZ, float farZ)
{
	this->projectionMatrix = DirectX::XMMatrixOrthographicLH(viewWidth, viewHeight, nearZ, farZ);
}