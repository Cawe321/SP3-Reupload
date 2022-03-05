#pragma once

#include "MyMath.h"
#include "Vector3.h"

class CCollider
{
public:

	// Bounding Box specifications
	// Bounding Box top right position
	Vector3 vec3TopRight;
	// Bounding Box bottom left position
	Vector3 vec3BottomLeft;
	// Bounding Box colour
	Vector3 vec3Colour;

	// Boolean flag to indicate if Bounding Box is displayed
	bool bIsDisplayed;
	// Line width
	float fLineWidth;

	// Constructor(s)
	CCollider(void);
	// Destructor
	virtual ~CCollider(void);

	// Initialise this class instance
	bool Init(void);

	virtual void Render(void);

protected:
	unsigned int VAO, VBO;
};

