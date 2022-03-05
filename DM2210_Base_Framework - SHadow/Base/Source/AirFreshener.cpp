#include "AirFreshener.h"
#include "SoundController.h"
#include "filesystem.h"

/**
@brief Default Constructor
*/
CAirFreshener::CAirFreshener()
{
}

/**
@brief Default Destructor
*/
CAirFreshener::~CAirFreshener()
{
}

/**
@brief Initialise this instance to default values
*/
bool CAirFreshener::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();

	// Set weapon type
	SetWeaponType(CONTAMINATED_AIR_FRESHNER);
	// The damage of the weapon
	//SetWeaponDamage(2);

	// Range of the weapon
	SetWeaponRange(10);

	SetUses(5);

	// The time between shots
	dTimeBetweenUses = 3.f;
	// The elapsed time (between shots)
	dElapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;

	return true;
}

CProjectile* CAirFreshener::Use(Vector3 vec3Position, Vector3 vec3Front, CEntity* pSource)
{
	if (bFire)
	{
		if (iUses > 0)
		{
			iUses--;

			CProjectile* aProjectile = new CProjectile();
			aProjectile->Init(vec3Position, vec3Front, 10.0f, 0.f);
			aProjectile->ActivateCollider(Vector3(-75.f, -2.f, -75.f), Vector3(75.f, 2.f, 75.f));
			aProjectile->SetStatus(true);
			aProjectile->SetSource(pSource);
			aProjectile->SetProjectileType(CProjectile::PROJECTILE_TYPE::AIR_FRESHENER);

			CSoundController::GetInstance()->PlaySoundByID(4);

			// Lock the weapon after this discharge
			bFire = false;

			return aProjectile;
		}
	}

	return NULL;
}
