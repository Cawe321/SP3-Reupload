#ifndef CCAMERA_H
#define CCAMERA_H

#include "Camera.h"
#include "Player.h"

class CCamera : public Camera
{
public:
	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;
	Vector3 view;
	Vector3 right;


	CCamera();
	~CCamera();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);

	virtual void Reset();
};

#endif