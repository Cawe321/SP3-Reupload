#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include "Vector3.h"
#include "Collider.h"
#include <vector>
#include <iostream>

class CEntity
{
	public:
	enum ENTITY_TYPE
	{
		PLAYER = 0,
		NPC,
		PROJECTILE,
		STRUCTURE,
		HITBOX,
		PICKUP,
		NUM_ENTITIES,
	};

	CCollider* cCollider;

	CEntity();
	~CEntity();

	virtual void Init(const Vector3 &pos, const Vector3 &dir, std::vector<unsigned char> m_heightMap);
	virtual void Reset();
	virtual void Update(double dt) = 0;

	virtual void SetPosition(Vector3 pos);
	virtual void SetDirection(Vector3 dir);
	virtual void SetType(ENTITY_TYPE eType);
	virtual void SetToDelete(const bool bDelete);
	virtual void SetSpeed(const float fSpeed);

	virtual Vector3 GetPosition(void);
	virtual Vector3 GetDirection(void);
	virtual CEntity::ENTITY_TYPE GetType(void) const;
	virtual float GetSpeed(void) const;
	virtual bool IsToDelete(void);

	virtual bool Constraint(void);

	virtual void ActivateCollider(void);

	virtual bool CheckForCollision(const CEntity* cEntity);

	virtual void RollBackPosition(void);

	protected:
	Vector3 vec3Position;
	Vector3 vec3PreviousPosition;
	Vector3 vec3DirectionFront;

	std::vector<unsigned char> m_heightMap;
	ENTITY_TYPE eType;

	bool bDelete;

	Vector3 vec3Scale;

	float fSpeed;
	// Rotation angle and axis
	float fRotationAngle;
	Vector3 vec3RotationAxis;

	//Collision variables
	Vector3 tempVec3A_BottomLeft;
	Vector3 tempVec3A_TopRight;
	Vector3 tempVec3B_BottomLeft;
	Vector3 tempVec3B_TopRight;
	Vector3 vec3ColliderScale;

	/*SceneBasic::GEOMETRY_TYPE gLLeg;
	SceneBasic::GEOMETRY_TYPE gRLeg;
	SceneBasic::GEOMETRY_TYPE gLArm;
	SceneBasic::GEOMETRY_TYPE gRArm;
	SceneBasic::GEOMETRY_TYPE gHead;
	SceneBasic::GEOMETRY_TYPE gBody;*/

	//More members to be created upon upgrades and weapons class creations

};

#endif