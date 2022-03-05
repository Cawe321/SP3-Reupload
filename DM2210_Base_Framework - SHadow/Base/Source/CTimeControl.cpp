#include "CTimeControl.h"
#include "GL/glew.h"
CTimeControl::CTimeControl()
{
	eGameState = NIGHT;
	cLight = nullptr;
	cSky = nullptr;
	dHour = dMinute = 0;
	bTimePaused = false;
}

CTimeControl::~CTimeControl()
{
}

CTimeControl::TIME CTimeControl::GetTimeState()
{
	return eGameState;
}

bool CTimeControl::isDay()
{
	return eGameState != NIGHT;
}

void CTimeControl::AttachLight(Light& cLight, unsigned U_LIGHT0_POWER, unsigned U_LIGHT0_COLOR)
{
	this->cLight = &cLight;
	this->U_LIGHT0_POWER = U_LIGHT0_POWER;
	this->U_LIGHT0_COLOR = U_LIGHT0_COLOR;
}

void CTimeControl::AttachSky(Mesh* cSky)
{
	this->cSky = cSky;
}

void CTimeControl::Update(double dTimeToNight, double dCurrentTime, double dDayPhaseTime)
{
	if (dCurrentTime >= dTimeToNight)
	{
		eGameState = NIGHT;
		dHour = 19;
		dMinute = 0;
		return;
	}
	double dTimeLeftToNight = dTimeToNight - dCurrentTime;
	double dTimePassedSinceDay = dDayPhaseTime - dTimeLeftToNight; // Milliseconds passed since day started

	// Morning starts at 07:00 and ends at 11:59 // Evening starts at 12:00 and ends at 18:59
	double dMillisecondsPerHour = dDayPhaseTime / 12.0;
	double dMillisecondsPerMinute = dMillisecondsPerHour / 60;
	dHour = 7 + dTimePassedSinceDay / dMillisecondsPerHour;
	dMinute = ((int)dTimePassedSinceDay % (int)dMillisecondsPerHour) / dMillisecondsPerMinute;
	if (dHour < 12)
	{
		eGameState = MORNING;
	}
	else if (dHour >= 12 && dHour < 19)
	{
		eGameState = EVENING;
	}
	else if (dHour > 19)
	{
		eGameState = NIGHT;
	}
	if (dHour > 240)
	{
		std::cout << "Something went wrong with CTimeControl::Update()\n";
	}


	if (isDay())
	{
		double dElevationPercentage = ((((int)dHour - 7) * 60 + dMinute) / 720); // 300 is 5 hours x 60 minutes
		cSky->material.kAmbient.Set(0.2 + sin(dElevationPercentage * Math::PI), 0.2 + sin(dElevationPercentage * Math::PI), 0.2 + sin(dElevationPercentage * Math::PI));
		cLight->position.Set(cos(Math::DegreeToRadian(dElevationPercentage * 120 + 30)) * 1000, sin(Math::DegreeToRadian(dElevationPercentage * 120 + 30)) * 1000, 0);
			
		cLight->power = 10 + sin(dElevationPercentage * Math::PI) * 20;
		glUniform1f(U_LIGHT0_POWER, cLight->power);
		cLight->color = Color(1, 0.5 + sin(dElevationPercentage * Math::PI) * 0.5, sin(dElevationPercentage * Math::PI));
		glUniform3fv(U_LIGHT0_COLOR, 1, &cLight->color.r);
		
		

	}
}

double CTimeControl::GetHour()
{
	return dHour;
}

double CTimeControl::GetMinute()
{
	return dMinute;
}

bool CTimeControl::GetTimePaused()
{
	return bTimePaused;
}

void CTimeControl::SetTimePaused(bool bTimePaused)
{
	this->bTimePaused = bTimePaused;
}

