#include "Cough.h"
#include "SoundController.h"

/**
@brief Default Constructor
*/
CCough::CCough()
{
}

/**
@brief Default Destructor
*/
CCough::~CCough()
{
}

/**
@brief Initialise this instance to default values
*/
bool CCough::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();

	// Set weapon type
	SetWeaponType(COUGH);
	// The damage of the weapon
	SetWeaponDamage(2);

	// Range of the weapon
	SetWeaponRange(50);

	SetUses(10000);

	// The time between shots
	dTimeBetweenUses = 1.5f;
	// The elapsed time (between shots)
	dElapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;

	return true;
}

CProjectile* CCough::Use(Vector3 vec3Position, Vector3 vec3Front, CEntity* pSource)
{
	if (bFire)
	{
		if (iUses > 0)
		{
			iUses--;

			CProjectile* aProjectile = new CProjectile();
			aProjectile->Init(vec3Position + vec3Front * 0.75f, vec3Front, 100.0f, 15.0f);
			aProjectile->ActivateCollider(Vector3(-4.f, -2.f, -4.f), Vector3(4.f, 2.f, 4.f));
			aProjectile->SetStatus(true);
			aProjectile->SetSource(pSource);
			aProjectile->SetProjectileType(CProjectile::PROJECTILE_TYPE::COUGH);

			for (int incVol = 0; incVol < 10; ++incVol)
				CSoundController::GetInstance()->VolumeIncrease(2);

			CSoundController::GetInstance()->PlaySoundByID(2);
			// Lock the weapon after this discharge
			bFire = false;

			return aProjectile;
		}
	}

	return NULL;
}
