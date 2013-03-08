////////////////////////////////////////////////////////////////////////////////
// Filename: camera.cpp
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

namespace TD
{

	Camera::Camera()
	{
		position.x = 0.0f;
		position.y = 0.0f;
		position.z = 0.0f;
		 
		acceleration.x = 0.0f;
		acceleration.y = 0.0f; 
		acceleration.z = 0.0f; 

		moveSpeed.x = D3DXToRadian(0.0001f);
		moveSpeed.y = D3DXToRadian(0.0001f); 
		moveSpeed.z = D3DXToRadian(0.0001f); 

		maxMoveSpeed.x = D3DXToRadian(0.01f);
		maxMoveSpeed.y = D3DXToRadian(0.01f); 
		maxMoveSpeed.z = D3DXToRadian(0.01f); 

		rotation.x = 0.0f;
		rotation.y = 0.0f;
		rotation.z = 0.0f;

		rotationAcceleration.x = 0.0f;
		rotationAcceleration.y = 0.0f;
		rotationAcceleration.z = 0.0f;

		rotationSpeed.x = D3DXToRadian(0.0001f);
		rotationSpeed.y = D3DXToRadian(0.0005f);
		rotationSpeed.z = D3DXToRadian(0.0000f);
		
		maxRotationSpeed.x = D3DXToRadian(0.1f);
		maxRotationSpeed.y = D3DXToRadian(0.3f);
		maxRotationSpeed.z = D3DXToRadian(0.0f);

		frameTime = 0.0f;
		friction = D3DXToRadian(0.001f);
	}


	Camera::Camera(const Camera& other)
	{
	}


	Camera::~Camera()
	{
	}

	void Camera::SetFrameTime(float frameTime)
	{
		this->frameTime = frameTime;
	}

	void Camera::SetPosition(D3DXVECTOR3 position)
	{
		this->position = position;
		return;
	}


	void Camera::SetRotation(D3DXVECTOR3 rotation)
	{
		this->rotation = rotation;
		return;
	}


	void Camera::GetPosition(D3DXVECTOR3& position)
	{
		position = this->position;
		return;
	}


	void Camera::GetRotation(D3DXVECTOR3& rotation)
	{
		rotation = this->rotation;
		return;
	}

	void Camera::Move(D3DXVECTOR3 move)
	{
		D3DXVec3TransformCoord(&move, &move, &rotationMatrix);

		position += move;
		/*


		forwardSpeed += frameTime * 0.001f;

		if(forwardSpeed > (frameTime * 0.03f))
		{
			forwardSpeed = frameTime * 0.03f;
		}

		forwardSpeed -= frameTime * 0.0007f;

		if(forwardSpeed < 0.0f)
		{
			forwardSpeed = 0.0f;
		}


		// Convert degrees to radians.
		radians = rotationY * 0.0174532925f;

		// Update the position.
		positionX += sinf(radians) * forwardSpeed;
		positionZ += cosf(radians) * forwardSpeed;
		*/
		return;
	}


	void Camera::Rotate(D3DXVECTOR3 rotate)
	{
		if(rotate.x > 0.0001f || rotate.x < -0.0001f)
		{
			rotationAcceleration.x += frameTime * rotationSpeed.x * rotate.x;

			if(rotationAcceleration.x > (frameTime * maxRotationSpeed.x))
			{
				rotationAcceleration.x = frameTime * maxRotationSpeed.x;
			}
			else if(rotationAcceleration.x < (frameTime * -maxRotationSpeed.x))
			{
				rotationAcceleration.x = frameTime * -maxRotationSpeed.x;
			}
		}
		else if(rotationAcceleration.x > 0.0001f)
		{
			rotationAcceleration.x -= frameTime * rotationSpeed.x * 10.0f;

			if(rotationAcceleration.x < -0.0001f)
			{
				rotationAcceleration.x = 0.0f;
			}
		}
		else if(rotationAcceleration.x < -0.0001f)
		{
			rotationAcceleration.x += frameTime * rotationSpeed.x * 10.0f;

			if(rotationAcceleration.x > 0.0001f)
			{
				rotationAcceleration.x = 0.0f;
			}
		}


		if(rotate.y > 0.0001f || rotate.y < -0.0001f)
		{
			rotationAcceleration.y += frameTime * rotationSpeed.y * rotate.y;

			if(rotationAcceleration.y > (frameTime * maxRotationSpeed.y))
			{
				rotationAcceleration.y = frameTime * maxRotationSpeed.y;
			}
			else if(rotationAcceleration.y < (frameTime * -maxRotationSpeed.y))
			{
				rotationAcceleration.y = frameTime * -maxRotationSpeed.y;
			}
		}
		else if(rotationAcceleration.y > 0.0001f)
		{
			rotationAcceleration.y -= frameTime * rotationSpeed.y * 10.0f;

			if(rotationAcceleration.y < -0.0001f)
			{
				rotationAcceleration.y = 0.0f;
			}
		}
		else if(rotationAcceleration.y < -0.0001f)
		{
			rotationAcceleration.y += frameTime * rotationSpeed.y * 10.0f;

			if(rotationAcceleration.y > 0.0001f)
			{
				rotationAcceleration.y = 0.0f;
			}
		}

		rotation.x += rotationAcceleration.x;
		rotation.y += rotationAcceleration.y;

		if(rotation.x > D3DX_PI/2)
		{
			rotation.x = D3DX_PI /2;
		}

		if(rotation.x < -D3DX_PI/2)
		{
			rotation.x = -D3DX_PI /2;
		}

		// Keep the rotation in the 0 to 360 range.
		if(rotation.y > D3DX_PI*2)
		{
			rotation.y -= D3DX_PI *2;
		}

		return;
	}

	void Camera::Render()
	{
		D3DXVECTOR3 up, lookAt;

		// Setup the vector that points upwards.
		up.x = 0.0f;
		up.y = 1.0f;
		up.z = 0.0f;


		// Setup where the camera is looking by default.
		lookAt.x = 0.0f;
		lookAt.y = 0.0f;
		lookAt.z = 1.0f;


		// Create the rotation matrix from the yaw, pitch, and roll values.
		D3DXMatrixRotationYawPitchRoll(&rotationMatrix, rotation.y, rotation.x, rotation.z);

		// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
		D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
		D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

		// Translate the rotated camera position to the location of the viewer.
		lookAt = position + lookAt;

		// Finally create the view matrix from the three updated vectors.
		D3DXMatrixLookAtLH(&viewMatrix, &position, &lookAt, &up);

		return;
	}


	void Camera::GetViewMatrix(D3DXMATRIX& viewMatrix)
	{
		viewMatrix = this->viewMatrix;
		return;
	}
}