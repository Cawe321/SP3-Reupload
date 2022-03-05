#include "PickUp.h"
#include <iostream>
using namespace std;



CPickUp::CPickUp(void)
{
}

CPickUp::CPickUp(const Vector3 vec3Position, const Vector3 vec3Front)
{
}

/**
 @brief Destructor
 */
CPickUp::~CPickUp(void)
{
	
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool CPickUp::Init(void)
{
	// Set the type
	SetType(CEntity::ENTITY_TYPE::PICKUP);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	float vertices[] = {
		// Rear face
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		// Front face
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

		// Left face
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		// Right face
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		// Bottom face
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

		// Top face
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
	};

	//vec3Scale = Vector3(0.8, 0.5, 0.8);

	return true;
}

void CPickUp::SetPickUpType(PICKUP_TYPE PUType)
{
	this->PUType = PUType;
}

CPickUp::PICKUP_TYPE CPickUp::GetPickUpType(void) const
{
	return PUType;
}

/**
@brief Update this class instance
*/
void CPickUp::Update(const double dElapsedTime)
{
}

/**
@brief Activate the CCollider for this class instance
@param cLineShader A Shader* variable which stores a shader which renders lines
*/
void CPickUp::ActivateCollider(void)
{
	// Create a new CCollider
	cCollider = new CCollider();
	// Initialise the cCollider
	cCollider->Init();
}

void CPickUp::Effect(void) const
{
	if (PUType == CPickUp::PICKUP_TYPE::VIRUS_LAUNCHER)
	{
		CPlayer::GetInstance()->AccessWeapon(1)->AddUses(30);
	}
	else if (PUType == CPickUp::PICKUP_TYPE::AIR_FRESHENER)
	{
		CPlayer::GetInstance()->AccessWeapon(2)->AddUses(5);
	}
	else if (PUType == CPickUp::MUTATION_PTS)
	{
		CVirus::GetInstance()->IncreasePoints(150);
	}
}