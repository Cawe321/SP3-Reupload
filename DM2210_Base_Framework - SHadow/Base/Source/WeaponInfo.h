#pragma once
#include "MyMath.h"
#include "Entity.h"
#include "Projectile.h"

class CWeaponInfo : public CEntity
{
public:

	enum WEAPON_TYPE
	{
		COUGH,
		VIRUS_LAUNCHER,
		CONTAMINATED_AIR_FRESHNER,
		TOTAL_WEAPON_TYPE,
	};

	// Default Constructor
	CWeaponInfo(void);
	// Destructor
	virtual ~CWeaponInfo(void);

	// The current total number of rounds currently carried by this player
	virtual void SetUses(const int iTotalRounds);

	// Get the current total number of rounds currently carried by this player
	virtual int GetUses(void) const;

	// Set the time between shots
	virtual void SetTimeBetweenShots(const double dTimeBetweenShots);
	// Set the firing rate in rounds per min
	virtual void SetFiringRate(const int iFiringRate);
	// Set the firing flag
	virtual void SetCanFire(const bool bFire);
	// Set weapon type
	virtual void SetWeaponType(WEAPON_TYPE weapType);
	// Enable/Disable weapon fire
	virtual void SetbFire(const bool bFire);
	// Set weapon damage
	virtual void SetWeaponDamage(const int iDamage);
	// Set weapon range
	virtual void SetWeaponRange(const int iRange);

	// Get the time between shots
	virtual double GetTimeBetweenUses(void) const;
	// Get the firing rate
	virtual int GetFiringRate(void) const;
	// Get the firing flag
	virtual bool GetCanFire(void) const;
	// Get the weapon type
	virtual WEAPON_TYPE GetWeaponType(void) const;
	// Get weapon damage
	virtual int GetWeaponDamage(void) const;
	// Get weapon range
	virtual int GetWeaponRange(void) const;

	// Initialise this instance to default values
	virtual bool Init(void);
	// Update the elapsed time
	virtual void Update(const double dElapsedTime);

	virtual CProjectile* Use(Vector3 vec3Position, Vector3 vec3Front, CEntity* pSource = NULL) = 0;

	// Reload this weapon
	virtual void Reload(void);
	// Add rounds
	virtual void AddUses(const int iNewRounds);

protected:
	// The current total number of rounds currently carried by this player
	int iUses;
	// The amount of damage the weapon deals
	int iDamage;
	// The range of the weapon
	int iRange;

	// The time between shots in milliseconds
	double dTimeBetweenUses;
	// The elapsed time (between shots) in milliseconds
	double dElapsedTime;
	// Boolean flag to indicate if weapon can fire now
	bool bFire;
	// Boolean flag to indicate if weapon is to be rendered
	bool bActive;

	WEAPON_TYPE weapType;
};
