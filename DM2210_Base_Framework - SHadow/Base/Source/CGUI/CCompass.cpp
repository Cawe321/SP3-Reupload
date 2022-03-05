
#include "CCompass.h"

#include "GL/glew.h"

#include <GLFW/glfw3.h>

#include "../MeshBuilder.h"
#include "../LoadTGA.h"

#include "../Application.h"

CCompass::CCompass()
{
	camera = nullptr;
	compassMesh = nullptr;
	U_MVP = U_COLOR_TEXTURE_ENABLED = U_COLOR_TEXTURE = 0;
	bFollowMouse = false;
}

CCompass::~CCompass()
{
	if (camera != nullptr)
	{
		delete camera;
		camera = nullptr;
	}
	if (compassMesh != nullptr)
	{
		delete compassMesh;
		compassMesh = nullptr;
	}

}

void CCompass::Init()
{
	compassMesh = MeshBuilder::GenerateOBJ("compass", "OBJ//compass.obj");
	compassMesh->textureArray[0] = LoadTGA("Image//UI/compass.tga");
}

void CCompass::SetFollowMouse(bool bFollowMouse)
{
	this->bFollowMouse = bFollowMouse;
}

void CCompass::AttachParameters(unsigned U_MVP, unsigned U_COLOR_TEXTURE_ENABLED, unsigned U_COLOR_TEXTURE)
{
	CGUI::AttachParameters(U_MVP, U_COLOR_TEXTURE_ENABLED, U_COLOR_TEXTURE);
	this->U_MVP = U_MVP;
	this->U_COLOR_TEXTURE_ENABLED = U_COLOR_TEXTURE_ENABLED;
	this->U_COLOR_TEXTURE = U_COLOR_TEXTURE;
}

void CCompass::AttachCamera(Camera& camera)
{
	this->camera = &camera;
}

void CCompass::SetCompassMesh(Mesh* imageMesh)
{
	this->compassMesh = imageMesh;
}

void CCompass::Update(double dt)
{
	if (CGUI::isActive())
	{
		CGUI::Update(dt);
	}

}

void CCompass::Render(MS& projectionStack, MS& viewStack, MS& modelStack)
{
	if (CGUI::isActive())
	{
		double x, y;
		Application::GetCursorPos(&x, &y);
		x = x / Application::GetWindowWidth() * LENGTHSCALE; // In case of resize
		y = Application::GetWindowHeight() - y;
		y = y / Application::GetWindowHeight() * HEIGHTSCALE;
		
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
		
		modelStack.Scale(CGUI::GetLength() / 10, CGUI::GetHeight() / 5, 1);
		// Divide by 5 to match with 800 x 600 pixels
		modelStack.Rotate(-atan2(camera->target.z - camera->position.z, camera->target.x - camera->position.x) * (180 / 3.142) - 95, 0.f, 1.f, 0.f);

		Mtx44 MVP, modelView, modelView_inverse_transpose;

		MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(U_MVP, 1, GL_FALSE, &MVP.a[0]);
		if (compassMesh->textureArray[0] > 0)
		{
			glUniform1i(U_COLOR_TEXTURE_ENABLED, 1);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, compassMesh->textureArray[0]);
			glUniform1i(U_COLOR_TEXTURE, 0);
		}
		else
		{
			glUniform1i(U_COLOR_TEXTURE_ENABLED, 0);
		}
		compassMesh->Render();
		if (compassMesh->textureArray[0] > 0)
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		modelStack.PopMatrix();
		viewStack.PopMatrix();
		projectionStack.PopMatrix();
	}
	
}
