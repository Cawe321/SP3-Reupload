#include "Structure.h"

void CStructure::Init()
{
	SetType(STRUCTURE);
	this->cCollider->vec3TopRight.Set(110, 25, 48);
	this->cCollider->vec3BottomLeft.Set(-80, -25, -48);
}

void CStructure::Update(double dt)
{
}

CStructure::CStructure()
{
}

CStructure::~CStructure()
{
}
