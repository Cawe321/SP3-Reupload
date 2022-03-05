#include "WeaponInfo.h"

#include <iostream>
using namespace std;

/**
@brief Default Constructor
*/
CWeaponInfo::CWeaponInfo()
	: iUses(1)
	, iDamage(1)
	, iRange(10)
	, dTimeBetweenUses(0.5)
	, dElapsedTime(0.0)
	, bFire(true)
{
}

/**
 @brief Default Destructor
 */
CWeaponInfo::~CWeaponInfo()
{
}

/**
 @brief The current total number of rounds currently carried by this player
 */
void CWeaponInfo::SetUses(const int iUses)
{
	this->iUses = iUses;
}


/**
 @brief Get the current total number of rounds currently carried by this player
 */
int CWeaponInfo::GetUses(void) const
{
	return iUses;
}

/**
 @brief Set the time between shots
 */
void CWeaponInfo::SetTimeBetweenShots(const double dTimeBetweenShots)
{
	this->dTimeBetweenUses = dTimeBetweenShots;
}

/**
 @brief Set the firing rate in rounds per min
 */
void CWeaponInfo::SetFiringRate(const int firingRate)
{
	dTimeBetweenUses = 60.0 / (double)firingRate;	// 60 seconds divided by firing rate
}

/**
 @brief Set the firing flag
 */
void CWeaponInfo::SetCanFire(const bool bFire)
{
	this->bFire = bFire;
}

void CWeaponInfo::SetWeaponType(WEAPON_TYPE weapType)
{
	this->weapType = weapType;
}

void CWeaponInfo::SetbFire(const bool bFire)
{
	this->bFire = bFire;
}

void CWeaponInfo::SetWeaponDamage(const int iDamage)
{
	this->iDamage = iDamage;
}

void CWeaponInfo::SetWeaponRange(const int iRange)
{
	this->iRange = iRange;
}

/**
 @brief Get the time between shots
 */
double CWeaponInfo::GetTimeBetweenUses(void) const
{
	return dTimeBetweenUses;
}

/**
 @brief Get the firing rate
 */
int CWeaponInfo::GetFiringRate(void) const
{
	return (int)(60.0 / dTimeBetweenUses);	// 60 seconds divided by dTimeBetweenShots
}

/**
 @brief Get the firing flag
 */
bool CWeaponInfo::GetCanFire(void) const
{
	return bFire;
}

CWeaponInfo::WEAPON_TYPE CWeaponInfo::GetWeaponType(void) const
{
	return weapType;
}

int CWeaponInfo::GetWeaponDamage(void) const
{
	return iDamage;
}

int CWeaponInfo::GetWeaponRange(void) const
{
	return iRange;
}

/**
 @brief Initialise this instance to default values
 */
bool CWeaponInfo::Init(void)
{
	// The current total number of rounds currently carried by this player
	iUses = 8;

	// The time between shots
	dTimeBetweenUses = 0.5;
	// The elapsed time (between shots)
	dElapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;

	return true;
}

/**
 @brief Update the elapsed time
 */
void CWeaponInfo::Update(const double dt)
{
	// If the weapon can fire, then don't check further
	if (bFire)
		return;

	dElapsedTime += dt;
	if (dElapsedTime > dTimeBetweenUses)
	{
		bFire = true;
		dElapsedTime = 0.0;
	}
}

/**
 @brief Reload this weapon
 */
void CWeaponInfo::Reload(void)
{
	//Weapon cooldown
}

/**
 @brief Add rounds
 */
void CWeaponInfo::AddUses(const int moreUses)
{
	this->iUses += moreUses;
}