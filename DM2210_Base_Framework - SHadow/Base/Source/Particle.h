#pragma once
#include <MatrixStack.h>

class ParticleObject
{
public:
	enum PraticleObject_TYPE
	{
		P_NONE = 0,
		P_VIRUS,
		P_AIRFRESHNER,
		P_VIRUS_SHOT,
		P_TOTAL
	};

	PraticleObject_TYPE TYPE;

	Vector3 pos;
	Vector3 focalPos;
	Vector3 vel;
	Vector3 scale;
	Vector3 dir;

	Vector3 camerapos;
	

	float rotation;
	float rotationSpeed;

	bool active;
	double timealive;

	float offset;

	ParticleObject(PraticleObject_TYPE = P_NONE);
	~ParticleObject();

	bool operator<(const ParticleObject& rhs) const;
};

