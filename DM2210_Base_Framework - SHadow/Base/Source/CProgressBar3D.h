#ifndef CProgressBar3D_H
#define CProgressBar3D_H

#include <Vector>
#include <Vector3.h>
#include <MatrixStack.h>
#include "SpriteAnimation.h"

using namespace std;

//A 2D progress Bar in a 3D space (idk what else to call it thats more apportarate that dosnt conflict with CProgressBar)
class CProgressBar3D /*: public CGUI //It was so diffrent at this point there is no point making this a inherted class :D - Clement*/
{
public:
	enum BAR_TYPE
	{
		HPBAR,
	};


	CProgressBar3D();
	~CProgressBar3D();
	
	void SetValue(double dValue);
	double GetValue();

	double GetxScale();

	void SetLength(double dNewLength);
	void SetHeight(double dNewHeight);

	double GetLength();
	double GetHeight();

	void SetMaxValue(double dMaxValue);
	double GetMaxValue();

	bool GetActive();
	void SetActive(bool bActive);

	void AttachParameters(unsigned U_MVP, unsigned U_COLOR_TEXTURE_ENABLED, unsigned U_COLOR_TEXTURE, unsigned U_MODELVIEW, unsigned U_MODELSTACK);

	void AddImageMesh(Mesh* imageMesh);
	Mesh* GetImageMesh();
	void ClearImageMesh();


	void AddBackgroundMesh(Mesh* imageMesh);
	Mesh* GetBackgroundMesh();
	void ClearBackgroundMesh();
	
	void SetBarType(BAR_TYPE eNewType);

	void Update(double dt);
	//void Render(MS& projectionStack, MS& viewStack, MS& modelStack, Vector3 CameraPos);

	void SetPosition(Vector3 vNewPos);
	Vector3 GetPosition();

private:
	BAR_TYPE eBarType;
	bool bFollowMouse;

	double dValue;
	double dMaxValue;

	//Amount of x Scale/Offset based on & of value to max value
	double dxScale;

	double dHeight;
	double dLength;

	bool bActive;
	
	vector<Mesh*> vimageMesh;
	vector<Mesh*> vBackgroundMesh;

	unsigned U_MVP, U_COLOR_TEXTURE_ENABLED, U_COLOR_TEXTURE, U_MODELVIEW, U_MODELSTACK;

	Vector3 vPosition;
};
#endif
