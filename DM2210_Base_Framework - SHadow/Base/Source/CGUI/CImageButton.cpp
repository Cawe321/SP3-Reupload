
#include "CImageButton.h"

#include "GL/glew.h"

#include <GLFW/glfw3.h>

#include "../Application.h"

CImageButton::CImageButton()
{
	bEnableAnimation = false;
	imageMesh = nullptr;
	U_MVP = U_COLOR_TEXTURE_ENABLED = U_COLOR_TEXTURE = 0;
}

CImageButton::~CImageButton()
{
	if (imageMesh != nullptr)
	{
		delete imageMesh;
		imageMesh = nullptr;
	}

}

void CImageButton::AttachParameters(unsigned U_MVP, unsigned U_COLOR_TEXTURE_ENABLED, unsigned U_COLOR_TEXTURE)
{
	CGUI::AttachParameters(U_MVP, U_COLOR_TEXTURE_ENABLED, U_COLOR_TEXTURE);
	this->U_MVP = U_MVP;
	this->U_COLOR_TEXTURE_ENABLED = U_COLOR_TEXTURE_ENABLED;
	this->U_COLOR_TEXTURE = U_COLOR_TEXTURE;
}

bool CImageButton::isAnimationEnabled()
{
	return bEnableAnimation;
}

void CImageButton::SetAnimationEnabled(bool bEnableAnimation)
{
	this->bEnableAnimation = bEnableAnimation;
}

void CImageButton::SetImageMesh(Mesh* imageMesh)
{
	this->imageMesh = imageMesh;
}

void CImageButton::StopAnimation()
{
	if (CGUI::isActive() && bEnableAnimation && imageMesh != nullptr)
	{
		SpriteAnimation* sa = dynamic_cast<SpriteAnimation*>(imageMesh);
		sa->Reset();
		sa->Pause();
	}
}

void CImageButton::StartAnimation()
{
	if (CGUI::isActive() && bEnableAnimation && imageMesh != nullptr)
	{
		SpriteAnimation* sa = dynamic_cast<SpriteAnimation*>(imageMesh);
		sa->Resume();
	}
}



bool CImageButton::isLeftMouseButtonPressed()
{
	if (CGUI::IsMouseHover() && CGUI::IsMousePressed(0) && CGUI::isActive() && CGUI::GetPositionX() <= Application::GetWindowWidth() && CGUI::GetPositionY() <= Application::GetWindowHeight())
		return true;
	else return false;
}

void CImageButton::Update(double dt)
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

void CImageButton::Render(MS& projectionStack, MS& viewStack, MS& modelStack)
{
	if (CGUI::isActive())
	{
		CGUI::Render(projectionStack, viewStack, modelStack);
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
		modelStack.Translate(CGUI::GetPositionX() / 5 - 80, CGUI::GetPositionY() / 5 - 60, CGUI::GetRenderOrder() + 0.01); // if translate (0, 0, 0), it is centered, not top left so minus
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
