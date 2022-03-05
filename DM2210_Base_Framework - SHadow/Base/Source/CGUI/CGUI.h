#ifndef CGUI_H
#define CGUI_H
#include "../MeshBuilder.h"
#include "MatrixStack.h"

#include "../SpriteAnimation.h"
#define LENGTHSCALE 800
#define HEIGHTSCALE 600

class CGUI
{
private:
	// Values Unique to CGUI
	Mesh* mesh;
	double dGuiLength; // Length Of Gui Object | 0 is left
	double dGuiHeight; // Height of GUI Object | 0 is top
	double dPositionX;
	double dPositionY;
	bool bEnlarge; // used to make the gui appear bigger, checked by manager
	bool bMouseHover;

	// Button Cooldown
	double dDebounceTime;
	double dCooldownTime;

	// Values used for enlarge
	double extraValueX;
	double extraValueY;

	bool bActive;


	bool bEnableAnimation;

	// Shader Stuff
	unsigned U_MVP, U_COLOR_TEXTURE_ENABLED, U_COLOR_TEXTURE;
	double dRenderOrder;


public:
	// Constructor & Destructor
	CGUI();
	~CGUI();


	// Functions to attach shader variables
	virtual void AttachParameters(unsigned U_MVP, unsigned U_COLOR_TEXTURE_ENABLED, unsigned U_COLOR_TEXTURE);

	// Functions for int itextureArray[0]
	virtual void SetBackgroundMesh(Mesh* mesh); //set background mesh
	void StopBackgroundAnimation(); // only use if animation is enabled
	void StartBackgroundAnimation(); // only use if animation is enabled
	

	Mesh* GetBackgroundMesh();

	// Functions for double dGuiLength
	virtual void SetLength(double dGuiLength);
	double GetLength();
	
	// Functions for double dGuiHeight
	virtual void SetHeight(double dGuiHeight);
	double GetHeight();

	// Functions for dPositionX & dPositionY
	virtual void SetPosition(double dPositionX, double dPositionY);
	double GetPositionX();
	double GetPositionY();

	// Functions for bool bEnlarge
	virtual void SetEnlarge(bool bEnlarge);
	virtual bool GetEnlarge();
	
	// Functions for Button Cooldown
	void SetCooldownTime(double dCooldownTime);
	void ResetTimer();
	bool isCooldownOver();

	// Functions for bool bActive
	bool isActive();
	void SetActive(bool bActive);

	// Functions for bool bEnableAnimation
	bool isBackgroundAnimationEnabled();
	void SetBackgroundAnimationEnabled(bool bEnableAnimation);

	void SetRenderOrder(double dRenderOrder);
	double GetRenderOrder();
	// Unique Functions
	virtual bool IsMouseHover();

	
	static bool IsMousePressed(unsigned short key);
	static bool IsKeyPressed(unsigned short key);
	static bool IsKeyDown(unsigned short key);
	

	// Standard Functions
	virtual void Update(double dt);
	virtual void Render(MS& projectionStack, MS& viewStack, MS& modelStack);
	void RenderBackground(MS& projectionStack, MS& viewStack, MS& modelStack, double dPosX, double dPosY);
};

#endif