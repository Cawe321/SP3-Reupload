#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "WeaponInfo.h"
#include "Projectile.h"
#include "SingletonTemplate.h"

const unsigned PLAYER_SPEED = 50.f;

class CPlayer : public CSingletonTemplate<CPlayer>, public CEntity
{
	friend CSingletonTemplate<CPlayer>;
public:
	virtual void Init(const Vector3& pos, const Vector3& dir, std::vector<unsigned char> m_heightMap);
	virtual void Reset();
	virtual void Update(double dt);

	virtual void SetWeapon(const int iSlot, CWeaponInfo* cWeaponInfo);
	virtual CWeaponInfo* GetWeapon();

	virtual void SetCurrentWeapon(const int iSlot);

	virtual CProjectile* Attack(void) const;

	virtual CWeaponInfo* AccessWeapon(const int iSlot);
	
	CPlayer();
	~CPlayer();

private:
	//Member variables
	//To be added:
	//Weapons
	int iSlot;
	int iCurrentWeapon;
	CWeaponInfo* cPrimaryWeapon;
	CWeaponInfo* cSecondaryWeapon;
	CWeaponInfo* cTertiaryWeapon;

	CWeaponInfo* cEquippedWeapon;
};

#endif
