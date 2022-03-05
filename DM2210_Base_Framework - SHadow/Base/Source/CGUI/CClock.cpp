
#include "CClock.h"

#include "GL/glew.h"

#include <GLFW/glfw3.h>

#include "../MeshBuilder.h"
#include "../LoadTGA.h"
#include "../LoadTexture.h"

#include "../Application.h"

CClock::CClock()
{
	dHour = dMinute = 0;

	CGUI::SetBackgroundMesh(nullptr);

	hourMesh = nullptr;

	minuteMesh = nullptr;

	U_MVP = U_COLOR_TEXTURE_ENABLED = U_COLOR_TEXTURE = 0;
	bFollowMouse = false;
}

CClock::~CClock()
{
	if (hourMesh != nullptr)
	{
		delete hourMesh;
		hourMesh = nullptr;
	}
	if (minuteMesh != nullptr)
	{
		delete minuteMesh;
		minuteMesh = nullptr;
	}

}

void CClock::Init()
{
	if (hourMesh != nullptr)
	{
		delete hourMesh;
		hourMesh = nullptr;
	}
	if (minuteMesh != nullptr)
	{
		delete minuteMesh;
		minuteMesh = nullptr;
	}

	Mesh* storage = MeshBuilder::GenerateQuad("clock", Color(0, 0, 0), 1.f);
	storage->textureArray[0] = LoadTexture("Image//UI/clock.png");
	CGUI::SetBackgroundMesh(storage);

	hourMesh = MeshBuilder::GenerateQuad("hourhand", Color(0, 1, 0), 1.f);
	hourMesh->textureArray[0] = LoadTexture("Image//UI/clock_hour.png");

	minuteMesh = MeshBuilder::GenerateQuad("minutehand", Color(0, 1, 0), 1.f);
	minuteMesh->textureArray[0] = LoadTexture("Image//UI/clock_minute.png");
}

/*
Follows 24 Hour Clock, 23 : 59
*/
void CClock::SetClockTime(double dHour, double dMinute)
{
	this->dHour = dHour;
	this->dMinute = dMinute;
}

void CClock::SetFollowMouse(bool bFollowMouse)
{
	this->bFollowMouse = bFollowMouse;
}

void CClock::AttachParameters(unsigned U_MVP, unsigned U_COLOR_TEXTURE_ENABLED, unsigned U_COLOR_TEXTURE)
{
	CGUI::AttachParameters(U_MVP, U_COLOR_TEXTURE_ENABLED, U_COLOR_TEXTURE);
	this->U_MVP = U_MVP;
	this->U_COLOR_TEXTURE_ENABLED = U_COLOR_TEXTURE_ENABLED;
	this->U_COLOR_TEXTURE = U_COLOR_TEXTURE;
}

void CClock::Update(double dt)
{
	if (CGUI::isActive())
	{
		CGUI::Update(dt);
	}

}

void CClock::Render(MS& projectionStack, MS& viewStack, MS& modelStack)
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
		
	
		
		Mtx44 MVP, modelView, modelView_inverse_transpose;
		// Start of Render Minute Hand
		modelStack.PushMatrix();
			modelStack.Rotate(-(dMinute / 60.0 * 360.0), 0.f, 0.f, 1.f);
			// Divide by 5 to match with 800 x 600 pixels
			modelStack.Translate(0.f, 0.f, CGUI::GetRenderOrder() + 0.01f);
			modelStack.Scale(CGUI::GetLength() / 10, CGUI::GetHeight() / 5, 1);
		
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
			glUniformMatrix4fv(U_MVP, 1, GL_FALSE, &MVP.a[0]);
			if (minuteMesh->textureArray[0] > 0)
			{
				glUniform1i(U_COLOR_TEXTURE_ENABLED, 1);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, minuteMesh->textureArray[0]);
				glUniform1i(U_COLOR_TEXTURE, 0);
			}
			else
			{
				glUniform1i(U_COLOR_TEXTURE_ENABLED, 0);
			}
			minuteMesh->Render();
			if (minuteMesh->textureArray[0] > 0)
			{
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		modelStack.PopMatrix();
		// End of Render Minute Hand


		// Start of Render Hour Hand
		modelStack.PushMatrix();
			double angle = (int)dHour / 12.0 * 360 + dMinute / 60.0 * 30;
			modelStack.Rotate(-angle, 0.f, 0.f, 1.f);
			// Divide by 5 to match with 800 x 600 pixels
			modelStack.Translate(0.f, 0.f, CGUI::GetRenderOrder() + 0.02f);
			modelStack.Scale(CGUI::GetLength() / 10, CGUI::GetHeight() / 5, 1);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
			glUniformMatrix4fv(U_MVP, 1, GL_FALSE, &MVP.a[0]);
			if (hourMesh->textureArray[0] > 0)
			{
				glUniform1i(U_COLOR_TEXTURE_ENABLED, 1);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, hourMesh->textureArray[0]);
				glUniform1i(U_COLOR_TEXTURE, 0);
			}
			else
			{
				glUniform1i(U_COLOR_TEXTURE_ENABLED, 0);
			}
			hourMesh->Render();
			if (hourMesh->textureArray[0] > 0)
			{
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		modelStack.PopMatrix();
		// End of Render Hour Hand

		modelStack.PopMatrix();
		viewStack.PopMatrix();
		projectionStack.PopMatrix();
	}
	
}
