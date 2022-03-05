#ifndef CTEXTBOX_H
#define CTEXTBOX_H

#include "CGUI.h"
#include <string>
#include "../light.h"


class CTextBox : public CGUI
{
private:

	// Unique Values
	std::string text;
	double dTextSize;
	Color textColor;
	bool bActive; // whether text input is active
	int iLimit;

	// Shader stuff
	Mesh* textMesh;
	unsigned U_MVP, U_COLOR_TEXTURE_ENABLED, U_COLOR_TEXTURE, U_TEXT_ENABLED, U_TEXT_COLOR, U_LIGHTENABLED;
public:
	CTextBox();
	~CTextBox();

	void AttachParameters(unsigned U_MVP, unsigned U_COLOR_TEXTURE_ENABLED, unsigned U_COLOR_TEXTURE, unsigned U_TEXT_ENABLED, unsigned U_TEXT_COLOR, unsigned U_LIGHTENABLED);

	void CheckInput();
	void ResetInput();
	bool isEnabled();

	void SetEnabled(bool bActive);

	void SetTextColor(Color textColor);


	void SetLimit(int iLimit);

	void SetTextMesh(Mesh* textMesh);

	std::string GetText();
	void ClearText();

	void Update(double dt);
	void Render(MS& projectionStack, MS& viewStack, MS& modelStack);
};
#endif
