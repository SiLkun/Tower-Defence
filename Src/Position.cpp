////////////////////////////////////////////////////////////////////////////////
// Filename: Position.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

namespace TD
{

	Position::Position()
	{
		positionX = 0.0f;
		positionY = 0.0f;
		positionZ = 0.0f;
	
		rotationX = 0.0f;
		rotationY = 0.0f;
		rotationZ = 0.0f;

		frameTime = 0.0f;

		forwardSpeed   = 0.0f;
		backwardSpeed  = 0.0f;
		upwardSpeed    = 0.0f;
		downwardSpeed  = 0.0f;
		leftTurnSpeed  = 0.0f;
		rightTurnSpeed = 0.0f;
		lookUpSpeed    = 0.0f;
		lookDownSpeed  = 0.0f;
	}


	Position::Position(const Position& other)
	{
	}


	Position::~Position()
	{
	}


	void Position::SetPosition(float x, float y, float z)
	{
		positionX = x;
		positionY = y;
		positionZ = z;
		return;
	}


	void Position::SetRotation(float x, float y, float z)
	{
		rotationX = x;
		rotationY = y;
		rotationZ = z;
		return;
	}


	void Position::GetPosition(float& x, float& y, float& z)
	{
		x = positionX;
		y = positionY;
		z = positionZ;
		return;
	}


	void Position::GetRotation(float& x, float& y, float& z)
	{
		x = rotationX;
		y = rotationY;
		z = rotationZ;
		return;
	}


	void Position::SetFrameTime(float time)
	{
		frameTime = time;
		return;
	}


	void Position::MoveForward(bool keydown)
	{
		float radians;


		// Update the forward speed movement based on the frame time and whether the user is holding the key down or not.
		if(keydown)
		{
			forwardSpeed += frameTime * 0.001f;

			if(forwardSpeed > (frameTime * 0.03f))
			{
				forwardSpeed = frameTime * 0.03f;
			}
		}
		else
		{
			forwardSpeed -= frameTime * 0.0007f;

			if(forwardSpeed < 0.0f)
			{
				forwardSpeed = 0.0f;
			}
		}

		// Convert degrees to radians.
		radians = rotationY * 0.0174532925f;

		// Update the position.
		positionX += sinf(radians) * forwardSpeed;
		positionZ += cosf(radians) * forwardSpeed;

		return;
	}


	void Position::MoveBackward(bool keydown)
	{
		float radians;


		// Update the backward speed movement based on the frame time and whether the user is holding the key down or not.
		if(keydown)
		{
			backwardSpeed += frameTime * 0.001f;

			if(backwardSpeed > (frameTime * 0.03f))
			{
				backwardSpeed = frameTime * 0.03f;
			}
		}
		else
		{
			backwardSpeed -= frameTime * 0.0007f;
		
			if(backwardSpeed < 0.0f)
			{
				backwardSpeed = 0.0f;
			}
		}

		// Convert degrees to radians.
		radians = rotationY * 0.0174532925f;

		// Update the position.
		positionX -= sinf(radians) * backwardSpeed;
		positionZ -= cosf(radians) * backwardSpeed;

		return;
	}


	void Position::MoveUpward(bool keydown)
	{
		// Update the upward speed movement based on the frame time and whether the user is holding the key down or not.
		if(keydown)
		{
			upwardSpeed += frameTime * 0.003f;

			if(upwardSpeed > (frameTime * 0.03f))
			{
				upwardSpeed = frameTime * 0.03f;
			}
		}
		else
		{
			upwardSpeed -= frameTime * 0.002f;

			if(upwardSpeed < 0.0f)
			{
				upwardSpeed = 0.0f;
			}
		}

		// Update the height position.
		positionY += upwardSpeed;

		return;
	}


	void Position::MoveDownward(bool keydown)
	{
		// Update the downward speed movement based on the frame time and whether the user is holding the key down or not.
		if(keydown)
		{
			downwardSpeed += frameTime * 0.003f;

			if(downwardSpeed > (frameTime * 0.03f))
			{
				downwardSpeed = frameTime * 0.03f;
			}
		}
		else
		{
			downwardSpeed -= frameTime * 0.002f;

			if(downwardSpeed < 0.0f)
			{
				downwardSpeed = 0.0f;
			}
		}

		// Update the height position.
		positionY -= downwardSpeed;

		return;
	}


	void Position::TurnLeft(bool keydown)
	{
		// Update the left turn speed movement based on the frame time and whether the user is holding the key down or not.
		if(keydown)
		{
			leftTurnSpeed += frameTime * 0.01f;

			if(leftTurnSpeed > (frameTime * 0.15f))
			{
				leftTurnSpeed = frameTime * 0.15f;
			}
		}
		else
		{
			leftTurnSpeed -= frameTime* 0.005f;

			if(leftTurnSpeed < 0.0f)
			{
				leftTurnSpeed = 0.0f;
			}
		}

		// Update the rotation.
		rotationY -= leftTurnSpeed;

		// Keep the rotation in the 0 to 360 range.
		if(rotationY < 0.0f)
		{
			rotationY += 360.0f;
		}

		return;
	}


	void Position::TurnRight(bool keydown)
	{
		// Update the right turn speed movement based on the frame time and whether the user is holding the key down or not.
		if(keydown)
		{
			rightTurnSpeed += frameTime * 0.01f;

			if(rightTurnSpeed > (frameTime * 0.15f))
			{
				rightTurnSpeed = frameTime * 0.15f;
			}
		}
		else
		{
			rightTurnSpeed -= frameTime* 0.005f;

			if(rightTurnSpeed < 0.0f)
			{
				rightTurnSpeed = 0.0f;
			}
		}

		// Update the rotation.
		rotationY += rightTurnSpeed;

		// Keep the rotation in the 0 to 360 range.
		if(rotationY > 360.0f)
		{
			rotationY -= 360.0f;
		}

		return;
	}


	void Position::LookUpward(bool keydown)
	{
		// Update the upward rotation speed movement based on the frame time and whether the user is holding the key down or not.
		if(keydown)
		{
			lookUpSpeed += frameTime * 0.01f;

			if(lookUpSpeed > (frameTime * 0.15f))
			{
				lookUpSpeed = frameTime * 0.15f;
			}
		}
		else
		{
			lookUpSpeed -= frameTime* 0.005f;

			if(lookUpSpeed < 0.0f)
			{
				lookUpSpeed = 0.0f;
			}
		}

		// Update the rotation.
		rotationX -= lookUpSpeed;

		// Keep the rotation maximum 90 degrees.
		if(rotationX > 90.0f)
		{
			rotationX = 90.0f;
		}

		return;
	}


	void Position::LookDownward(bool keydown)
	{
		// Update the downward rotation speed movement based on the frame time and whether the user is holding the key down or not.
		if(keydown)
		{
			lookDownSpeed += frameTime * 0.01f;

			if(lookDownSpeed > (frameTime * 0.15f))
			{
				lookDownSpeed = frameTime * 0.15f;
			}
		}
		else
		{
			lookDownSpeed -= frameTime* 0.005f;

			if(lookDownSpeed < 0.0f)
			{
				lookDownSpeed = 0.0f;
			}
		}

		// Update the rotation.
		rotationX += lookDownSpeed;

		// Keep the rotation maximum 90 degrees.
		if(rotationX < -90.0f)
		{
			rotationX = -90.0f;
		}

		return;
	}
}