#ifndef CPROGRESSBAR_H
#define CPROGRESSBAR_H

#include "CGUI.h"



class CProgressBar : public CGUI
{
private:
	bool bFollowMouse;

	double dValue;
	double dMaxValue;

	Mesh* imageMesh;	
	unsigned U_MVP, U_COLOR_TEXTURE_ENABLED, U_COLOR_TEXTURE;
public:
	CProgressBar();
	~CProgressBar();
	
	void SetValue(double dValue);
	void SetMaxValue(double dMaxValue);

	void SetFollowMouse(bool bFollowMouse);

	void AttachParameters(unsigned U_MVP, unsigned U_COLOR_TEXTURE_ENABLED, unsigned U_COLOR_TEXTURE);

	void SetImageMesh(Mesh* imageMesh);

	void Update(double dt);
	void Render(MS& projectionStack, MS& viewStack, MS& modelStack);
};
#endif
