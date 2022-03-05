#include "EntityManager.h"
#include "NPC.h"
#include "Player.h"
#include "PickUp.h"

void CEntityManager::Init(void)
{
	lEntity.clear();
}

void CEntityManager::Add(CEntity* cEntity)
{
	lEntity.push_back(cEntity);
}

bool CEntityManager::Erase(CEntity* cEntity)
{
	// Find the entity's iterator
	std::list<CEntity*>::iterator findIter = std::find(lEntity.begin(), lEntity.end(), cEntity);

	// Delete the entity if found
	if (findIter != lEntity.end())
	{
		// Delete the CEntity3D
		delete* findIter;
		// Go to the next iteration after erasing from the list
		findIter = lEntity.erase(findIter);
		return true;
	}
	// Return false if not found
	return false;
}

bool CEntityManager::CollisionCheck(CEntity* cEntity)
{
	bool bResult = false;

	std::list<CEntity*>::iterator it, end;
	end = lEntity.end();
	for (it = lEntity.begin(); it != end; ++it)
	{
		// Check for collisions between the 2 entities
		if (cEntity->CheckForCollision(*it) == true)
		{
			if ((*it)->GetType() == CEntity::ENTITY_TYPE::NPC)
			{
				if (((CNPC*)(*it))->GetActive() == true)
				{
					cEntity->RollBackPosition();
					(*it)->RollBackPosition();
					((CNPC*)(*it))->UpdateMovement();

					bResult = true;
					break;
				}
			}
			else if ((*it)->GetType() == CEntity::ENTITY_TYPE::PROJECTILE)
			{
				break;
			}
			else if ((*it)->GetType() == CEntity::ENTITY_TYPE::STRUCTURE)
			{
				cEntity->RollBackPosition();
				bResult = true;
				break;
			}
			else if ((*it)->GetType() == CEntity::ENTITY_TYPE::PICKUP)
			{
				((CPickUp*)(*it))->Effect();
				(*it)->SetToDelete(true);
				bResult = true;
				break;
			}
		}
	}
	return bResult;
}

void CEntityManager::Update(const double dElapsedTime)
{
	std::list<CEntity*>::iterator it, end;
	std::list<CEntity*>::iterator it_other;

	// Update all CEntity3D
	end = lEntity.end();

	for (it = lEntity.begin(); it != end; ++it)
	{
		(*it)->Update(dElapsedTime);

		if ((*it)->GetType() == CEntity::ENTITY_TYPE::PROJECTILE && ((CProjectile*)(*it))->GetProjectileType() != CProjectile::PROJECTILE_TYPE::AIR_FRESHENER)
		{
			if (CPlayer::GetInstance()->GetWeapon() != nullptr)
			{
				if ((CPlayer::GetInstance()->GetPosition() - (*it)->GetPosition()).Length() >= CPlayer::GetInstance()->GetWeapon()->GetWeaponRange())
				{
					(*it)->SetToDelete(true);
					((CProjectile*)(*it))->GetParticle()->active = false; //Set particle to off
				}
			}
		}
	}

	// Check for collisions among them
	end = lEntity.end();
	for (it = lEntity.begin(); it != end; ++it)
	{
		for (it_other = it; it_other != end; ++it_other)
		{
			// If the 2 entities to check are the same, then skip this iteration
			if (it_other == it)
				continue;

			// Check for collisions between the 2 entities
			if ((*it)->CheckForCollision(*it_other) == true)
			{
				if (((*it)->GetType() == CEntity::ENTITY_TYPE::NPC) &&
					((*it_other)->GetType() == CEntity::ENTITY_TYPE::PROJECTILE))
				{
					if (((CNPC*)(*it))->GetActive() == true)
					{
						if (((CNPC*)(*it))->isInvinc() == false)
						{
							((CNPC*)(*it))->SetHealth(((CNPC*)(*it))->GetHealth() - ((CProjectile*)(*it_other))->GetProjectileDamage());

							if (((CProjectile*)(*it_other))->GetProjectileType() == CProjectile::PROJECTILE_TYPE::VIRUS_LAUNCHER)
							{
								(*it_other)->SetToDelete(true);
								((CProjectile*)(*it_other))->GetParticle()->active = false; //Set particle to off
								((CNPC*)(*it))->SetiFrame(0.f);
							}

							if ((((CProjectile*)(*it_other))->GetProjectileType() == CProjectile::PROJECTILE_TYPE::COUGH || ((CProjectile*)(*it_other))->GetProjectileType() == CProjectile::PROJECTILE_TYPE::AIR_FRESHENER))
							{
								((CNPC*)(*it))->SetiFrame(1.0);
							}
						}
						((CNPC*)(*it))->SetProgressBarTimer(1.5);
					}
				}
				else if (((*it)->GetType() == CEntity::ENTITY_TYPE::PROJECTILE) &&
					((*it_other)->GetType() == CEntity::ENTITY_TYPE::NPC))
				{
					if (((CNPC*)(*it_other))->GetActive() == true)
					{
						if (((CNPC*)(*it_other))->isInvinc() == false)
						{
							((CNPC*)(*it_other))->SetHealth(((CNPC*)(*it_other))->GetHealth() - ((CProjectile*)(*it))->GetProjectileDamage());

							if (((CProjectile*)(*it))->GetProjectileType() == CProjectile::PROJECTILE_TYPE::VIRUS_LAUNCHER)
							{
								(*it)->SetToDelete(true);
								((CProjectile*)(*it))->GetParticle()->active = false; //Set particle to off
								((CNPC*)(*it_other))->SetiFrame(0.f);
							}

							if ((((CProjectile*)(*it))->GetProjectileType() == CProjectile::PROJECTILE_TYPE::COUGH || ((CProjectile*)(*it))->GetProjectileType() == CProjectile::PROJECTILE_TYPE::AIR_FRESHENER))
							{
								((CNPC*)(*it_other))->SetiFrame(1.0);
							}
						}
						((CNPC*)(*it_other))->SetProgressBarTimer(1.5);
					}
				}
				else if (((*it)->GetType() == CEntity::ENTITY_TYPE::NPC) &&
					((*it_other)->GetType() == CEntity::ENTITY_TYPE::NPC))
				{
					if (((CNPC*)(*it))->GetActive() == true && ((CNPC*)(*it_other))->GetActive())
					{
						(*it)->RollBackPosition();
						(*it_other)->RollBackPosition();

						if (((CNPC*)(*it))->GetState() == CNPC::STATE::INFECTED && ((CNPC*)(*it_other))->GetState() == CNPC::STATE::HEALTHY)
						{
							((CNPC*)(*it_other))->SetHealth(((CNPC*)(*it_other))->GetHealth() - CVirus::GetInstance()->GetStrength() / 2);
						}
						else if (((CNPC*)(*it_other))->GetState() == CNPC::STATE::INFECTED && ((CNPC*)(*it))->GetState() == CNPC::STATE::HEALTHY)
						{
							((CNPC*)(*it))->SetHealth(((CNPC*)(*it_other))->GetHealth() - CVirus::GetInstance()->GetStrength() / 2);
						}

						((CNPC*)(*it))->UpdateMovement();
						((CNPC*)(*it_other))->UpdateMovement();
					}
				}
				else if (((*it)->GetType() == CEntity::ENTITY_TYPE::NPC) &&
					((*it_other)->GetType() == CEntity::ENTITY_TYPE::STRUCTURE))
				{
					if (((CNPC*)(*it))->GetActive() == true)
					{
						(*it)->RollBackPosition();

						((CNPC*)(*it))->UpdateMovement();
					}

					
				}
				else if (((*it)->GetType() == CEntity::ENTITY_TYPE::STRUCTURE) &&
					((*it_other)->GetType() == CEntity::ENTITY_TYPE::NPC))
				{
					if (((CNPC*)(*it_other))->GetActive() == true)
					{
						(*it_other)->RollBackPosition();

						((CNPC*)(*it_other))->UpdateMovement();
					}
				}
			}
		}
	}

	CleanUp();
}

void CEntityManager::CleanUp(void)
{
	std::list<CEntity*>::iterator it, end;
	it = lEntity.begin();
	end = lEntity.end();
	while (it != end)
	{
		if ((*it)->IsToDelete())
		{
			// Delete the CEntity3D
			delete* it;
			// Go to the next iteration after erasing from the list
			it = lEntity.erase(it);
		}
		else
		{
			// Go to the next iteration
			++it;
		}
	}
}

CEntityManager::CEntityManager(void)
{
}

CEntityManager::~CEntityManager()
{
}
