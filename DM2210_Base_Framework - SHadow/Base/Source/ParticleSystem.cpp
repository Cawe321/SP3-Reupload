#include "ParticleSystem.h"


#include <algorithm>

bool CompareDist(const ParticleObject* Obj1, const ParticleObject* Obj2)
{
	if (Obj1->active == true && Obj2->active == true)
	{
		//return abs((Obj1->pos - Obj1->camerapos).Length()) < abs((Obj2->pos - Obj1->camerapos).Length());
		return sqrt(pow(Obj1->pos.x - Obj1->camerapos.x, 2) + pow(Obj1->pos.y - Obj1->camerapos.y, 2) + pow(Obj1->pos.z - Obj1->camerapos.z, 2)) < sqrt(pow(Obj2->pos.x - Obj2->camerapos.x, 2) + pow(Obj2->pos.y - Obj2->camerapos.y, 2) + pow(Obj2->pos.z - Obj2->camerapos.z, 2));
	}
	else
	{
		return false;
	}
}


ParticleSystem::ParticleSystem():
	v3gravity(Vector3(0,-9.8f,0)),
	iMAX_PARTICLES(4500),
	fWorldHeight(100),
	fWorldLength(100),
	fWorldWidth(100),
	iActiveParticleNum(0),
	m_heightMap(NULL)
{
	for (int x = 0; x < iMAX_PARTICLES; ++x)
	{
		ParticleObject* Part = new ParticleObject(ParticleObject::P_VIRUS);
		Part->active = false;
		m_PList.push_back(Part);
	}
}


ParticleSystem::~ParticleSystem()
{
	while (m_PList.size() > 0)
	{
		ParticleObject* Part = m_PList.back();
		delete Part;
		m_PList.pop_back();
	}
}

void ParticleSystem::Init()
{

}

void ParticleSystem::Update(double dt, Vector3 Camerapos)
{

	for (std::vector<ParticleObject*>::iterator it = m_PList.begin(); it != m_PList.end(); ++it)
	{
		ParticleObject* Part = (ParticleObject*)*it;

		Part->camerapos = Camerapos;
		if (Part->active)
		{
			if (Part->TYPE == ParticleObject::P_VIRUS)
			{
				Part->vel += v3gravity * dt * 0.05; //Falling
				Part->pos += Part->vel * (float)dt * 7.0f;

				Part->timealive -= dt;

				if (Part->timealive <= 0)
				{
					Part->active = false;
					--iActiveParticleNum;
				}
			}
			else if (Part->TYPE == ParticleObject::P_AIRFRESHNER)
			{
				Part->pos += Part->vel * (float)dt * 7.0f;

				Part->timealive -= dt;

				if (Part->timealive <= 0)
				{
					Part->active = false;
					--iActiveParticleNum;
				}
			}
			else if (Part->TYPE == ParticleObject::P_VIRUS_SHOT)
			{
				//Dont update its updates in projectile
			}
			
		}
	}

	std::sort(m_PList.begin(), m_PList.end(),CompareDist);
	//cout << iActiveParticleNum << endl;
	 
}

vector<ParticleObject*> ParticleSystem::ReturnList()
{
	return m_PList;
}

void ParticleSystem::ReturnParticleObject(ParticleObject* PO)
{
	//Look for it within the list incase something gets passed in thats not a particle from here
	for (std::vector<ParticleObject*>::iterator it = m_PList.begin(); it != m_PList.end(); ++it)
	{
		ParticleObject* go = (ParticleObject*)*it;
		if (PO == go) //If its found in the list
		{
			PO->active = false;
			--iActiveParticleNum;
		}
	}

}

ParticleObject* ParticleSystem::GetNewParticle(ParticleObject::PraticleObject_TYPE Type)
{
	if (iActiveParticleNum < iMAX_PARTICLES)
	{
 		for (std::vector<ParticleObject*>::iterator it = m_PList.begin(); it != m_PList.end(); ++it)
		{
			ParticleObject* go = (ParticleObject*)*it;

			if (go->active == false)
			{
				++iActiveParticleNum;
				go->active = true;
				switch (Type)
				{
				case ParticleObject::PraticleObject_TYPE::P_VIRUS:
					go->TYPE = ParticleObject::PraticleObject_TYPE::P_VIRUS;
					break;
				case ParticleObject::PraticleObject_TYPE::P_AIRFRESHNER:
					go->TYPE = ParticleObject::PraticleObject_TYPE::P_AIRFRESHNER;
					break;
				}
				return go;
			}
		}

	}
	return NULL;
}

void ParticleSystem::SetSystemGravity(Vector3 v3Grav)
{
	v3gravity = v3Grav;
}

void ParticleSystem::SetSystemWorldSize(Vector3 v3Size)
{
	fWorldLength = v3Size.x;
	fWorldHeight = v3Size.y;
	fWorldWidth = v3Size.z;
}

void ParticleSystem::SetSystemMap(vector<unsigned char> vheightMap)
{
	m_heightMap = vheightMap;
}

void ParticleSystem::SetParticlePosition(ParticleObject* go, Vector3 v3Position)
{
	go->pos = v3Position;
}

ParticleObject* ParticleSystem::SpawnVirusParticle(Vector3 v3Pos)
{
	ParticleObject* cPO = GetNewParticle(ParticleObject::PraticleObject_TYPE::P_VIRUS); //c Particle Object
	cPO->pos = v3Pos;
	cPO->focalPos = v3Pos;
	cPO->timealive = 1;
	cPO->vel.Set((rand() % 10) - 5, (rand() % 10) - 5, (rand() % 10) - 5);
	cPO->scale.Set(5, 5, 5);

	return cPO;
}

ParticleObject* ParticleSystem::SpawnAirFreshnerParticle(Vector3 v3Pos)
{
	ParticleObject* cPO = GetNewParticle(ParticleObject::PraticleObject_TYPE::P_AIRFRESHNER); //c Particle Object
	cPO->pos = v3Pos;
	cPO->focalPos = v3Pos;
	cPO->timealive = 1;
	cPO->vel.Set(0, 9.8, 0);
	cPO->scale.Set(5, 5, 5);

	return cPO;
}

ParticleObject* ParticleSystem::SpawnVirusShotParticle(Vector3 v3Pos)
{
	ParticleObject* cPO = GetNewParticle(ParticleObject::PraticleObject_TYPE::P_VIRUS_SHOT); //c Particle Object
	cPO->pos = v3Pos;
	cPO->focalPos = v3Pos;

	cPO->vel.SetZero();
	cPO->scale.Set(7, 7, 7);
	return cPO;
}




