#include "CVaccine.h"

CVaccine::CVaccine() :
	fVaccineProgress(0),
	fVaccineCompletedNum(100.0f),
	fBaseUpgradeAmount(10.f),
	dVaccineAwareness(0),
	dMaxVaccineAwareness(5), //+5
	bVaccineBeingMade(false)
{
}

CVaccine::~CVaccine()
{
}

void CVaccine::Init()
{
	fVaccineProgress = 0;
	fVaccineCompletedNum = 100.0f;
	fBaseUpgradeAmount = 10.f;
	dVaccineAwareness = 0;
	dMaxVaccineAwareness = 5; //+5
	bVaccineBeingMade = false;
}

bool CVaccine::Update(double dt)
{
	return true;
}

void CVaccine::SetVacineState(bool bactive)
{
	bVaccineBeingMade = bactive;
}

bool CVaccine::GetVacineState()
{
	return bVaccineBeingMade;
}

void CVaccine::SetAwareness(float fDeadNPCs)
{
	dVaccineAwareness = 0.25 * fDeadNPCs;
	if (dVaccineAwareness > dMaxVaccineAwareness)
	{
		dVaccineAwareness = dMaxVaccineAwareness;
	}
}

double CVaccine::GetAwarness()
{
	return dVaccineAwareness;
}

void CVaccine::SetProgress(float fProgress)
{
	fVaccineProgress = fProgress;
}

float CVaccine::GetProgress()
{
	return fVaccineProgress;
}

void CVaccine::DestoryProgress(float dDestoyedAmount)
{
	fVaccineProgress -= dDestoyedAmount;

	if (fVaccineProgress < 0)
	{
		fVaccineProgress = 0;
	}
}

bool CVaccine::CheckCompleted()
{
	if (fVaccineProgress >= fVaccineCompletedNum)
	{
		return true;
	}
	return false;
}

void CVaccine::Restart()
{
	fVaccineProgress = 0;
	dVaccineAwareness = 0;

	dVaccineAwareness = 0;	
	bVaccineBeingMade = false;
}





