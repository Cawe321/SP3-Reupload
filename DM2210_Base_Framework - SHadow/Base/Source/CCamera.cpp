#include "CCamera.h"
#include "Application.h"
#include "Mtx44.h"

CCamera::CCamera()
{
}

CCamera::~CCamera()
{
}

void CCamera::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	 view = (target - position).Normalized();
	right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
}

void CCamera::Update(double dt)
{
	static const float CAMERA_SPEED = 200.f;
	view = (target - position).Normalized();
	right = view.Cross(up);
	right.y = 0;
	right.Normalize();

	if(Application::IsKeyPressed('R'))
	{
		Reset();
	}
}

void CCamera::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}