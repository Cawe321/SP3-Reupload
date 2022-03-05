#include "Projectile.h"

#include "GL/glew.h"

#include <GLFW/glfw3.h>

#include <iostream>
#include "Player.h"
using namespace std;

/**
@brief Default Constructor
*/
CProjectile::CProjectile(void)
	: bStatus(false)
	, dLifetime(0.0f)
	, fSpeed(1.0f)
	, pSource(NULL)
	, iDamage(1)
	, eType(COUGH)
	, dParticleTime(0)
	, eParticle(NULL)
{
}

/**
@brief Destructor
*/
CProjectile::~CProjectile(void)
{
	// We set it to NULL only since it was declared somewhere else
	pSource = NULL;

	
	eParticle == NULL;
	
}

/** 
 @brief Set the position and direction of the projectile
 @param vec3Position A const glm::vec3 variable which contains the position of the projectile
 @param vec3Front A const glm::vec3 variable which contains the up direction of the projectile
 @param dLifetime A const float variable which contains the lifetime for this projectile
 @param fSpeed A const float variable which contains the speed of the projectile
 */
bool CProjectile::Init(	Vector3 vec3Position,
						Vector3 vec3Front,
						const double dLifetime,
						const float fSpeed)
{

	// Set the type
	SetType(PROJECTILE);

	// Set the attributes for this projectile
	this->vec3Position = vec3Position;
	this->vec3DirectionFront = vec3Front;
	this->dLifetime = dLifetime;
	this->fSpeed = fSpeed;
	vec3Colour = Vector3(0, 1, 1);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	float vertices[] = {
		// Rear face
		-0.05f, -0.05f, -0.05f, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		-0.05f, 0.05f, -0.05f, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		0.05f, 0.05f, -0.05f, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		0.05f, 0.05f, -0.05f, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		0.05f, -0.05f, -0.05f, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		-0.05f, -0.05f, -0.05f, vec3Colour.x, vec3Colour.y, vec3Colour.z,

		// Front face
		-0.05f, -0.05f, 0.05f, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		0.05f, -0.05f, 0.05f, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		0.05f, 0.05f, 0.05f, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		0.05f, 0.05f, 0.05f, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		-0.05f, 0.05f, 0.05f, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		-0.05f, -0.05f, 0.05f, vec3Colour.x, vec3Colour.y, vec3Colour.z,

		// Left face
		-0.05f, 0.05f, 0.05f, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		-0.05f, 0.05f, -0.05f, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		-0.05f, -0.05f, -0.05f, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		-0.05f, -0.05f, -0.05f, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		-0.05f, -0.05f, 0.05f, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		-0.05f, 0.05f, 0.05f, vec3Colour.x, vec3Colour.y, vec3Colour.z,

		// Right face
		0.05f, 0.05f, 0.05f, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		0.05f, -0.05f, 0.05f, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		0.05f, -0.05f, -0.05f, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		0.05f, -0.05f, -0.05f, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		0.05f, 0.05f, -0.05f, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		0.05f, 0.05f, 0.05f, vec3Colour.x, vec3Colour.y, vec3Colour.z,

		// Bottom face
		-0.05f, -0.05f, -0.05f, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		0.05f, -0.05f, -0.05f, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		0.05f, -0.05f, 0.05f, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		0.05f, -0.05f, 0.05f, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		-0.05f, -0.05f, 0.05f, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		-0.05f, -0.05f, -0.05f, vec3Colour.x, vec3Colour.y, vec3Colour.z,

		// Top face
		-0.05f, 0.05f, -0.05f, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		-0.05f, 0.05f, 0.05f, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		0.05f, 0.05f, 0.05f, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		0.05f, 0.05f, 0.05f, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		0.05f, 0.05f, -0.05f, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		-0.05f, 0.05f, -0.05f, vec3Colour.x, vec3Colour.y, vec3Colour.z
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	return true;
}


/**
@brief Activate the CCollider for this class instance
@param cLineShader A Shader* variable which stores a shader which renders lines
*/
void CProjectile::ActivateCollider(Vector3 vec3BottomLeft, Vector3 vec3TopRight)
{
	// Since we are changing the colour, we don't call the parent's ActivateCollider() method.
	// Instead we create it here and insert our colour changing codes
	//CEntity3D::ActivateCollider(cLineShader);

	// Create a new CCollider
	cCollider = new CCollider();
	// Set the bottom left of the CCollider
	cCollider->vec3BottomLeft = vec3BottomLeft;
	// Set the top right of the CCollider
	cCollider->vec3TopRight = vec3TopRight;
	// Set the colour of the CCollider to Blue
	cCollider->vec3Colour = Vector3(1.0f, 1.0f, 0.0f);
	// Initialise the cCollider
	cCollider->Init();
}

/** 
 @brief Activate the projectile. true == active, false == inactive
 @param bStatus A const bool variable which is true if this projectile is active, otherwise false.
 */
void CProjectile::SetStatus(const bool bStatus)
{
	if (bStatus == false)
		dLifetime = 0.0f;
	this->bStatus = bStatus;
}

/** 
 @brief Set the direction of the projectile
 @param vec3Front A const glm::vec3 variable which contains the up direction of the projectile
 */
void CProjectile::SetDirection(Vector3 vec3Front)
{
	this->vec3DirectionFront = vec3Front;
}

/** 
 @brief Set the lifetime of the projectile
 @param dLifetime A const float variable which contains the lifetime for this projectile
 */
void CProjectile::SetLifetime(const double dLifetime)
{
	this->dLifetime = dLifetime;
}

/** 
 @brief Set the speed of the projectile
 @param fSpeed A const float variable which contains the speed of the projectile
 */
void CProjectile::SetSpeed(const float fSpeed)
{
	this->fSpeed = fSpeed;
}

/** 
 @brief Set the source of the projectile
 @param pSource A CEntity3D* variable which is the CEntity3D who created this projectile
 */
void CProjectile::SetSource(CEntity* pSource)
{
	this->pSource = pSource;
}

void CProjectile::SetProjectileDamage(int iDamage)
{
	this->iDamage = iDamage;
}

void CProjectile::SetProjectileType(PROJECTILE_TYPE eType)
{
	this->eType = eType;
}

/** 
 @brief get status of the projectile. true == active, false == inactive
 @return A const bool variable which is true if this projectile is active, otherwise false.
 */
bool CProjectile::GetStatus(void) const
{
	return bStatus;
}

/** 
 @brief Get the direction of the projectile
 @return A const glm::vec3 variable which contains the up direction of the projectile
 */
Vector3 CProjectile::GetDirection(void)
{
	return vec3DirectionFront;
}

/** 
 @brief Get the lifetime of the projectile
 @return A const float variable which contains the lifetime for this projectile
 */
double CProjectile::GetLifetime(void) const
{
	return dLifetime;
}

/** 
 @brief Get the speed of the projectile
 @return A const float variable which contains the speed of the projectile
 */
float CProjectile::GetSpeed(void) const
{
	return fSpeed;
}

/** 
 @brief Get the source of the projectile
 @return A CEntity3D* variable which is the CEntity3D who created this projectile
 */
CEntity* CProjectile::GetSource(void) const
{
	return pSource;
}

int CProjectile::GetProjectileDamage(void) const
{
	return iDamage;
}

CProjectile::PROJECTILE_TYPE CProjectile::GetProjectileType(void) const
{
	return eType;
}

ParticleObject* CProjectile::GetParticle()
{
	return eParticle;
}

/** 
 @brief Update the status of this projectile
 @param dElapsedTime A const double variable which contains the delta time for the realtime loop
 */
void CProjectile::Update(const double dElapsedTime)
{
	if (bStatus == false)
		return;

	// Update TimeLife of projectile. Set to inactive if too long
	dLifetime -= dElapsedTime;
	if (dLifetime < 0.0f)
	{
		SetStatus(false);
		SetToDelete(true);
		if (eParticle)
		{
			ParticleSystem::GetInstance()->ReturnParticleObject(eParticle);
		}
		return;
	}

	// Update Position
	if (eType != AIR_FRESHENER)
		vec3Position = vec3Position + vec3DirectionFront * fSpeed;
	else
		vec3Position = vec3Position;

	dParticleTime -= dElapsedTime;
	//Update spawnning particles
	if (eType == AIR_FRESHENER && dParticleTime <= 0)
	{
		dParticleTime = 2.0;
		//Spawn particles in a ring around the center point
		double dAngle;
		for (int iOffset = 0; iOffset < 8; ++iOffset)
		{
			dAngle = (360 / 7) * (iOffset);
			Vector3 v3Pos;
			
			v3Pos.x = this->GetPosition().x + (75 * sin(dAngle));
			v3Pos.y = this->GetPosition().y;   
			v3Pos.z = this->GetPosition().z + (75 * cos(dAngle));

			ParticleSystem::GetInstance()->SpawnAirFreshnerParticle(v3Pos);
		}
	}
	else if (eType == PROJECTILE_TYPE::COUGH || eType == PROJECTILE_TYPE::VIRUS_LAUNCHER)
	{
		if (eParticle == NULL)
		{
			eParticle = ParticleSystem::GetInstance()->SpawnVirusShotParticle(this->GetPosition());
		}
		eParticle->pos = GetPosition();
	}
}