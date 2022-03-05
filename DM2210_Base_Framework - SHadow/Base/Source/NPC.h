#ifndef NPC_H
#define NPC_H

#include "Entity.h"
#include "CProgressBar3D.h"
#include "ParticleSystem.h"

const unsigned NPC_SPEED = 50.f;

class CNPC : public CEntity
{
public:
	enum STATE
	{
		HEALTHY = 0,
		INFECTED,
		DEAD,
		VACCINATED,
		NUM_STATE,
	};

	virtual void Init(const Vector3& pos, const Vector3& dir, std::vector<unsigned char> m_heightMap);
	virtual void Reset();
	virtual void Update(double dt);

	virtual void SetHealth(float fHealth);
	virtual void SetiFrame(double diFrame);
	virtual void ResetParticleTime();
	virtual void SetActive(bool bSetActive);
	virtual void SetProgressActive(bool bActive);
	virtual void SetInfection();
	virtual void SetProgressBarTimer(double dTime);

	virtual void ReduceDaysLeft(int iDaysLeft);

	virtual float GetHealth(void);
	virtual double GetiFrame(void);
	virtual bool isInvinc(void);
	virtual bool CanParticleSPawn(void);
	virtual bool GetActive();

	virtual bool GetProgressBarActive();
	virtual CProgressBar3D* GetcProgressBar();
	virtual CNPC::STATE GetState(void) const;
	virtual void SetDaysLeft(int iDaysLeft);
	virtual int GetDaysLeft(void);




	virtual void SetState(CNPC::STATE state);

	virtual void UpdateMovement();



	CNPC();
	~CNPC();

private:
	//Member variables
	float fHealth;
	float fMaxHealth;
	int iDaysLeft;
	bool bInfected;
	bool bDead;

	bool bActive;

	double dDirectionChange;
	double dElapsedTime;
	double dParticleTime;
	double dProgressBarTimer;

	double diFrame;
	
	STATE eState;

	CProgressBar3D* cProgressBar3D;
};

#endif
