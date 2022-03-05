#include "CVirus.h"

CVirus::CVirus():
	sVirusName(""),
	fVirusStrength(0),
	fVirusLethality(1),
	fMaxVirusLethality(10),
	fMaxVirusStrength(100),
	iMutationPoints(0)
{
}

CVirus::~CVirus()
{
}

void CVirus::init()
{
	sVirusName = "";
	fVirusStrength = 0; 
	fMaxVirusStrength = 100;
	fVirusLethality = 1; 
	fMaxVirusLethality = 100;
	iMutationPoints = 0;
}

void CVirus::SetName(string sNewName)
{
	sVirusName = sNewName;
}

string CVirus::GetName()
{
	return sVirusName;
}

float CVirus::GetStrength()
{
	return fVirusStrength;
}

void CVirus::IncreaseStrength(float fIncreasedAmount)
{
	fVirusStrength += fIncreasedAmount;
	if (fVirusStrength > fMaxVirusStrength)
	{
		fVirusStrength = fMaxVirusStrength;
	}
}

float CVirus::GetMaxStrength()
{
	return fMaxVirusStrength;
}

float CVirus::GetLethality()
{
	return fVirusLethality;
}

void CVirus::IncreaseLethality(float fIncreasedAmount)
{
	fVirusLethality += fIncreasedAmount;
	if (fVirusLethality > fMaxVirusLethality)
	{
		fVirusLethality = fMaxVirusLethality;
	}
}

float CVirus::GetMaxLethality()
{
	return fMaxVirusLethality;
}

void CVirus::ReducePoints(int iReducedPoints)
{
	iMutationPoints -= iReducedPoints;
	if (iMutationPoints < 0)
	{
		iMutationPoints = 0;
	}
}

void CVirus::IncreasePoints(int iIncreasePoints)
{
	iMutationPoints += iIncreasePoints;
}

int CVirus::GetPoints()
{
	return iMutationPoints;
}

void CVirus::Restart(string sVirusNewName)
{
	sVirusName = sVirusNewName;
	fVirusStrength = 0;
	
	fVirusLethality = 0;

	iMutationPoints = 0;
}







