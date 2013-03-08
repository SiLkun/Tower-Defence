////////////////////////////////////////////////////////////////////////////////
// Filename: Camera.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERA_H_
#define _CAMERA_H_


//////////////
// INCLUDES //
//////////////
#include "stdafx.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: Camera
////////////////////////////////////////////////////////////////////////////////
class Camera
{
public:
	Camera();
	Camera(const Camera&);
	~Camera();

	void SetPosition(D3DXVECTOR3);
	void SetRotation(D3DXVECTOR3);

	void GetPosition(D3DXVECTOR3&);
	void GetRotation(D3DXVECTOR3&);

	void SetFrameTime(float);

	void Move(D3DXVECTOR3);
	void Rotate(D3DXVECTOR3);
	void Render();
	void GetViewMatrix(D3DXMATRIX&);

private:
	D3DXVECTOR3 position;
	D3DXVECTOR3 acceleration;
	D3DXVECTOR3 moveSpeed;
	D3DXVECTOR3 maxMoveSpeed;
	
	D3DXVECTOR3 rotation;
	D3DXVECTOR3 rotationAcceleration;
	D3DXVECTOR3 rotationSpeed;
	D3DXVECTOR3 maxRotationSpeed;

	float friction;
	float frameTime;

	D3DXMATRIX rotationMatrix;
	D3DXMATRIX viewMatrix;

};

#endif