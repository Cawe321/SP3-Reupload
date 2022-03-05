#ifndef CCOMPASS_H
#define CCOMPASS_H

#include "CGUI.h"

#include "../Camera.h"

class CCompass : public CGUI
{
private:
	bool bFollowMouse;
	Camera* camera;
	Mesh* compassMesh;	
	unsigned U_MVP, U_COLOR_TEXTURE_ENABLED, U_COLOR_TEXTURE;
public:
	CCompass();
	~CCompass();
	void Init();

	void SetFollowMouse(bool bFollowMouse);

	void AttachParameters(unsigned U_MVP, unsigned U_COLOR_TEXTURE_ENABLED, unsigned U_COLOR_TEXTURE);
	void AttachCamera(Camera& camera);

	void SetCompassMesh(Mesh* compassMesh);

	void Update(double dt);
	void Render(MS& projectionStack, MS& viewStack, MS& modelStack);
};
#endif
