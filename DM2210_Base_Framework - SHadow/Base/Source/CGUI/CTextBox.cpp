#include "CTextBox.h"
#include "../Win32Input.h"

#include "GL/glew.h"

#include <GLFW/glfw3.h>

#include "../Application.h"

CTextBox::CTextBox()
{
	bActive = false;
	dTextSize = 0;
	textMesh = nullptr;;
	U_MVP = U_COLOR_TEXTURE_ENABLED = U_COLOR_TEXTURE = U_TEXT_ENABLED = U_TEXT_COLOR = U_LIGHTENABLED = 0;
	iLimit = 999;
}

CTextBox::~CTextBox()
{
	if (textMesh != nullptr)
	{
		delete textMesh;
		textMesh = nullptr;
	}
}

void CTextBox::AttachParameters(unsigned U_MVP, unsigned U_COLOR_TEXTURE_ENABLED, unsigned U_COLOR_TEXTURE, unsigned U_TEXT_ENABLED, unsigned U_TEXT_COLOR, unsigned U_LIGHTENABLED)
{
	CGUI::AttachParameters(U_MVP, U_COLOR_TEXTURE_ENABLED, U_COLOR_TEXTURE);
	this->U_MVP = U_MVP;
	this->U_COLOR_TEXTURE_ENABLED = U_COLOR_TEXTURE_ENABLED;
	this->U_COLOR_TEXTURE = U_COLOR_TEXTURE;
	this->U_TEXT_ENABLED = U_TEXT_ENABLED;
	this->U_TEXT_COLOR = U_TEXT_COLOR;
	this->U_LIGHTENABLED = U_LIGHTENABLED;
}

void CTextBox::CheckInput()
{
	if (text.length() < iLimit)
	{
		for (int input = 'A'; input <= 'Z'; ++input)
		{

			if (CGUI::IsKeyPressed(input))
			{
				if (CGUI::IsKeyDown(VK_SHIFT) && text.length() < iLimit)
					text.push_back(input);
				else if (text.length() < iLimit)
					text.push_back(std::tolower(input));
			}
		}
		for (int input = '0'; input <= '9'; ++input)
		{
			if (CGUI::IsKeyPressed(input) && text.length() < iLimit)
			{
				text.push_back(input);
			}
		}
		if (CGUI::IsKeyPressed(VK_SPACE) && text.length() < iLimit)
		{

			text.push_back(' ');
		}
	}
	if (CGUI::IsKeyPressed(VK_BACK))
	{
		if (text.length() != 0)
			text.pop_back();
	}

	if (CGUI::IsKeyDown(VK_RETURN))
	{
		this->bActive = false;
	}

}

void CTextBox::ResetInput()
{
	for (int input = 'A'; input <= 'Z'; ++input)
	{
		CGUI::IsKeyPressed(input);
	}
	for (int input = '0'; input <= '9'; ++input)
	{
		CGUI::IsKeyPressed(input);
	}
	CGUI::IsKeyPressed(VK_SPACE);
	CGUI::IsKeyPressed(VK_BACK);
	CGUI::IsKeyPressed(VK_RETURN);
}

bool CTextBox::isEnabled()
{
	if (CGUI::IsMousePressed(0))
	{
		if (CGUI::IsMouseHover())
		{
			bActive = true;
			ResetInput();
		}
		else bActive = false;
	}
	return bActive;
}

void CTextBox::SetEnabled(bool bActive)
{
	this->bActive = bActive;
}

void CTextBox::SetTextColor(Color textColor)
{
	this->textColor = textColor;
}

void CTextBox::SetLimit(int iLimit)
{
	this->iLimit = iLimit;
}

void CTextBox::SetTextMesh(Mesh* textMesh)
{
	this->textMesh = textMesh;
}

std::string CTextBox::GetText()
{
	return text;
}

void CTextBox::ClearText()
{
	this->text = "";
}

void CTextBox::Update(double dt)
{
	if (CGUI::isActive())
	{
		CGUI::Update(dt);

		if (isEnabled())
			CheckInput();
	}

}

void CTextBox::Render(MS& projectionStack, MS& viewStack, MS& modelStack)
{
	if (CGUI::isActive())
	{
		CGUI::Render(projectionStack, viewStack, modelStack);
		glEnable(GL_DEPTH_TEST);
		int numCharPerLine = CGUI::GetLength() / 100 / (CGUI::GetHeight() / 100);

		double count = 1;
		std::string print = text;
		while (true)
		{
			if (print.length() >= numCharPerLine)
			{
				count *= 2;
				numCharPerLine *= 2;
			}
			else break;
		}
		// Calculate Position To Center Text
		dTextSize = CGUI::GetHeight() / 10 / (count);


		if (!textMesh || textMesh->textureID <= 0)
			return;

		glDisable(GL_DEPTH_TEST);
		Mtx44 ortho;
		ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
		projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
		viewStack.PushMatrix();
		viewStack.LoadIdentity();
		modelStack.PushMatrix();
		modelStack.LoadIdentity();
		modelStack.Translate((float)CGUI::GetPositionX() / 10 - CGUI::GetLength() / 20 + 1, (float)CGUI::GetPositionY() / 10 - (dTextSize / 2), CGUI::GetRenderOrder() + 0.01);
		//modelStack.Translate(0, 0, 0);
		modelStack.Scale(dTextSize, dTextSize, 1);
		glUniform1i(U_TEXT_ENABLED, 1);
		glUniform3fv(U_TEXT_COLOR, 1, &textColor.r);
		glUniform1i(U_LIGHTENABLED, 0);
		glUniform1i(U_COLOR_TEXTURE_ENABLED, 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textMesh->textureID);
		glUniform1i(U_COLOR_TEXTURE, 0);
		for (unsigned i = 0; i < text.length(); ++i)
		{
			Mtx44 characterSpacing;
			characterSpacing.SetToTranslation(i * 1.0f + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
			Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
			glUniformMatrix4fv(U_MVP, 1, GL_FALSE, &MVP.a[0]);

			textMesh->Render((unsigned)text[i] * 6, 6);
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		glUniform1i(U_TEXT_ENABLED, 0);
		modelStack.PopMatrix();
		viewStack.PopMatrix();
		projectionStack.PopMatrix();
	}
	

}
