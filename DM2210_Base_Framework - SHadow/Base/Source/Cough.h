#pragma once

#include "WeaponInfo.h"

class CCough :	public CWeaponInfo
{
public:
	// Default Constructor
	CCough(void);
	// Destructor
	virtual ~CCough(void);

	// Initialise this instance to default values
	bool Init(void);

	virtual CProjectile* Use(Vector3 vec3Position, Vector3 vec3Front, CEntity* pSource = NULL);
};

