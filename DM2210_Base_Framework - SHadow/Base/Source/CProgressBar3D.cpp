
#include "CProgressBar3D.h"

#include "GL/glew.h"

#include <GLFW/glfw3.h>

#include "Application.h"

CProgressBar3D::CProgressBar3D()
{

	eBarType = HPBAR;
	dMaxValue = 100; // Just incase
	dValue = 100;
	U_MVP = U_COLOR_TEXTURE_ENABLED = U_COLOR_TEXTURE = 0;
}

CProgressBar3D::~CProgressBar3D()
{
	//for (int i = 0, int x = vimageMesh.size(); i < x; ++i)
	//{
	//	Mesh* cToBeDeleted = vimageMesh.at(i);
	//	delete cToBeDeleted;
	//}
	//Just clear its called somewhere else
	vimageMesh.clear();

	//for (int i = 0, int x = vBackgroundMesh.size(); i < x; ++i)
	//{
	//	Mesh* cToBeDeleted = vBackgroundMesh.at(i);
	//	delete cToBeDeleted;
	//}
	//Just clear its called somewhere else
	vBackgroundMesh.clear();
}

void CProgressBar3D::SetValue(double dValue)
{
	//With within 0 and max value
	if (dValue <= dMaxValue && dValue >= 0)
		this->dValue = dValue;

	if (dValue > dMaxValue)
	{
		dValue = dMaxValue;
	}
	else if (dValue < 0)
	{
		this->dValue = 0;
	}
	dxScale = 1 - (1 * (dValue / dMaxValue));
}

double CProgressBar3D::GetValue()
{
	return dValue;
}

double CProgressBar3D::GetxScale()
{
	return dxScale;
}

void CProgressBar3D::SetLength(double dNewLength)
{
	this->dLength = dNewLength;
}

void CProgressBar3D::SetHeight(double dNewHeight)
{
	this->dHeight = dNewHeight;
}

double CProgressBar3D::GetLength()
{
	return dLength;
}

double CProgressBar3D::GetHeight()
{
	return dHeight;
}

void CProgressBar3D::SetMaxValue(double dMaxValue)
{

	if (dMaxValue == 0)
	{
		dMaxValue = 0.0001; //Prevent /0 error
	
	}
	else
	{
		//If its negative flip the value
		if (dMaxValue < 0)
		{
			dMaxValue = -dMaxValue;
		}
		this->dMaxValue = dMaxValue;
	}
	

	if (dValue > dMaxValue)
	{
		dValue = dMaxValue;
	}
	dxScale = 1 - (1 * (dValue / dMaxValue));
}

double CProgressBar3D::GetMaxValue()
{
	return dMaxValue;
}

bool CProgressBar3D::GetActive()
{
	return bActive;
}

void CProgressBar3D::SetActive(bool bActive)
{
	this->bActive = bActive;
}


void CProgressBar3D::AttachParameters(unsigned U_MVP, unsigned U_COLOR_TEXTURE_ENABLED, unsigned U_COLOR_TEXTURE, unsigned U_MODELVIEW, unsigned U_MODELSTACK)
{
	this->U_MVP = U_MVP;
	this->U_COLOR_TEXTURE_ENABLED = U_COLOR_TEXTURE_ENABLED;
	this->U_COLOR_TEXTURE = U_COLOR_TEXTURE;
	this->U_MODELVIEW = U_MODELVIEW;
	this->U_MODELSTACK = U_MODELSTACK;
}

void CProgressBar3D::AddImageMesh(Mesh* imageMesh)
{
	vimageMesh.push_back(imageMesh);
}

Mesh* CProgressBar3D::GetImageMesh()
{
	switch (eBarType)
	{
	default:
	case HPBAR:

		if (dValue <= 0)
		{
			return vimageMesh[2]; //Red Bar
		}
		else if (dValue > dMaxValue * 0.5)
		{
			return vimageMesh[0]; //Green Bar
		}
		else
		{
			return vimageMesh[1];
		}
		break;
	}
}

void CProgressBar3D::ClearImageMesh()
{
	vimageMesh.clear();
}

void CProgressBar3D::AddBackgroundMesh(Mesh* imageMesh)
{
	vBackgroundMesh.push_back(imageMesh);
}

Mesh* CProgressBar3D::GetBackgroundMesh()
{
	switch (eBarType)
	{
	default:
	case HPBAR:

		if (dValue > dMaxValue * 0.5)
		{
			return vBackgroundMesh[0]; //Green Bar
		}
		else if (dValue == 0)
		{
			return vBackgroundMesh[2];
		}
		else
		{
			return vBackgroundMesh[1];
		}
		break;
	}
}

void CProgressBar3D::ClearBackgroundMesh()
{
	vBackgroundMesh.clear();
}

void CProgressBar3D::SetBarType(BAR_TYPE eNewType)
{
	eBarType = eNewType;
}



void CProgressBar3D::Update(double dt)
{
	

}

//void CProgressBar3D::Render(MS& projectionStack, MS& viewStack, MS& modelStack, Vector3 CameraPos)
//{
//	//if (CGUI::isActive())
//	//{
//	//	double x, y;
//	//	Application::GetCursorPos(&x, &y);
//	//	x = x / Application::GetWindowWidth() * LENGTHSCALE; // In case of resize
//	//	y = Application::GetWindowHeight() - y;
//	//	y = y / Application::GetWindowHeight() * HEIGHTSCALE;
//	//	if (bFollowMouse)
//	//	{
//	//		CGUI::RenderBackground(projectionStack, viewStack, modelStack, x + CGUI::GetLength() / 2 + 10.0, y - CGUI::GetHeight() / 2 - 10.0);
//	//	}
//	//	else
//	//	{
//	//		CGUI::Render(projectionStack, viewStack, modelStack);
//	//	}
//	//	Mtx44 ortho;
//	//	ortho.SetToOrtho(-80, 80, -60, 60, -10, 10);
//	//	modelStack.LoadIdentity();
//
//	//	projectionStack.PushMatrix();
//	//	projectionStack.LoadMatrix(ortho);
//	//	viewStack.PushMatrix();
//	//	viewStack.LoadIdentity();
//	//	modelStack.PushMatrix();
//	//	modelStack.LoadIdentity();
//	//	
//	//	
//	//	Vector3 vFinalPosition = Vector3 (vPosition.x + CGUI::GetLength(), vPosition.y, vPosition.z);
//
//	//	modelStack.Translate(vFinalPosition.x, vFinalPosition.y, vFinalPosition.z); // if translate (0, 0, 0), it is centered, not top left so minus
//
//	//	modelStack.Rotate(Math::RadianToDegree(atan2(CameraPos.x - vPosition.x, CameraPos.z - vPosition.z)), 0, 1, 0);
//	//
//	//	modelStack.Scale(CGUI::GetLength() / 5 * (dValue/dMaxValue), CGUI::GetHeight() / 5, 1);
//	//	// Divide by 5 to match with 800 x 600 pixels
//
//	//	Mtx44 MVP, modelView, modelView_inverse_transpose;
//
//	//	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
//	//	glUniformMatrix4fv(U_MVP, 1, GL_FALSE, &MVP.a[0]);
//	//	if (imageMesh->textureArray[0] > 0)
//	//	{
//	//		glUniform1i(U_COLOR_TEXTURE_ENABLED, 1);
//	//		glActiveTexture(GL_TEXTURE0);
//	//		glBindTexture(GL_TEXTURE_2D, imageMesh->textureArray[0]);
//	//		glUniform1i(U_COLOR_TEXTURE, 0);
//	//	}
//	//	else
//	//	{
//	//		glUniform1i(U_COLOR_TEXTURE_ENABLED, 0);
//	//	}
//	//	imageMesh->Render();
//	//	if (imageMesh->textureArray[0] > 0)
//	//	{
//	//		glBindTexture(GL_TEXTURE_2D, 0);
//	//	}
//
//	//	modelStack.PopMatrix();
//	//	viewStack.PopMatrix();
//	//	projectionStack.PopMatrix();
//	//}
//
//	Mtx44 perspective;
//	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
//	modelStack.LoadIdentity();
//
//
//	projectionStack.PushMatrix();
//	projectionStack.LoadMatrix(perspective);
//	viewStack.PushMatrix();
//	viewStack.LoadIdentity();
//	modelStack.PushMatrix();
//	modelStack.LoadIdentity();
//
//		//Front
//	modelStack.PushMatrix();
//
//
//	//Rotate about center
//	//modelStack.Translate(vPosition.x, vPosition.y, vPosition.z);
//	//modelStack.Rotate(Math::RadianToDegree(atan2(CameraPos.x - vPosition.x, CameraPos.z - vPosition.z)), 0, 1, 0);
//	//modelStack.Translate(-vPosition.x, -vPosition.y, -vPosition.z);
//
//	//Translate to actual position
//	//modelStack.Translate(vPosition.x - (GetLength() * dxScale * 0.5), vPosition.y, vPosition.z + 0.01f);
//
//	//Scale
//	//modelStack.Scale(GetLength() * (dValue/ dMaxValue), GetHeight(), GetLength());
//	
//	modelStack.Translate(10, 200, 10);
//	modelStack.Scale(50, 50, 50);
//	
//	Mtx44 MVP, modelView, modelView_inverse_transpose;
//
//	
//
//	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
//	glUniformMatrix4fv(U_MVP, 1, GL_FALSE, &MVP.a[0]);
//	modelView = viewStack.Top() * modelStack.Top();
//	glUniformMatrix4fv(U_MODELVIEW, 1, GL_FALSE, &modelView.a[0]);
//	glUniformMatrix4fv(U_MODELSTACK, 1, GL_FALSE, &modelStack.Top().a[0]);
//
//	
//	if (GetImageMesh()->textureArray[0] > 0)
//	{
//		glUniform1i(U_COLOR_TEXTURE_ENABLED, 1);
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, GetImageMesh()->textureArray[0]);
//		glUniform1i(U_COLOR_TEXTURE, 0);
//	}
//	else
//	{
//		glUniform1i(U_COLOR_TEXTURE_ENABLED, 0);
//	}
//	
//	GetImageMesh()->Render(); 
//
//	if (vimageMesh[0]->textureArray[0] > 0)
//	{
//		modelStack.PopMatrix();
//		glBindTexture(GL_TEXTURE_2D, 0);
//	}
//
//	//Back
//	modelStack.PushMatrix();
//
//	modelStack.Translate(vPosition.x, vPosition.y, vPosition.z);
//
//	modelStack.Rotate(Math::RadianToDegree(atan2(CameraPos.x - vPosition.x, CameraPos.z - vPosition.z)), 0, 1, 0);
//	
//	modelStack.Scale(dLength, dHeight, dLength);
//
//
//	for (int i = 0; i < MAX_TEXTURES; ++i)
//	{
//		if (vBackgroundMesh[0]->textureArray[i] > 0)
//		{
//			glUniform1i((U_COLOR_TEXTURE_ENABLED + i), 1);
//			glActiveTexture(GL_TEXTURE0 + i);
//			glBindTexture(GL_TEXTURE_2D, vBackgroundMesh[0]->textureArray[i]);
//			glUniform1i((U_COLOR_TEXTURE + i), i);
//		}
//		else
//		{
//			glUniform1i((U_COLOR_TEXTURE_ENABLED + i), 0);
//		}
//	}
//	
//	vBackgroundMesh[0]->Render();
//
//	if (vBackgroundMesh[0]->textureArray[0] > 0)
//	{
//		glBindTexture(GL_TEXTURE_2D, 0);
//	}
//
//	modelStack.PopMatrix();
//
//	modelStack.PopMatrix();
//	viewStack.PopMatrix();
//	projectionStack.PopMatrix();
//	
//}

void CProgressBar3D::SetPosition(Vector3 vNewPos)
{
	vPosition = vNewPos;
}

Vector3 CProgressBar3D::GetPosition()
{
	return vPosition;
}
