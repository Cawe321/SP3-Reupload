#include "Collider.h"

#include "GL/glew.h"

#include <GLFW/glfw3.h>

/**
 @brief Default Constructor
 */
CCollider::CCollider(void)
	: vec3TopRight(Vector3(9.f, 9.f, 9.f))
	, vec3BottomLeft(Vector3(-9.f, -9.f, -9.f))
	, vec3Colour(Vector3(0.0f, 1.0f, 0.0f))
	, bIsDisplayed(false)
	, fLineWidth(3.0)
{
}

/**
 @brief Default Destructor
 */
CCollider::~CCollider(void)
{
}

/**
 @brief Initialise this class instance
 */
bool CCollider::Init(void)
{
	// set up vertex data (and buffer(s)) and configure vertex attributes
	float vertices[] = {
		vec3BottomLeft.x, vec3BottomLeft.y, vec3BottomLeft.z, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		vec3TopRight.x, vec3BottomLeft.y, vec3BottomLeft.z, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		vec3TopRight.x, vec3TopRight.y, vec3BottomLeft.z, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		vec3TopRight.x, vec3TopRight.y, vec3BottomLeft.z, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		vec3BottomLeft.x, vec3TopRight.y, vec3BottomLeft.z, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		vec3BottomLeft.x, vec3BottomLeft.y, vec3BottomLeft.z, vec3Colour.x, vec3Colour.y, vec3Colour.z,

		vec3BottomLeft.x, vec3BottomLeft.y, vec3TopRight.z, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		vec3TopRight.x, vec3BottomLeft.y, vec3TopRight.z, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		vec3TopRight.x, vec3TopRight.y, vec3TopRight.z, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		vec3TopRight.x, vec3TopRight.y, vec3TopRight.z, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		vec3BottomLeft.x, vec3TopRight.y, vec3TopRight.z, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		vec3BottomLeft.x, vec3BottomLeft.y, vec3TopRight.z, vec3Colour.x, vec3Colour.y, vec3Colour.z,

		vec3BottomLeft.x, vec3TopRight.y, vec3TopRight.z, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		vec3BottomLeft.x, vec3TopRight.y, vec3BottomLeft.z, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		vec3BottomLeft.x, vec3BottomLeft.y, vec3BottomLeft.z, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		vec3BottomLeft.x, vec3BottomLeft.y, vec3BottomLeft.z, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		vec3BottomLeft.x, vec3BottomLeft.y, vec3TopRight.z, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		vec3BottomLeft.x, vec3TopRight.y, vec3TopRight.z, vec3Colour.x, vec3Colour.y, vec3Colour.z,

		vec3TopRight.x, vec3TopRight.y, vec3TopRight.z, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		vec3TopRight.x, vec3TopRight.y, vec3BottomLeft.z, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		vec3TopRight.x, vec3BottomLeft.y, vec3BottomLeft.z, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		vec3TopRight.x, vec3BottomLeft.y, vec3BottomLeft.z, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		vec3TopRight.x, vec3BottomLeft.y, vec3TopRight.z, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		vec3TopRight.x, vec3TopRight.y, vec3TopRight.z, vec3Colour.x, vec3Colour.y, vec3Colour.z,

		vec3BottomLeft.x, vec3BottomLeft.y, vec3BottomLeft.z, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		vec3TopRight.x, vec3BottomLeft.y, vec3BottomLeft.z, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		vec3TopRight.x, vec3BottomLeft.y, vec3TopRight.z, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		vec3TopRight.x, vec3BottomLeft.y, vec3TopRight.z, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		vec3BottomLeft.x, vec3BottomLeft.y, vec3TopRight.z, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		vec3BottomLeft.x, vec3BottomLeft.y, vec3BottomLeft.z, vec3Colour.x, vec3Colour.y, vec3Colour.z,

		vec3BottomLeft.x, vec3TopRight.y, vec3BottomLeft.z, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		vec3TopRight.x, vec3TopRight.y, vec3BottomLeft.z, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		vec3TopRight.x, vec3TopRight.y, vec3TopRight.z, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		vec3TopRight.x, vec3TopRight.y, vec3TopRight.z, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		vec3BottomLeft.x, vec3TopRight.y, vec3TopRight.z, vec3Colour.x, vec3Colour.y, vec3Colour.z,
		vec3BottomLeft.x, vec3TopRight.y, vec3BottomLeft.z, vec3Colour.x, vec3Colour.y, vec3Colour.z
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glLineWidth(fLineWidth);

	return true;
}

void CCollider::Render(void)
{
	if (!bIsDisplayed)
		return;


	// render box
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINE_LOOP, 0, 36);
}
