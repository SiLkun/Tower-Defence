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

		moveSpeed.x = (float)D3DXToRadian(0.005f);
		moveSpeed.y = (float)D3DXToRadian(0.0f); 
		moveSpeed.z = (float)D3DXToRadian(0.01f); 

		maxMoveSpeed.x = (float)D3DXToRadian(2.0f);
		maxMoveSpeed.y = (float)D3DXToRadian(0.00f); 
		maxMoveSpeed.z = (float)D3DXToRadian(3.0f); 

		rotation.x = 0.0f;
		rotation.y = 0.0f;
		rotation.z = 0.0f;

		rotationAcceleration.x = 0.0f;
		rotationAcceleration.y = 0.0f;
		rotationAcceleration.z = 0.0f;

		rotationSpeed.x = (float)D3DXToRadian(0.00003f);
		rotationSpeed.y = (float)D3DXToRadian(0.00005f);
		rotationSpeed.z = (float)D3DXToRadian(0.0000f);
		
		maxRotationSpeed.x = (float)D3DXToRadian(0.05f);
		maxRotationSpeed.y = (float)D3DXToRadian(0.10f);
		maxRotationSpeed.z = (float)D3DXToRadian(0.0f);

		frameTime = 0.0f;
		friction = (float)D3DXToRadian(0.001f);
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

		if(move.x > 0.0001f || move.x < -0.0001f)
		{
			acceleration.x += frameTime * moveSpeed.x * move.x;

			if(acceleration.x > (frameTime * maxMoveSpeed.x))
			{
				acceleration.x = frameTime * maxMoveSpeed.x;
			}
			else if(acceleration.x < (frameTime * -maxMoveSpeed.x))
			{
				acceleration.x = frameTime * -maxMoveSpeed.x;
			}
		}
		else if(acceleration.x > 0.0001f)
		{
			acceleration.x -= frameTime * moveSpeed.x * 10.0f;

			if(acceleration.x < -0.0001f)
			{
				acceleration.x = 0.0f;
			}
		}
		else if(acceleration.x < -0.0001f)
		{
			acceleration.x += frameTime * moveSpeed.x * 10.0f;

			if(acceleration.x > 0.0001f)
			{
				acceleration.x = 0.0f;
			}
		}

		if(move.z > 0.0001f || move.z < -0.0001f)
		{
			acceleration.z += frameTime * moveSpeed.z * move.z;

			if(acceleration.z > (frameTime * maxMoveSpeed.z))
			{
				acceleration.z = frameTime * maxMoveSpeed.z;
			}
			else if(acceleration.z < (frameTime * -maxMoveSpeed.z))
			{
				acceleration.z = frameTime * -maxMoveSpeed.z;
			}
		}
		else if(acceleration.z > 0.0001f)
		{
			acceleration.z -= frameTime * moveSpeed.z * 10.0f;

			if(acceleration.z < -0.0001f)
			{
				acceleration.z = 0.0f;
			}
		}
		else if(acceleration.z < -0.0001f)
		{
			acceleration.z += frameTime * moveSpeed.z * 10.0f;

			if(acceleration.z > 0.0001f)
			{
				acceleration.z = 0.0f;
			}
		}


		D3DXVECTOR3 rotatedAcceleration(0,0,0);
		D3DXVec3TransformCoord(&rotatedAcceleration, &acceleration, &rotationMatrix);

		position += rotatedAcceleration;


		if(position.y < 2.0f)
		{
			position.y = 2.0f;
		}
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
		else if(rotationAcceleration.x > -0.0001f)
		{
			rotationAcceleration.x -= frameTime  * 0.1f * maxRotationSpeed.x;

			if(rotationAcceleration.x < -0.0001f)
			{
				rotationAcceleration.x = 0.0f;
			}
		}
		else if(rotationAcceleration.x < 0.0001f)
		{
			rotationAcceleration.x += frameTime  * 0.1f * maxRotationSpeed.x;

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
			rotationAcceleration.y -= frameTime  * 0.05f  * maxRotationSpeed.y;

			if(rotationAcceleration.y < -0.0001f)
			{
				rotationAcceleration.y = 0.0f;
			}
		}
		else if(rotationAcceleration.y < -0.0001f)
		{
			rotationAcceleration.y += frameTime * 0.05f  * maxRotationSpeed.y;

			if(rotationAcceleration.y > 0.0001f)
			{
				rotationAcceleration.y = 0.0f;
			}
		}

		rotation.x += rotationAcceleration.x;
		rotation.y += rotationAcceleration.y;

		if(rotation.x < 0.0f)
		{
			rotationAcceleration.x = 0.0f;
			rotation.x = 0.0f;
		}

		if(rotation.x > D3DX_PI/2.0f)
		{
			rotationAcceleration.x = 0.0f;
			rotation.x = (float)D3DX_PI /2.0f;
		}

		// Keep the rotation in the 0 to 360 range.
		if(rotation.y > D3DX_PI*2.0f)
		{
			rotation.y -= (float)D3DX_PI *2.0f;
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