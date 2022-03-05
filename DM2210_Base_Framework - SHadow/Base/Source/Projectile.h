#pragma once

// Include Entity
#include "Entity.h"
#include "Vector3.h"
#include "ParticleSystem.h"

class CProjectile : public CEntity
{
public:
	enum PROJECTILE_TYPE
	{
		COUGH,
		VIRUS_LAUNCHER,
		AIR_FRESHENER,
		NUM_PROJECTILE
	};

	// Default Constructor
	CProjectile(void);
	// Destructor
	virtual ~CProjectile(void);

	// Init the projectile
	bool Init(	Vector3 vec3Position,
				Vector3 vec3Front,
				const double dLifetime,
				const float fSpeed);

	// Activate the CCollider for this class instance
	virtual void ActivateCollider(Vector3 vec3BottomLeft, Vector3 vec3TopRight);

	// Activate the projectile. true == active, false == inactive
	void SetStatus(const bool bStatus);
	void SetDirection(Vector3 vec3Front);
	// Set the lifetime of the projectile
	void SetLifetime(const double dLifetime);
	// Set the speed of the projectile
	void SetSpeed(const float fSpeed);
	// Set the source of the projectile
	void SetSource(CEntity* pSource);
	// Set the damage of the projectile
	void SetProjectileDamage(int iDamage);
	// Set the type of projectile
	void SetProjectileType(PROJECTILE_TYPE eType);

	// get status of the projectile. true == active, false == inactive
	bool GetStatus(void) const;
	// Get the direction of the projectile
	Vector3 GetDirection(void);
	// Get the lifetime of the projectile
	double GetLifetime(void) const;
	// Get the speed of the projectile
	float GetSpeed(void) const;
	// Get the source of the projectile
	CEntity* GetSource(void) const;
	// Get the damage of the projectile
	int GetProjectileDamage(void) const;
	// Gte the type of projectile
	CProjectile::PROJECTILE_TYPE GetProjectileType(void) const;
	//Get the particle object
	ParticleObject* GetParticle();

	// Update the status of this projectile
	virtual void Update(const double dElapsedTime);

protected:
	// Boolean flag to indicate if this projectile is active. If not active, then do not compute/update
	bool bStatus;
	// Remaining lifetime in seconds
	double dLifetime;
	//Remaining time to next particle spawn :D
	double dParticleTime;


	// Speed of this projectile in unit / sec
	float fSpeed;
	// The source which fired this projectile
	CEntity* pSource;

	PROJECTILE_TYPE eType;

	ParticleObject* eParticle;

	Vector3 vec3Colour;

	unsigned int VAO, VBO;

	// The damage the projectile deals
	int iDamage;
};

