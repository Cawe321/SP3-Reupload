#ifndef CIMAGEBUTTON_H
#define CIMAGEBUTTON_H

#include "CGUI.h"



class CImageButton : public CGUI
{
private:
	Mesh* imageMesh;	
	unsigned U_MVP, U_COLOR_TEXTURE_ENABLED, U_COLOR_TEXTURE;

	bool bEnableAnimation;
public:
	CImageButton();
	~CImageButton();

	void SetImageMesh(Mesh* imageMesh);
	void StopAnimation(); // only use if animation is enabled
	void StartAnimation(); // only use if animation is enabled
	
	void AttachParameters(unsigned U_MVP, unsigned U_COLOR_TEXTURE_ENABLED, unsigned U_COLOR_TEXTURE);

	// Functions for bool bEnableAnimation
	bool isAnimationEnabled();
	void SetAnimationEnabled(bool bEnableAnimation);

	bool isLeftMouseButtonPressed();

	void Update(double dt);
	void Render(MS& projectionStack, MS& viewStack, MS& modelStack);
};
#endif
