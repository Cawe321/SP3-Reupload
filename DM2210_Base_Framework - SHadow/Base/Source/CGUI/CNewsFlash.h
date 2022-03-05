#ifndef CNEWSFLASH_H
#define CNEWSFLASH_H

#include "CGUI.h"



class CNewsFlash : public CGUI
{
private:

	std::string text;
	std::string print;
	double dTextSize;
	double dTextEnlargeSize;
	Color textColor;
	
	Mesh* textMesh;	
	unsigned U_MVP, U_COLOR_TEXTURE_ENABLED, U_COLOR_TEXTURE, U_TEXT_ENABLED, U_TEXT_COLOR, U_LIGHTENABLED;
public:
	CNewsFlash();
	~CNewsFlash();

	void AttachParameters(unsigned U_MVP, unsigned U_COLOR_TEXTURE_ENABLED, unsigned U_COLOR_TEXTURE, unsigned U_TEXT_ENABLED, unsigned U_TEXT_COLOR, unsigned U_LIGHTENABLED);

	std::string GetText();
	void SetText(std::string text);
	void SetTextSize(double dTextSize);
	void SetTextColor(Color textColor);
	void SetTextMesh(Mesh* textMesh);

	void Update(double dt);
	void Render(MS& projectionStack, MS& viewStack, MS& modelStack);
};
#endif
