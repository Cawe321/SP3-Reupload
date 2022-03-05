#include "Particle.h"

ParticleObject::ParticleObject(PraticleObject_TYPE type)
	:TYPE(type),
	scale(5,5,5),
	active (true),
	pos(0,0,0),
	focalPos(0,0,0),
	vel(0,0,0),
	rotation(0),
	rotationSpeed(0),
	offset(0),
	dir(0,0,0),
	camerapos(0,0,0),
	timealive(10000000)
{
}

ParticleObject::~ParticleObject()
{
}

bool ParticleObject::operator<(const ParticleObject& rhs) const
{
	//Squareroot ((x1-x2)^2 + (y1-y2)^2 + (z1-z2)^2)
	if (sqrt(pow(this->pos.x - camerapos.x, 2) + pow(this->pos.y - camerapos.y, 2) + pow(this->pos.z - camerapos.z, 2)) <=
		sqrt(pow(rhs.pos.x - camerapos.x, 2) + pow(rhs.pos.y - camerapos.y, 2) + pow(rhs.pos.z - camerapos.z, 2)))
	{
		return true;
	}
	return false;
}
