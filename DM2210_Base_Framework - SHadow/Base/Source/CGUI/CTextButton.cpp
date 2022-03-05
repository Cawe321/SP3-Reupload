
#include "CTextButton.h"

#include "GL/glew.h"

#include <GLFW/glfw3.h>

#include "../Application.h"

CTextButton::CTextButton()
{
	U_MVP = U_COLOR_TEXTURE_ENABLED = U_COLOR_TEXTURE = U_TEXT_ENABLED = U_TEXT_COLOR = U_LIGHTENABLED = 0;
	dTextEnlargeSize = dTextSize = 0;
	textMesh = nullptr;
}

CTextButton::~CTextButton()
{
	if (textMesh != nullptr)
	{
		delete textMesh;
		textMesh = nullptr;
	}
}

void CTextButton::AttachParameters(unsigned U_MVP, unsigned U_COLOR_TEXTURE_ENABLED, unsigned U_COLOR_TEXTURE, unsigned U_TEXT_ENABLED, unsigned U_TEXT_COLOR, unsigned U_LIGHTENABLED)
{
	CGUI::AttachParameters(U_MVP, U_COLOR_TEXTURE_ENABLED, U_COLOR_TEXTURE);
	this->U_MVP = U_MVP;
	this->U_COLOR_TEXTURE_ENABLED = U_COLOR_TEXTURE_ENABLED;
	this->U_COLOR_TEXTURE = U_COLOR_TEXTURE;
	this->U_TEXT_ENABLED = U_TEXT_ENABLED;
	this->U_TEXT_COLOR = U_TEXT_COLOR;
	this->U_LIGHTENABLED = U_LIGHTENABLED;
}

void CTextButton::SetText(std::string text)
{
	this->text = text;
}

void CTextButton::SetTextSize(double dTextSize)
{
	this->dTextSize = dTextSize;
	this->dTextEnlargeSize = dTextSize * 1.2;
}

void CTextButton::SetTextColor(Color textColor)
{
	this->textColor = textColor;
}

void CTextButton::SetTextMesh(Mesh* textMesh)
{
	this->textMesh = textMesh;
}

bool CTextButton::isLeftMouseButtonPressed()
{
	if (CGUI::IsMouseHover() && CGUI::IsMousePressed(0) && CGUI::isActive() && CGUI::GetPositionX() <= Application::GetWindowWidth() && CGUI::GetPositionY() <= Application::GetWindowHeight())
		return true;
	else return false;
}


void CTextButton::Update(double dt)
{
	if (CGUI::isActive())
	{
		CGUI::Update(dt);
	}
	
}

void CTextButton::Render(MS& projectionStack, MS& viewStack, MS& modelStack)
{
	if (CGUI::isActive())
	{
		CGUI::Render(projectionStack, viewStack, modelStack);
		if (!textMesh || textMesh->textureID <= 0)
			return;

		
		Mtx44 ortho;
		ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
		projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
		viewStack.PushMatrix();
		viewStack.LoadIdentity();
		modelStack.PushMatrix();
		modelStack.LoadIdentity();
		if (CGUI::GetEnlarge())
		{
			modelStack.Translate((float)CGUI::GetPositionX() / 10 - (dTextEnlargeSize) *  (text.length() / 2.f) + dTextEnlargeSize/ 4, (float)CGUI::GetPositionY() / 10 - (dTextEnlargeSize / 2), CGUI::GetRenderOrder()  + 0.01f);
			//modelStack.Translate(0, 0, 0);
			modelStack.Scale((float)dTextEnlargeSize, (float)dTextEnlargeSize, 1.f);
		}
		else
		{
			modelStack.Translate((float)CGUI::GetPositionX() / 10 - (dTextSize) * (text.length() / 2.f) + dTextSize / 4, (float)CGUI::GetPositionY() / 10 - (dTextSize / 2), CGUI::GetRenderOrder() + 0.01f);
			//modelStack.Translate(0, 0, 0);
			modelStack.Scale((float)dTextSize, (float)dTextSize, 1.f);
		}
	
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
