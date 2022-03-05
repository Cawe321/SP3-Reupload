#pragma once

#include "WeaponInfo.h"

class CAirFreshener : public CWeaponInfo
{
public:
	// Default Constructor
	CAirFreshener(void);
	// Destructor
	virtual ~CAirFreshener(void);

	// Initialise this instance to default values
	bool Init(void);

	virtual CProjectile* Use(Vector3 vec3Position, Vector3 vec3Front, CEntity* pSource = NULL);
};

