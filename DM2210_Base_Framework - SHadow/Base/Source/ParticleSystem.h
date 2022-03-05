#pragma once

#include "Particle.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <array>
#include "LoadHmap.h"

#include "SpriteAnimation.h"
#include "SingletonTemplate.h"




using namespace std;

class ParticleSystem : public CSingletonTemplate<ParticleSystem>
{
	friend class CSingletonTemplate<ParticleSystem>;
public:
	void Init();
	void Update(double dt, Vector3 CameraPos);
	vector<ParticleObject*> ReturnList();
	void ReturnParticleObject(ParticleObject* PO);

	ParticleObject* GetNewParticle(ParticleObject::PraticleObject_TYPE Type = ParticleObject::P_NONE);

	void SetSystemGravity(Vector3 v3Grav);
	void SetSystemWorldSize(Vector3 v3Size);
	void SetSystemMap(vector<unsigned char> vheightMap);

	void SetParticlePosition(ParticleObject* go, Vector3 v3Pos);
	

	ParticleObject* SpawnVirusParticle(Vector3 v3Pos);
	ParticleObject* SpawnAirFreshnerParticle(Vector3 v3Pos);
	ParticleObject* SpawnVirusShotParticle(Vector3 v3Pos);

protected:
	ParticleSystem();
	~ParticleSystem();

private:

	Vector3 v3gravity;
	vector<ParticleObject*> m_PList; //Particle List
	const int iMAX_PARTICLES;


	int iActiveParticleNum;


	float fWorldWidth;
	float fWorldHeight;
	float fWorldLength;

	std::vector<unsigned char> m_heightMap;

	
};

