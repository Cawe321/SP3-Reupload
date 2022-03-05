
#include "CTextLabel.h"

#include "GL/glew.h"

#include <GLFW/glfw3.h>

#include "../Application.h"

CTextLabel::CTextLabel()
{
	U_MVP = U_COLOR_TEXTURE_ENABLED = U_COLOR_TEXTURE = U_TEXT_ENABLED = U_TEXT_COLOR = U_LIGHTENABLED = 0;
	bFollowMouse = dTextEnlargeSize = dTextSize = 0;
	textMesh = nullptr;
	bCenter = false;
}

CTextLabel::~CTextLabel()
{
	if (textMesh != nullptr)
	{
		delete textMesh;
		textMesh = nullptr;
	}
}

void CTextLabel::AttachParameters(unsigned U_MVP, unsigned U_COLOR_TEXTURE_ENABLED, unsigned U_COLOR_TEXTURE, unsigned U_TEXT_ENABLED, unsigned U_TEXT_COLOR, unsigned U_LIGHTENABLED)
{
	CGUI::AttachParameters(U_MVP, U_COLOR_TEXTURE_ENABLED, U_COLOR_TEXTURE);
	this->U_MVP = U_MVP;
	this->U_COLOR_TEXTURE_ENABLED = U_COLOR_TEXTURE_ENABLED;
	this->U_COLOR_TEXTURE = U_COLOR_TEXTURE;
	this->U_TEXT_ENABLED = U_TEXT_ENABLED;
	this->U_TEXT_COLOR = U_TEXT_COLOR;
	this->U_LIGHTENABLED = U_LIGHTENABLED;
}

void CTextLabel::SetCenter(bool bCenter)
{
	this->bCenter = bCenter;
}

void CTextLabel::SetFollowMouse(bool bFollowMouse)
{
	this->bFollowMouse = bFollowMouse;
}

void CTextLabel::SetText(std::string text)
{
	this->text = text;
}

void CTextLabel::SetTextSize(double dTextSize)
{
	this->dTextSize = dTextSize;
	this->dTextEnlargeSize = dTextSize * 1.2;
}

void CTextLabel::SetTextColor(Color textColor)
{
	this->textColor = textColor;
}

void CTextLabel::SetTextMesh(Mesh* textMesh)
{
	this->textMesh = textMesh;
}

void CTextLabel::Update(double dt)
{
	if (CGUI::isActive())
	{
		CGUI::Update(dt);
	}
	
}

void CTextLabel::Render(MS& projectionStack, MS& viewStack, MS& modelStack)
{
	if (CGUI::isActive())
	{
		double x, y;
		Application::GetCursorPos(&x, &y);
		x = x / Application::GetWindowWidth() * LENGTHSCALE; // In case of resize
		y = Application::GetWindowHeight() - y;
		y = y / Application::GetWindowHeight() * HEIGHTSCALE;
		if (bFollowMouse)
		{
			CGUI::RenderBackground(projectionStack, viewStack, modelStack, x + CGUI::GetLength() / 2 +  10.0, y - CGUI::GetHeight() / 2 - 10.0);
		}
		else
		{
			CGUI::Render(projectionStack, viewStack, modelStack);
		}
		if (!textMesh || textMesh->textureID <= 0)
			return;

		std::vector<std::string> print;
		std::string temp = "";
		for (int i = 0; text[i] != '\0'; ++i)
		{
			if (text[i] == '\n')
			{
				print.push_back(temp);
				temp = "";
			}
			else
			{
				temp.push_back(text[i]);
			}
		}
		print.push_back(temp);


		Mtx44 ortho;
		ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
		projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
		viewStack.PushMatrix();
		viewStack.LoadIdentity();
		modelStack.PushMatrix();
		modelStack.LoadIdentity();
	
	
	
		glUniform1i(U_TEXT_ENABLED, 1);
		glUniform3fv(U_TEXT_COLOR, 1, &textColor.r);
		glUniform1i(U_LIGHTENABLED, 0);
		glUniform1i(U_COLOR_TEXTURE_ENABLED, 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textMesh->textureID);
		glUniform1i(U_COLOR_TEXTURE, 0);
		int iLines = print.size();
		for (int j = 0; j < iLines; ++j)
		{
			modelStack.PushMatrix();
			if (bFollowMouse)
			{


				if (CGUI::GetEnlarge())
				{
					modelStack.Translate(x / 10 + 1, y / 10 - dTextEnlargeSize - 1, CGUI::GetRenderOrder() + 0.01);
					modelStack.Scale(dTextEnlargeSize, dTextEnlargeSize, 1);
				}
				else
				{
					modelStack.Translate(x / 10 + 1, y / 10 - dTextSize - 1, CGUI::GetRenderOrder() + 0.01);
					modelStack.Scale(dTextSize, dTextSize, 1);
				}
			}
			else
			{
				if (bCenter)
				{
					if (CGUI::GetEnlarge())
					{
						modelStack.Translate(CGUI::GetPositionX() / 10 - (dTextEnlargeSize) * (print[j].length() / 2.f) + dTextEnlargeSize / 4, CGUI::GetPositionY() / 10 + CGUI::GetHeight() / 20 - dTextEnlargeSize * 1.2, CGUI::GetRenderOrder() + 0.01);
						modelStack.Scale(dTextEnlargeSize, dTextEnlargeSize, 1);
					}
					else
					{
						modelStack.Translate(CGUI::GetPositionX() / 10 - (dTextSize) * (print[j].length() / 2.f) + dTextSize / 4, CGUI::GetPositionY() / 10 + CGUI::GetHeight() / 20 - dTextSize * 1.2, CGUI::GetRenderOrder() + 0.01);
						modelStack.Scale(dTextSize, dTextSize, 1);
					}
				}
				else
				{
					if (CGUI::GetEnlarge())
					{
						modelStack.Translate(CGUI::GetPositionX() / 10 - CGUI::GetLength() / 20 + 1, CGUI::GetPositionY() / 10 + CGUI::GetHeight() / 20 - dTextEnlargeSize * 1.2, CGUI::GetRenderOrder() + 0.01);
						modelStack.Scale(dTextEnlargeSize, dTextEnlargeSize, 1);
					}
					else
					{
						modelStack.Translate(CGUI::GetPositionX() / 10 - CGUI::GetLength() / 20 + 1, CGUI::GetPositionY() / 10 + CGUI::GetHeight() / 20 - dTextSize * 1.2, CGUI::GetRenderOrder() + 0.01);
						modelStack.Scale(dTextSize, dTextSize, 1);
					}
				}
			}
			




			for (unsigned i = 0; i < print[j].length(); ++i)
			{
				Mtx44 characterSpacing;
				characterSpacing.SetToTranslation(i * 1.0f + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
				Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
				glUniformMatrix4fv(U_MVP, 1, GL_FALSE, &MVP.a[0]);

				textMesh->Render((unsigned)print[j][i] * 6, 6);
			}
			modelStack.PopMatrix();
			if (CGUI::GetEnlarge())
			{
				modelStack.Translate(0, -dTextEnlargeSize, 0);
			}
			else
			{
				modelStack.Translate(0, -dTextSize, 0);
			}
			
		}
		
		glBindTexture(GL_TEXTURE_2D, 0);
		glUniform1i(U_TEXT_ENABLED, 0);
		modelStack.PopMatrix();
		viewStack.PopMatrix();
		projectionStack.PopMatrix();
	}
}
