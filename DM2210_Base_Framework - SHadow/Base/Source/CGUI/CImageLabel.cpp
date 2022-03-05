
#include "CImageLabel.h"

#include "GL/glew.h"

#include <GLFW/glfw3.h>

#include "../Application.h"

CImageLabel::CImageLabel()
{
	bEnableAnimation = false;
	imageMesh = nullptr;
	U_MVP = U_COLOR_TEXTURE_ENABLED = U_COLOR_TEXTURE = 0;
}

CImageLabel::~CImageLabel()
{
	if (imageMesh != nullptr)
	{
		delete imageMesh;
		imageMesh = nullptr;
	}
}

void CImageLabel::SetFollowMouse(bool bFollowMouse)
{
	this->bFollowMouse = bFollowMouse;
}

void CImageLabel::AttachParameters(unsigned U_MVP, unsigned U_COLOR_TEXTURE_ENABLED, unsigned U_COLOR_TEXTURE)
{
	CGUI::AttachParameters(U_MVP, U_COLOR_TEXTURE_ENABLED, U_COLOR_TEXTURE);
	this->U_MVP = U_MVP;
	this->U_COLOR_TEXTURE_ENABLED = U_COLOR_TEXTURE_ENABLED;
	this->U_COLOR_TEXTURE = U_COLOR_TEXTURE;
}

void CImageLabel::SetImageMesh(Mesh* imageMesh)
{
	this->imageMesh = imageMesh;
}

void CImageLabel::StopAnimation()
{
	if (CGUI::isActive() && bEnableAnimation && imageMesh != nullptr)
	{
		SpriteAnimation* sa = dynamic_cast<SpriteAnimation*>(imageMesh);
		sa->Reset();
		sa->Pause();
	}
}

void CImageLabel::StartAnimation()
{
	if (CGUI::isActive() && bEnableAnimation && imageMesh != nullptr)
	{
		SpriteAnimation* sa = dynamic_cast<SpriteAnimation*>(imageMesh);
		sa->Resume();
	}
}

bool CImageLabel::isAnimationEnabled()
{
	return bEnableAnimation;
}

void CImageLabel::SetAnimationEnabled(bool bEnableAnimation)
{
	this->bEnableAnimation = bEnableAnimation;
}

void CImageLabel::Update(double dt)
{
	if (CGUI::isActive())
	{
		CGUI::Update(dt);
		if (isAnimationEnabled())
		{
			SpriteAnimation* sa = dynamic_cast<SpriteAnimation*>(imageMesh);
			sa->Update(dt);
		}
	}

}

void CImageLabel::Render(MS& projectionStack, MS& viewStack, MS& modelStack)
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
			CGUI::RenderBackground(projectionStack, viewStack, modelStack, x + CGUI::GetLength() / 2 + 10.0, y - CGUI::GetHeight() / 2 - 10.0);
		}
		else
		{
			CGUI::Render(projectionStack, viewStack, modelStack);
		}

		if (imageMesh == nullptr)
			return;

		Mtx44 ortho;
		ortho.SetToOrtho(-80, 80, -60, 60, -10, 10);
		modelStack.LoadIdentity();

		projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
		viewStack.PushMatrix();
		viewStack.LoadIdentity();
		modelStack.PushMatrix();
		modelStack.LoadIdentity();
		
		if (bFollowMouse)
		{
			modelStack.Translate((x + CGUI::GetLength() / 2 + 10.0) / 5 - 80, (y - CGUI::GetHeight() / 2 - 10.0) / 5 - 60, CGUI::GetRenderOrder() + 0.01); // if translate (0, 0, 0), it is centered, not top left so minus
		}
		else
		{
			modelStack.Translate(CGUI::GetPositionX() / 5 - 80, CGUI::GetPositionY() / 5 - 60, CGUI::GetRenderOrder() + 0.01); // if translate (0, 0, 0), it is centered, not top left so minus
		}
		
		modelStack.Scale(CGUI::GetLength() / 5, CGUI::GetHeight() / 5, 1);
		// Divide by 5 to match with 800 x 600 pixels

		Mtx44 MVP, modelView, modelView_inverse_transpose;

		MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(U_MVP, 1, GL_FALSE, &MVP.a[0]);
		if (imageMesh->textureArray[0] > 0)
		{
			glUniform1i(U_COLOR_TEXTURE_ENABLED, 1);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, imageMesh->textureArray[0]);
			glUniform1i(U_COLOR_TEXTURE, 0);
		}
		else
		{
			glUniform1i(U_COLOR_TEXTURE_ENABLED, 0);
		}
		imageMesh->Render();
		if (imageMesh->textureArray[0] > 0)
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		modelStack.PopMatrix();
		viewStack.PopMatrix();
		projectionStack.PopMatrix();
	}
	
}
