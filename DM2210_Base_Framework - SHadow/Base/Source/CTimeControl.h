#ifndef CTIMECONTROL_H
#define CTIMECONTROL_H
#include "Light.h"
#include "Mesh.h"
class CTimeControl
{
public:
	enum TIME
	{
		MORNING,
		EVENING,
		NIGHT
	};
	// Constructors & Destructors
	CTimeControl();
	~CTimeControl();

	// Functions for eGameState
	TIME GetTimeState();
	bool isDay();

	// Functions for cLight
	void AttachLight(Light& cLight, unsigned U_LIGHT0_POWER, unsigned U_LIGHT0_COLOR);
	
	// Functions for cSky
	void AttachSky(Mesh* cSky);

	// Functions for dHour & dMinute
	void Update(double dTimeToNight, double dCurrentTime, double dDayPhaseTime); // Converts the values to dHour and dMinute
	double GetHour();
	double GetMinute();

	// Functions for bTimePaused
	bool GetTimePaused();
	void SetTimePaused(bool bTimePaused);

private:

	TIME eGameState;

	// Modifiable Environmental Values
	Light* cLight;	// Light[0] | Sunlight
	unsigned U_LIGHT0_POWER, U_LIGHT0_COLOR;
	Mesh* cSky;		// meshList[GEO_SKYPLANE] | Skyplane

	// Time Values
	double dHour;
	double dMinute;

	// Time Control
	bool bTimePaused;

};
#endif

