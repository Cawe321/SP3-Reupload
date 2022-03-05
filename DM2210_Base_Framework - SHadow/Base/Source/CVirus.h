#pragma once
#include <string>
#include <iostream>

#include "SingletonTemplate.h"

using namespace std;

class CVirus : public CSingletonTemplate<CVirus>
{
	friend CSingletonTemplate<CVirus>;
public:
	CVirus();
	~CVirus();

	void init();

	void SetName(string sNewName);
	string GetName();

	//Strength
	float GetStrength();
	float GetMaxStrength();
	void IncreaseStrength(float fIncreasedAmount);

	//Lethality
	float GetLethality();
	float GetMaxLethality();
	void IncreaseLethality(float fIncreasedAmount);

	//Mutation Points
	void ReducePoints(int iReducedPoints);
	void IncreasePoints(int iIncreasePoints);
	int GetPoints();

	void Restart(string sVirusNewName);


private:
	//% chance for anything is value% 

	//Virus Values
	string sVirusName; //Virus Name
	float fVirusStrength; //% chance to spread, /100
	float fMaxVirusStrength; //Max % chance to spread, /100
	float fVirusLethality; //% chance to kill, /100
	float fMaxVirusLethality; //Max % chance to kill, /100
	int iMutationPoints; //Points earned to mutate the virus
};

