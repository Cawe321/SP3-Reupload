#ifndef CCLOCK_H
#define CCLOCK_H

#include "CGUI.h"

#include "../Camera3.h"

class CClock : public CGUI
{
private:
	bool bFollowMouse;

	Mesh* hourMesh;
	Mesh* minuteMesh;

	double dHour, dMinute;

	unsigned U_MVP, U_COLOR_TEXTURE_ENABLED, U_COLOR_TEXTURE;
public:
	CClock();
	~CClock ();
	void Init();

	void SetClockTime(double dHour, double dMinute);

	void SetFollowMouse(bool bFollowMouse);

	void AttachParameters(unsigned U_MVP, unsigned U_COLOR_TEXTURE_ENABLED, unsigned U_COLOR_TEXTURE);

	void Update(double dt);
	void Render(MS& projectionStack, MS& viewStack, MS& modelStack);
};
#endif
