#include "Player.h"
#include "Application.h"
#include "LoadHmap.h"
#include "SoundController.h"
#include "EntityManager.h"

void CPlayer::Init(const Vector3& pos, const Vector3& dir, std::vector<unsigned char> m_heightMap)
{
	this->vec3Position = pos;
	this->vec3DirectionFront = dir;
	this->m_heightMap = m_heightMap;

	SetType(PLAYER);

	SetSpeed(80.f);
}

void CPlayer::Reset()
{
	vec3Position.Set(1, 0, 0);
	vec3DirectionFront.Set(0, 0, 1);
	cPrimaryWeapon = NULL;
	cSecondaryWeapon = NULL;
	cTertiaryWeapon = NULL;
	cEquippedWeapon = NULL;
}

void CPlayer::Update(double dt)
{
	static const float m_worldWidth = 4000.f;
	vec3Position.y = 350.f * ReadHeightMap(m_heightMap, vec3Position.x / m_worldWidth, vec3Position.z / m_worldWidth);

	vec3PreviousPosition = vec3Position;
	vec3DirectionFront.Normalize();

	if (cPrimaryWeapon)
	{
		cPrimaryWeapon->Update(dt);
	}
	if (cSecondaryWeapon)
	{
		cSecondaryWeapon->Update(dt);
	}
	if (cTertiaryWeapon)
	{
		cTertiaryWeapon->Update(dt);
	}

	if (Application::IsKeyPressed('A')) //Rotate left
	{
		//vec3Position.x += (float)(PLAYER_SPEED * dt);

		fRotationAngle -= 1.f;
		vec3DirectionFront.x = cos(Math::DegreeToRadian(fRotationAngle));
		vec3DirectionFront.z = sin(Math::DegreeToRadian(fRotationAngle));
		vec3DirectionFront.Normalize();
	}
	if (Application::IsKeyPressed('D')) //Rotate right
	{
		//vec3Position.x -= (float)(PLAYER_SPEED * dt);

		fRotationAngle += 1.f;

		vec3DirectionFront.x = cos(Math::DegreeToRadian(fRotationAngle));
		vec3DirectionFront.z = sin(Math::DegreeToRadian(fRotationAngle));
		vec3DirectionFront.Normalize();
	}
	if (Application::IsKeyPressed('W')) //Move forward
	{
		//vec3Position.z += (float)(PLAYER_SPEED * dt);
		vec3Position += vec3DirectionFront * (float)(fSpeed * dt);
	}
	if (Application::IsKeyPressed('S')) //Move backward
	{
		//vec3Position.z -= (float)(PLAYER_SPEED * dt);
		vec3Position -= vec3DirectionFront * (float)(fSpeed * dt);
	}

	if (Constraint())
		RollBackPosition();
}

void CPlayer::SetWeapon(const int iSlot, CWeaponInfo* cWeaponInfo)
{
	if (iSlot == 0)
		cPrimaryWeapon = cWeaponInfo;
	else if (iSlot == 1)
		cSecondaryWeapon = cWeaponInfo;
	else if (iSlot == 2)
		cTertiaryWeapon = cWeaponInfo;
	//More ifs for more weapons
}

CWeaponInfo* CPlayer::GetWeapon()
{
	if (iCurrentWeapon == 0)
	{
		cEquippedWeapon = cPrimaryWeapon;
		return cPrimaryWeapon;
	}
	else if (iCurrentWeapon == 1)
	{
		cEquippedWeapon = cSecondaryWeapon;
		return cSecondaryWeapon;
	}
	else if (iCurrentWeapon == 2)
	{
		cEquippedWeapon = cTertiaryWeapon;
		return cTertiaryWeapon;
	}
	return NULL;
}

void CPlayer::SetCurrentWeapon(const int iSlot)
{
	iCurrentWeapon = iSlot;
}

CProjectile* CPlayer::Attack(void) const
{
	if (cEquippedWeapon == cPrimaryWeapon)
	{
		return cPrimaryWeapon->Use(Vector3(vec3Position.x, vec3Position.y + 25, vec3Position.z), vec3DirectionFront, (CEntity*)this);
	}
	if (cEquippedWeapon == cSecondaryWeapon)
	{
		return cSecondaryWeapon->Use(Vector3(vec3Position.x, vec3Position.y + 25, vec3Position.z), vec3DirectionFront, (CEntity*)this);
	}
	if (cEquippedWeapon == cTertiaryWeapon)
	{
		return cTertiaryWeapon->Use(Vector3(vec3Position.x, vec3Position.y + 25, vec3Position.z), vec3DirectionFront, (CEntity*)this);
	}

	return NULL;
}

CWeaponInfo* CPlayer::AccessWeapon(const int iSlot)
{
	if (iSlot == 0)
		return cPrimaryWeapon;
	else if (iSlot == 1)
		return cSecondaryWeapon;
	else if (iSlot == 2)
		return cTertiaryWeapon;

	return NULL;
}

CPlayer::CPlayer()
	: iSlot(0)
	, iCurrentWeapon(0)
	, cPrimaryWeapon(NULL)
	, cSecondaryWeapon(NULL)
	, cTertiaryWeapon(NULL)
	, cEquippedWeapon(NULL)
{
}

CPlayer::~CPlayer()
{
	Reset();
}
