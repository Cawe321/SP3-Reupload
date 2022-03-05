#include "CUpgrade.h"

CUpgrade::CUpgrade()
{
	cCough = nullptr;
	cVirus = nullptr;
	dDayPhaseTime = nullptr;
	cPlayer = nullptr;
	cVaccine = nullptr;
	iCoughRangeLevel = iVirusStrengthLevel = iLengthOfDayLevel = iMovementSpeedLevel = iMutateVirusLevel = 0;
	iVirusLethalityLevel = 1;
}

CUpgrade::~CUpgrade()
{
}

void CUpgrade::SetCoughRangeLevel(int iCoughRangeLevel)
{
	this->iCoughRangeLevel = iCoughRangeLevel;
}

int CUpgrade::GetCoughRangeLevel()
{
	return iCoughRangeLevel;
}

int CUpgrade::GetCoughUpgradeCost()
{
	double dMultiplier = 1;
	for (int i = 0; i < iCoughRangeLevel; ++i)
	{
		dMultiplier *= dCoughRangeCostMultiplier;
	}
	return iCoughRangeCost * dMultiplier;
}

bool CUpgrade::CanAffordCoughRangeUpgrade()
{
	return (cVirus->GetPoints() >= GetCoughUpgradeCost());
}

void CUpgrade::BuyCoughRangeUpgrade()
{
	if (CanAffordCoughRangeUpgrade() && iCoughRangeLevel < 10)
	{
		cVirus->ReducePoints(GetCoughUpgradeCost());
		++iCoughRangeLevel;
	}
}

void CUpgrade::SetVirusStrengthLevel(int iVirusStrengthLevel)
{
	this->iVirusStrengthLevel = iVirusStrengthLevel;
}

int CUpgrade::GetVirusStrengthLevel()
{
	return iVirusStrengthLevel;
}

int CUpgrade::GetVirusStrengthCost()
{
	double dMultiplier = 1;
	for (int i = 0; i < iVirusStrengthLevel; ++i)
	{
		dMultiplier *= dVirusStrengthCostMultiplier;
	}
	return iVirusStrengthCost * dMultiplier;
}

bool CUpgrade::CanAffordVirusStrengthUpgrade()
{
	return (cVirus->GetPoints() >= GetVirusStrengthCost());
}

void CUpgrade::BuyVirusStrengthUpgrade()
{
	if (CanAffordVirusStrengthUpgrade() && iVirusStrengthLevel < 10)
	{
		cVirus->ReducePoints(GetVirusStrengthCost());
		++iVirusStrengthLevel;
	}
}

void CUpgrade::SetVirusLethalityLevel(int iVirusLethalityLevel)
{
	this->iVirusLethalityLevel = iVirusLethalityLevel;
}

int CUpgrade::GetVirusLethalityLevel()
{
	return iVirusLethalityLevel;
}

int CUpgrade::GetVirusLethalityCost()
{
	double dMultiplier = 1;
	for (int i = 0; i < iVirusLethalityLevel; ++i)
	{
		dMultiplier *= dVirusLethalityCostMultiplier;
	}
	return iVirusLethalityCost * dMultiplier;
}

bool CUpgrade::CanAffordVirusLethalityUpgrade()
{
	return (cVirus->GetPoints() >= GetVirusLethalityCost());
}

void CUpgrade::BuyVirusLethalityUpgrade()
{
	if (CanAffordVirusLethalityUpgrade() && iVirusLethalityLevel < 10)
	{
		cVirus->ReducePoints(GetVirusLethalityCost());
		++iVirusLethalityLevel;
	}
}

void CUpgrade::SetLengthOfDayLevel(int iLengthOfDayLevel)
{
	this->iLengthOfDayLevel = iLengthOfDayLevel;
}

int CUpgrade::GetLengthOfDayLevel()
{
	return iLengthOfDayLevel;
}

int CUpgrade::GetLengthOfDayCost()
{
	double dMultiplier = 1;
	for (int i = 0; i < iLengthOfDayLevel; ++i)
	{
		dMultiplier *= dLengthOfDayCostMultiplier;
	}
	return iLengthOfDayCost * dMultiplier;
}

bool CUpgrade::CanAffordLengthOfDayUpgrade()
{
	return (cVirus->GetPoints() >= GetLengthOfDayCost());
}

void CUpgrade::BuyLengthOfDayUpgrade()
{
	if (CanAffordLengthOfDayUpgrade() && iLengthOfDayLevel < 10)
	{
		cVirus->ReducePoints(GetLengthOfDayCost());
		++iLengthOfDayLevel;
	}
}

void CUpgrade::SetMovementSpeedLevel(int iMovementSpeedLevel)
{
	this->iMovementSpeedLevel = iMovementSpeedLevel;
}

int CUpgrade::GetMovementSpeedLevel()
{
	return iMovementSpeedLevel;
}

int CUpgrade::GetMovementSpeedCost()
{
	double dMultiplier = 1;
	for (int i = 0; i < iMovementSpeedLevel; ++i)
	{
		dMultiplier *= dMovementSpeedCostMultiplier;
	}
	return iMovementSpeedCost * dMultiplier;
}

bool CUpgrade::CanAffordMovementSpeedUpgrade()
{
	return (cVirus->GetPoints() >= GetMovementSpeedCost());
}

void CUpgrade::BuyMovementSpeedUpgrade()
{
	if (CanAffordMovementSpeedUpgrade() && iMovementSpeedLevel < 10)
	{
		cVirus->ReducePoints(GetMovementSpeedCost());
		++iMovementSpeedLevel;
	}
}

void CUpgrade::SetMutateVirusLevel(int iMutateVirusLevel)
{
	this->iMutateVirusLevel = iMutateVirusLevel;
}

int CUpgrade::GetMutateVirusLevel()
{
	return iMutateVirusLevel;
}

int CUpgrade::GetMutateVirusCost()
{
	double dMultiplier = 1;
	for (int i = 0; i < iMutateVirusLevel; ++i)
	{
		dMultiplier *= dDestroyVaccineCostMultiplier;
	}
	return iDestroyVaccineCost * dMultiplier;
}

bool CUpgrade::CanAffordMutateVirusUpgrade()
{
	return (cVirus->GetPoints() >= GetMutateVirusCost());
}

void CUpgrade::BuyMutateVirusUpgrade()
{
	if (CanAffordMutateVirusUpgrade() && iMutateVirusLevel < 10)
	{
		cVirus->ReducePoints(GetMutateVirusCost());
		++iMutateVirusLevel;
		cVaccine->DestoryProgress(dDestroyVaccineProgressPercentage / 100.0 * cVaccine->GetProgress());
	}
}

int CUpgrade::GetVirusLauncherAmmoCost()
{
	return iVirusLauncherAmmoCost;
}

int CUpgrade::GetInfectedAirFreshenerAmmoCost()
{
	return iInfectedAirFreshenerCost;
}

void CUpgrade::InitLevels(int iCoughRangeLevel, int iVirusStrengthLevel, int iVirusLethalityLevel, int iLengthOfDayLevel, int iMovementSpeedLevel, int iMutateVirusLevel)
{
	this->iCoughRangeLevel = iCoughRangeLevel;
	this->iVirusStrengthLevel = iVirusStrengthLevel;
	this->iVirusLethalityLevel = iVirusLethalityLevel;
	this->iLengthOfDayLevel = iLengthOfDayLevel;
	this->iMovementSpeedLevel = iMovementSpeedLevel;
	this->iMutateVirusLevel = iMutateVirusLevel;
}

void CUpgrade::AttachHandlers(CCough* cCough, CVirus* cVirus, double* dDayPhaseTime, CPlayer* cPlayer, CVaccine* cVaccine)
{
	this->cCough = cCough;
	this->cVirus = cVirus;
	this->dDayPhaseTime = dDayPhaseTime;
	this->cPlayer = cPlayer;
	this->cVaccine = cVaccine;
}

void CUpgrade::UpdateValues()
{
	double dMultiplier = 1;
	for (int i = 0; i < iCoughRangeLevel; ++i)
	{
		dMultiplier *= dCoughRangeMultiplier;
	}
	cCough->SetWeaponRange((double)iInitialCoughRange * dMultiplier);

	dMultiplier = 1;
	for (int i = 0; i < iVirusStrengthLevel; ++i)
	{
		dMultiplier *= dVirusStrengthMultiplier;
	}
	cVirus->IncreaseStrength(fInitialVirusStrength * dMultiplier - cVirus->GetStrength());

	dMultiplier = 1;
	for (int i = 0; i < iVirusLethalityLevel; ++i)
	{
		dMultiplier *= dVirusLethalityMultiplier;
	}
	cVirus->IncreaseLethality(fInitialVirusLethality * dMultiplier - cVirus->GetLethality());

	dMultiplier = 1;
	for (int i = 0; i < iLengthOfDayLevel; ++i)
	{
		dMultiplier *= dLengthOfDayMultiplier;
	}
	*dDayPhaseTime = dInitialLengthOfDay * dMultiplier;

	dMultiplier = 1;
	cPlayer->SetSpeed(iInitialMovementSpeed + dMovementSpeedBonus * iMovementSpeedLevel);

	if (cVaccine->GetVacineState() == true)
	{
		cVaccine->SetProgress(cVaccine->GetProgress() + (dVaccineBaseProgress + cVaccine->GetAwarness()));
	}
}
