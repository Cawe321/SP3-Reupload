#pragma once
#include "CVaccine.h"
#include "CVirus.h"
#include "Player.h"
#include "Cough.h"

#ifndef CUPGRADE_H
#define CUPGRADE_H
class CUpgrade
{

private:
	// Handlers
	CCough* cCough;
	CVirus* cVirus;
	double* dDayPhaseTime;
	CPlayer* cPlayer;
	CVaccine* cVaccine;

	// Upgrade Levels | TO BE SAVED IN SAVE FILE
	int iCoughRangeLevel;
	int iVirusStrengthLevel;
	int iVirusLethalityLevel;
	int iLengthOfDayLevel;
	int iMovementSpeedLevel;
	int iMutateVirusLevel; // just to keep track of the costs
	
public:
	// Initial Values for NEW game to be affected by Upgrades
	const int iInitialCoughRange = 50;
	const float fInitialVirusStrength = 10; // 1
	const float fInitialVirusLethality = 1;
	const double dInitialLengthOfDay = 60000; // 60 seconds == 60000 milliseconds
	const int iInitialMovementSpeed = 80;

	// Default Multipliers Per Level for Upgrade Levels
	const double dCoughRangeMultiplier = 1.2;
	const double dVirusStrengthMultiplier = 1.275;
	const double dVirusLethalityMultiplier = 1.2;
	const double dLengthOfDayMultiplier = 1.2;
	const double dMovementSpeedBonus = 10; // Default Player Speed += 10 * iMovementSpeedLevel
	const double dDestroyVaccineProgressPercentage = 10; // 10 % of vaccine progress

	// Initial Costs & Multiplier Per Level
	const int iCoughRangeCost = 100;
	const double dCoughRangeCostMultiplier = 1.1;

	const int iVirusStrengthCost = 100;
	const double dVirusStrengthCostMultiplier = 1.1;

	const int iVirusLethalityCost = 100;
	const double dVirusLethalityCostMultiplier = 1.1;

	const int iLengthOfDayCost = 200;
	const double dLengthOfDayCostMultiplier = 1.1;

	const int iMovementSpeedCost = 200;
	const double dMovementSpeedCostMultiplier = 1.1;

	const double dVaccineBaseProgress = 10;
	const int iDestroyVaccineCost = 200;
	const double dDestroyVaccineCostMultiplier = 1.5;

	const int iVirusLauncherAmmoCost = 20;

	const int iInfectedAirFreshenerCost = 20;

	// Constructors
	CUpgrade();
	~CUpgrade();

	// Cough Range Functions
	void SetCoughRangeLevel(int iCoughRangeLevel); // To init the value from load file or just 0
	int GetCoughRangeLevel();
	int GetCoughUpgradeCost();
	bool CanAffordCoughRangeUpgrade();
	void BuyCoughRangeUpgrade();

	// Virus Strength Functions
	void SetVirusStrengthLevel(int iVirusStrengthLevel);
	int GetVirusStrengthLevel();
	int GetVirusStrengthCost();
	bool CanAffordVirusStrengthUpgrade();
	void BuyVirusStrengthUpgrade();

	// Virus Lethality Functions
	void SetVirusLethalityLevel(int iVirusLethalityLevel);
	int GetVirusLethalityLevel();
	int GetVirusLethalityCost();
	bool CanAffordVirusLethalityUpgrade();
	void BuyVirusLethalityUpgrade();

	// Length Of Day Functions
	void SetLengthOfDayLevel(int iLengthOfDayLevel);
	int GetLengthOfDayLevel();
	int GetLengthOfDayCost();
	bool CanAffordLengthOfDayUpgrade();
	void BuyLengthOfDayUpgrade();

	// Movement Speed Functions
	void SetMovementSpeedLevel(int iMovementSpeedLevel);
	int GetMovementSpeedLevel();
	int GetMovementSpeedCost();
	bool CanAffordMovementSpeedUpgrade();
	void BuyMovementSpeedUpgrade();

	// Mutate Virus Functions
	void SetMutateVirusLevel(int iMutateVirusLevel);
	int GetMutateVirusLevel();
	int GetMutateVirusCost();
	bool CanAffordMutateVirusUpgrade();
	void BuyMutateVirusUpgrade();

	// Ammo costs
	int GetVirusLauncherAmmoCost();
	int GetInfectedAirFreshenerAmmoCost();

	// General Functions
	void InitLevels(int iCoughRangeLevel, int iVirusStrengthLevel, int iVirusLethalityLevel, int iLengthOfDayLevel, int iMovementSpeedLevel, int iMutateVirusLevel);
	void AttachHandlers(CCough* cCough, CVirus* cVirus, double* dDayPhaseTime, CPlayer* cPlayer, CVaccine* cVaccine); // Attach Handlers
	void UpdateValues();

};

#endif