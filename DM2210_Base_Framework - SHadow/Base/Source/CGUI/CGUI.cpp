#include "CGUI.h"


#include "GL/glew.h"

#include <GLFW/glfw3.h>

#include "../Application.h"

#define LENGTHSCALE 800
#define HEIGHTSCALE 600

CGUI::CGUI()
{
	mesh = nullptr;
	dGuiLength = dGuiHeight = dPositionX = dPositionY = bEnlarge = bMouseHover = dDebounceTime = dCooldownTime =  0;
	U_MVP = U_COLOR_TEXTURE_ENABLED = U_COLOR_TEXTURE = 0;
	bActive = true;
	bEnableAnimation = false;
	extraValueX = extraValueY = 0;
	dRenderOrder = 0;
}

CGUI::~CGUI()
{
	if (mesh != nullptr)
	{
		delete mesh;
		mesh = nullptr;
	}

}

void CGUI::AttachParameters(unsigned U_MVP, unsigned U_COLOR_TEXTURE_ENABLED, unsigned U_COLOR_TEXTURE)
{
	this->U_MVP = U_MVP;
	this->U_COLOR_TEXTURE_ENABLED = U_COLOR_TEXTURE_ENABLED;
	this->U_COLOR_TEXTURE = U_COLOR_TEXTURE;
}

void CGUI::SetBackgroundMesh(Mesh* mesh)
{
	this->mesh = mesh;
}


void CGUI::StopBackgroundAnimation() // only use if animation is enabled
{
	if (bEnableAnimation && mesh != nullptr)
	{
		SpriteAnimation* sa = dynamic_cast<SpriteAnimation*>(mesh);
		sa->Reset();
		sa->Pause();
	}
}


void CGUI::StartBackgroundAnimation() // only use if animation is enabled
{
	if (bEnableAnimation && mesh != nullptr)
	{
		SpriteAnimation* sa = dynamic_cast<SpriteAnimation*>(mesh);
		sa->Resume();
	}
}

Mesh* CGUI::GetBackgroundMesh()
{
	return mesh;
}

void CGUI::SetLength(double dGuiLength)
{
	this->dGuiLength = dGuiLength;
}

double CGUI::GetLength()
{
	return dGuiLength;
}

void CGUI::SetHeight(double dGuiHeight)
{
	this->dGuiHeight = dGuiHeight;
}

double CGUI::GetHeight()
{
	return dGuiHeight;
}

void CGUI::SetPosition(double dPositionX, double dPositionY)
{
	this->dPositionX = dPositionX;
	this->dPositionY = dPositionY;
}

double CGUI::GetPositionX()
{
	return dPositionX;
}

double CGUI::GetPositionY()
{
	return dPositionY;
}

void CGUI::SetEnlarge(bool bEnlarge)
{
	// Enlarge function
	// Enlarge
	if (bEnlarge && !this->bEnlarge) // if command to enlarge and cGUI is not enlarged yet
	{
		// Enlarge
		extraValueX = 0.2 * dGuiLength;
		extraValueY = 0.2 * dGuiHeight;

		dGuiLength += extraValueX;
		dGuiHeight += extraValueY;

	}
	else if (!bEnlarge && this->bEnlarge)
	{
		dGuiLength -= extraValueX;
		dGuiHeight -= extraValueY;


	}
	this->bEnlarge = bEnlarge;

	
}

bool CGUI::GetEnlarge()
{
	return bEnlarge;
}

void CGUI::SetCooldownTime(double dCooldownTime)
{
	this->dCooldownTime = dCooldownTime;
}

void CGUI::ResetTimer()
{
	dDebounceTime = 0;
}

bool CGUI::isCooldownOver()
{
	return dDebounceTime > dCooldownTime;
}

bool CGUI::isActive()
{
	return bActive;
}

void CGUI::SetActive(bool bActive)
{
	this->bActive = bActive;
}

bool CGUI::isBackgroundAnimationEnabled()
{
	return bEnableAnimation;
}

void CGUI::SetBackgroundAnimationEnabled(bool bEnableAnimation)
{
	this->bEnableAnimation = bEnableAnimation;
}

void CGUI::SetRenderOrder(double dRenderOrder)
{
	this->dRenderOrder = dRenderOrder;
}

double CGUI::GetRenderOrder()
{
	return dRenderOrder;
}



bool CGUI::IsMouseHover()
{
	if (!bActive)
		return false;
	double x, y;
	Application::GetCursorPos(&x, &y);
	y = Application::GetWindowHeight() - y; // Since y is inverted
	if (x > (dPositionX - dGuiLength / 2) / LENGTHSCALE * Application::GetWindowWidth()
		&& x < (dPositionX + dGuiLength / 2) / LENGTHSCALE * Application::GetWindowWidth()
		&& y >(dPositionY - dGuiHeight / 2) / HEIGHTSCALE * Application::GetWindowHeight()
		&& y < (dPositionY + dGuiHeight / 2) / HEIGHTSCALE * Application::GetWindowHeight()
		&& x <= Application::GetWindowWidth() && y <= Application::GetWindowHeight())
	{
		return true;
	}
	else return false;
}

void CGUI::Update(double dt)
{
	dDebounceTime += dt;
	if (isBackgroundAnimationEnabled() && mesh != nullptr)
	{
		SpriteAnimation* sa = dynamic_cast<SpriteAnimation*>(mesh);
		sa->Update(dt);
	}
}

void CGUI::Render(MS &projectionStack, MS &viewStack, MS &modelStack)
{

	if (mesh != nullptr && bActive)
	{
		Mtx44 ortho;
		ortho.SetToOrtho(-80, 80, -60, 60, -10, 10);
		modelStack.LoadIdentity();

		projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
		viewStack.PushMatrix();
		viewStack.LoadIdentity();
		modelStack.PushMatrix();
		modelStack.LoadIdentity();
		modelStack.Translate(dPositionX / 5 - 80, dPositionY / 5 - 60, dRenderOrder); // if translate (0, 0, 0), it is centered, not top left so minus
		modelStack.Scale(dGuiLength / 5, dGuiHeight / 5, 1);
		// Divide by 5 to match with 800 x 600 pixels

		Mtx44 MVP, modelView, modelView_inverse_transpose;

		MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(U_MVP, 1, GL_FALSE, &MVP.a[0]);
		if (mesh->textureArray[0] > 0)
		{
			glUniform1i(U_COLOR_TEXTURE_ENABLED, 1);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mesh->textureArray[0]);
			glUniform1i(U_COLOR_TEXTURE, 0);
		}
		else
		{
			glUniform1i(U_COLOR_TEXTURE_ENABLED, 0);
		}
		mesh->Render();
	
		if (mesh->textureArray[0] > 0)
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		modelStack.PopMatrix();
		viewStack.PopMatrix();
		projectionStack.PopMatrix();
	}
	
}

void CGUI::RenderBackground(MS& projectionStack, MS& viewStack, MS& modelStack, double dPosX, double dPosY)
{
	if (mesh != nullptr)
	{
		Mtx44 ortho;
		ortho.SetToOrtho(-80, 80, -60, 60, -10, 10);
		modelStack.LoadIdentity();

		projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
		viewStack.PushMatrix();
		viewStack.LoadIdentity();
		modelStack.PushMatrix();
		modelStack.LoadIdentity();
		modelStack.Translate(dPosX / 5 - 80, dPosY / 5 - 60, dRenderOrder); // if translate (0, 0, 0), it is centered, not top left so minus
		modelStack.Scale(dGuiLength / 5, dGuiHeight / 5, 1);
		// Divide by 5 to match with 800 x 600 pixels

		Mtx44 MVP, modelView, modelView_inverse_transpose;

		MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(U_MVP, 1, GL_FALSE, &MVP.a[0]);
		if (mesh->textureArray[0] > 0)
		{
			glUniform1i(U_COLOR_TEXTURE_ENABLED, 1);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mesh->textureArray[0]);
			glUniform1i(U_COLOR_TEXTURE, 0);
		}
		else
		{
			glUniform1i(U_COLOR_TEXTURE_ENABLED, 0);
		}
		mesh->Render();
		if (mesh->textureArray[0] > 0)
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		modelStack.PopMatrix();
		viewStack.PopMatrix();
		projectionStack.PopMatrix();
	}
}

bool CGUI::IsMousePressed(unsigned short key) // 0 for left, 1 for right, 2 for middle
{
	return glfwGetMouseButton(Application::GetWindow(), key) != 0;
}

bool CGUI::IsKeyPressed(unsigned short key)
{
	return ((GetAsyncKeyState(key) & 1) != 0);
}

bool CGUI::IsKeyDown(unsigned short key)
{
	return ((GetAsyncKeyState(key) & 0x8001) != 0);
}
