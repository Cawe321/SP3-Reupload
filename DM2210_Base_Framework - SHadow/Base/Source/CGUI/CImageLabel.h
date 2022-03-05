#ifndef CIMAGELABEL_H
#define CIMAGELABEL_H

#include "CGUI.h"



class CImageLabel : public CGUI
{
private:
	bool bFollowMouse;
	bool bEnableAnimation;
	Mesh* imageMesh;	
	unsigned U_MVP, U_COLOR_TEXTURE_ENABLED, U_COLOR_TEXTURE;
public:
	CImageLabel();
	~CImageLabel();
	
	void SetFollowMouse(bool bFollowMouse);

	void AttachParameters(unsigned U_MVP, unsigned U_COLOR_TEXTURE_ENABLED, unsigned U_COLOR_TEXTURE);

	void SetImageMesh(Mesh* imageMesh);
	void StopAnimation(); // only use if animation is enabled
	void StartAnimation(); // only use if animation is enabled
	
	// Functions for bool bEnableAnimation
	bool isAnimationEnabled();
	void SetAnimationEnabled(bool bEnableAnimation);


	void Update(double dt);
	void Render(MS& projectionStack, MS& viewStack, MS& modelStack);
};
#endif
