#ifndef CTEXTLABEL_H
#define CTEXTLABEL_H

#include "CGUI.h"



class CTextLabel : public CGUI
{
private:

	std::string text;
	double dTextSize;
	double dTextEnlargeSize;
	Color textColor;
	
	bool bCenter;
	bool bFollowMouse;

	Mesh* textMesh;	
	unsigned U_MVP, U_COLOR_TEXTURE_ENABLED, U_COLOR_TEXTURE, U_TEXT_ENABLED, U_TEXT_COLOR, U_LIGHTENABLED;
public:
	CTextLabel();
	~CTextLabel();

	void AttachParameters(unsigned U_MVP, unsigned U_COLOR_TEXTURE_ENABLED, unsigned U_COLOR_TEXTURE, unsigned U_TEXT_ENABLED, unsigned U_TEXT_COLOR, unsigned U_LIGHTENABLED);

	void SetCenter(bool bCenter);
	void SetFollowMouse(bool bFollowMouse);
	void SetText(std::string text);
	void SetTextSize(double dTextSize);
	void SetTextColor(Color textColor);
	void SetTextMesh(Mesh* textMesh);

	void Update(double dt);
	void Render(MS& projectionStack, MS& viewStack, MS& modelStack);
};
#endif
