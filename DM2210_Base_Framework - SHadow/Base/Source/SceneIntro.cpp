#include "SceneIntro.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "LoadTexture.h"
#include "Entity.h"
#include "filesystem.h"
#include <sstream>
#include <vector>
#include "GLFW/glfw3.h"

SceneIntro::SceneIntro()
{
}

SceneIntro::~SceneIntro()
{
}

bool SceneIntro::Init()
{
	// Black background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	//m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//MultiTexture.fragmentshader" );
	m_programID = LoadShaders("Shader//Shadow.vertexshader", "Shader//Shadow.fragmentshader");

	m_gPassShaderID = LoadShaders("Shader//GPass.vertexshader",
		"Shader//GPass.fragmentshader");
	m_parameters[U_LIGHT_DEPTH_MVP_GPASS] =
		glGetUniformLocation(m_gPassShaderID, "lightDepthMVP");

	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELSTACK] = glGetUniformLocation(m_programID, "MS");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled[0]");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture[0]");
	m_parameters[U_COLOR_TEXTURE_ENABLED1] = glGetUniformLocation(m_programID, "colorTextureEnabled[1]");
	m_parameters[U_COLOR_TEXTURE1] = glGetUniformLocation(m_programID, "colorTexture[1]");
	m_parameters[U_COLOR_TEXTURE_ENABLED2] = glGetUniformLocation(m_programID, "colorTextureEnabled[2]");
	m_parameters[U_COLOR_TEXTURE2] = glGetUniformLocation(m_programID, "colorTexture[2]");

	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	m_parameters[U_WATER] = glGetUniformLocation(m_programID, "water");
	m_parameters[U_WATERENABLED] = glGetUniformLocation(m_programID, "waterEnabled");
	m_parameters[U_WAVENUMBER] = glGetUniformLocation(m_programID, "waveNumber");
	m_parameters[U_GREYMODE] = glGetUniformLocation(m_programID, "greymode");
	m_parameters[U_GLITCHMODE] = glGetUniformLocation(m_programID, "glitchmode");

	m_parameters[U_FOG_COLOR] = glGetUniformLocation(m_programID, "fogParam.color");
	m_parameters[U_FOG_COLOR] = glGetUniformLocation(m_programID, "fogParam.start");
	m_parameters[U_FOG_END] = glGetUniformLocation(m_programID, "fogParam.end");
	m_parameters[U_FOG_DENSITY] = glGetUniformLocation(m_programID, "fogParam.density");
	m_parameters[U_FOG_TYPE] = glGetUniformLocation(m_programID, "fogParam.type");
	m_parameters[U_FOG_ENABLED] = glGetUniformLocation(m_programID, "fogParam.enabled");

	m_parameters[U_LIGHT_DEPTH_MVP] = glGetUniformLocation(m_programID, "lightDepthMVP");
	m_parameters[U_SHADOW_MAP] = glGetUniformLocation(m_programID,
		"shadowMap");

	m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_gPassShaderID, "colorTextureEnabled[0]");
	m_parameters[U_SHADOW_COLOR_TEXTURE] = glGetUniformLocation(m_gPassShaderID, "colorTexture[0]");
	m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED1] = glGetUniformLocation(m_gPassShaderID, "colorTextureEnabled[1]");
	m_parameters[U_SHADOW_COLOR_TEXTURE1] = glGetUniformLocation(m_gPassShaderID, "colorTexture[1]");
	m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED2] = glGetUniformLocation(m_gPassShaderID, "colorTextureEnabled[2]");
	m_parameters[U_SHADOW_COLOR_TEXTURE2] = glGetUniformLocation(m_gPassShaderID, "colorTexture[2]");

	water = 0;
	waterEnabled = false;
	waterwavesnum = 3; //NUMBER OF WAVES! :D

	greymode = false;
	glitchmode = false;

	moveVal = 0.f;

	devCam = true;

	FPos.Set(0, 0, 0);
	VPos.Set(FPos.x, FPos.y + 10, FPos.z);

	cSoundController = CSoundController::GetInstance();
	cSoundController->Init();
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\Background1.wav"), 1, true, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\Button.wav"), 2, true, false);
	cSoundController->PlaySoundByID(1);
	cSoundController->VolumeDecrease(1);
	cSoundController->VolumeDecrease(1);
	// Use our shader
	glUseProgram(m_programID);

	fogdensity = 0.01f;
	//init fog
	Color fogColor(0.5f, 0.5f, 0.5f);
	glUniform3fv(m_parameters[U_FOG_COLOR], 1, &fogColor.r);
	glUniform1f(m_parameters[U_FOG_START], 10);
	glUniform1f(m_parameters[U_FOG_END], 1000);
	glUniform1f(m_parameters[U_FOG_DENSITY], fogdensity);
	glUniform1i(m_parameters[U_FOG_TYPE], 1);
	glUniform1i(m_parameters[U_FOG_ENABLED], 0);

	m_lightDepthFBO.Init(4096, 4096);
	m_lightWaterFBO.Init(4096, 4096);




	lights[0].type = Light::LIGHT_DIRECTIONAL;
	lights[0].position.Set(2, 10, 0);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 10.f;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_NUMLIGHTS], 1);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);


	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference");
	meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateCrossHair("crosshair");
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_QUAD]->textureArray[0] = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);
	

	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);

	meshList[GEO_GROUND] = MeshBuilder::GenerateQuad("Ground", Color(0.6, 0.6, 0.6));
	meshList[GEO_GROUND]->textureArray[0] = LoadTexture("Image//Cement.jpg");

	//Sky Plane
	meshList[GEO_SKYPLANE] = MeshBuilder::GenerateSkyPlane("GEO_SKYPLANE", Color(1, 1, 1), 10, 200.0f, 2000.0f, 1.0f, 1.0f);
	meshList[GEO_SKYPLANE]->textureArray[0] = LoadTexture("Image//top.tga");

	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("Top", Color(1, 1, 1));
	meshList[GEO_TOP]->textureArray[0] = LoadTexture("Image//Skybox//top.jpg");

	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("Left", Color(1, 1, 1));
	meshList[GEO_LEFT]->textureArray[0] = LoadTexture("Image//Skybox//left.jpg");

	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("Right", Color(1, 1, 1));
	meshList[GEO_RIGHT]->textureArray[0] = LoadTexture("Image//Skybox//right.jpg");

	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("Front", Color(1, 1, 1));
	meshList[GEO_FRONT]->textureArray[0] = LoadTexture("Image//Skybox//front.jpg");

	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("Back", Color(1, 1, 1));
	meshList[GEO_BACK]->textureArray[0] = LoadTexture("Image//Skybox//back.jpg");

	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("Bot", Color(1, 1, 1));
	meshList[GEO_BOTTOM]->textureArray[0] = LoadTexture("Image//Skybox//bottom.jpg");

	meshList[GEO_FOUNTAIN] = MeshBuilder::GenerateOBJ("Fountain", "OBJ//Fountain.obj");
	meshList[GEO_FOUNTAIN]->textureArray[0] = LoadTexture("Image//Fountain1.jpg");

	meshList[GEO_VIRUS] = MeshBuilder::GenerateOBJ("Virus", "OBJ//virus.obj");
	meshList[GEO_VIRUS]->textureArray[0] = LoadTexture("Image//virus.png");

	meshList[GEO_ANIMATED_SPRITE] = MeshBuilder::GenerateSpriteAnimation("Animated", 4, 4);
	meshList[GEO_ANIMATED_SPRITE]->textureArray[0] = LoadTexture("Image//Fire.jpeg ");
	meshList[GEO_ANIMATED_SPRITE]->material.kDiffuse.Set(0.04f, 0.04f, 0.04f);
	meshList[GEO_ANIMATED_SPRITE]->material.kSpecular.Set(0.f, 0.f, 0.f);
	meshList[GEO_ANIMATED_SPRITE]->material.kShininess = 32.f;
	meshList[GEO_ANIMATED_SPRITE]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);

	meshList[GEO_LOAD_QUAD] = MeshBuilder::GenerateQuad("Load", Color(1, 1, 1), 1.f);
	meshList[GEO_LOAD_QUAD]->textureArray[0] = LoadTexture("Image//UI/loading.png");

	sa = dynamic_cast<SpriteAnimation*>(meshList[GEO_ANIMATED_SPRITE]);
	sa->AddSequeneAnimation("Animation1", 7, 0, 4, 8, 12, 13, 10, 6);
	sa->AddSequeneAnimation("Animation2", 4, 4, 5, 6, 7);
	sa->AddAnimation("Animation3", 0, 4);
	sa->PlayAnimation("Animation3", -1, 1.0f);


	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);

	rotateAngle = 0;

	bLightEnabled = true;

	fogenabled = false;

	srand(time(NULL));

	fWindowWidth = Application::GetWindowWidth();
	fWindowHeight = Application::GetWindowHeight();

	//Inital Values

	bouncetime[0] = bouncetime[1] = bouncetime[2] = bouncetime[3] = GetTickCount64();

	camera.Init(Vector3(FPos.x - 120, FPos.y + 80, FPos.z - 120), Vector3(FPos.x, FPos.x, FPos.z), Vector3(0, 1, 0));
	cCamera.Init(Vector3(FPos.x -120, FPos.y + 80, FPos.z - 120), Vector3(FPos.x, FPos.y, FPos.z), Vector3(0, 1, 0));

	m_speed = 0.5f;
	m_objectCount = 0;
	m_gravity.Set(0, -9.8f, 0);
	m_worldWidth = 4000;
	m_worldHeight = 1500;

	cKeyboardController = CKeyboardController::GetInstance();

	InitUIs();


	Particles = ParticleSystem::GetInstance();
	Particles->SetSystemMap(m_heightMap);
	Particles->SetSystemWorldSize(Vector3(m_worldWidth, m_worldHeight, m_worldWidth));

	dAnimTimer = -1;

	return true;
}

bool SceneIntro::InitSections(double& dValue, double dMaxValue, std::string& sRenderMessage)
{
	if (dValue >= dMaxValue)
	{
		return true;
	}
	if (dValue == 0)
	{
		// Black background
		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
		// Enable depth test
		glEnable(GL_DEPTH_TEST);
		// Accept fragment if it closer to the camera than the former one
		glDepthFunc(GL_LESS);

		glEnable(GL_CULL_FACE);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glGenVertexArrays(1, &m_vertexArrayID);
		glBindVertexArray(m_vertexArrayID);

		//m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//MultiTexture.fragmentshader" );
		m_programID = LoadShaders("Shader//Shadow.vertexshader", "Shader//Shadow.fragmentshader");

		m_gPassShaderID = LoadShaders("Shader//GPass.vertexshader",
			"Shader//GPass.fragmentshader");
		m_parameters[U_LIGHT_DEPTH_MVP_GPASS] =
			glGetUniformLocation(m_gPassShaderID, "lightDepthMVP");

		sRenderMessage = "Shading Stuff";
		dValue = 15;
		return false;
	}
	else if (dValue == 15)
	{

		// Get a handle for our uniform
		m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
		m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
		m_parameters[U_MODELSTACK] = glGetUniformLocation(m_programID, "MS");
		m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
		m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
		m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
		m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
		m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
		m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
		m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
		m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
		m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
		m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
		m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
		m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
		m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
		m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
		m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
		m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
		m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
		m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

		// Get a handle for our "colorTexture" uniform
		m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled[0]");
		m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture[0]");
		m_parameters[U_COLOR_TEXTURE_ENABLED1] = glGetUniformLocation(m_programID, "colorTextureEnabled[1]");
		m_parameters[U_COLOR_TEXTURE1] = glGetUniformLocation(m_programID, "colorTexture[1]");
		m_parameters[U_COLOR_TEXTURE_ENABLED2] = glGetUniformLocation(m_programID, "colorTextureEnabled[2]");
		m_parameters[U_COLOR_TEXTURE2] = glGetUniformLocation(m_programID, "colorTexture[2]");

		// Get a handle for our "textColor" uniform
		m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
		m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

		m_parameters[U_WATER] = glGetUniformLocation(m_programID, "water");
		m_parameters[U_WATERENABLED] = glGetUniformLocation(m_programID, "waterEnabled");
		m_parameters[U_WAVENUMBER] = glGetUniformLocation(m_programID, "waveNumber");
		m_parameters[U_GREYMODE] = glGetUniformLocation(m_programID, "greymode");
		m_parameters[U_GLITCHMODE] = glGetUniformLocation(m_programID, "glitchmode");

		m_parameters[U_FOG_COLOR] = glGetUniformLocation(m_programID, "fogParam.color");
		m_parameters[U_FOG_COLOR] = glGetUniformLocation(m_programID, "fogParam.start");
		m_parameters[U_FOG_END] = glGetUniformLocation(m_programID, "fogParam.end");
		m_parameters[U_FOG_DENSITY] = glGetUniformLocation(m_programID, "fogParam.density");
		m_parameters[U_FOG_TYPE] = glGetUniformLocation(m_programID, "fogParam.type");
		m_parameters[U_FOG_ENABLED] = glGetUniformLocation(m_programID, "fogParam.enabled");

		m_parameters[U_LIGHT_DEPTH_MVP] = glGetUniformLocation(m_programID, "lightDepthMVP");
		m_parameters[U_SHADOW_MAP] = glGetUniformLocation(m_programID,
			"shadowMap");

		m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_gPassShaderID, "colorTextureEnabled[0]");
		m_parameters[U_SHADOW_COLOR_TEXTURE] = glGetUniformLocation(m_gPassShaderID, "colorTexture[0]");
		m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED1] = glGetUniformLocation(m_gPassShaderID, "colorTextureEnabled[1]");
		m_parameters[U_SHADOW_COLOR_TEXTURE1] = glGetUniformLocation(m_gPassShaderID, "colorTexture[1]");
		m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED2] = glGetUniformLocation(m_gPassShaderID, "colorTextureEnabled[2]");
		m_parameters[U_SHADOW_COLOR_TEXTURE2] = glGetUniformLocation(m_gPassShaderID, "colorTexture[2]");

		water = 0;
		waterEnabled = false;
		waterwavesnum = 3; //NUMBER OF WAVES! :D

		greymode = false;
		glitchmode = false;

		moveVal = 0.f;

		devCam = true;

		sRenderMessage = "Wearing Uniforms";
		dValue = 35;
		return false;
	}
	else if (dValue == 35)
	{


		FPos.Set(0, 0, 0);
		VPos.Set(FPos.x, FPos.y + 10, FPos.z);

		cSoundController = CSoundController::GetInstance();
		cSoundController->Init();
		cSoundController->LoadSound(FileSystem::getPath("Sounds\\Background1.wav"), 1, true, true);
		cSoundController->LoadSound(FileSystem::getPath("Sounds\\Button.wav"), 2, true, false);
		cSoundController->PlaySoundByID(1);
		cSoundController->VolumeDecrease(1);
		cSoundController->VolumeDecrease(1);

		// Use our shader
		glUseProgram(m_programID);

		fogdensity = 0.01f;
		//init fog
		Color fogColor(0.5f, 0.5f, 0.5f);
		glUniform3fv(m_parameters[U_FOG_COLOR], 1, &fogColor.r);
		glUniform1f(m_parameters[U_FOG_START], 10);
		glUniform1f(m_parameters[U_FOG_END], 1000);
		glUniform1f(m_parameters[U_FOG_DENSITY], fogdensity);
		glUniform1i(m_parameters[U_FOG_TYPE], 1);
		glUniform1i(m_parameters[U_FOG_ENABLED], 0);

		m_lightDepthFBO.Init(4096, 4096);
		m_lightWaterFBO.Init(4096, 4096);




		lights[0].type = Light::LIGHT_DIRECTIONAL;
		lights[0].position.Set(2, 10, 0);
		lights[0].color.Set(1, 1, 1);
		lights[0].power = 10.f;
		lights[0].kC = 1.f;
		lights[0].kL = 0.01f;
		lights[0].kQ = 0.001f;
		lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
		lights[0].cosInner = cos(Math::DegreeToRadian(30));
		lights[0].exponent = 3.f;
		lights[0].spotDirection.Set(0.f, 1.f, 0.f);

		glUniform1i(m_parameters[U_NUMLIGHTS], 1);
		glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
		glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
		glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
		glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
		glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
		glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
		glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
		glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
		glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);

		sRenderMessage = "GENERATING THE SUN";
		dValue = 45;
		return false;
	}

	else if (dValue == 45)
	{

		for (int i = 0; i < NUM_GEOMETRY; ++i)
		{
			meshList[i] = NULL;
		}
		meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference");
		meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateCrossHair("crosshair");
		meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
		meshList[GEO_QUAD]->textureArray[0] = LoadTGA("Image//calibri.tga");
		meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
		meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
		meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);
		
	
		meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);
		meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(1, 0, 0), 18, 36, 10.f);


		meshList[GEO_GROUND] = MeshBuilder::GenerateQuad("Ground", Color(0.6, 0.6, 0.6));
		meshList[GEO_GROUND]->textureArray[0] = LoadTexture("Image//Cement.jpg");

		sRenderMessage = "Making Fun shapes";
		dValue = 50;
		return false;

	}
	else if (dValue == 50)
	{
		//Sky Plane
		meshList[GEO_SKYPLANE] = MeshBuilder::GenerateSkyPlane("GEO_SKYPLANE", Color(1, 1, 1), 10, 200.0f, 2000.0f, 1.0f, 1.0f);
		meshList[GEO_SKYPLANE]->textureArray[0] = LoadTexture("Image//top.tga");

		meshList[GEO_TOP] = MeshBuilder::GenerateQuad("Top", Color(1, 1, 1));
		meshList[GEO_TOP]->textureArray[0] = LoadTexture("Image//Skybox//top.jpg");

		meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("Left", Color(1, 1, 1));
		meshList[GEO_LEFT]->textureArray[0] = LoadTexture("Image//Skybox//left.jpg");

		meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("Right", Color(1, 1, 1));
		meshList[GEO_RIGHT]->textureArray[0] = LoadTexture("Image//Skybox//right.jpg");

		meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("Front", Color(1, 1, 1));
		meshList[GEO_FRONT]->textureArray[0] = LoadTexture("Image//Skybox//front.jpg");

		meshList[GEO_BACK] = MeshBuilder::GenerateQuad("Back", Color(1, 1, 1));
		meshList[GEO_BACK]->textureArray[0] = LoadTexture("Image//Skybox//back.jpg");

		meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("Bot", Color(1, 1, 1));
		meshList[GEO_BOTTOM]->textureArray[0] = LoadTexture("Image//Skybox//bottom.jpg");

		meshList[GEO_FOUNTAIN] = MeshBuilder::GenerateOBJ("Fountain", "OBJ//Fountain.obj");
		meshList[GEO_FOUNTAIN]->textureArray[0] = LoadTexture("Image//Fountain1.jpg");

		meshList[GEO_VIRUS] = MeshBuilder::GenerateOBJ("Virus", "OBJ//virus.obj");
		meshList[GEO_VIRUS]->textureArray[0] = LoadTexture("Image//virus.png");

		sRenderMessage = "Making Fountain go sloopsh";
		dValue = 60;
		return false;
	}
	else if (dValue == 60)
	{

		meshList[GEO_ANIMATED_SPRITE] = MeshBuilder::GenerateSpriteAnimation("Animated", 4, 4);
		meshList[GEO_ANIMATED_SPRITE]->textureArray[0] = LoadTexture("Image//Fire.jpeg ");
		meshList[GEO_ANIMATED_SPRITE]->material.kDiffuse.Set(0.04f, 0.04f, 0.04f);
		meshList[GEO_ANIMATED_SPRITE]->material.kSpecular.Set(0.f, 0.f, 0.f);
		meshList[GEO_ANIMATED_SPRITE]->material.kShininess = 32.f;
		meshList[GEO_ANIMATED_SPRITE]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);

		meshList[GEO_LOAD_QUAD] = MeshBuilder::GenerateQuad("Load", Color(1, 1, 1), 1.f);
		meshList[GEO_LOAD_QUAD]->textureArray[0] = LoadTexture("Image//UI/loading.png");

		sa = dynamic_cast<SpriteAnimation*>(meshList[GEO_ANIMATED_SPRITE]);
		sa->AddSequeneAnimation("Animation1", 7, 0, 4, 8, 12, 13, 10, 6);
		sa->AddSequeneAnimation("Animation2", 4, 4, 5, 6, 7);
		sa->AddAnimation("Animation3", 0, 4);
		sa->PlayAnimation("Animation3", -1, 1.0f);

		sRenderMessage = "Loading this screen but somehwere else :D";
		dValue = 70;
		return false;
	}
	else if (dValue == 70)
	{


		// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
		Mtx44 perspective;
		perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
		//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
		projectionStack.LoadMatrix(perspective);

		rotateAngle = 0;

		bLightEnabled = true;

		fogenabled = false;

		srand(time(NULL));

		fWindowWidth = Application::GetWindowWidth();
		fWindowHeight = Application::GetWindowHeight();

		//Inital Values

		bouncetime[0] = bouncetime[1] = bouncetime[2] = bouncetime[3] = GetTickCount64();

		camera.Init(Vector3(FPos.x - 120, FPos.y + 80, FPos.z - 120), Vector3(FPos.x, FPos.x, FPos.z), Vector3(0, 1, 0));
		cCamera.Init(Vector3(FPos.x - 120, FPos.y + 80, FPos.z - 120), Vector3(FPos.x, FPos.y, FPos.z), Vector3(0, 1, 0));

		m_speed = 0.5f;
		m_objectCount = 0;
		m_gravity.Set(0, -9.8f, 0);
		m_worldWidth = 4000;
		m_worldHeight = 1500;

		cKeyboardController = CKeyboardController::GetInstance();
		sRenderMessage = "Dragging screens from the void";
		dValue = 80;
		return false;

	}
	else if (dValue == 80)
	{

		InitUIs();
		sRenderMessage = "Almost done!";
		dValue = 95;
		return false;
	}
	else if (dValue == 95)
	{


		Particles = ParticleSystem::GetInstance();
		Particles->SetSystemMap(m_heightMap);
		Particles->SetSystemWorldSize(Vector3(m_worldWidth, m_worldHeight, m_worldWidth));

		dAnimTimer = -1;
		sRenderMessage = "Done!";
		dValue = 100;
		return false;
	}

	return false;
}



void SceneIntro::Update(double dt)
{
	double dcurrenttime = GetTickCount64(); //Save on resources instead of callling a function everytime
	fWindowWidth = Application::GetWindowWidth();
	fWindowHeight = Application::GetWindowHeight();

	UpdateParticles(dt);

	water += 0.5f * dt;

	moveVal += dt * 10;

	//Update sprite aniamtion
	sa->Update(dt);


	//if (Application::IsKeyPressed(VK_F1)) // Linear fog 
	//	glUniform1i(m_parameters[U_FOG_TYPE], 0);
	//if (Application::IsKeyPressed(VK_F2)) // Exponential fog
	//	glUniform1i(m_parameters[U_FOG_TYPE], 1);
	//if (Application::IsKeyPressed(VK_F3)) //  fog 
	//	glUniform1i(m_parameters[U_FOG_TYPE], 2);

	//if (Application::IsKeyPressed('1'))
	//	glEnable(GL_CULL_FACE);
	//if (Application::IsKeyPressed('2'))
	//	glDisable(GL_CULL_FACE);
	//if (Application::IsKeyPressed('3'))
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//if (Application::IsKeyPressed('4'))
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//if (Application::IsKeyPressed('5'))
	//{

	//	lights[0].type = Light::LIGHT_POINT;
	//	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	//}
	//else if (Application::IsKeyPressed('6'))
	//{
	//	lights[0].type = Light::LIGHT_DIRECTIONAL;
	//	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	//}
	//else if (Application::IsKeyPressed('7'))
	//{
	//	lights[0].type = Light::LIGHT_SPOT;
	//	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	//}
	//else if (Application::IsKeyPressed('8'))
	//{
	//	bLightEnabled = true;
	//}
	//else if (Application::IsKeyPressed('9'))
	//{
	//	bLightEnabled = false;
	//}

	//if (Application::IsKeyPressed(VK_TAB) && bouncetime[0] <= dcurrenttime)
	//{
	//	bouncetime[0] = dcurrenttime + 150;

	//	if (greymode == false && glitchmode == false)
	//	{
	//		greymode = true;
	//		glUniform1i(m_parameters[U_GREYMODE], greymode);
	//		glUniform1i(m_parameters[U_GLITCHMODE], glitchmode);
	//	}
	//	else if (greymode == true && glitchmode == false)
	//	{
	//		greymode = false;
	//		glitchmode = true;
	//		glUniform1i(m_parameters[U_GREYMODE], greymode);
	//		glUniform1i(m_parameters[U_GLITCHMODE], glitchmode);
	//	}
	//	else if (greymode == false && glitchmode == true)
	//	{
	//		greymode = true;
	//		glitchmode = true;
	//		glUniform1i(m_parameters[U_GREYMODE], greymode);
	//		glUniform1i(m_parameters[U_GLITCHMODE], glitchmode);
	//	}
	//	else if (greymode == true && glitchmode == true)
	//	{
	//		greymode = false;
	//		glitchmode = false;
	//		glUniform1i(m_parameters[U_GREYMODE], greymode);
	//		glUniform1i(m_parameters[U_GLITCHMODE], glitchmode);
	//	}
	//}

	if (Application::IsKeyPressed('0') && bouncetime[1] <= dcurrenttime)
	{
		bouncetime[1] = dcurrenttime + 150;
		if (devCam == true)
			devCam = false;
		else
		{
			camera.target = cCamera.target;
			camera.position = cCamera.position;
			devCam = true;
		}
	}

	rotateAngle += (float)(10 * dt);
	if (devCam == true)
		Vector3 PositionOld = camera.position;
	else
		Vector3 PositionOld = cCamera.position;

	// Update UIs
	UpdateUIs(dt);

	//Camera Update
	if (devCam == true)
		camera.Update(dt);
	else
	{
		cCamera.position -= cCamera.right * 200.f * dt;
		cCamera.Update(dt);
	}

	fps = (float)(1.f / dt);
}

void SceneIntro::Render()
{
	//******************************* PRE RENDER PASS	*************************************
	RenderPassGPass();
	//******************************* MAIN RENDER PASS ************************************
	RenderPassMain();
}

void SceneIntro::Exit()
{
	// Cleanup VBO
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
			delete meshList[i];
	}
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);

	glDeleteProgram(m_gPassShaderID);
}

void SceneIntro::DeleteInstances()
{
	if (Particles)
	{
		Particles->Destroy();
		Particles = NULL;
	}

	if (cKeyboardController)
	{
		cKeyboardController->Destroy();
		cKeyboardController = NULL;
	}

	if (cSoundController)
	{
		cSoundController->Destroy();
		cSoundController = NULL;
	}

}

void SceneIntro::RenderNextProgressBar(double dt, double& dValue, double dMaxValue, string sRenderMessage)
{
	m_renderPass = RENDER_PASS_MAIN;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Application::GetWindowWidth(), Application::GetWindowHeight());
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_programID);
	//pass light depth texture
	m_lightDepthFBO.BindForReading(GL_TEXTURE8);
	glUniform1i(m_parameters[U_SHADOW_MAP], 8);

	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);

	// Camera matrix
	viewStack.LoadIdentity();
	if (devCam == true)
		viewStack.LookAt(
			camera.position.x, camera.position.y, camera.position.z,
			camera.target.x, camera.target.y, camera.target.z,
			camera.up.x, camera.up.y, camera.up.z
		);
	else
		viewStack.LookAt(
			cCamera.position.x, cCamera.position.y, cCamera.position.z,
			cCamera.target.x, cCamera.target.y, cCamera.target.z,
			cCamera.up.x, cCamera.up.y, cCamera.up.z
		);

	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();
	

	cLoadingBar.SetValue(dValue);
	cLoadingBar.SetMaxValue(dMaxValue);

  	RenderUIs(projectionStack,viewStack,modelStack);

	RenderTextOnScreen(meshList[GEO_TEXT], sRenderMessage, Color(1, 1, 1), 4.5, 8, 8);


}

void SceneIntro::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void SceneIntro::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation((i * 0.7f), 0.5f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SceneIntro::RenderMeshIn2D(Mesh* mesh, bool enableLight, float size, float x, float y)
{
	Mtx44 ortho;
	ortho.SetToOrtho(-80, 80, -60, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Scale(size, size, size);
	modelStack.Translate(x, y, 0);

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
}

void SceneIntro::RenderMesh(Mesh* mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;


	if (m_renderPass == RENDER_PASS_PRE)
	{
		Mtx44 lightDepthMVP = m_lightDepthProj *
			m_lightDepthView * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_LIGHT_DEPTH_MVP_GPASS], 1,
			GL_FALSE, &lightDepthMVP.a[0]);
		for (int i = 0; i < MAX_TEXTURES; ++i)
		{
			if (mesh->textureArray[i] > 0)
			{
				glUniform1i(m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED
					+ i], 1);
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, mesh->textureArray[i]);
				glUniform1i(m_parameters[U_SHADOW_COLOR_TEXTURE + i], i);
			}
			else
				glUniform1i(m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED
					+ i], 0);
		}
		mesh->Render();
		return;
	}

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);

	glUniformMatrix4fv(m_parameters[U_MODELSTACK], 1, GL_FALSE, &modelStack.Top().a[0]);
	glUniform1i(m_parameters[U_WATERENABLED], waterEnabled);
	glUniform1f(m_parameters[U_WATER], water);
	glUniform1f(m_parameters[U_WAVENUMBER], waterwavesnum);

	if (enableLight && bLightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose =
			modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);
		Mtx44 lightDepthMVP = m_lightDepthProj *
			m_lightDepthView * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_LIGHT_DEPTH_MVP], 1,
			GL_FALSE, &lightDepthMVP.a[0]);
		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	for (int i = 0; i < MAX_TEXTURES; ++i)
	{
		if (mesh->textureArray[i] > 0)
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 1);
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, mesh->textureArray[i]);
			glUniform1i(m_parameters[U_COLOR_TEXTURE + i], i);
		}
		else
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 0);
		}
	}
	mesh->Render();

	for (int i = 0; i < MAX_TEXTURES; ++i)
	{
		if (mesh->textureArray[i] > 0)
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
}

void SceneIntro::RenderGround()
{
	modelStack.PushMatrix();
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Translate(0, 0, 0);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(4000.0f, 4000.0f, 4000.0f);
	RenderMesh(meshList[GEO_GROUND], false);
	modelStack.PopMatrix();
}

void SceneIntro::RenderSkybox()
{
	modelStack.PushMatrix();

	modelStack.PushMatrix();
	///scale, translate, rotate 
	modelStack.Translate(-999.8f, 250.f, 0.f);
	modelStack.Scale(2000.f, 2000.f, 2000.f);
	modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
	RenderMesh(meshList[GEO_LEFT], false); 
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	///scale, translate, rotate 
	modelStack.Translate(999.8f, 250.f, 0.f);
	modelStack.Scale(2000.f, 2000.f, 2000.f);
	modelStack.Rotate(-90.f, 0.f, 1.f, 0.f);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	///scale, translate, rotate 
	modelStack.Translate(0.f, 250.f, -999.8f);
	modelStack.Scale(2000.f, 2000.f, 2000.f);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	///scale, translate, rotate 
	modelStack.Translate(0.f, 250.f, 999.8f);
	modelStack.Scale(2000.f, 2000.f, 2000.f);
	modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();

	///scale, translate, rotate 
	modelStack.Translate(0.f, 1199.8f, 0.f);
	modelStack.Scale(2000.f, 2000.f, 2000.f);
	modelStack.Rotate(90.f, 1.f, 0.f, 0.f);
	modelStack.PushMatrix();
	//modelStack.Rotate(90.f, 0.f, 0.f, 1.f);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PushMatrix();

	///scale, translate, rotate 
	modelStack.Translate(0.f, 0.f, 0.f);
	modelStack.Scale(2000.f, 2000.f, 2000.f);
	modelStack.Rotate(-90.f, 1.f, 0.f, 0.f);
	modelStack.PushMatrix();
	modelStack.Rotate(90.f, 0.f, 0.f, 1.f);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
}

void SceneIntro::RenderSkyPlane()
{
	modelStack.PushMatrix();
	modelStack.Translate(0, 1800, 0);

	RenderMesh(meshList[GEO_SKYPLANE], false);

	modelStack.PopMatrix();
}

void SceneIntro::RenderParticles()
{
	//vector<ParticleObject*> PartList = Particles->ReturnList();
	//for (std::vector<ParticleObject*>::iterator it = PartList.begin(); it != PartList.end(); ++it)
	//{
	//	ParticleObject* Part = (ParticleObject*)*it;
	//	if (Part->active)
	//	{
	//		if (Part->TYPE == ParticleObject::PraticleObject_TYPE::P_RAIN)
	//		{
	//			modelStack.PushMatrix();
	//			modelStack.Translate(Part->pos.x, Part->pos.y, Part->pos.z);
	//			if (devCam == true)
	//				modelStack.Rotate(Math::RadianToDegree(atan2(camera.position.x - Part->pos.x, camera.position.z - Part->pos.z)), 0, 1, 0);
	//			else
	//				modelStack.Rotate(Math::RadianToDegree(atan2(cCamera.position.x - Part->pos.x, cCamera.position.z - Part->pos.z)), 0, 1, 0);
	//			modelStack.Scale(Part->scale.x, Part->scale.y, Part->scale.z);
	//			RenderMesh(meshList[], false);
	//			modelStack.PopMatrix();
	//		}
	//	}
	//}
	//glEnable(GL_DEPTH_TEST);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void SceneIntro::RenderVirusFountain()
{
	glEnable(GL_DEPTH_TEST);
	modelStack.PushMatrix();
	modelStack.Translate(FPos.x, FPos.y, FPos.z);
	modelStack.Scale(0.2, 0.2, 0.2);
	RenderMesh(meshList[GEO_FOUNTAIN], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(VPos.x + 60, VPos.y + sin(moveVal) * 5 + 12, VPos.z);
	modelStack.Scale(0.05, 0.05, 0.05);
	RenderMesh(meshList[GEO_VIRUS], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(VPos.x - 60, VPos.y + cos(moveVal) * 5 + 12, VPos.z);
	modelStack.Scale(0.05, 0.05, 0.05);
	RenderMesh(meshList[GEO_VIRUS], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(VPos.x, VPos.y + sin(moveVal) * 5 + 12, VPos.z + 60);
	modelStack.Scale(0.05, 0.05, 0.05);
	RenderMesh(meshList[GEO_VIRUS], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(VPos.x, VPos.y + cos(moveVal) * 5 + 12, VPos.z - 60);
	modelStack.Scale(0.05, 0.05, 0.05);
	RenderMesh(meshList[GEO_VIRUS], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(VPos.x + 45, VPos.y + sin(moveVal) * 5 + 12, VPos.z + 45);
	modelStack.Scale(0.05, 0.05, 0.05);
	RenderMesh(meshList[GEO_VIRUS], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(VPos.x - 45, VPos.y + cos(moveVal) * 5 + 12, VPos.z - 45);
	modelStack.Scale(0.05, 0.05, 0.05);
	RenderMesh(meshList[GEO_VIRUS], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(VPos.x + 45, VPos.y + sin(moveVal) * 5 + 12, VPos.z - 45);
	modelStack.Scale(0.05, 0.05, 0.05);
	RenderMesh(meshList[GEO_VIRUS], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(VPos.x - 45, VPos.y + cos(moveVal) * 5 + 12, VPos.z + 45);
	modelStack.Scale(0.05, 0.05, 0.05);
	RenderMesh(meshList[GEO_VIRUS], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(VPos.x, VPos.y + 65, VPos.z);
	modelStack.Scale(0.1, 0.1, 0.1);
	RenderMesh(meshList[GEO_VIRUS], false);
	modelStack.PopMatrix();
	glDisable(GL_DEPTH_TEST);
}

void SceneIntro::UpdateParticles(double dt)
{
	dt *= m_speed;
	std::vector<ParticleObject*>inactive_PList;
	if (devCam == true)
		Particles->Update(dt, camera.position);
	else
		Particles->Update(dt, cCamera.position);
}

ParticleObject* SceneIntro::Part_P_One()
{
	return nullptr;
}

void SceneIntro::RenderParticles(ParticleObject* Part)
{
}

void SceneIntro::RenderPassGPass()
{
	m_renderPass = RENDER_PASS_PRE;
	m_lightDepthFBO.BindForWriting();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_gPassShaderID);
	//These matrices should change when light position or direction changes
	if (lights[0].type == Light::LIGHT_DIRECTIONAL)
		m_lightDepthProj.SetToOrtho(-500, 500, -500, 500, -500, 2000);
	else

		m_lightDepthProj.SetToPerspective(90, 1.f, 0.1, 200);
	m_lightDepthView.SetToLookAt(lights[0].position.x, lights[0].position.y,
		lights[0].position.z, 0, 0, 0, 0, 1, 0);
	RenderParticles();
}

void SceneIntro::RenderPassMain()
{
	m_renderPass = RENDER_PASS_MAIN;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Application::GetWindowWidth(), Application::GetWindowHeight());
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_programID);
	//pass light depth texture
	m_lightDepthFBO.BindForReading(GL_TEXTURE8);
	glUniform1i(m_parameters[U_SHADOW_MAP], 8);

	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);

	// Camera matrix
	viewStack.LoadIdentity();
	if (devCam == true)
		viewStack.LookAt(
			camera.position.x, camera.position.y, camera.position.z,
			camera.target.x, camera.target.y, camera.target.z,
			camera.up.x, camera.up.y, camera.up.z
		);
	else
		viewStack.LookAt(
			cCamera.position.x, cCamera.position.y, cCamera.position.z,
			cCamera.target.x, cCamera.target.y, cCamera.target.z,
			cCamera.up.x, cCamera.up.y, cCamera.up.z
		);

	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	if (lights[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[0].position.x, lights[0].position.y, lights[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (lights[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}

	//RenderMesh(meshList[GEO_AXES], false);

	RenderSkybox();
	//RenderSkyPlane();
	//RenderGround();
	RenderVirusFountain();

	glUniform1i(m_parameters[U_FOG_ENABLED], 0);

	////On screen text
	//float Textoffset = 6;
	//std::ostringstream ss;
	//ss.str("");

	////Display FPS
	//ss.precision(5);
	//ss << "FPS: " << fps;
	//RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 0), 3, 0, Textoffset);
	//Textoffset += 3;


	RenderUIs(projectionStack, viewStack, modelStack);

	if (fogenabled == true)
	{
		glUniform1i(m_parameters[U_FOG_ENABLED], 1);
	}
}

void SceneIntro::RenderWorld()
{
}

void SceneIntro::InitUIs()
{
	dGUIPositionXOffSet = 800;
	dGUIPositionXOffSetDestination = 0;
	if (!Application::GetIntroDisplayRendered())
	{
		Mesh* meshIntro = MeshBuilder::GenerateSpriteAnimation("meshIntro", 11, 5);
		meshIntro->textureArray[0] = LoadTexture("Image//UI/intro.png");
		SpriteAnimation* meshIntroAnimation = dynamic_cast<SpriteAnimation*>(meshIntro);
		meshIntroAnimation->AddAnimation("Default", 0, 51);
		meshIntroAnimation->PlayAnimation("Default", 0, 5.f);
		cIntroDisplay.SetBackgroundMesh(meshIntro);
		cIntroDisplay.SetBackgroundAnimationEnabled(true);
		cIntroDisplay.SetLength(800);
		cIntroDisplay.SetHeight(600);
		cIntroDisplay.SetPosition(400, 300);
		cIntroDisplay.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE]);
	}

	cLoadScreen.SetBackgroundMesh(meshList[GEO_LOAD_QUAD]);
	cLoadScreen.SetLength(800);
	cLoadScreen.SetHeight(600);
	cLoadScreen.SetPosition(400, 300);
	cLoadScreen.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE]);
	cLoadScreen.SetRenderOrder(0.1);
	cLoadScreen.SetActive(false);



	//cLoading Bar
	Mesh* cLoadingBarBackground = MeshBuilder::GenerateQuad("cLoadingBar", Color(1, 1, 1), 1.f);
	cLoadingBarBackground->textureArray[0] = LoadTexture("Image//UI/Virus_HPBarGreen.tga");
	Mesh* cLoadingBarFront = MeshBuilder::GenerateQuad("cLoadingBar", Color(1, 1, 1), 1.f);
	cLoadingBarFront->textureArray[0] = LoadTexture("Image//UI/Virus_HPGreen.tga");

	cLoadingBar.SetBackgroundMesh(cLoadingBarBackground);
	cLoadingBar.SetImageMesh(cLoadingBarFront);
	cLoadingBar.SetLength(750);
	cLoadingBar.SetHeight(100);
	cLoadingBar.SetPosition(400, 100);
	cLoadingBar.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE]);
	cLoadingBar.SetRenderOrder(0.1);
	cLoadingBar.SetActive(false);
	cLoadingBar.SetFollowMouse(false);

	Mesh* cGameTitleMesh = MeshBuilder::GenerateQuad("cGameTitleMesh", Color(1,1,1), 1.f);
	cGameTitleMesh->textureArray[0] = LoadTexture("Image//UI/gamename.png");
	cGameTitle.SetBackgroundMesh(cGameTitleMesh);
	cGameTitle.SetLength(600);
	cGameTitle.SetHeight(200);
	//cGameTitle.SetPosition(400, 500);
	cGameTitle.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE]);
	cGameTitle.SetActive(false);

	Mesh* cGroupNameMesh = MeshBuilder::GenerateQuad("cGroupNameMesh", Color(1, 1, 1), 1.f);
	cGroupNameMesh->textureArray[0] = LoadTexture("Image//UI/groupname.png");
	cGroupName.SetBackgroundMesh(cGroupNameMesh);
	cGroupName.SetLength(300);
	cGroupName.SetHeight(150);
	//cGroupName.SetPosition(400, 400);
	cGroupName.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE]);
	cGroupName.SetRenderOrder(0.01);
	cGroupName.SetActive(false);

	Mesh* buttonTemplate = MeshBuilder::GenerateQuad("buttonTemplate", Color(1, 1, 1), 1.f);
	buttonTemplate->textureArray[0] = LoadTexture("Image//UI/buttontemplate.tga");

	Mesh* frameTemplate = MeshBuilder::GenerateQuad("frameTemplate", Color(1, 1, 1), 1.f);
	frameTemplate->textureArray[0] = LoadTexture("Image//UI/frametemplate.png");

	cBackButton.SetBackgroundMesh(buttonTemplate);
	cBackButton.SetTextMesh(meshList[GEO_TEXT]);
	cBackButton.SetTextColor(Color(10, 10, 10));
	cBackButton.SetText("BACK");
	cBackButton.SetTextSize(2);
	cBackButton.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cBackButton.SetLength(100);
	cBackButton.SetHeight(50);
	cBackButton.SetPosition(740, 570);
	cBackButton.SetRenderOrder(0.1);
	cBackButton.SetCooldownTime(0.2);
	cBackButton.SetActive(false);

	// Frame 1
	cStartButton.SetBackgroundMesh(buttonTemplate);
	cStartButton.SetTextMesh(meshList[GEO_TEXT]);
	cStartButton.SetTextColor(Color(10, 10, 10));
	cStartButton.SetText("START");
	cStartButton.SetTextSize(3);
	cStartButton.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cStartButton.SetLength(250);
	cStartButton.SetHeight(75);
	//cStartButton.SetPosition(400, 300);
	cStartButton.SetRenderOrder(0.02);
	cStartButton.SetActive(false);

	cControlsButton.SetBackgroundMesh(buttonTemplate);
	cControlsButton.SetTextMesh(meshList[GEO_TEXT]);
	cControlsButton.SetTextColor(Color(10, 10, 10));
	cControlsButton.SetText("EXTRAS");
	cControlsButton.SetTextSize(3);
	cControlsButton.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cControlsButton.SetLength(250);
	cControlsButton.SetHeight(75);
	//cControlsButton.SetPosition(400, 200);
	cControlsButton.SetRenderOrder(0.02);
	cControlsButton.SetActive(false);

	cExitButton.SetBackgroundMesh(buttonTemplate);
	cExitButton.SetTextMesh(meshList[GEO_TEXT]);
	cExitButton.SetTextColor(Color(10, 10, 10));
	cExitButton.SetText("EXIT");
	cExitButton.SetTextSize(3);
	cExitButton.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cExitButton.SetLength(250);
	cExitButton.SetHeight(75);
	//cExitButton.SetPosition(400, 100);
	cExitButton.SetRenderOrder(0.02);
	cExitButton.SetCooldownTime(0.2);
	cExitButton.SetActive(false);

	// Frame 2
	cFrame2.SetBackgroundMesh(frameTemplate);
	cFrame2.SetLength(500);
	cFrame2.SetHeight(500);
	//cFrame2.SetPosition(400, 300);
	cFrame2.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE]);
	cFrame2.SetActive(false);

	cLoadFileHeader.SetBackgroundMesh(nullptr);
	cLoadFileHeader.SetTextMesh(meshList[GEO_TEXT]);
	cLoadFileHeader.SetTextColor(Color(10, 10, 10));
	cLoadFileHeader.SetText("Do you want to\nload save file?");
	cLoadFileHeader.SetTextSize(3);
	cLoadFileHeader.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cLoadFileHeader.SetLength(350);
	cLoadFileHeader.SetHeight(75);
	cLoadFileHeader.SetCenter(true);
	//cLoadFileHeader.SetPosition(400, 490);
	cLoadFileHeader.SetRenderOrder(0.01);
	cLoadFileHeader.SetActive(false);

	cLoadFileAlert.SetBackgroundMesh(nullptr);
	cLoadFileAlert.SetTextMesh(meshList[GEO_TEXT]);
	cLoadFileAlert.SetTextColor(Color(10, 0, 0));
	cLoadFileAlert.SetText("No save file\ndetected!");
	cLoadFileAlert.SetTextSize(2.5);
	cLoadFileAlert.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cLoadFileAlert.SetLength(350);
	cLoadFileAlert.SetHeight(75);
	cLoadFileAlert.SetCenter(true);
	//cLoadFileAlert.SetPosition(400, 490);
	cLoadFileAlert.SetRenderOrder(0.02);
	cLoadFileAlert.SetActive(false);

	cLoadFileYes.SetBackgroundMesh(buttonTemplate);
	cLoadFileYes.SetTextMesh(meshList[GEO_TEXT]);
	cLoadFileYes.SetTextColor(Color(10, 10, 10));
	cLoadFileYes.SetText("YES");
	cLoadFileYes.SetTextSize(5);
	cLoadFileYes.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cLoadFileYes.SetLength(200);
	cLoadFileYes.SetHeight(100);
	//cLoadFileYes.SetPosition(400, 325);
	cLoadFileYes.SetRenderOrder(0.02);
	cLoadFileYes.SetActive(false);

	cLoadFileNo.SetBackgroundMesh(buttonTemplate);
	cLoadFileNo.SetTextMesh(meshList[GEO_TEXT]);
	cLoadFileNo.SetTextColor(Color(10, 10, 10));
	cLoadFileNo.SetText("NO");
	cLoadFileNo.SetTextSize(5);
	cLoadFileNo.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cLoadFileNo.SetLength(200);
	cLoadFileNo.SetHeight(100);
	//cLoadFileNo.SetPosition(400, 250);
	cLoadFileNo.SetRenderOrder(0.02);
	cLoadFileNo.SetActive(false);


	// Frame 3
	cFrame3.SetBackgroundMesh(frameTemplate);
	cFrame3.SetLength(500);
	cFrame3.SetHeight(500);
	//cFrame3.SetPosition(400, 300);
	cFrame3.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE]);
	cFrame3.SetActive(false);

	cVirusNameHeader.SetBackgroundMesh(nullptr);
	cVirusNameHeader.SetTextMesh(meshList[GEO_TEXT]);
	cVirusNameHeader.SetTextColor(Color(10, 10, 10));
	cVirusNameHeader.SetText("Name your\nvirus!\n\nMax 12\nCharacters!");
	cVirusNameHeader.SetTextSize(3);
	cVirusNameHeader.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cVirusNameHeader.SetLength(350);
	cVirusNameHeader.SetHeight(75);
	cVirusNameHeader.SetCenter(true);
	//cVirusNameHeader.SetPosition(400, 490);
	cVirusNameHeader.SetRenderOrder(0.01);
	cVirusNameHeader.SetActive(false);

	Mesh* cVirusNameTextBoxMesh = MeshBuilder::GenerateQuad("cVirusNameTextBoxMesh", Color(1, 1, 1), 1.f);
	cVirusNameTextBoxMesh->textureArray[0] = LoadTexture("Image//UI/textbox.png");
	cVirusNameTextBox.SetBackgroundMesh(cVirusNameTextBoxMesh);
	cVirusNameTextBox.SetTextMesh(meshList[GEO_TEXT]);
	cVirusNameTextBox.SetTextColor(Color(10, 10, 10));
	cVirusNameTextBox.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cVirusNameTextBox.SetLength(350);
	cVirusNameTextBox.SetHeight(50);
	cVirusNameTextBox.SetRenderOrder(0.02);
	cVirusNameTextBox.SetLimit(12);
	cVirusNameTextBox.SetActive(false);

	cVirusNameConfirmButton.SetBackgroundMesh(buttonTemplate);
	cVirusNameConfirmButton.SetTextMesh(meshList[GEO_TEXT]);
	cVirusNameConfirmButton.SetTextColor(Color(10, 10, 10));
	cVirusNameConfirmButton.SetText("Confirm");
	cVirusNameConfirmButton.SetTextSize(3);
	cVirusNameConfirmButton.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cVirusNameConfirmButton.SetLength(250);
	cVirusNameConfirmButton.SetHeight(75);
	//cVirusNameConfirmButton.SetPosition(400, 100);
	cVirusNameConfirmButton.SetRenderOrder(0.02);
	cVirusNameConfirmButton.SetActive(false);


	// Frame 4
	Mesh* cFrame4Mesh = MeshBuilder::GenerateQuad("cFrame4Mesh", Color(1, 1, 1), 1.f);
	cFrame4Mesh->textureArray[0] = LoadTexture("Image//UI/introductionframe.png");
	cFrame4.SetBackgroundMesh(cFrame4Mesh);
	cFrame4.SetLength(800);
	cFrame4.SetHeight(600);
	cFrame4.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE]);
	cFrame4.SetActive(false);

	cFrame4Title.SetBackgroundMesh(nullptr);
	cFrame4Title.SetTextMesh(meshList[GEO_TEXT]);
	cFrame4Title.SetTextColor(Color(10, 0, 0));
	cFrame4Title.SetText("Welcome to\nPatient Zero!");
	cFrame4Title.SetTextSize(5);
	cFrame4Title.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cFrame4Title.SetLength(350);
	cFrame4Title.SetHeight(75);
	cFrame4Title.SetCenter(true);
	//cFrame4Title.SetPosition(400, 490);
	cFrame4Title.SetRenderOrder(0.01);
	cFrame4Title.SetActive(false);

	Mesh* cFrame4TextMesh = MeshBuilder::GenerateQuad("e", Color(1, 1, 1), 1.f);
	cFrame4TextMesh->textureArray[0] = LoadTexture("Image//UI/storyline.png");
	cFrame4Text.SetBackgroundMesh(cFrame4TextMesh);
	cFrame4Text.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE]);
	cFrame4Text.SetLength(750);
	cFrame4Text.SetHeight(400);
	//cFrame4Text.SetPosition(400, 490);
	cFrame4Text.SetRenderOrder(0.01);
	cFrame4Text.SetActive(false);

	cFrame4Button.SetBackgroundMesh(buttonTemplate);
	cFrame4Button.SetTextMesh(meshList[GEO_TEXT]);
	cFrame4Button.SetTextColor(Color(10, 10, 10));
	cFrame4Button.SetText("OKAY!");
	cFrame4Button.SetTextSize(3);
	cFrame4Button.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cFrame4Button.SetLength(175);
	cFrame4Button.SetHeight(75);
	//cFrame4Button.SetPosition(400, 100);
	cFrame4Button.SetRenderOrder(0.02);
	cFrame4Button.SetActive(false);


	// Controls Frame
	Mesh* cControlFrameMesh = MeshBuilder::GenerateQuad("cControlFrame", Color(1, 1, 1), 1.f);
	cControlFrameMesh->textureArray[0] = LoadTexture("Image//UI/ControlsMenu.tga");

	cControlFrame.SetBackgroundMesh(cControlFrameMesh);
	cControlFrame.SetPosition(400, 300);
	cControlFrame.SetLength(800);
	cControlFrame.SetHeight(600);
	cControlFrame.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE]);
	cControlFrame.SetActive(false);

	cControlText.SetBackgroundMesh(nullptr);
	cControlText.SetTextMesh(meshList[GEO_TEXT]);
	cControlText.SetTextColor(Color(10, 0, 0));
	//cControlText.SetText("Player Movement:\n - W: Move Forward\n - A: Rotate Anticlockwise\n - S: Move Backwards\n - D: Rotate Clockwise\n\nToggle Weapons:\n - 1:Equip Cough\n - 2:Equip Virus Launcher\n - 3:Equip Contaminated Air\n     Freshener\n\nSpace:Use Weapon\n\nArrow Keys:Camera Movement");
	cControlText.SetTextSize(2.5);
	cControlText.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cControlText.SetLength(750);
	cControlText.SetHeight(450);
	cControlText.SetPosition(400, 350);
	cControlText.SetRenderOrder(0.01);
	cControlText.SetActive(false);

	cControlCloseButton.SetBackgroundMesh(buttonTemplate);
	cControlCloseButton.SetTextMesh(meshList[GEO_TEXT]);
	cControlCloseButton.SetTextColor(Color(10, 10, 10));
	cControlCloseButton.SetText("BACK");
	cControlCloseButton.SetTextSize(3);
	cControlCloseButton.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cControlCloseButton.SetLength(250);
	cControlCloseButton.SetHeight(75);
	cControlCloseButton.SetPosition(610, 75);
	cControlCloseButton.SetRenderOrder(0.03);
	cControlCloseButton.SetActive(false);

}

void SceneIntro::UpdateUIs(double dt)
{
	// UI Logic
	if (dGUIPositionXOffSet != dGUIPositionXOffSetDestination && Application::GetIntroDisplayRendered())
	{
		dGUIPositionXOffSet += (dGUIPositionXOffSetDestination - dGUIPositionXOffSet) * dt * 4;
	}


	/* Start Of Intro Display */
	if (!Application::GetIntroDisplayRendered())
	{
		if (dAnimTimer >= 0)
		{
			dAnimTimer += dt;
			// Update UIs
			cIntroDisplay.Update(dt);
		}
		

		if (Application::IsKeyPressed(VK_RETURN) || dAnimTimer > 8) // Animation Duration + 3 seconds = 8
		{
			dAnimTimer = 8.1;
			Application::SetIntroDisplayRendered(true);
		}
	}
	else
	{
		if (dAnimTimer > 8 || dAnimTimer == -1)
		{
			dAnimTimer = -2;
			devCam = false;

			// Frame 1 Activate
			cGameTitle.SetActive(true);
			cGroupName.SetActive(true);
			cStartButton.SetActive(true);
			cControlsButton.SetActive(true);
			cExitButton.SetActive(true);

			// Frame 2 Activate
			cFrame2.SetActive(true);
			cLoadFileHeader.SetActive(true);
			cLoadFileYes.SetActive(true);
			cLoadFileNo.SetActive(true);

			// Frame 3 Activate
			cFrame3.SetActive(true);
			cVirusNameHeader.SetActive(true);
			cVirusNameTextBox.SetActive(true);
			cVirusNameConfirmButton.SetActive(true);

			// Frame 4 Activate
			cFrame4.SetActive(true);
			cFrame4Title.SetActive(true);
			cFrame4Text.SetActive(true);
			cFrame4Button.SetActive(true);
		}
		
	}
	/* End Of Intro Display */


	if (cBackButton.isActive())
	{
		cBackButton.Update(dt);
		if (cBackButton.IsMouseHover())
		{
			cBackButton.SetEnlarge(true);
			if (cBackButton.isLeftMouseButtonPressed() && cBackButton.isCooldownOver())
			{
				cBackButton.ResetTimer();
				dGUIPositionXOffSetDestination += 800;
				if (dGUIPositionXOffSetDestination >= -1) // in case it ranges from 0 to -1
				{
					cBackButton.SetActive(false);
				}
				if (bouncetime[3] <= GetTickCount64())
				{
					cSoundController->PlaySoundByID(2);
					bouncetime[3] = GetTickCount64() + 100;
				}

			}
		}
		else
		{
			cBackButton.SetEnlarge(false);
		}
	}

	// Start of Frame 1
	if (cGameTitle.isActive())
	{
		cGameTitle.Update(dt);
		cGameTitle.SetPosition(400 + dGUIPositionXOffSet, 500);
	}
	if (cGroupName.isActive())
	{
		cGroupName.Update(dt);
		cGroupName.SetPosition(400 + dGUIPositionXOffSet, 400);
	}
	if (cStartButton.isActive())
	{
		cStartButton.Update(dt);
		cStartButton.SetPosition(400 + dGUIPositionXOffSet, 275);
		if (cStartButton.IsMouseHover())
		{
			cStartButton.SetEnlarge(true);
			if (cStartButton.isLeftMouseButtonPressed())
			{
				dGUIPositionXOffSetDestination = -800;
				cBackButton.SetActive(true);
				if (bouncetime[3] <= GetTickCount64())
				{
					cSoundController->PlaySoundByID(2);
					bouncetime[3] = GetTickCount64() + 100;
				}
			}
		}
		else
		{
			cStartButton.SetEnlarge(false);
		}
	}
	if (cControlsButton.isActive())
	{
		cControlsButton.Update(dt);
		cControlsButton.SetPosition(400 + dGUIPositionXOffSet, 175);
		if (cControlsButton.IsMouseHover())
		{
			cControlsButton.SetEnlarge(true);
			if (cControlsButton.isLeftMouseButtonPressed())
			{
				if (bouncetime[3] <= GetTickCount64())
				{
					cSoundController->PlaySoundByID(2);
					bouncetime[3] = GetTickCount64() + 100;
				}


				// Frame 1 Deactivate
				cGameTitle.SetActive(false);
				cGroupName.SetActive(false);
				cStartButton.SetActive(false);
				cControlsButton.SetActive(false);
				cExitButton.SetActive(false);

				// Frame 2 Deactivate
				cFrame2.SetActive(false);
				cLoadFileHeader.SetActive(false);
				cLoadFileYes.SetActive(false);
				cLoadFileNo.SetActive(false);

				// Frame 3 Deactivate
				cFrame3.SetActive(false);
				cVirusNameHeader.SetActive(false);
				cVirusNameTextBox.SetActive(false);
				cVirusNameConfirmButton.SetActive(false);

				// Frame 4 Deactivate
				cFrame4.SetActive(false);
				cFrame4Title.SetActive(false);
				cFrame4Text.SetActive(false);
				cFrame4Button.SetActive(false);

				cBackButton.SetActive(false);

				// Activate Controls Frame
				cControlFrame.SetActive(true);
				cControlText.SetActive(true);
				cControlCloseButton.SetActive(true);
			}
		}
		else
		{
			cControlsButton.SetEnlarge(false);
		}
	}
	if (cExitButton.isActive())
	{
		cExitButton.Update(dt);
		cExitButton.SetPosition(400 + dGUIPositionXOffSet, 75);
		if (cExitButton.IsMouseHover())
		{
			if (cExitButton.isLeftMouseButtonPressed() && cExitButton.isCooldownOver())
			{
				if (bouncetime[3] <= GetTickCount64())
				{
					cSoundController->PlaySoundByID(2);
					bouncetime[3] = GetTickCount64() + 100;
				}
				Application::SetSceneType(Application::SCENETYPE::EXIT);
			}
			cExitButton.SetEnlarge(true);
		}
		else
		{
			cExitButton.SetEnlarge(false);
		}
	}
	// End of Frame 1

	// Start of Frame 2
	if (cFrame2.isActive())
	{
		cFrame2.Update(dt);
		cFrame2.SetPosition(1200 + dGUIPositionXOffSet, 300);
	}
	if (cLoadFileHeader.isActive())
	{
		cLoadFileHeader.Update(dt);
		cLoadFileHeader.SetPosition(1200 + dGUIPositionXOffSet, 490);
	}
	if (cLoadFileAlert.isActive())
	{
		cLoadFileAlert.Update(dt);
		cLoadFileAlert.SetPosition(1200 + dGUIPositionXOffSet, 400);
		if (cLoadFileAlert.isCooldownOver())
		{
			cLoadFileAlert.SetActive(false);
		}
	}
	if (cLoadFileYes.isActive())
	{
		cLoadFileYes.Update(dt);
		cLoadFileYes.SetPosition(1200 + dGUIPositionXOffSet, 300);
		if (cLoadFileYes.IsMouseHover())
		{
			cLoadFileYes.SetEnlarge(true);
			if (cLoadFileYes.isLeftMouseButtonPressed())
			{
				if (CheckSaveFiles()) // Check whether save file exists
				{

					Application::sPassValue.load = true;
					SwapToGameScene();

				}
				else
				{	
					// No Save File Detected
					cLoadFileAlert.SetActive(true);
					cLoadFileAlert.SetCooldownTime(5);
					cLoadFileAlert.ResetTimer();
				}
				if (bouncetime[3] <= GetTickCount64())
				{
					cSoundController->PlaySoundByID(2);
					bouncetime[3] = GetTickCount64() + 100;
				}
			}
		}
		else
		{
			cLoadFileYes.SetEnlarge(false);
		}
	}
	if (cLoadFileNo.isActive())
	{
		cLoadFileNo.Update(dt);
		cLoadFileNo.SetPosition(1200 + dGUIPositionXOffSet, 150);
		if (cLoadFileNo.IsMouseHover())
		{
			cLoadFileNo.SetEnlarge(true);
			if (cLoadFileNo.isLeftMouseButtonPressed())
			{
				dGUIPositionXOffSetDestination = -1600;
				Application::sPassValue.load = false;
				if (bouncetime[3] <= GetTickCount64())
				{
					cSoundController->PlaySoundByID(2);
					bouncetime[3] = GetTickCount64() + 100;
				}

			}
		}
		else
		{
			cLoadFileNo.SetEnlarge(false);
		}
	}
	// End of Frame 2

	// Start of Frame 3
	if (cFrame3.isActive())
	{
		cFrame3.Update(dt);
		cFrame3.SetPosition(2000 + dGUIPositionXOffSet, 300);
	}
	if (cVirusNameHeader.isActive())
	{
		cVirusNameHeader.Update(dt);
		cVirusNameHeader.SetPosition(2000 + dGUIPositionXOffSet, 490);
	}
	if (cVirusNameTextBox.isActive())
	{
		cVirusNameTextBox.Update(dt);

		cVirusNameTextBox.SetPosition(2000 + dGUIPositionXOffSet, 300);
		if (cVirusNameTextBox.isEnabled())
		{
			cVirusNameTextBox.SetEnlarge(true);
		}
		else
		{
			cVirusNameTextBox.SetEnlarge(false);
		}
	}
	if (cVirusNameConfirmButton.isActive())
	{
		cVirusNameConfirmButton.Update(dt);
		cVirusNameConfirmButton.SetPosition(2000 + dGUIPositionXOffSet, 175);
		if (cVirusNameConfirmButton.IsMouseHover())
		{
			cVirusNameConfirmButton.SetEnlarge(true);
			if (cVirusNameConfirmButton.isLeftMouseButtonPressed())
			{
				dGUIPositionXOffSetDestination = -2400;
				string virusname = cVirusNameTextBox.GetText(); // Send this text to somewhere
				if (virusname == "")
				{
					// Since virus is nameless, name it as nameless haha
					virusname == "Nameless";
				}
				Application::sPassValue.virusName = virusname;
				if (bouncetime[3] <= GetTickCount64())
				{
					cSoundController->PlaySoundByID(2);
					bouncetime[3] = GetTickCount64() + 100;
				}

				
				
			}
		}
		else
		{
			cVirusNameConfirmButton.SetEnlarge(false);
		}
	}

	// Frame 4
	if (cFrame4.isActive())
	{
		cFrame4.Update(dt);
		cFrame4.SetPosition(2800 + dGUIPositionXOffSet, 300);
	}
	if (cFrame4Title.isActive())
	{
		cFrame4Title.Update(dt);
		cFrame4Title.SetPosition(2800 + dGUIPositionXOffSet, 550);
	}
	if (cFrame4Text.isActive())
	{
		cFrame4Text.Update(dt);
		cFrame4Text.SetPosition(2800 + dGUIPositionXOffSet, 250);
	}
	if (cFrame4Button.isActive())
	{
		cFrame4Button.Update(dt);
		cFrame4Button.SetPosition(2800 + dGUIPositionXOffSet + 275, 65);
		if (cFrame4Button.IsMouseHover())
		{
			cFrame4Button.SetEnlarge(true);
			if (cFrame4Button.isLeftMouseButtonPressed())
			{
				if (bouncetime[3] <= GetTickCount64())
				{
					cSoundController->PlaySoundByID(2);
					bouncetime[3] = GetTickCount64() + 100;
				}

				SwapToGameScene();
			}
		}
		else
		{
			cFrame4Button.SetEnlarge(false);
		}
	}


	// Controls Frame
	if (cControlFrame.isActive())
	{
		cControlFrame.Update(dt);
	}
	if (cControlText.isActive())
	{
		cControlText.Update(dt);
	}
	if (cControlCloseButton.isActive())
	{
		if (cControlCloseButton.IsMouseHover())
		{
			cControlCloseButton.SetEnlarge(true);
			if (cControlCloseButton.isLeftMouseButtonPressed())
			{
				if (bouncetime[3] <= GetTickCount64())
				{
					cSoundController->PlaySoundByID(2);
					bouncetime[3] = GetTickCount64() + 100;
				}

				cExitButton.ResetTimer();

				// Frame 1 Activate
				cGameTitle.SetActive(true);
				cGroupName.SetActive(true);
				cStartButton.SetActive(true);
				cControlsButton.SetActive(true);
				cExitButton.SetActive(true);

				// Frame 2 Activate
				cFrame2.SetActive(true);
				cLoadFileHeader.SetActive(true);
				cLoadFileYes.SetActive(true);
				cLoadFileNo.SetActive(true);

				// Frame 3 Activate
				cFrame3.SetActive(true);
				cVirusNameHeader.SetActive(true);
				cVirusNameTextBox.SetActive(true);
				cVirusNameConfirmButton.SetActive(true);

				// Frame 4 Activate
				cFrame4.SetActive(true);
				cFrame4Title.SetActive(true);
				cFrame4Text.SetActive(true);
				cFrame4Button.SetActive(true);

				// Deactivate Controls Frame
				cControlFrame.SetActive(false);
				cControlText.SetActive(false);
				cControlCloseButton.SetActive(false);
			}
		}
		else
		{
			cControlCloseButton.SetEnlarge(false);
		}
	}
}

void SceneIntro::RenderUIs(MS& projectionStack, MS& viewStack, MS& modelStack)
{
	if (!Application::GetIntroDisplayRendered())
	{
		cIntroDisplay.Render(projectionStack, viewStack, modelStack);
		if (dAnimTimer < 0) // Ensures that the timer starts only when the scene starts to render
			dAnimTimer = 0;
	}



	// Frame 1
	cGameTitle.Render(projectionStack, viewStack, modelStack);
	cGroupName.Render(projectionStack, viewStack, modelStack);
	cStartButton.Render(projectionStack, viewStack, modelStack);
	cControlsButton.Render(projectionStack, viewStack, modelStack);
	cExitButton.Render(projectionStack, viewStack, modelStack);

	// Frame 2
	cFrame2.Render(projectionStack, viewStack, modelStack);
	cLoadFileHeader.Render(projectionStack, viewStack, modelStack);
	cLoadFileAlert.Render(projectionStack, viewStack, modelStack);
	cLoadFileYes.Render(projectionStack, viewStack, modelStack);
	cLoadFileNo.Render(projectionStack, viewStack, modelStack);

	// Frame 3
	cFrame3.Render(projectionStack, viewStack, modelStack);
	cVirusNameHeader.Render(projectionStack, viewStack, modelStack);
	cVirusNameTextBox.Render(projectionStack, viewStack, modelStack);
	cVirusNameConfirmButton.Render(projectionStack, viewStack, modelStack);

	// Frame 4
	cFrame4.Render(projectionStack, viewStack, modelStack);
	cFrame4Title.Render(projectionStack, viewStack, modelStack);
	cFrame4Text.Render(projectionStack, viewStack, modelStack);
	cFrame4Button.Render(projectionStack, viewStack, modelStack);

	cBackButton.Render(projectionStack, viewStack, modelStack);

	cControlFrame.Render(projectionStack, viewStack, modelStack);
	cControlCloseButton.Render(projectionStack, viewStack, modelStack);
	cControlText.Render(projectionStack, viewStack, modelStack);

	//Loading Screen
	cLoadScreen.Render(projectionStack, viewStack, modelStack);
	cLoadingBar.Render(projectionStack, viewStack, modelStack);
}

void SceneIntro::SwapToGameScene()
{
	// Render Load Screen Hehe
	cLoadScreen.SetActive(true);
	cLoadingBar.SetActive(true);


	Application::SetSceneType(Application::SCENETYPE::GAME); // Send command to Application to swap scene
}

bool SceneIntro::CheckSaveFiles()
{
	ifstream infile1("Save_Entities.txt");
	if (infile1.good())
		infile1.close();
	else
		return false;

	ifstream infile2("Save_Values.txt");
	if (infile2.good())
		infile2.close();
	else
		return false;

	return true;
}
