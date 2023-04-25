#include "camera_controller.h"

void CameraController::Init()
{
    // Create cam and initialize it
    float aspectRatio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;    
    cam.SetProjectionValues(aspectRatio * zoomLevel, zoomLevel, .1f, 500.0f);
    cam.SetPosition(camStartingPos);
    cam.SetLookAtPos(camStartingDir);

	// Update constant Buffer
	float4x4 world = DirectX::XMMatrixIdentity();
	float4x4 mvp = world * cam.GetViewMatrix() * cam.GetProjectionMatrix();
	float4x4 mvpTransposed = DirectX::XMMatrixTranspose(mvp);

    // Create const buffer for MVP matrix
    mvpBuffer.SetData(mvpTransposed);
    mvpBuffer.Create();
    mvpBuffer.SetRegister(0);
    mvpBuffer.Bind();
}

void CameraController::Update(float dt)
{
    bool updateMVP = false;

    if (Input::GetKey('W'))
    {
        cam.AdjustPosition(0, movementSpeed * dt * zoomLevel, 0);
        updateMVP = true;
    }

    if (Input::GetKey('S'))
    {
        cam.AdjustPosition(0, -movementSpeed * dt * zoomLevel, 0);
        updateMVP = true;
    }

    if (Input::GetKey('D'))
    {
        cam.AdjustPosition(movementSpeed * dt * zoomLevel, 0, 0);
        updateMVP = true;
    }

    if (Input::GetKey('A'))
    {
        cam.AdjustPosition(-movementSpeed * dt * zoomLevel, 0, 0);
        updateMVP = true;
    }

    int scrollDir = Input::GetMouseScroll();

    // If user changes scroll wheel
    if (scrollDir != 0)
    {
        float aspectRatio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;    
        
        // Make sure new scroll value is not negative
        if ((zoomLevel - scrollDir * .01f) * aspectRatio > .0001f)
        {
            zoomLevel -= scrollDir * .01f;

            // Set new zoom level, and update MVP
            cam.SetProjectionValues(aspectRatio * zoomLevel, zoomLevel, .1f, 500.0f);
            updateMVP = true;
        }
    }

    // Update MVP buffer if any changes happened
    if (updateMVP)
    {
        float4x4 world = DirectX::XMMatrixIdentity();
        float4x4 mvp = world * cam.GetViewMatrix() * cam.GetProjectionMatrix();
        float4x4 mvpTransposed = DirectX::XMMatrixTranspose(mvp);

        mvpBuffer.SetData(mvpTransposed);
        mvpBuffer.Refresh();
    }

}