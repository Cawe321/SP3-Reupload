#pragma once

#include "WeaponInfo.h"

class CVirusLauncher :	public CWeaponInfo
{
public:
	// Default Constructor
	CVirusLauncher(void);
	// Destructor
	virtual ~CVirusLauncher(void);

	// Initialise this instance to default values
	bool Init(void);

	virtual CProjectile* Use(Vector3 vec3Position, Vector3 vec3Front, CEntity* pSource = NULL);
};

