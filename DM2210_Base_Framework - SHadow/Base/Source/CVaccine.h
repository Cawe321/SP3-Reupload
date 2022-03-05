#pragma once

#include <iostream>
#include <Windows.h>

using namespace std;

class CVaccine
{
public:
	CVaccine();
	~CVaccine();

	void Init();

	bool Update(double dt);

	void SetVacineState(bool bactive);
	bool GetVacineState();

	void SetAwareness(float dDeadNPCs);
	double GetAwarness();

	void SetProgress(float fProgress);
	float GetProgress();
	void DestoryProgress(float dDestoyedAmount);

	bool CheckCompleted();

	void Restart();

private:
	float fVaccineProgress; //The total amount of progress made on the vaccines
	float fVaccineCompletedNum; // The number reached for vaccine to be completed
	float fBaseUpgradeAmount; //The base amount the vaccine progress after each vaccine upgrade

	double dVaccineAwareness; //Penality to the multiplier of the dVaccine (Based on number of NPCS dead/infected)
	double dMaxVaccineAwareness; //Max Penality to reudce the time to next vaccine upgrade

	bool bVaccineBeingMade; //Bool to check if the progress on the vaccine is being made
};

