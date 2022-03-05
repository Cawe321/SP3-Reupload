#include "VirusLauncher.h"
#include "SoundController.h"

/**
@brief Default Constructor
*/
CVirusLauncher::CVirusLauncher()
{
}

/**
@brief Default Destructor
*/
CVirusLauncher::~CVirusLauncher()
{
}

/**
@brief Initialise this instance to default values
*/
bool CVirusLauncher::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();

	// Set weapon type
	SetWeaponType(VIRUS_LAUNCHER);
	// The damage of the weapon
	//SetWeaponDamage(1);

	// Range of the weapon
	SetWeaponRange(500);

	SetUses(30);

	// The time between shots
	dTimeBetweenUses = 0.2f;
	// The elapsed time (between shots)
	dElapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;

	return true;
}

CProjectile* CVirusLauncher::Use(Vector3 vec3Position, Vector3 vec3Front, CEntity* pSource)
{
	if (bFire)
	{
		if (iUses > 0)
		{
			iUses--;

			CProjectile* aProjectile = new CProjectile();
			aProjectile->Init(vec3Position + vec3Front * 0.75f, vec3Front, 100.0f, 25.0f);
			aProjectile->ActivateCollider(Vector3(-4.f, -2.f, -4.f), Vector3(4.f, 2.f, 4.f));
			aProjectile->SetStatus(true);
			aProjectile->SetSource(pSource);
			aProjectile->SetProjectileType(CProjectile::PROJECTILE_TYPE::VIRUS_LAUNCHER);

			CSoundController::GetInstance()->PlaySoundByID(3);

			// Lock the weapon after this discharge
			bFire = false;

			return aProjectile;
		}
	}

	return NULL;
}
