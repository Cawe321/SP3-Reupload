#include "NPC.h"
#include "LoadHmap.h"
#include "MyMath.h"

void CNPC::Init(const Vector3& pos, const Vector3& dir, std::vector<unsigned char> m_heightMap)
{
	this->vec3Position = pos;
	this->vec3DirectionFront = dir;
	this->m_heightMap = m_heightMap;

	cProgressBar3D = new CProgressBar3D();
	
	SetType(NPC);
	SetSpeed(70.f);
	SetHealth(100);
	eState = HEALTHY;
	fMaxHealth = fHealth;

	dProgressBarTimer = 0;

 	cProgressBar3D->SetMaxValue(fMaxHealth);
	cProgressBar3D->SetValue(fMaxHealth);
	cProgressBar3D->SetPosition(pos);
}

void CNPC::Reset()
{
	vec3Position.Set(1, 0, 0);
	vec3DirectionFront.Set(0, 0, 1);
}

void CNPC::Update(double dt)
{
	vec3PreviousPosition = vec3Position;
	static const float m_worldWidth = 4000.f;
	vec3Position.y = 350.f * ReadHeightMap(m_heightMap, vec3Position.x / m_worldWidth, vec3Position.z / m_worldWidth) - 2;

	if (iDaysLeft <= 0)
		eState = STATE::DEAD;

	if (isInvinc())
		SetSpeed(35.f);
	else
		SetSpeed(70.f);

	if (eState == STATE::INFECTED)
	{
		SetSpeed(40.f);
	}

	vec3Position.x += fSpeed * dt * vec3DirectionFront.x;
	vec3Position.z += fSpeed * dt * vec3DirectionFront.z;


	if (Constraint())
	{
		RollBackPosition();
		UpdateMovement();
	}

	dElapsedTime += dt;

	if (dElapsedTime > dDirectionChange)
	{
		UpdateMovement();
	}

	diFrame -= dt;

	if (fHealth <= 0)
		SetInfection();

	//Progress Bar
	dProgressBarTimer -= dt;

	if (dProgressBarTimer <= 0)
	{
		cProgressBar3D->SetActive(false);
	}
	else
	{
		cProgressBar3D->SetActive(true);
	}

	cProgressBar3D->SetPosition(Vector3(vec3Position.x, vec3Position.y + 50, vec3Position.z));
	cProgressBar3D->SetValue(fHealth);


	dParticleTime -= dt;
	if (CanParticleSPawn() == true)
	{
		ResetParticleTime();
		ParticleSystem::GetInstance()->SpawnVirusParticle(Vector3(vec3Position.x, vec3Position.y + 20, vec3Position.z));
	}
}

void CNPC::SetHealth(float fHealth)
{
	this->fHealth = fHealth;
	cProgressBar3D->SetValue(fHealth);
}

void CNPC::SetiFrame(double diFrame)
{
	this->diFrame = diFrame;
}

void CNPC::ResetParticleTime()
{
	dParticleTime = 0.5;
}

void CNPC::SetActive(bool bSetActive)
{
	this->bActive = bSetActive;
}

void CNPC::SetProgressActive(bool bActive)
{
	cProgressBar3D->SetActive(bActive);
}

float CNPC::GetHealth(void)
{
	return fHealth;
}

double CNPC::GetiFrame(void)
{
	return diFrame;
}

bool CNPC::isInvinc(void)
{
	if (diFrame > 0)
		return true;
	else
		return false;
}

bool CNPC::CanParticleSPawn(void)
{
	if (dParticleTime <= 0 && eState == STATE::INFECTED && bActive == true)
	{
		return true;
	}
	else
		return false;
}

bool CNPC::GetActive()
{
	return bActive;
}


bool CNPC::GetProgressBarActive()
{
	return cProgressBar3D->GetActive();
}

int CNPC::GetDaysLeft(void)
{
	return iDaysLeft;
}

void CNPC::SetInfection()
{
	eState = INFECTED;
}


void CNPC::SetProgressBarTimer(double dTime)
{
	dProgressBarTimer = dTime;
}

void CNPC::ReduceDaysLeft(int iDaysLeft)
{
	this->iDaysLeft -= iDaysLeft;
	if (this->iDaysLeft <= 0)
	{
		eState = DEAD;
	}
}

CNPC::STATE CNPC::GetState(void) const
{
	return eState;
}

void CNPC::SetDaysLeft(int iDaysLeft)
{
	this->iDaysLeft = iDaysLeft;
}

void CNPC::SetState(CNPC::STATE state)
{
	eState = state;
}

void CNPC::UpdateMovement()
{
	Vector3 newDirection = Vector3(Math::RandFloatMinMax(-1, 1), 0, Math::RandFloatMinMax(-1, 1));
	newDirection.Normalize();

	vec3DirectionFront = newDirection;
	dElapsedTime = 0.0;
}

CProgressBar3D* CNPC::GetcProgressBar()
{
	if (cProgressBar3D == NULL)
	{
		cProgressBar3D = new CProgressBar3D;
		cProgressBar3D->SetMaxValue(fMaxHealth);
		cProgressBar3D->SetValue(fHealth);
		cProgressBar3D->SetPosition(vec3Position);
	}

	return cProgressBar3D;
}

CNPC::CNPC()
	: fHealth(10)
	, fMaxHealth(10)
	, iDaysLeft(6)
	, dElapsedTime(0.0)
	, dDirectionChange(10.0)
	, diFrame(0.0)
{
}

CNPC::~CNPC()
{
	Reset();
}
