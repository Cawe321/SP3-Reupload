#include "Entity.h"

CEntity::CEntity()
	: vec3Position(1, 0, 0)
	, vec3DirectionFront(1, 0, 0)
	, m_heightMap(NULL)
	, cCollider(NULL)
	, vec3ColliderScale(2, 2, 2)
	, fRotationAngle(0.f)
	, bDelete(false)
	, fSpeed(50.f)
{
	
}

CEntity::~CEntity()
{

}

void CEntity::Init(const Vector3& pos, const Vector3& dir, std::vector<unsigned char> m_heightMap)
{
	this->vec3Position = pos;
	this->vec3DirectionFront = dir;
	this->m_heightMap = m_heightMap;
}

void CEntity::Reset()
{
	vec3Position.Set(1, 0, 0);
	vec3DirectionFront.Set(1, 0, 0);
}

void CEntity::SetPosition(Vector3 pos)
{
	this->vec3Position = pos;
}

void CEntity::SetDirection(Vector3 dir)
{
	this->vec3DirectionFront = dir;
}

void CEntity::SetType(ENTITY_TYPE eType)
{
	this->eType = eType;
}

void CEntity::SetToDelete(const bool bDelete)
{
	this->bDelete = bDelete;
}

void CEntity::SetSpeed(const float fSpeed)
{
	this->fSpeed = fSpeed;
}

Vector3 CEntity::GetPosition(void)
{
	return vec3Position;
}

Vector3 CEntity::GetDirection(void)
{
	return vec3DirectionFront;
}

CEntity::ENTITY_TYPE CEntity::GetType(void) const
{
	return eType;
}

float CEntity::GetSpeed(void) const
{
	return fSpeed;
}

bool CEntity::IsToDelete(void)
{
	return bDelete;
}

bool CEntity::Constraint(void)
{
	return (vec3Position.z > 830 || vec3Position.z < -1010 || vec3Position.x > 1525 || vec3Position.x < -1545);
}

void CEntity::ActivateCollider(void)
{
	cCollider = new CCollider();
	cCollider->Init();
}

bool CEntity::CheckForCollision(const CEntity* cEntity)
{
	tempVec3A_BottomLeft = vec3Position + cCollider->vec3BottomLeft;
	tempVec3A_TopRight = vec3Position + cCollider->vec3TopRight;
	tempVec3B_BottomLeft = cEntity->vec3Position + cEntity->cCollider->vec3BottomLeft;
	tempVec3B_TopRight = cEntity->vec3Position + cEntity->cCollider->vec3TopRight;

	return (tempVec3A_BottomLeft.x <= tempVec3B_TopRight.x && tempVec3A_TopRight.x >= tempVec3B_BottomLeft.x)
		//&& (tempVec3A_BottomLeft.y <= tempVec3B_TopRight.y && tempVec3A_TopRight.y >= tempVec3B_BottomLeft.y)
		&& (tempVec3A_BottomLeft.z <= tempVec3B_TopRight.z && tempVec3A_TopRight.z >= tempVec3B_BottomLeft.z);

	return false;
}

void CEntity::RollBackPosition(void)
{
	vec3Position = vec3PreviousPosition;
}
