#include "SceneBasic.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "LoadTexture.h"
#include "SoundController.h"
#include "Entity.h"
#include "Cough.h"
#include "VirusLauncher.h"
#include "AirFreshener.h"
#include "filesystem.h"
#include <sstream>
#include <vector>

using namespace std;

bool SceneBasic::CompareDist(const Vector3& Obj1, const Vector3& Obj2)
{
	Vector3 vCameraPos;
	if (devCam == true)
		vCameraPos = camera.position;
	else
		vCameraPos = cCamera.position;
		
	return sqrt(pow(Obj1.x - vCameraPos.x, 2) + pow(Obj1.y - vCameraPos.y, 2) + pow(Obj1.z - vCameraPos.z, 2)) < sqrt(pow(Obj2.x - vCameraPos.x, 2) + pow(Obj2.y - vCameraPos.y, 2) + pow(Obj2.z - vCameraPos.z, 2));

}

SceneBasic::SceneBasic()
	: cKeyboardController(NULL)
	, cSoundController(NULL)
	, cEntityManager(NULL)
	, cVirus (NULL)
	, sa(NULL)
	, cParticles(NULL)
	, cPlayer(NULL)
	, cTimeControl(NULL)

{
}

SceneBasic::~SceneBasic()
{
}

bool SceneBasic::Init()
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
	m_programID = LoadShaders("Shader//Shadow.vertexshader", "Shader//Shadow.fragmentshader" );

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
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");

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
		
	m_parameters[U_GREYMODE] = glGetUniformLocation(m_programID, "bgreymode");
	m_parameters[U_GLITCHMODE] = glGetUniformLocation(m_programID, "binversemode");

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

	eFilter = NORMAL;

	bgreymode = false;
	binversemode = false;

	//glUniform1i(m_parameters[U_GREYMODE], bgreymode);
	//glUniform1i(m_parameters[U_GLITCHMODE], binversemode);

	devCam = false;

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
	m_lightWaterFBO.Init(1024, 1024);

	
	lights[0].type = Light::LIGHT_DIRECTIONAL;
	lights[0].position.Set(2, -100, 100);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 10.f;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);

	lights[1].type = Light::LIGHT_POINT;
	lights[1].position.Set(-300, 1, -300);
	lights[1].color.Set(0.956, 0.5647, 0.2196);
	lights[1].power = 0.f;
	lights[1].kC = 1.f;
	lights[1].kL = 0.01f;
	lights[1].kQ = 0.001f;
	lights[1].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[1].cosInner = cos(Math::DegreeToRadian(30));
	lights[1].exponent = 3.f;
	lights[1].spotDirection.Set(0.f, 1.f, 0.f);
	
	glUniform1i(m_parameters[U_NUMLIGHTS], 2);
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
	
	glUniform1i(m_parameters[U_LIGHT1_TYPE], lights[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &lights[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], lights[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], lights[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], lights[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], lights[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], lights[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], lights[1].exponent);

	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference");
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(1, 0, 0), 1.f);
	meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateCrossHair("crosshair");
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 0, 0), 1.f);
	//meshList[GEO_QUAD]->textureArray[0] = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);
	
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);
	


	// Load the ground mesh and texture
	meshList[GEO_GRASS_DARKGREEN] = MeshBuilder::GenerateQuad("GRASS_DARKGREEN", Color(1, 1, 1), 1.f);
	meshList[GEO_GRASS_DARKGREEN]->textureArray[0] = LoadTexture("Image//grass_darkgreen.tga");
	meshList[GEO_GRASS_LIGHTGREEN] = MeshBuilder::GenerateQuad("GEO_GRASS_LIGHTGREEN", Color(1, 1, 1), 1.f);
	meshList[GEO_GRASS_LIGHTGREEN]->textureArray[0] = LoadTexture("Image//grass_lightgreen.tga");

	//Sky Plane
	meshList[GEO_SKYPLANE] = MeshBuilder::GenerateSkyPlane("GEO_SKYPLANE", Color(1, 1, 1), 10, 200.0f, 2000.0f, 1.0f, 1.0f);
	meshList[GEO_SKYPLANE]->textureArray[0] = LoadTexture("Image//sky.tga");	
	
	meshList[GEO_TERRAIN] = MeshBuilder::GenerateTerrain("GEO_TERRAIN", "Image//heightmap1.raw", m_heightMap);
	meshList[GEO_TERRAIN]->textureArray[0] = LoadTGA("Image//sand.tga");

	meshList[GEO_TERRAIN]->material.kDiffuse.Set(0.04f, 0.04f, 0.04f);
	meshList[GEO_TERRAIN]->material.kSpecular.Set(0.f, 0.f, 0.f);
	meshList[GEO_TERRAIN]->material.kShininess = 32.f;
	meshList[GEO_TERRAIN]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);

	//Objects
	
	

	meshList[GEO_SUN] = MeshBuilder::GenerateQuad("sun", Color(1,1,1), 1.f);
	meshList[GEO_SUN]->textureArray[0] = LoadTexture("Image//sun.png");
	/*meshList[GEO_SUN]->material.kDiffuse.Set(0.04f, 0.04f, 0.04f);
	meshList[GEO_SUN]->material.kSpecular.Set(0.f, 0.f, 0.f);
	meshList[GEO_SUN]->material.kShininess = 32.f;
	meshList[GEO_SUN]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);*/

	//meshList[GEO_SUN]->textureArray[1] = LoadTGA("Image//SunRed.tga");

	meshList[GEO_ROAD] = MeshBuilder::GenerateOBJ("Road", "OBJ//Road.obj");
	meshList[GEO_ROAD]->textureArray[0] = LoadTexture("Image//Road.jpg");
	meshList[GEO_ROAD]->material.kDiffuse.Set(0.04f, 0.04f, 0.04f);
	meshList[GEO_ROAD]->material.kSpecular.Set(0.f, 0.f, 0.f);
	meshList[GEO_ROAD]->material.kShininess = 32.f;
	meshList[GEO_ROAD]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);


	meshList[GEO_SCENE] = MeshBuilder::GenerateQuad("Scene", Color(1, 1, 1));
	meshList[GEO_SCENE]->textureArray[0] = LoadTGA("Image//Scene.tga");


	meshList[GEO_POND] = MeshBuilder::GenerateSlicedQuad("GEO_POND", Color(1, 1, 1), 20.f, 1.f, 1.f, 500, 500);
	meshList[GEO_POND]->textureArray[0] = m_lightWaterFBO.GetTexture();
	meshList[GEO_POND]->textureArray[1] = LoadTGA("Image//Water.tga");
	meshList[GEO_POND]->textureArray[2] = LoadTGA("Image//Pond.tga");
	meshList[GEO_POND]->textureArray[3] = LoadTGA("Image//Blue.tga");
	meshList[GEO_POND]->material.kDiffuse.Set(0.04f, 0.04f, 0.04f);
	meshList[GEO_POND]->material.kSpecular.Set(0.f, 0.f, 0.f);
	meshList[GEO_POND]->material.kShininess = 32.f;
	meshList[GEO_POND]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);


	


	meshList[GEO_LOGCABIN] = MeshBuilder::GenerateOBJ("GEO_LOGCABIN", "OBJ//LogCabin.obj");
	meshList[GEO_LOGCABIN]->textureArray[0] = LoadTGA("Image//LogCabin.tga");
	meshList[GEO_LOGCABIN]->material.kDiffuse.Set(0.04f, 0.04f, 0.04f);
	meshList[GEO_LOGCABIN]->material.kSpecular.Set(0.f, 0.f, 0.f);
	meshList[GEO_LOGCABIN]->material.kShininess = 32.f;
	meshList[GEO_LOGCABIN]->material.kAmbient.Set(0.15f, 0.15f, 0.15f);

	meshList[GEO_PLAYER] = MeshBuilder::GenerateOBJ("Player", "OBJ//Zombie.obj");
	meshList[GEO_PLAYER]->textureArray[0] = LoadTGA("Image//Zombie.tga");

	//meshList[GEO_NPC] = MeshBuilder::GenerateOBJ("NPC", "OBJ//NPC.obj");
	//meshList[GEO_NPC]->textureArray[0] = LoadTGA("Image//NPC.tga");

	meshList[GEO_ANIMATED_SPRITE] = MeshBuilder::GenerateSpriteAnimation("Animated", 4, 4);
	meshList[GEO_ANIMATED_SPRITE]->textureArray[0] = LoadTexture("Image//Fire.jpeg ");
	meshList[GEO_ANIMATED_SPRITE]->material.kDiffuse.Set(0.04f, 0.04f, 0.04f);
	meshList[GEO_ANIMATED_SPRITE]->material.kSpecular.Set(0.f, 0.f, 0.f);
	meshList[GEO_ANIMATED_SPRITE]->material.kShininess = 32.f;
	meshList[GEO_ANIMATED_SPRITE]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);

	
	sa = dynamic_cast<SpriteAnimation*>(meshList[GEO_ANIMATED_SPRITE]);
	sa->AddSequeneAnimation("Animation1", 7, 0, 4, 8, 12, 13, 10, 6);
	sa->AddSequeneAnimation("Animation2", 4, 4, 5, 6, 7);
	sa->AddAnimation("Animation3", 0, 4);
	sa->PlayAnimation("Animation3", -1, 1.0f);	
	

	meshList[GEO_QUAD_PART] = MeshBuilder::GenerateQuad("GEO_QUAD_PART", Color(1, 1, 1), 1.f);
	meshList[GEO_QUAD_PART]->textureArray[0] = LoadTGA("Image//particle.tga");

	meshList[GEO_LIGHT_DEPTH_QUAD] = MeshBuilder::GenerateQuad("LIGHT_DEPTH_TEXTURE", Color(1, 1, 1), 1.f);
	meshList[GEO_LIGHT_DEPTH_QUAD] -> textureArray[0] = m_lightWaterFBO.GetTexture();


	meshList[GEO_VIRUS_BUTTON] = MeshBuilder::GenerateQuad("GEO_VIRUS_MENU", Color(1, 1, 1), 1.f);
	meshList[GEO_VIRUS_BUTTON]->textureArray[0] = LoadTGA("Image//Virus_ButtonTemplate.tga");

	meshList[GEO_HPBAR_GREEN] = MeshBuilder::GenerateQuad("GEO_HPBAR_GREEN", Color(1, 1, 1), 1.f);
	meshList[GEO_HPBAR_GREEN]->textureArray[0] = LoadTGA("Image//UI//Virus_HPBarGreen.tga");

	meshList[GEO_HPBAR_YELLOW] = MeshBuilder::GenerateQuad("GEO_HPBAR_YELLOW", Color(1, 1, 1), 1.f);
	meshList[GEO_HPBAR_YELLOW]->textureArray[0] = LoadTGA("Image//UI//Virus_HPBarYellow.tga");

	meshList[GEO_HP_GREEN] = MeshBuilder::GenerateQuad("GEO_HP_GREEN", Color(1,1,1), 1.f);
	meshList[GEO_HP_GREEN]->textureArray[0] = LoadTGA("Image//UI//Virus_HPGreen.tga");
	
	meshList[GEO_HP_YELLOW] = MeshBuilder::GenerateQuad("GEO_HP_GREEN", Color(1, 1, 1), 1.f);
	meshList[GEO_HP_YELLOW]->textureArray[0] = LoadTGA("Image//UI//Virus_HPYellow.tga");
	
	meshList[GEO_HP_RED] = MeshBuilder::GenerateQuad("GEO_HP_RED", Color(1, 1, 1), 1.f);
	meshList[GEO_HP_RED]->textureArray[0] = LoadTGA("Image//UI//Virus_HPRed.tga");

	meshList[GEO_VIRUS_PARTICLE] = MeshBuilder::GenerateSpriteAnimation("Virus_Particle", 12, 5);
	meshList[GEO_VIRUS_PARTICLE]->textureArray[0] = LoadTexture("Image//UI//virus_spritesheet.png ");
	cVirusParticle = dynamic_cast<SpriteAnimation*>(meshList[GEO_VIRUS_PARTICLE]);
	cVirusParticle->AddAnimation("Animation3", 0, 56);
	cVirusParticle->PlayAnimation("Animation3", -1, 1.5f);


	meshList[GEO_LOAD_QUAD] = MeshBuilder::GenerateQuad("Load", Color(1, 1, 1), 1.f);
	meshList[GEO_LOAD_QUAD]->textureArray[0] = LoadTexture("Image//UI/loading.png");

	meshList[GEO_VIRUS_LAUNCHER] = MeshBuilder::GenerateOBJ("VirusLauncher", "OBJ//Launcher.obj");
	meshList[GEO_VIRUS_LAUNCHER]->textureArray[0] = LoadTGA("Image//Launcher.tga");
	meshList[GEO_VIRUS_LAUNCHER]->material.kDiffuse.Set(0.04f, 0.04f, 0.04f);
	meshList[GEO_VIRUS_LAUNCHER]->material.kSpecular.Set(0.f, 0.f, 0.f);
	meshList[GEO_VIRUS_LAUNCHER]->material.kShininess = 32.f;
	meshList[GEO_VIRUS_LAUNCHER]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);
	
	rotateAngle = 0;

	bLightEnabled = true;

	fogenabled = false;

	lights[1].position.Set(-300, 30 + 350.f * ReadHeightMap(m_heightMap, -300 / 4000, -300 / 4000), -300);

	std::srand(time(NULL));
	
	fWindowWidth = Application::GetWindowWidth();
	fWindowHeight = Application::GetWindowHeight();


	//Inital Values

	bouncetime[0] = bouncetime[1] = bouncetime[2] = bouncetime[3]= GetTickCount64();
	
	cPlayer = CPlayer::GetInstance();
	cPlayer->Init(Vector3(250, 0, 0), Vector3(0, 0, 1), m_heightMap);

	float Ix = cPlayer->GetPosition().x - cPlayer->GetDirection().x * 50;
	float Iz = cPlayer->GetPosition().z - cPlayer->GetDirection().z * 50;
	Vector3 cameraIPos(Ix, 350.f * ReadHeightMap(m_heightMap, Ix / m_worldWidth, Iz / m_worldWidth) + 50, Iz);

	camera.Init(cameraIPos, cameraIPos + cPlayer->GetDirection(), Vector3(0, 1, 0));
	cCamera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));

	//Week 11

	//Init Sounds
	cSoundController = CSoundController::GetInstance();
	cSoundController->Init();
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\Background.wav"), 1, true, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\Cough.wav"), 2, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\VirusLauncher.wav"), 3, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\AirFreshener.wav"), 4, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\Button.wav"), 5, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\Cheat.wav"), 6, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\Win.wav"), 7, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\Loss.wav"), 8, true);


	/*cSoundController->PlaySoundByID(1);
	cSoundController->VolumeDecrease(1);*/
	//Inital Values

	m_speed = 0.5f;
	m_objectCount = 0;
	m_gravity.Set(0, -9.8f, 0);
	m_worldWidth = 4000;
	m_worldHeight = 1500;

	cEntityManager = CEntityManager::GetInstance();
	cEntityManager->Init();

	bouncetime[0] = bouncetime[1] = bouncetime[2] = bouncetime[3] = GetTickCount64();
	
	cPlayer = CPlayer::GetInstance();
	cPlayer->Init(Vector3(250, 0, 0), Vector3(1, 0, 0), m_heightMap);
	cPlayer->ActivateCollider();

	for (int i = 0; i < MAX_NPC; ++i)
	{
		int randX = Math::RandIntMinMax(-1000, 1000);
		int randZ = Math::RandIntMinMax(-1000, 1000);
		int YPos = 350.f * ReadHeightMap(m_heightMap, randX / m_worldWidth, randZ / m_worldWidth);
		CNPC* cNPC = new CNPC();
		cNPC->Init(Vector3(randX, YPos, randZ), Vector3(0, 0, 1), m_heightMap);
		cNPC->ActivateCollider();
		InitcProgressBar3D(cNPC->GetcProgressBar());
		if ((cNPC->GetPosition() - cPlayer->GetPosition()).Length() < 10)
		{
			--i;
		}
		else
		{
			vNPCS.push_back(cNPC);
			cEntityManager->Add(cNPC);
		}
	}
	
	CStructure* cStructure = new CStructure();
	cStructure->SetPosition(Vector3(-310.f, 350.f * ReadHeightMap(m_heightMap, -310.f / m_worldWidth, -452.f / m_worldWidth), -425.f));
	cStructure->ActivateCollider();
	cStructure->Init();
	cEntityManager->Add(cStructure);

	cStructure = new CStructure();
	cStructure->SetPosition(Vector3(450, 350.f * ReadHeightMap(m_heightMap, -200 / m_worldWidth, -400.f / m_worldWidth), -110));
	cStructure->ActivateCollider();
	cStructure->Init();
	cEntityManager->Add(cStructure);

	cStructure = new CStructure();
	cStructure->SetPosition(Vector3(-1100, 350.f * ReadHeightMap(m_heightMap, -1100 / m_worldWidth, -200.f / m_worldWidth), -200));
	cStructure->ActivateCollider();
	cStructure->Init();
	cEntityManager->Add(cStructure);
	
	cKeyboardController = CKeyboardController::GetInstance();

	cParticles = ParticleSystem::GetInstance();
	cParticles->SetSystemMap(m_heightMap);
	cParticles->SetSystemWorldSize(Vector3(m_worldWidth,m_worldHeight,m_worldWidth));

	//NPCS init
	iTotalInfected = 0;
	iTotalAlive = 100;
	iInfectedSpawned = 0;

	//Vaccine init
	cVaccine.Init();

	//Virus init
	cVirus = CVirus::GetInstance();
	cVirus->init();

	//Weapons init
	CCough* cCough = new CCough();
	cCough->Init();
	cPlayer->SetWeapon(0, cCough);

	CVirusLauncher* cVirusLauncher = new CVirusLauncher();
	cVirusLauncher->Init();
	cPlayer->SetWeapon(1, cVirusLauncher);

	CAirFreshener* cAirFreshener = new CAirFreshener();
	cAirFreshener->Init();
	cPlayer->SetWeapon(2, cAirFreshener);

	//Pickups init
	for (int i = 0; i < 5; ++i)
	{
		int randX = Math::RandIntMinMax(-1000, 1000);
		int randZ = Math::RandIntMinMax(-1000, 1000);
		int YPos = 350.f * ReadHeightMap(m_heightMap, randX / m_worldWidth, randZ / m_worldWidth) + 10;

		CPickUp* cLauncherAmmo = new CPickUp();
		cLauncherAmmo->SetPosition(Vector3(randX, YPos, randZ));
		cLauncherAmmo->ActivateCollider();
		cLauncherAmmo->SetPickUpType(CPickUp::PICKUP_TYPE::VIRUS_LAUNCHER);
		cLauncherAmmo->Init();

		vPickUps.push_back(cLauncherAmmo);
		cEntityManager->Add(cLauncherAmmo);
	}


	//Misc Values
	fScore = 0.f;
	fHighScore = 0.f;
	//Yime Control init
	cTimeControl = new CTimeControl;
	cTimeControl->AttachLight(lights[0], m_parameters[U_LIGHT0_POWER], m_parameters[U_LIGHT0_COLOR]);
	cTimeControl->AttachSky(meshList[GEO_SKYPLANE]);

	iDayNumber = 0;
	dTimeToNight = GetTickCount64();

	dInitalDayPhaseTime = 60000; //60s for each day phase
	dDayPhaseTime = dInitalDayPhaseTime;
	
	eGameState = GAME;

	bNightStarted = false;
	InitUIs();

	return true;
}

bool SceneBasic::InitSections(double& dValue, double dMaxValue, std::string& sRenderMessage)
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
		dValue = 5;
		return false;
	}
	else if (dValue == 5)
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
		m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
		m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
		m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
		m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
		m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
		m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
		m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
		m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
		m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
		m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
		m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");

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

		m_parameters[U_GREYMODE] = glGetUniformLocation(m_programID, "bgreymode");
		m_parameters[U_GLITCHMODE] = glGetUniformLocation(m_programID, "binversemode");

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

		eFilter = NORMAL;

		bgreymode = false;
		binversemode = false;



		//glUniform1i(m_parameters[U_GREYMODE], bgreymode);
		//glUniform1i(m_parameters[U_GLITCHMODE], binversemode);

		devCam = false;

		sRenderMessage = "Channeling Water from rivers";
		dValue = 17;
		return false;
	}
	else if (dValue == 17)
	{

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
		m_lightWaterFBO.Init(1024, 1024);




		lights[0].type = Light::LIGHT_DIRECTIONAL;
		lights[0].position.Set(2, -100, 100);
		lights[0].color.Set(1, 1, 1);
		lights[0].power = 10.f;
		lights[0].kC = 1.f;
		lights[0].kL = 0.01f;
		lights[0].kQ = 0.001f;
		lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
		lights[0].cosInner = cos(Math::DegreeToRadian(30));
		lights[0].exponent = 3.f;
		lights[0].spotDirection.Set(0.f, 1.f, 0.f);

		lights[1].type = Light::LIGHT_POINT;
		lights[1].position.Set(-300, 1, -300);
		lights[1].color.Set(0.956, 0.5647, 0.2196);
		lights[1].power = 0.f;
		lights[1].kC = 1.f;
		lights[1].kL = 0.01f;
		lights[1].kQ = 0.001f;
		lights[1].cosCutoff = cos(Math::DegreeToRadian(45));
		lights[1].cosInner = cos(Math::DegreeToRadian(30));
		lights[1].exponent = 3.f;
		lights[1].spotDirection.Set(0.f, 1.f, 0.f);

		glUniform1i(m_parameters[U_NUMLIGHTS], 2);
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

		glUniform1i(m_parameters[U_LIGHT1_TYPE], lights[1].type);
		glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &lights[1].color.r);
		glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
		glUniform1f(m_parameters[U_LIGHT1_KC], lights[1].kC);
		glUniform1f(m_parameters[U_LIGHT1_KL], lights[1].kL);
		glUniform1f(m_parameters[U_LIGHT1_KQ], lights[1].kQ);
		glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], lights[1].cosCutoff);
		glUniform1f(m_parameters[U_LIGHT1_COSINNER], lights[1].cosInner);
		glUniform1f(m_parameters[U_LIGHT1_EXPONENT], lights[1].exponent);

		sRenderMessage = "Enlightening the people";
		dValue = 25;
		return false;
	}
	else if (dValue == 25)
	{
		for (int i = 0; i < NUM_GEOMETRY; ++i)
		{
			meshList[i] = NULL;
		}
		meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference");
		meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(1, 0, 0), 1.f);
		meshList[GEO_CIRCLE] = MeshBuilder::GenerateCircle("circle", Color(1, 0, 0), 18, 5);
		meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateCrossHair("crosshair");
		meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 0, 0), 1.f);
		//meshList[GEO_QUAD]->textureArray[0] = LoadTGA("Image//calibri.tga");
		meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
		meshList[GEO_TEXT]->textureID = LoadTexture("Image//calibri.tga");
		meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);

		meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);
	

		// Load the ground mesh and texture
		meshList[GEO_GRASS_DARKGREEN] = MeshBuilder::GenerateQuad("GRASS_DARKGREEN", Color(1, 1, 1), 1.f);
		meshList[GEO_GRASS_DARKGREEN]->textureArray[0] = LoadTexture("Image//grass_darkgreen.tga");
		meshList[GEO_GRASS_LIGHTGREEN] = MeshBuilder::GenerateQuad("GEO_GRASS_LIGHTGREEN", Color(1, 1, 1), 1.f);
		meshList[GEO_GRASS_LIGHTGREEN]->textureArray[0] = LoadTexture("Image//grass_lightgreen.tga");


		//Sky Plane
		meshList[GEO_SKYPLANE] = MeshBuilder::GenerateSkyPlane("GEO_SKYPLANE", Color(1, 1, 1), 10, 200.0f, 2000.0f, 1.0f, 1.0f);
		meshList[GEO_SKYPLANE]->textureArray[0] = LoadTexture("Image//sky.tga");

		meshList[GEO_PALMTREE] = MeshBuilder::GenerateQuad("Palmtree", Color(1, 1, 1));
		meshList[GEO_PALMTREE]->textureArray[0] = LoadTexture("Image//Palm.png");


		//Terrain
		meshList[GEO_TERRAIN] = MeshBuilder::GenerateTerrain("GEO_TERRAIN", "Image//heightmap1.raw", m_heightMap);
		//meshList[GEO_TERRAIN]->textureArray[0] = LoadTGA("Image//brick.tga");	
		meshList[GEO_TERRAIN]->textureArray[0] = LoadTGA("Image//sand.tga");
		//meshList[GEO_TERRAIN]->textureArray[1] = LoadTGA("Image//grass_lightgreen.tga");

		meshList[GEO_TERRAIN]->material.kDiffuse.Set(0.04f, 0.04f, 0.04f);
		meshList[GEO_TERRAIN]->material.kSpecular.Set(0.f, 0.f, 0.f);
		meshList[GEO_TERRAIN]->material.kShininess = 32.f;
		meshList[GEO_TERRAIN]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);

		meshList[GEO_LOAD_QUAD] = MeshBuilder::GenerateQuad("Load", Color(1, 1, 1), 1.f);
		meshList[GEO_LOAD_QUAD]->textureArray[0] = LoadTexture("Image//UI/loading.png");

		sRenderMessage = "Crafting the world";
		dValue = 30;
		return false;
	}
	else if (dValue == 30)
	{

		//Objects


		meshList[GEO_SUN] = MeshBuilder::GenerateQuad("sun", Color(1, 1, 1), 1.f);
		meshList[GEO_SUN]->textureArray[0] = LoadTexture("Image//sun.png");
		/*meshList[GEO_SUN]->material.kDiffuse.Set(0.04f, 0.04f, 0.04f);
		meshList[GEO_SUN]->material.kSpecular.Set(0.f, 0.f, 0.f);
		meshList[GEO_SUN]->material.kShininess = 32.f;
		meshList[GEO_SUN]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);*/

		//meshList[GEO_SUN]->textureArray[1] = LoadTGA("Image//SunRed.tga");

		meshList[GEO_ROAD] = MeshBuilder::GenerateOBJ("Road", "OBJ//Road.obj");
		meshList[GEO_ROAD]->textureArray[0] = LoadTexture("Image//Road.jpg");
		meshList[GEO_ROAD]->material.kDiffuse.Set(0.04f, 0.04f, 0.04f);
		meshList[GEO_ROAD]->material.kSpecular.Set(0.f, 0.f, 0.f);
		meshList[GEO_ROAD]->material.kShininess = 32.f;
		meshList[GEO_ROAD]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);


		meshList[GEO_SCENE] = MeshBuilder::GenerateQuad("Scene", Color(1, 1, 1));
		meshList[GEO_SCENE]->textureArray[0] = LoadTGA("Image//Scene.tga");

		sRenderMessage = "Creating far off houses";
		dValue = 35;
		return false;
	}
	else if (dValue == 35)
	{




		meshList[GEO_POND] = MeshBuilder::GenerateSlicedQuad("GEO_POND", Color(1, 1, 1), 20.f, 1.f, 1.f, 500, 500);
		meshList[GEO_POND]->textureArray[0] = m_lightWaterFBO.GetTexture();
		meshList[GEO_POND]->textureArray[1] = LoadTGA("Image//Water.tga");
		meshList[GEO_POND]->textureArray[2] = LoadTGA("Image//Pond.tga");
		meshList[GEO_POND]->textureArray[3] = LoadTGA("Image//Blue.tga");
		meshList[GEO_POND]->material.kDiffuse.Set(0.04f, 0.04f, 0.04f);
		meshList[GEO_POND]->material.kSpecular.Set(0.f, 0.f, 0.f);
		meshList[GEO_POND]->material.kShininess = 32.f;
		meshList[GEO_POND]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);


		

		


		meshList[GEO_LOGCABIN] = MeshBuilder::GenerateOBJ("GEO_LOGCABIN", "OBJ//LogCabin.obj");
		meshList[GEO_LOGCABIN]->textureArray[0] = LoadTGA("Image//LogCabin.tga");
		meshList[GEO_LOGCABIN]->material.kDiffuse.Set(0.04f, 0.04f, 0.04f);
		meshList[GEO_LOGCABIN]->material.kSpecular.Set(0.f, 0.f, 0.f);
		meshList[GEO_LOGCABIN]->material.kShininess = 32.f;
		meshList[GEO_LOGCABIN]->material.kAmbient.Set(0.15f, 0.15f, 0.15f);

		meshList[GEO_PLAYER] = MeshBuilder::GenerateOBJ("Player", "OBJ//Player.obj");
		meshList[GEO_PLAYER]->textureArray[0] = LoadTGA("Image//Player.tga");

		meshList[GEO_NPC] = MeshBuilder::GenerateOBJ("NPC", "OBJ//Steve.obj");
		meshList[GEO_NPC]->textureArray[0] = LoadTGA("Image//Steve.tga");

		sRenderMessage = "Molding Bodies";
		dValue = 42;
		return false;
	}
	else if (dValue == 42)
	{

		//meshList[GEO_NPC] = MeshBuilder::GenerateOBJ("NPC", "OBJ//NPC.obj");
		//meshList[GEO_NPC]->textureArray[0] = LoadTGA("Image//NPC.tga");

		meshList[GEO_ANIMATED_SPRITE] = MeshBuilder::GenerateSpriteAnimation("Animated", 4, 4);
		meshList[GEO_ANIMATED_SPRITE]->textureArray[0] = LoadTexture("Image//Fire.jpeg ");
		meshList[GEO_ANIMATED_SPRITE]->material.kDiffuse.Set(0.04f, 0.04f, 0.04f);
		meshList[GEO_ANIMATED_SPRITE]->material.kSpecular.Set(0.f, 0.f, 0.f);
		meshList[GEO_ANIMATED_SPRITE]->material.kShininess = 32.f;
		meshList[GEO_ANIMATED_SPRITE]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);


		sa = dynamic_cast<SpriteAnimation*>(meshList[GEO_ANIMATED_SPRITE]);
		sa->AddSequeneAnimation("Animation1", 7, 0, 4, 8, 12, 13, 10, 6);
		sa->AddSequeneAnimation("Animation2", 4, 4, 5, 6, 7);
		sa->AddAnimation("Animation3", 0, 4);
		sa->PlayAnimation("Animation3", -1, 1.0f);


		meshList[GEO_QUAD_PART] = MeshBuilder::GenerateQuad("GEO_QUAD_PART", Color(1, 1, 1), 1.f);
		meshList[GEO_QUAD_PART]->textureArray[0] = LoadTGA("Image//particle.tga");

		meshList[GEO_LIGHT_DEPTH_QUAD] = MeshBuilder::GenerateQuad("LIGHT_DEPTH_TEXTURE", Color(1, 1, 1), 1.f);
		meshList[GEO_LIGHT_DEPTH_QUAD]->textureArray[0] = m_lightWaterFBO.GetTexture();


		meshList[GEO_VIRUS_BUTTON] = MeshBuilder::GenerateQuad("GEO_VIRUS_MENU", Color(1, 1, 1), 1.f);
		meshList[GEO_VIRUS_BUTTON]->textureArray[0] = LoadTGA("Image//Virus_ButtonTemplate.tga");

		meshList[GEO_HPBAR_GREEN] = MeshBuilder::GenerateQuad("GEO_HPBAR_GREEN", Color(1, 1, 1), 1.f);
		meshList[GEO_HPBAR_GREEN]->textureArray[0] = LoadTGA("Image//UI//Virus_HPBarGreen.tga");

		meshList[GEO_HPBAR_YELLOW] = MeshBuilder::GenerateQuad("GEO_HPBAR_YELLOW", Color(1, 1, 1), 1.f);
		meshList[GEO_HPBAR_YELLOW]->textureArray[0] = LoadTGA("Image//UI//Virus_HPBarYellow.tga");

		meshList[GEO_HP_GREEN] = MeshBuilder::GenerateQuad("GEO_HP_GREEN", Color(1, 1, 1), 1.f);
		meshList[GEO_HP_GREEN]->textureArray[0] = LoadTGA("Image//UI//Virus_HPGreen.tga");

		meshList[GEO_HP_YELLOW] = MeshBuilder::GenerateQuad("GEO_HP_GREEN", Color(1, 1, 1), 1.f);
		meshList[GEO_HP_YELLOW]->textureArray[0] = LoadTGA("Image//UI//Virus_HPYellow.tga");

		meshList[GEO_HP_RED] = MeshBuilder::GenerateQuad("GEO_HP_RED", Color(1, 1, 1), 1.f);
		meshList[GEO_HP_RED]->textureArray[0] = LoadTGA("Image//UI//Virus_HPRed.tga");

		meshList[GEO_VIRUS_PARTICLE] = MeshBuilder::GenerateSpriteAnimation("Virus_Particle", 12, 5);
		meshList[GEO_VIRUS_PARTICLE]->textureArray[0] = LoadTexture("Image//UI//virus_spritesheet.png ");
		cVirusParticle = dynamic_cast<SpriteAnimation*>(meshList[GEO_VIRUS_PARTICLE]);
		cVirusParticle->AddAnimation("Animation3", 0, 56);
		cVirusParticle->PlayAnimation("Animation3", -1, 1.5f);

		meshList[GEO_LOAD_QUAD] = MeshBuilder::GenerateQuad("Load", Color(1, 1, 1), 1.f);
		meshList[GEO_LOAD_QUAD]->textureArray[0] = LoadTexture("Image//UI/loading.png");

		meshList[GEO_VIRUS_LAUNCHER] = MeshBuilder::GenerateOBJ("VirusLauncher", "OBJ//Launcher.obj");
		meshList[GEO_VIRUS_LAUNCHER]->textureArray[0] = LoadTGA("Image//Launcher.tga");
		meshList[GEO_VIRUS_LAUNCHER]->material.kDiffuse.Set(0.04f, 0.04f, 0.04f);
		meshList[GEO_VIRUS_LAUNCHER]->material.kSpecular.Set(0.f, 0.f, 0.f);
		meshList[GEO_VIRUS_LAUNCHER]->material.kShininess = 32.f;
		meshList[GEO_VIRUS_LAUNCHER]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);

		meshList[GEO_AIR_FRESHENER] = MeshBuilder::GenerateOBJ("AirFreshener", "OBJ//AirFreshener.obj");
		meshList[GEO_AIR_FRESHENER]->textureArray[0] = LoadTGA("Image//AirFreshener.tga");

		meshList[GEO_MUTATION_POINTS] = MeshBuilder::GenerateSpriteAnimation("cMutationPt", 7, 5);
		meshList[GEO_MUTATION_POINTS]->textureArray[0] = LoadTexture("Image//UI/win_decor.png");

		cMutationPt = dynamic_cast<SpriteAnimation*>(meshList[GEO_MUTATION_POINTS]);
		cMutationPt->AddAnimation("Animation4", 0, 31);
		cMutationPt->PlayAnimation("Animation4", -1, 3.f);

		sRenderMessage = "Making immune systems";
		dValue = 48;
		return false;
	}
	else if (dValue == 48)
	{

		// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
		Mtx44 perspective;
		perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
		//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
		projectionStack.LoadMatrix(perspective);

		rotateAngle = 0;

		bLightEnabled = true;

		fogenabled = false;

		lights[1].position.Set(-300, 30 + 350.f * ReadHeightMap(m_heightMap, -300 / 4000, -300 / 4000), -300);

		std::srand(time(NULL));

		fWindowWidth = Application::GetWindowWidth();
		fWindowHeight = Application::GetWindowHeight();


		//Inital Values

		bouncetime[0] = bouncetime[1] = bouncetime[2] = bouncetime[3] = GetTickCount64();

		cPlayer = CPlayer::GetInstance();
		cPlayer->Init(Vector3(250, 0, 0), Vector3(0, 0, 1), m_heightMap);

		float Ix = cPlayer->GetPosition().x - cPlayer->GetDirection().x * 50;
		float Iz = cPlayer->GetPosition().z - cPlayer->GetDirection().z * 50;
		Vector3 cameraIPos(Ix, 350.f * ReadHeightMap(m_heightMap, Ix / m_worldWidth, Iz / m_worldWidth) + 50, Iz);

		camera.Init(cameraIPos, cameraIPos + cPlayer->GetDirection(), Vector3(0, 1, 0));
		cCamera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));

		sRenderMessage = "Making eyes";
		dValue = 52;
		return false;
	}
	else if (dValue == 52)
	{

		//Week 11

		////Init Sounds
		cSoundController = CSoundController::GetInstance();
		cSoundController->Init();
		cSoundController->LoadSound(FileSystem::getPath("Sounds\\Background.wav"), 1, true, true);
		cSoundController->LoadSound(FileSystem::getPath("Sounds\\Cough.wav"), 2, true);
		cSoundController->LoadSound(FileSystem::getPath("Sounds\\VirusLauncher.wav"), 3, true);
		cSoundController->LoadSound(FileSystem::getPath("Sounds\\AirFreshener.wav"), 4, true);
		cSoundController->LoadSound(FileSystem::getPath("Sounds\\Button.wav"), 5, true);
		cSoundController->LoadSound(FileSystem::getPath("Sounds\\Cheat.wav"), 6, true);
		cSoundController->LoadSound(FileSystem::getPath("Sounds\\Win.wav"), 7, true);
		cSoundController->LoadSound(FileSystem::getPath("Sounds\\Loss.wav"), 8, true);

		for (int i = 0; i < 5; ++i)
		{
			cSoundController->VolumeDecrease(1);
		}

		cSoundController->PlaySoundByID(1);

		sRenderMessage = "Making air";
		dValue = 57;
		return false;
	}
	
	else if (dValue == 57)
	{

		//Inital Values

		m_speed = 0.5f;
		m_objectCount = 0;
		m_gravity.Set(0, -9.8f, 0);
		m_worldWidth = 4000;
		m_worldHeight = 1500;

		cEntityManager = CEntityManager::GetInstance();
		cEntityManager->Init();

		bouncetime[0] = bouncetime[1] = bouncetime[2] = bouncetime[3] = GetTickCount64();

		//NPCS init
		iTotalInfected = 0;
		iTotalAlive = 100;
		iInfectedSpawned = 0;

		cPlayer = CPlayer::GetInstance();
		cPlayer->Init(Vector3(250, 0, 0), Vector3(1, 0, 0), m_heightMap);
		if (Application::sPassValue.load == true)
		{
			ifstream infile("Save_Entities.txt");
			if (infile.good())
			{
				string data, tmp;
				getline(infile, data);
				int size = data.size(), count = 0;
				float values[6] = { 0.f,0.f,0.f,0.f,0.f,0.f };
				
				for (int i = 0; i < size; i++)
				{
					if (data[i] == ',')
					{
						switch (count)
						{
						case 0:
							values[0] = stof(tmp);
							break;

						case 1:
							values[1] = stof(tmp);
							break;

						case 2:
							values[2] = stof(tmp);
							break;

						case 3:
							values[3] = stof(tmp);
							break;

						case 4:
							values[4] = stof(tmp);
							break;

						case 5:
							values[5] = stof(tmp);
							break;
						}
						count++;
						tmp = "";
					}
					else
					{
						tmp += data[i];
					}
				}
				cPlayer->SetPosition(Vector3(values[0], values[1], values[2]));
				cPlayer->SetDirection(Vector3(values[3], values[4], values[5]));
				infile.close();
			}
		}
		cPlayer->ActivateCollider();

		if (Application::sPassValue.load == true)
		{
			ifstream infile("Save_Entities.txt");
			if (infile.good())
			{
				string data, tmp;
				int Line = 0;
				float fvalues[6] = { 0.f,0.f,0.f,0.f,0.f,0.f };
				int ivalues[4] = { 0,0,0 };
				while (getline(infile, data))
				{
					int count = 0;
					int size = data.size();
					if (Line == 0)
					{ }
					else
					{
						for (int i = 0; i < size; i++)
						{
							if (data[i] == ',')
							{
								switch (count)
								{
								case 0:
									fvalues[0] = stof(tmp);
									break;

								case 1:
									fvalues[1] = stof(tmp);
									break;

								case 2:
									fvalues[2] = stof(tmp);
									break;

								case 3:
									fvalues[3] = stof(tmp);
									break;

								case 4:
									fvalues[4] = stof(tmp);
									break;

								case 5:
									fvalues[5] = stof(tmp);
									break;

								case 6:
									ivalues[0] = stoi(tmp);
									break;

								case 7:
									ivalues[1] = stoi(tmp);
									break;

								case 8:
									ivalues[2] = stoi(tmp);
									break;

								case 9:
									ivalues[3] = stoi(tmp);
									break;
								}
								count++;
								tmp = "";
							}
							else
							{
								tmp += data[i];
							}
						}
						CNPC* cNPC = new CNPC();
						cNPC->Init(Vector3(fvalues[0], fvalues[1], fvalues[2]), Vector3(fvalues[3], fvalues[4], fvalues[5]), m_heightMap);
						cNPC->SetSpeed(ivalues[0]);

						switch (ivalues[1])
						{
						case 0: //Healthy
							cNPC->SetState(CNPC::STATE::HEALTHY);
							break;

						case 1: //Infected
							cNPC->SetState(CNPC::STATE::INFECTED);
							iTotalInfected++;
							break;

						case 2: //Dead
							cNPC->SetState(CNPC::STATE::DEAD);
							iTotalAlive--;
							break;

						case 3: //Vaccinated
							cNPC->SetState(CNPC::STATE::VACCINATED);
							break;
						}
						cNPC->SetHealth(ivalues[2]);
						cNPC->SetDaysLeft(ivalues[3]);
						cNPC->ActivateCollider();
						InitcProgressBar3D(cNPC->GetcProgressBar());

						vNPCS.push_back(cNPC);
						cEntityManager->Add(cNPC);
					}
					Line++;
				}
				infile.close();
			}
		}
		else
		{
			for (int i = 0; i < MAX_NPC; ++i)
			{
				CNPC* cNPC = new CNPC();
				cNPC->Init(Vector3(Math::RandFloatMinMax(-1000, 1000), 0, Math::RandFloatMinMax(-1000, 1000)), Vector3(0, 0, 1), m_heightMap);
				cNPC->ActivateCollider();
				InitcProgressBar3D(cNPC->GetcProgressBar());

				vNPCS.push_back(cNPC);
				cEntityManager->Add(cNPC);
			}
		}
		
		sRenderMessage = "Making (people)victims";
		dValue = 65;
		return false;
	}
	else if (dValue == 65)
	{

		CStructure* cStructure = new CStructure();
		cStructure->SetPosition(Vector3(-310.f, 350.f * ReadHeightMap(m_heightMap, -310.f / m_worldWidth, -452.f / m_worldWidth), -425.f));
		cStructure->ActivateCollider();
		cStructure->Init();
		cEntityManager->Add(cStructure);

		CStructure* cStructure1 = new CStructure();
		cStructure1->SetPosition(Vector3(450, 350.f * ReadHeightMap(m_heightMap, -200 / m_worldWidth, -400.f / m_worldWidth), -110));
		cStructure1->ActivateCollider();
		cStructure1->Init();
		cEntityManager->Add(cStructure1);

		CStructure* cStructure2 = new CStructure();
		cStructure2->SetPosition(Vector3(-1100, 350.f * ReadHeightMap(m_heightMap, -1100 / m_worldWidth, -200.f / m_worldWidth), -200));
		cStructure2->ActivateCollider();
		cStructure2->Init();
		cEntityManager->Add(cStructure2);

		cKeyboardController = CKeyboardController::GetInstance();

		cParticles = ParticleSystem::GetInstance();
		cParticles->SetSystemMap(m_heightMap);
		cParticles->SetSystemWorldSize(Vector3(m_worldWidth, m_worldHeight, m_worldWidth));

		for (int check = 0; check < vNPCS.size(); ++check)
		{
			if (vNPCS[check]->CheckForCollision(cStructure) == true || vNPCS[check]->CheckForCollision(cStructure1) == true || vNPCS[check]->CheckForCollision(cStructure2) == true || vNPCS[check]->Constraint() == true)
			{
				int randX = Math::RandIntMinMax(-1000, 1000);
				int randZ = Math::RandIntMinMax(-1000, 1000);

				vNPCS[check]->SetPosition(Vector3(randX, 1, randZ));
				--check;
			}
			else
				continue;
		}

		//Vaccine init
		cVaccine.Init();
		if (Application::sPassValue.load == true)
		{
			int count = 0;
			string tmp;
			ifstream infile("Save_Values.txt");
			if (infile.is_open())
			{
				while (getline(infile, tmp))
				{
					count++;
					switch (count)
					{
					case 2:
						cVaccine.SetVacineState(stoi(tmp));
						break;

					case 3:
						cVaccine.SetAwareness(stof(tmp));
						break;

					case 4:
						cVaccine.SetProgress(stof(tmp));
						break;
					}
					tmp = "";
				}
				infile.close();
			}
		}

		//Virus init
		cVirus = CVirus::GetInstance();
		cVirus->init();
		if (Application::sPassValue.load == true)
		{
			int count = 0;
			string tmp;
			ifstream infile("Save_Values.txt");
			if (infile.is_open())
			{
				while (getline(infile, tmp))
				{
					count++;
					if (count == 13)
					{
						cVirus->SetName(tmp);
					}
					if (count == 14)
					{
						cVirus->IncreasePoints(stoi(tmp));
					}
					tmp = "";
				}
				infile.close();
			}
		}
		else
		{
			if (Application::sPassValue.virusName == "")
				cVirus->SetName("Nameless");
			else
				cVirus->SetName(Application::sPassValue.virusName);
			cVirus->IncreasePoints(100);
		}
		

		//Weapons init
		CCough* cCough = new CCough();
		cCough->Init();
		cPlayer->SetWeapon(0, cCough);

		// CUpgrade Init
		cUpgrade = new CUpgrade;
		cUpgrade->AttachHandlers(cCough, cVirus, &dDayPhaseTime, cPlayer, &cVaccine);
		sRenderMessage = "Synthesising vaccines";
		
		if (Application::sPassValue.load == true)
		{
			int count = 0;
			int value[6] = { 0,0,0,0,0,0 };
			string tmp;
			ifstream infile("Save_Values.txt");
			if (infile.is_open())
			{
				while (getline(infile, tmp))
				{
					count++;
					switch(count)
					{
					case 7:
						value[0] = stoi(tmp);
						break;

					case 8:
						value[1] = stoi(tmp);
						break;

					case 9:
						value[2] = stoi(tmp);
						break;

					case 10:
						value[3] = stoi(tmp);
						break;

					case 11:
						value[4] = stoi(tmp);
						break;

					case 12:
						value[5] = stoi(tmp);
						break;
					}
					tmp = "";
				}
				cUpgrade->InitLevels(value[0], value[1], value[2], value[3], value[4], value[5]);
				infile.close();
			}
		}
		else
			cUpgrade->InitLevels(0, 0, 1, 0, 0, 0);
		dValue = 70;
		return false;
	}
	else if (dValue == 70)
	{
		CVirusLauncher* cVirusLauncher = new CVirusLauncher();
		cVirusLauncher->Init();
		if (Application::sPassValue.load == true)
		{
			int count = 0;
			string tmp;
			ifstream infile("Save_Values.txt");
			if (infile.is_open())
			{
				while (getline(infile, tmp))
				{
					count++;
					if (count == 5)
					{
						cVirusLauncher->SetUses(stoi(tmp));
					}
				}
				infile.close();
			}
		}
		cPlayer->SetWeapon(1, cVirusLauncher);

		CAirFreshener* cAirFreshener = new CAirFreshener();
		cAirFreshener->Init();
		if (Application::sPassValue.load == true)
		{
			int count = 0;
			string tmp;
			ifstream infile("Save_Values.txt");
			if (infile.is_open())
			{
				while (getline(infile, tmp))
				{
					count++;
					if (count == 6)
					{
						cAirFreshener->SetUses(stoi(tmp));
					}
				}
				infile.close();
			}
		}
		cPlayer->SetWeapon(2, cAirFreshener);
		

		for (int i = 0; i < 5; ++i)
		{
			int randX = Math::RandIntMinMax(-1000, 1000);
			int randZ = Math::RandIntMinMax(-1000, 1000);
			int YPos = 350.f * ReadHeightMap(m_heightMap, randX / m_worldWidth, randZ / m_worldWidth) + 10;

			CPickUp* cLauncherAmmo = new CPickUp();
			cLauncherAmmo->SetPosition(Vector3(randX, YPos, randZ));
			cLauncherAmmo->ActivateCollider();
			cLauncherAmmo->SetPickUpType(CPickUp::PICKUP_TYPE::VIRUS_LAUNCHER);
			cLauncherAmmo->Init();

			if (cLauncherAmmo->Constraint() == true)
			{
				--i;
			}
			else
			{
				vPickUps.push_back(cLauncherAmmo);
				cEntityManager->Add(cLauncherAmmo);
			}
		}

		for (int j = 0; j < 5; ++j)
		{
			int randX = Math::RandIntMinMax(-1000, 1000);
			int randZ = Math::RandIntMinMax(-1000, 1000);
			int YPos = 350.f * ReadHeightMap(m_heightMap, randX / m_worldWidth, randZ / m_worldWidth) + 10;

			CPickUp* cAirFreshenerAmmo = new CPickUp();
			cAirFreshenerAmmo->SetPosition(Vector3(randX, YPos, randZ));
			cAirFreshenerAmmo->ActivateCollider();
			cAirFreshenerAmmo->SetPickUpType(CPickUp::PICKUP_TYPE::AIR_FRESHENER);
			cAirFreshenerAmmo->Init();

			if (cAirFreshenerAmmo->Constraint() == true)
			{
				--j;
			}
			else
			{
				vPickUps.push_back(cAirFreshenerAmmo);
				cEntityManager->Add(cAirFreshenerAmmo);
			}
		}

		for (int k = 0; k < 5; ++k)
		{
			int randX = Math::RandIntMinMax(-1000, 1000);
			int randZ = Math::RandIntMinMax(-1000, 1000);
			int YPos = 350.f * ReadHeightMap(m_heightMap, randX / m_worldWidth, randZ / m_worldWidth) + 10;

			CPickUp* cMutationPts = new CPickUp();
			cMutationPts->SetPosition(Vector3(randX, YPos, randZ));
			cMutationPts->ActivateCollider();
			cMutationPts->SetPickUpType(CPickUp::PICKUP_TYPE::MUTATION_PTS);
			cMutationPts->Init();

			if (cMutationPts->Constraint() == true)
			{
				--k;
			}
			else
			{
				vPickUps.push_back(cMutationPts);
				cEntityManager->Add(cMutationPts);
			}
		}

		sRenderMessage = "Making 'Weapons' ";
		dValue = 87;
		return false;

	}
	else if (dValue == 87)
	{

		//Misc Values
		fScore = 0.f;
		if (Application::sPassValue.load == true)
		{
			int count = 0;
			string tmp;
			ifstream infile("Save_Values.txt");
			if (infile.is_open())
			{
				while (getline(infile, tmp))
				{
					count++;
					if (count == 15)
					{
						fScore = stof(tmp);
					}
				}
				infile.close();
			}
		}
		fHighScore = 0.f;
		//Time Control init
		cTimeControl = new CTimeControl;
		cTimeControl->AttachLight(lights[0], m_parameters[U_LIGHT0_POWER], m_parameters[U_LIGHT0_COLOR]);
		cTimeControl->AttachSky(meshList[GEO_SKYPLANE]);

		iDayNumber = 0;
		if (Application::sPassValue.load == true)
		{
			string tmp;
			ifstream infile("Save_Values.txt");
			if (infile.is_open())
			{
				getline(infile, tmp);
				iDayNumber = iDayLoad = stoi(tmp);
			}
			infile.close();
		}
		dTimeToNight = GetTickCount64();

		dInitalDayPhaseTime = 60000; //60s for each day phase
		dDayPhaseTime = dInitalDayPhaseTime;



		eGameState = GAME;

		bNightStarted = false;
		sRenderMessage = "Making Screens ";
		dValue = 90;
		return false;
	}
	else if (dValue == 90)
	{
		InitUIs();

		sRenderMessage = "DONE! ";
		dValue = 100;
		return true;

	}
	return false;
}



void SceneBasic::Update(double dt)
{
	double dcurrenttime = GetTickCount64(); //Save on resources instead of callling a function everytime
	fWindowWidth = Application::GetWindowWidth();
	fWindowHeight = Application::GetWindowHeight();

	UpdateParticles(dt);

	water += 0.5f * dt;


	cPlayer->SetPosition(Vector3(cPlayer->GetPosition().x, 350.f * ReadHeightMap(m_heightMap, cPlayer->GetPosition().x, cPlayer->GetPosition().z), cPlayer->GetPosition().z));



	//Update sprite aniamtion
	sa->Update(dt);
	cVirusParticle->Update(dt);
	
	
	if (Application::IsKeyPressed(VK_F1)) // Linear fog 
		glUniform1i(m_parameters[U_FOG_TYPE], 0);
	if (Application::IsKeyPressed(VK_F2)) // Exponential fog
		glUniform1i(m_parameters[U_FOG_TYPE], 1);
	if (Application::IsKeyPressed(VK_F3)) //  fog 
		glUniform1i(m_parameters[U_FOG_TYPE], 2);

	if (Application::IsKeyPressed('5'))
	{
		lights[0].type = Light::LIGHT_POINT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	else if (Application::IsKeyPressed('6'))
	{
		lights[0].type = Light::LIGHT_DIRECTIONAL;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	else if (Application::IsKeyPressed('7'))
	{
		lights[0].type = Light::LIGHT_SPOT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	else if (Application::IsKeyPressed('8'))
	{
		bLightEnabled = true;
	}
	else if (Application::IsKeyPressed('9'))
	{
		bLightEnabled = false;
	}

	/*if (Application::IsKeyPressed('I'))
		lights[0].position.z -= (float)(10.f * dt);
	if (Application::IsKeyPressed('K'))
		lights[0].position.z += (float)(10.f * dt);
	if (Application::IsKeyPressed('J'))
		lights[0].position.x -= (float)(10.f * dt);
	if (Application::IsKeyPressed('L'))
		lights[0].position.x += (float)(10.f * dt);
	if (Application::IsKeyPressed('O'))
		lights[0].position.y -= (float)(10.f * dt);
	if (Application::IsKeyPressed('P'))
		lights[0].position.y += (float)(10.f * dt);*/


	

	if (Application::IsKeyPressed('0') && bouncetime[1] <= dcurrenttime)
	{
		bouncetime[1] = dcurrenttime + 150;
		if (devCam == true)
		{
			devCam = false;
			cCompass.AttachCamera(cCamera);
		}
		else
		{
			camera.target = cCamera.target;
			camera.position = cCamera.position;
			devCam = true;
			cCompass.AttachCamera(camera);
		}		
	}

	//Game Values Update

	switch (eGameState)
	{
	case MAINMENU:
		UpdateMainMenu(dt);
		break;

	case GAME:
		UpdateGame(dt);
		break;
	}
	UpdateUIs(dt);
	fps = (float)(1.f / dt);
}

void SceneBasic::RenderText(Mesh* mesh, std::string text, Color color)
{
	if(!mesh || mesh->textureID <= 0)
		return;
	
	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
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

void SceneBasic::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if(!mesh || mesh->textureID <= 0)
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
				for(unsigned i = 0; i < text.length(); ++i)
				{
					Mtx44 characterSpacing;
					characterSpacing.SetToTranslation(i * 1.0f + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
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

void SceneBasic::RenderMeshIn2D(Mesh *mesh, bool enableLight, float size, float x, float y)
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
				if(mesh->textureID > 0)
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
				if(mesh->textureID > 0)
				{
					glBindTexture(GL_TEXTURE_2D, 0);
				}
       
			modelStack.PopMatrix();
		viewStack.PopMatrix();
	projectionStack.PopMatrix();

}

void SceneBasic::RenderMesh(Mesh* mesh, bool enableLight)
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

	MVP = projectionStack.Top() * viewStack.Top() *	modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE,&modelView.a[0]);

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
	//if(mesh->textureID > 0)
	//{
	//	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	//	glActiveTexture(GL_TEXTURE0);
	//	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	//	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	//}
	//else
	//{
	//	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	//}
	//mesh->Render();
	//if(mesh->textureID > 0)
	//{
	//	glBindTexture(GL_TEXTURE_2D, 0);
	//}
}


void SceneBasic::RenderGround()
{
	modelStack.PushMatrix();
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Translate(0, 0, -10);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(400.0f, 400.0f, 400.0f);

	for (int x=0; x<10; x++)
	{
		for (int z=0; z<10; z++)
		{
			modelStack.PushMatrix();
			modelStack.Translate(x-5.0f, z-5.0f, 0.0f);
			if ( ((x*9+z) % 2) == 0)
				RenderMesh(meshList[GEO_GRASS_DARKGREEN], false);
			else
				RenderMesh(meshList[GEO_GRASS_LIGHTGREEN], false);
			modelStack.PopMatrix();
		}
	}
	modelStack.PopMatrix();
}

void SceneBasic::RenderSkyPlane()
{
	modelStack.PushMatrix();
	modelStack.Translate(0, 1800, 0);
	
	RenderMesh(meshList[GEO_SKYPLANE], true);
	
	modelStack.PopMatrix();
}

void SceneBasic::RenderTerrain()
{
	modelStack.PushMatrix();
	modelStack.Scale(m_worldWidth, 350.f, m_worldWidth); // values varies.
	RenderMesh(meshList[GEO_TERRAIN], true);
	modelStack.PopMatrix();
}


 
void SceneBasic::Render()
{
	//******************************* PRE RENDER PASS	*************************************
	RenderPassGPass();
	RenderWater();
	//******************************* MAIN RENDER PASS ************************************
	RenderPassMain();
}

void SceneBasic::Exit()
{
	// Cleanup VBO
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if(meshList[i])
			delete meshList[i];
	}
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);


	//Delete pointers

	
	if (cTimeControl != NULL)
	{
		delete cTimeControl;
		cTimeControl = NULL;
	}
	//if (sa != NULL)
	//{
	//	delete sa;
	//	sa = NULL;
	//}

	if (cUpgrade)
	{
		delete cUpgrade;
		cUpgrade = NULL;
	}

	//if (cPlayer)
	//{
	//	delete cPlayer;
	//	cPlayer = NULL;
	//}

	//while (m_PList.size() > 0)
	//{
	//	ParticleObject* Part = m_PList.back();
	//	delete Part;
	//	m_PList.pop_back();
	//}

	glDeleteProgram(m_gPassShaderID);
}

void SceneBasic::RenderNextProgressBar(double dt, double& dValue, double dMaxValue, string sRenderMessage)
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

	RenderUIs(projectionStack, viewStack, modelStack);

	RenderTextOnScreen(meshList[GEO_TEXT], sRenderMessage, Color(1, 1, 1), 4.5, 8.5, 8);




}

void SceneBasic::DeleteInstances()
{
	if (cKeyboardController)
	{
		cKeyboardController->Destroy();
		cKeyboardController = NULL;
	}

	if (cParticles)
	{
		cParticles->Destroy();
		cParticles = NULL;
	}

	if (cVirus)
	{
		cVirus->Destroy();
		cVirus = NULL;
	}
	if (cPlayer)
	{
		cPlayer->Destroy();
		cPlayer = NULL;
	}
	if (cEntityManager)
	{
		cEntityManager->Destroy();
		cEntityManager = NULL;
	}

	if (cSoundController)
	{
		
		cSoundController->Destroy();
		cSoundController = NULL;
	}

}

void SceneBasic::UpdateMainMenu(double dt)
{
	Application::SetMouseSeen(true);
	if (Application::IsKeyPressed(VK_SPACE))
	{
		bouncetime[2] = GetTickCount64() + 500;

		StartGame("Placeholder");

	}
}

void SceneBasic::UpdateGame(double dt)
{
	double dcurrenttime = GetTickCount64();

	



	//Day cycle uupdates

	if (cTimeControl->GetTimePaused())
	{
		dTimeToNight += dt * 1000; // seconds to milliseconds
	}
	else
	{
		//Day cycle uupdates
		if (cTimeControl->isDay())
		{
			cSoundController->SetListenerPosition(cPlayer->GetPosition().x, cPlayer->GetPosition().y, cPlayer->GetPosition().z);
			
			if (Application::IsKeyPressed('1'))
			{
				if (bouncetime[3] <= GetTickCount64())
				{
					cSoundController->PlaySoundByID(5);
					bouncetime[3] = GetTickCount64() + 100;
				}

				cPlayer->SetCurrentWeapon(0);
				cInventorySlot1.SetEnlarge(true);
				cInventorySlot1Label.SetEnlarge(true);

				cInventorySlot2.SetEnlarge(false);
				cInventorySlot2Label.SetEnlarge(false);
				cInventorySlot2Ammo.SetEnlarge(false);

				cInventorySlot3.SetEnlarge(false);
				cInventorySlot3Label.SetEnlarge(false);
				cInventorySlot3Ammo.SetEnlarge(false);
			}
			else if (Application::IsKeyPressed('2'))
			{
				if (bouncetime[3] <= GetTickCount64())
				{
					cSoundController->PlaySoundByID(5);
					bouncetime[3] = GetTickCount64() + 100;
				}

				cPlayer->SetCurrentWeapon(1);
				cInventorySlot1.SetEnlarge(false);
				cInventorySlot1Label.SetEnlarge(false);

				cInventorySlot2.SetEnlarge(true);
				cInventorySlot2Label.SetEnlarge(true);
				cInventorySlot2Ammo.SetEnlarge(true);

				cInventorySlot3.SetEnlarge(false);
				cInventorySlot3Label.SetEnlarge(false);
				cInventorySlot3Ammo.SetEnlarge(false);
			}
			else if (Application::IsKeyPressed('3'))
			{
				if (bouncetime[3] <= GetTickCount64())
				{
					cSoundController->PlaySoundByID(5);
					bouncetime[3] = GetTickCount64() + 100;
				}
				cPlayer->SetCurrentWeapon(2);
				cInventorySlot1.SetEnlarge(false);
				cInventorySlot1Label.SetEnlarge(false);

				cInventorySlot2.SetEnlarge(false);
				cInventorySlot2Label.SetEnlarge(false);
				cInventorySlot2Ammo.SetEnlarge(false);

				cInventorySlot3.SetEnlarge(true);
				cInventorySlot3Label.SetEnlarge(true);
				cInventorySlot3Ammo.SetEnlarge(true);
			}
	

			cPlayer->SetPosition(Vector3(cPlayer->GetPosition().x, 350.f * ReadHeightMap(m_heightMap, cPlayer->GetPosition().x/m_worldWidth, cPlayer->GetPosition().z / m_worldWidth), cPlayer->GetPosition().z));
			if (Application::IsKeyPressed(VK_SPACE)/* && cPlayer->GetWeapon()->GetCanFire() == true*/)
			{
				CProjectile* cProjectile = cPlayer->Attack();
				if (cProjectile)
				{
					cProjectile->SetProjectileDamage(cPlayer->GetWeapon()->GetWeaponDamage());
					cEntityManager->Add(cProjectile);
				}
			}

			//cPlayer->SetPosition(Vector3(cPlayer->GetPosition().x, 350.f * ReadHeightMap(m_heightMap, cPlayer->GetPosition().x / m_worldWidth, cPlayer->GetPosition().z / m_worldWidth), cPlayer->GetPosition().z));
			bNightStarted = false;
			cTimeControl->Update(dTimeToNight, dcurrenttime, dDayPhaseTime);



			//Player Update
			cPlayer->Update(dt);

			cEntityManager->Update(dt);

			rotateAngle += (float)(10 * dt);
			if (devCam == true)
				Vector3 PositionOld = camera.position;
			else
				Vector3 PositionOld = cCamera.position;
		

			if (cEntityManager->CollisionCheck(cPlayer))
			{

			}

			if (devCam == true)
				camera.Update(dt);
			else
			{
				cCamera.Update(dt);
				cCamera.target.Set(cPlayer->GetPosition().x, cPlayer->GetPosition().y + 30, cPlayer->GetPosition().z);
				cCamera.position.Set(cPlayer->GetPosition().x - cPlayer->GetDirection().x * 100, cPlayer->GetPosition().y + 50, cPlayer->GetPosition().z - cPlayer->GetDirection().z * 100);
			}
		}
		else if (!bNightStarted)// First Night Frame during each night
		{
			bNightStarted = true; // value that makes sure this part of the code is called ONCE
			cSunSet.StopBackgroundAnimation(); // Resets anim
			cSunSet.StartBackgroundAnimation();
			cSunSet.SetActive(true);
			cSunSetButton.SetActive(true);
			cSunSetText.SetActive(true);
			if (iDayNumber != iDayLoad)
			{
				cVirus->IncreasePoints(100); // Give 100 points to player every night
				for (int i = 0; i < vNPCS.size(); ++i)
				{
					if (vNPCS[i]->GetState() == CNPC::INFECTED)
					{
						vNPCS[i]->ReduceDaysLeft(cUpgrade->GetVirusLethalityLevel());
					}
				}
			}
			SaveGameEntities();
			SaveGameValues();

			//Should kill npcs here

			if (iDayNumber >= 2)
			{
				cVaccine.SetVacineState(true);
			}

			vNPCSpawned.clear();
			bool bAllDed = true;
			iTotalAlive = 0;
			iTotalInfected = 0;
			for (int i = 0; i < vNPCS.size(); ++i)
			{
				vNPCS[i]->SetActive(false);
				if (vNPCS[i]->GetState() != CNPC::DEAD)
				{
					bAllDed = false;
					++iTotalAlive;
					if (vNPCS[i]->GetState() == CNPC::INFECTED)
					{
						++iTotalInfected;
						cVirus->IncreasePoints(50);
					}
				}
			}

			cVaccine.SetAwareness(MAX_NPC - iTotalAlive);
		

			fScore += iTotalInfected;


			if (bAllDed == true)
			{
				//WIN CONDITION
				eGameState = WIN;
				cSoundController->PlaySoundByID(7);
				cWinFrame.SetActive(true);
				cWinDecor.SetActive(true);
				cWinText.SetActive(true);
				cWinDays.SetActive(true);
				cWinButton.SetActive(true);
			}

			int iSpawned = 0;
			int iNPCChosen[MAX_SPAWN_NPC];

			//Spawn the max num of npc
			while (iSpawned < MAX_SPAWN_NPC && iSpawned < iTotalAlive)
			{
				int iNPCNum = rand() % vNPCS.size();
				bool iAlreadySpawnned = false;
				for (int iCheck = 0; iCheck < iSpawned; ++iCheck)
				{
					if (iNPCNum == iNPCChosen[iCheck]) //Make sure the npc isnt already spawnned
					{
						iAlreadySpawnned == true;
						break;
					}
				}
				

				if (iAlreadySpawnned == false && vNPCS[iNPCNum]->GetState() != CNPC::DEAD) //Make sure its not ded
				{
					if (vNPCS[iNPCNum]->GetState() == CNPC::STATE::INFECTED)
					{
						if (rand() % 2 == 0) //If infected 50/50 chance to spawn
						{
							vNPCSpawned.push_back(vNPCS[iNPCNum]);
							iNPCChosen[iSpawned] = iNPCNum;
							vNPCS[iNPCNum]->SetActive(true);
							++iSpawned;
							++iInfectedSpawned;
						}
					}
					else //is healthy
					{
						vNPCSpawned.push_back(vNPCS[iNPCNum]);
						iNPCChosen[iSpawned] = iNPCNum;
						vNPCS[iNPCNum]->SetActive(true);

						++iSpawned;
					}

				}
			}
			for (int iCheck = 0; iCheck < iSpawned; ++iCheck)
			{
				if (cPlayer->CheckForCollision(vNPCSpawned[iCheck]) == true || vNPCSpawned[iCheck]->Constraint() == true)
				{
					int randX = Math::RandIntMinMax(-1000, 1000);
					int randZ = Math::RandIntMinMax(-1000, 1000);
					int YPos = 350.f * ReadHeightMap(m_heightMap, randX / m_worldWidth, randZ / m_worldWidth);
					vNPCSpawned[iCheck]->SetPosition(Vector3(randX, YPos, randZ));
					--iCheck;
				}
			}
			
			for (int i = 0; i < 5; ++i)
			{
				int randX = Math::RandIntMinMax(-1000, 1000);
				int randZ = Math::RandIntMinMax(-1000, 1000);
				int YPos = 350.f * ReadHeightMap(m_heightMap, randX / m_worldWidth, randZ / m_worldWidth) + 10;

				CPickUp* cLauncherAmmo = new CPickUp();
				cLauncherAmmo->SetPosition(Vector3(randX, YPos, randZ));
				cLauncherAmmo->ActivateCollider();
				cLauncherAmmo->SetPickUpType(CPickUp::PICKUP_TYPE::VIRUS_LAUNCHER);
				cLauncherAmmo->Init();

				if (cLauncherAmmo->Constraint() == true)
				{
					--i;
				}
				else
				{
					vPickUps.push_back(cLauncherAmmo);
					cEntityManager->Add(cLauncherAmmo);
				}
			}

			for (int j = 0; j < 5; ++j)
			{
				int randX = Math::RandIntMinMax(-1000, 1000);
				int randZ = Math::RandIntMinMax(-1000, 1000);
				int YPos = 350.f * ReadHeightMap(m_heightMap, randX / m_worldWidth, randZ / m_worldWidth) + 10;

				CPickUp* cAirFreshenerAmmo = new CPickUp();
				cAirFreshenerAmmo->SetPosition(Vector3(randX, YPos, randZ));
				cAirFreshenerAmmo->ActivateCollider();
				cAirFreshenerAmmo->SetPickUpType(CPickUp::PICKUP_TYPE::AIR_FRESHENER);
				cAirFreshenerAmmo->Init();

				if (cAirFreshenerAmmo->Constraint() == true)
				{
					--j;
				}
				else
				{
					vPickUps.push_back(cAirFreshenerAmmo);
					cEntityManager->Add(cAirFreshenerAmmo);
				}
			}

			for (int k = 0; k < 5; ++k)
			{
				int randX = Math::RandIntMinMax(-1000, 1000);
				int randZ = Math::RandIntMinMax(-1000, 1000);
				int YPos = 350.f * ReadHeightMap(m_heightMap, randX / m_worldWidth, randZ / m_worldWidth) + 10;

				CPickUp* cMutationPts = new CPickUp();
				cMutationPts->SetPosition(Vector3(randX, YPos, randZ));
				cMutationPts->ActivateCollider();
				cMutationPts->SetPickUpType(CPickUp::PICKUP_TYPE::MUTATION_PTS);
				cMutationPts->Init();

				if (cMutationPts->Constraint() == true)
				{
					--k;
				}
				else
				{
					vPickUps.push_back(cMutationPts);
					cEntityManager->Add(cMutationPts);
				}
			}
		}
	}
	for (int i = 0; i < vPickUps.size(); ++i)
	{
		if (vPickUps[i] == nullptr)
			continue;
		else if (vPickUps[i]->IsToDelete() == true)
			vPickUps[i] = nullptr;
	}
}

void SceneBasic::RenderMainMenu()
{

	RenderImageOnScreen(meshList[GEO_VIRUS_BUTTON], fWindowWidth, fWindowHeight, fWindowWidth *0.5, fWindowHeight * 0.5);
}

void SceneBasic::RenderGame()
{

	// Render the crosshair

	RenderPlayer();
	RenderWeaponHitBox();

	RenderNPC();

	//RenderGround();
	RenderSkyPlane();
	RenderTerrain();

	//RenderWall();

	modelStack.PushMatrix();
	modelStack.Translate(0, 80, 850);
	modelStack.Scale(2, 1, 1);
	RenderMesh(meshList[GEO_ROAD], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 290, 850);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(3200, 950, 1);
	RenderMesh(meshList[GEO_SCENE], false);
	modelStack.PopMatrix();

	//RenderWall();

	//Render water
	waterEnabled = true; //Enable waves
	waterwavesnum = 3; //NUMBER OF WAVES! :D
	modelStack.PushMatrix();
	modelStack.Translate(0, -70.f + 350.f * ReadHeightMap(m_heightMap, 0 /
		m_worldWidth, 0 / m_worldWidth), -1400);
	modelStack.Scale(6.2, 1, 2);
	RenderMesh(meshList[GEO_POND], false);
	modelStack.PopMatrix();
	waterEnabled = false; //Disable waves

	Vector3 Pos;



	for (int i = 0; i < vNPCSpawned.size(); ++i)
	{
		RenderCProgressBar3D(vNPCSpawned[i]->GetcProgressBar());
	}


	RenderPickUps();

	//Render log ccabin
	Pos.Set(-310.f, 0, -425.f);
	modelStack.PushMatrix();
	modelStack.Translate(Pos.x, 350.f * ReadHeightMap(m_heightMap, Pos.x / m_worldWidth, Pos.z / m_worldWidth), Pos.z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(4, 4, 4);
	RenderMesh(meshList[GEO_LOGCABIN], true);
	modelStack.PopMatrix();

	Pos.Set(450.f, 0, -110.f);
	modelStack.PushMatrix();
	modelStack.Translate(Pos.x, 350.f * ReadHeightMap(m_heightMap, Pos.x / m_worldWidth, Pos.z / m_worldWidth), Pos.z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(4, 4, 4);
	RenderMesh(meshList[GEO_LOGCABIN], true);
	modelStack.PopMatrix();

	Pos.Set(-1100.f, 0, -200.f);
	modelStack.PushMatrix();
	modelStack.Translate(Pos.x, 350.f * ReadHeightMap(m_heightMap, Pos.x / m_worldWidth, Pos.z / m_worldWidth), Pos.z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(4, 4, 4);
	RenderMesh(meshList[GEO_LOGCABIN], true);
	modelStack.PopMatrix();

	RenderParticles();
	RenderPalmTrees();


	//Pos.Set(-270.f, 0, -300.f);
	//modelStack.PushMatrix();
	//modelStack.Translate(Pos.x, 25.f + 350.f * ReadHeightMap(m_heightMap, Pos.x / m_worldWidth, Pos.z / m_worldWidth), Pos.z);
	//modelStack.Rotate(Math::RadianToDegree(atan2(camera.position.x - Pos.x, camera.position.z - Pos.z)), 0, 1, 0);
	//modelStack.Translate(-10, 0, 10);
	//modelStack.Scale(40, 40, 40);
	//RenderMesh(meshList[GEO_LOG_MENU], true);
	//modelStack.PopMatrix();



	//modelStack.PushMatrix();
	//modelStack.Scale(10, 10, 10);
	////RenderText(meshList[GEO_TEXT], "Hello World", Color(0, 1, 0));
	//RenderText(meshList[GEO_TEXT], "DM2210", Color(0, 1, 1));
	//modelStack.PopMatrix();

	// Render the crosshair



	if (devCam == true)
	{

		modelStack.PushMatrix();
		modelStack.Translate(lights[0].position.x, lights[0].position.y, lights[0].position.z);
		modelStack.Scale(500, 500, 500);
		float angle = Math::RadianToDegree(atan2(camera.position.z - lights[0].position.z, camera.position.x - lights[0].position.x)) - 90;
		modelStack.Rotate(-angle, 0, 1, 0);
		angle = Math::RadianToDegree(Vector3(lights[0].position.x - camera.position.x, lights[0].position.y - camera.position.y, 0).Normalize().Dot(Vector3(0, 1, 0)));
		modelStack.Rotate(angle, 1, 0, 0);
		RenderMesh(meshList[GEO_SUN], false);
		modelStack.PopMatrix();
	}
	else
	{
		modelStack.PushMatrix();
		modelStack.Translate(lights[0].position.x * 2, lights[0].position.y * 2, lights[0].position.z * 2);
		modelStack.Scale(1000, 1000, 1000);
		float angle = Math::RadianToDegree(atan2(cCamera.position.z - lights[0].position.z, cCamera.position.x - lights[0].position.x)) - 90;
		modelStack.Rotate(-angle, 0, 1, 0);
		angle = Math::RadianToDegree(Vector3(lights[0].position.x, lights[0].position.y, 0).Normalize().Dot(Vector3(0, 1, 0)));
		modelStack.Rotate(angle, 1, 0, 0);
		RenderMesh(meshList[GEO_SUN], false);
		modelStack.PopMatrix();
	}

	RenderUIs(projectionStack, viewStack, modelStack);
}

void SceneBasic::StartGame(string sVirusName)
{
	cVaccine.Restart();
	cVirus->Restart(sVirusName);

	cTimeControl->Update(1, 2, 0); // Set to night
	eGameState = GAMESTATE::GAME;

	eFilter = NORMAL;

	bgreymode = false;
	binversemode = false;

}

void SceneBasic::RenderImageOnScreen(Mesh* mesh, float sizex, float sizey, float x, float y)
{

	glDisable(GL_DEPTH_TEST);

	Mtx44 ortho;
	ortho.SetToOrtho(0, fWindowWidth, 0, fWindowHeight, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack

	modelStack.Translate(x, y, 0);
	modelStack.Scale(sizex, sizey, 1);

	RenderMesh(mesh, false);
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();


	glEnable(GL_DEPTH_TEST);

}

void SceneBasic::RenderParticles()
{
	//glBlendFunc(GL_ONE, GL_ONE);
	//glDisable(GL_DEPTH_TEST);

	vector<ParticleObject*> PartList = cParticles->ReturnList();
	for (std::vector<ParticleObject*>::iterator it = PartList.begin(); it != PartList.end(); ++it)
	{
		ParticleObject* Part = (ParticleObject*)*it;
		if (Part->active)
		{
			if (Part->TYPE == ParticleObject::PraticleObject_TYPE::P_VIRUS)
			{
				modelStack.PushMatrix();
				modelStack.Translate(Part->pos.x, Part->pos.y, Part->pos.z);
				if (devCam == true)
					modelStack.Rotate(Math::RadianToDegree(atan2(camera.position.x - Part->pos.x, camera.position.z - Part->pos.z)), 0, 1, 0);
				else
					modelStack.Rotate(Math::RadianToDegree(atan2(cCamera.position.x - Part->pos.x, cCamera.position.z - Part->pos.z)), 0, 1, 0);
				modelStack.Scale(Part->scale.x, Part->scale.y, Part->scale.z);
				RenderMesh(meshList[GEO_VIRUS_PARTICLE], false);
				modelStack.PopMatrix();
			}
			else if(Part->TYPE == ParticleObject::PraticleObject_TYPE::P_AIRFRESHNER)
			{
				modelStack.PushMatrix();
				modelStack.Translate(Part->pos.x, Part->pos.y, Part->pos.z);
				if (devCam == true)
					modelStack.Rotate(Math::RadianToDegree(atan2(camera.position.x - Part->pos.x, camera.position.z - Part->pos.z)), 0, 1, 0);
				else
					modelStack.Rotate(Math::RadianToDegree(atan2(cCamera.position.x - Part->pos.x, cCamera.position.z - Part->pos.z)), 0, 1, 0);
				modelStack.Scale(Part->scale.x, Part->scale.y, Part->scale.z);
				RenderMesh(meshList[GEO_VIRUS_PARTICLE], false);
				modelStack.PopMatrix();
			}
			else if (Part->TYPE == ParticleObject::PraticleObject_TYPE::P_VIRUS_SHOT)
			{
				modelStack.PushMatrix();
				modelStack.Translate(Part->pos.x, Part->pos.y, Part->pos.z);
				if (devCam == true)
					modelStack.Rotate(Math::RadianToDegree(atan2(camera.position.x - Part->pos.x, camera.position.z - Part->pos.z)), 0, 1, 0);
				else
					modelStack.Rotate(Math::RadianToDegree(atan2(cCamera.position.x - Part->pos.x, cCamera.position.z - Part->pos.z)), 0, 1, 0);
				modelStack.Scale(Part->scale.x, Part->scale.y, Part->scale.z);
				RenderMesh(meshList[GEO_VIRUS_PARTICLE], false);
				modelStack.PopMatrix();
			}
		}
	}
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void SceneBasic::UpdateParticles(double dt)
{
	dt *= m_speed;
	std::vector<ParticleObject*>inactive_PList;
	if (devCam == true)
		cParticles->Update(dt, camera.position);
	else
		cParticles->Update(dt, cCamera.position);

	

	
}

ParticleObject* SceneBasic::Part_P_One()
{
	//for (unsigned i = 0; i < MAX_PARTICLE; ++i)
	//{
	//	ParticleObject* Part = new ParticleObject(ParticleObject::P_RAIN);
	//
	//	//Rain
	//	Part->rotationSpeed = Math::RandFloatMinMax(20.f, 40.f);
	//	Part->pos.Set(Math::RandFloatMinMax(-m_worldWidth, m_worldWidth), Math::RandFloatMinMax(950, m_worldHeight), Math::RandFloatMinMax(-m_worldWidth, m_worldWidth));		Part-/>rotationSpeed/ = Math::RandFloatMinMax(20.f, 40.f);
	//	Part->pos.Set(Math::RandFloatMinMax(-m_worldWidth, m_worldWidth), Math::RandFloatMinMax(950, m_worldHeight), Math::RandFloatMinMax(-m_worldWidth, m_worldWidth));
	//
	//	++m_objectCount;
	//	m_PList.push_back(Part);
	//
	//}
	//ParticleObject* Part = m_PList.back();
	//return Part;
	return NULL;
}

void SceneBasic::RenderParticles(ParticleObject* Part)
{
}

//Have Shadows
void SceneBasic::RenderPassGPass()
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
	RenderWorld();
	
	RenderPlayer();
	RenderNPC();

	//Render log ccabin
	Vector3 Pos;
	Pos.Set(-310.f, 0, -425.f);
	modelStack.PushMatrix();
	modelStack.Translate(Pos.x, 350.f * ReadHeightMap(m_heightMap, Pos.x / m_worldWidth, Pos.z / m_worldWidth), Pos.z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(4, 4, 4);
	RenderMesh(meshList[GEO_LOGCABIN], true);
	modelStack.PopMatrix();

	//RenderPalmTrees();
}

void SceneBasic::RenderWater()
{
	m_renderPass = RENDER_PASS_MAIN;
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glUseProgram(m_programID);
	m_lightWaterFBO.BindForWriting();
	
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);

	// Camera matrix
	viewStack.LoadIdentity();
	//cout << camera.position << endl;
	float wHeight = 80;
	Vector3 pos;
	Vector3 target;
	if (devCam == true)
	{
		pos = Vector3(camera.position.x, wHeight - (camera.position.y - wHeight), camera.position.z);
		target = Vector3(camera.target.x, wHeight - (camera.target.y - wHeight), camera.target.z);
	}
	else
	{
		pos = Vector3(cCamera.position.x, wHeight - (cCamera.position.y - wHeight), cCamera.position.z);
		target = Vector3(cCamera.target.x, wHeight - (cCamera.target.y - wHeight), cCamera.target.z);
	}

	

	Vector3 view = (target - pos).Normalized();
	Vector3 right = view.Cross(Vector3(0, 1, 0));
	right.y = 0;
	right.Normalize();
	Vector3 up = right.Cross(view).Normalized();


	viewStack.LookAt(
		pos.x,pos.y,pos.z,
		target.x,target.y,target.z,
		up.x, up.y,up.z
	);
	modelStack.LoadIdentity();

	

	RenderSkyPlane();
	//RenderWorld();
	RenderTerrain();
	

	//Render log cabin
	Vector3 Pos;
	Pos.Set(-310.f, 0, -425.f);
	modelStack.PushMatrix();
	modelStack.Translate(Pos.x, 350.f * ReadHeightMap(m_heightMap, Pos.x / m_worldWidth, Pos.z / m_worldWidth), Pos.z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(4, 4, 4);
	RenderMesh(meshList[GEO_LOGCABIN], true);
	modelStack.PopMatrix();

	RenderParticles();
	 
}

void SceneBasic::RenderCProgressBar3D(CProgressBar3D* cProgressBar3D)
{
	//Render HP
	Vector3 vCameraPos = cCamera.position;

	if (devCam == true)
		vCameraPos = camera.position;
	else
		vCameraPos = cCamera.position;


	Vector3 Pos = cProgressBar3D->GetPosition();

	if ((abs((Pos - vCameraPos).Length()) < 350 || cProgressBar3D->GetActive() == true) && m_renderPass == RENDER_PASS_MAIN)
	{
		//Render Background Bar
		modelStack.PushMatrix();

		modelStack.Translate(Pos.x, Pos.y, Pos.z);

		modelStack.Rotate(Math::RadianToDegree(atan2(vCameraPos.x - Pos.x, vCameraPos.z - Pos.z)), 0, 1, 0);

		modelStack.Scale(cProgressBar3D->GetLength(), cProgressBar3D->GetHeight(), cProgressBar3D->GetLength());

		//modelStack.Scale(10,10,10);
		RenderMesh(cProgressBar3D->GetBackgroundMesh(), false);
		modelStack.PopMatrix();

		//Front
		modelStack.PushMatrix();


		//Rotate about center
		modelStack.Translate(Pos.x, Pos.y, Pos.z);
		modelStack.Rotate(Math::RadianToDegree(atan2(vCameraPos.x - Pos.x, vCameraPos.z - Pos.z)), 0, 1, 0);
		modelStack.Translate(-Pos.x, -Pos.y, -Pos.z);

		modelStack.Translate(Pos.x - (cProgressBar3D->GetLength() * cProgressBar3D->GetxScale() * 0.5), Pos.y, Pos.z + 1);

		modelStack.Scale(cProgressBar3D->GetLength() * (cProgressBar3D->GetValue() / cProgressBar3D->GetMaxValue()), cProgressBar3D->GetHeight(), cProgressBar3D->GetLength());

		RenderMesh(cProgressBar3D->GetImageMesh(), false);
		modelStack.PopMatrix();
	}
}

void SceneBasic::RenderPalmTrees()
{
	//Vector3 TPos(0, 193, 0);

	Vector3 T3Pos[20];
	


	Vector3 vCameraPos;
	float fCameraDir;

	
	if (devCam == true)
	{
		vCameraPos = camera.position;
		fCameraDir = camera.up.x;
	}
	else
	{
		vCameraPos = cCamera.position;
		fCameraDir = cPlayer->GetDirection().x;
	}


	
	if (fCameraDir >= 0)
	{
		for (int i = 0; i < 20; ++i)
		{
			T3Pos[i].Set(1320 - (i * 500), 193, 280);
		}
		
	}
	else
	{
		for (int i = 0; i < 20; ++i)
		{
			T3Pos[i].Set(-8180 + (i * 500), 193, 280);
		}
	}

	////Render Palmtree
	//modelStack.PushMatrix();
	//modelStack.Translate(TPos.x, TPos.y, TPos.z);
	//modelStack.Rotate(Math::RadianToDegree(atan2(vCameraPos.x - TPos.x, vCameraPos.z - TPos.z)), 0, 1, 0);
	//modelStack.Scale(200, 200, 200);
	//RenderMesh(meshList[GEO_PALMTREE], false);
	//modelStack.PopMatrix();


	for (int i = 0; i < 20; ++i)
	{
		//Vector3 T3Pos[i](1320 - (i * 500), 193, 280);
		modelStack.PushMatrix();
		modelStack.Translate(T3Pos[i].x, T3Pos[i].y, T3Pos[i].z);
		modelStack.Rotate(Math::RadianToDegree(atan2(vCameraPos.x - T3Pos[i].x, vCameraPos.z - T3Pos[i].z)), 0, 1, 0);
		modelStack.Scale(200, 200, 200);
		RenderMesh(meshList[GEO_PALMTREE], false);
		modelStack.PopMatrix();
	}
}

void SceneBasic::RenderPlayer()
{
	modelStack.PushMatrix();
	modelStack.Translate(cPlayer->GetPosition().x, cPlayer->GetPosition().y, cPlayer->GetPosition().z);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Rotate(Math::RadianToDegree(atan2f(cPlayer->GetDirection().x, cPlayer->GetDirection().z)), 0, 1, 0);

	modelStack.Scale(4, 4, 4);
	RenderMesh(meshList[GEO_PLAYER], false);
	modelStack.PopMatrix();
}

void SceneBasic::RenderWeaponHitBox()
{
	if (cPlayer->GetWeapon()->GetWeaponRange() >= 50)
	{
		modelStack.PushMatrix();
		modelStack.Translate(cPlayer->GetPosition().x + (cPlayer->GetDirection().x * cPlayer->GetWeapon()->GetWeaponRange() * 0.5), cPlayer->GetPosition().y + 5, cPlayer->GetPosition().z + (cPlayer->GetDirection().z * cPlayer->GetWeapon()->GetWeaponRange() * 0.5));
		modelStack.Rotate(-90, 0, 1, 0);
		modelStack.Rotate(Math::RadianToDegree(atan2f(cPlayer->GetDirection().x, cPlayer->GetDirection().z)), 0, 1, 0);
		//modelStack.Scale(cPlayer->GetDirection().x * cPlayer->GetWeapon()->GetWeaponRange(), 1, cPlayer->GetDirection().z * cPlayer->GetWeapon()->GetWeaponRange());
		modelStack.Scale(cPlayer->GetWeapon()->GetWeaponRange(), 1, 20);
		RenderMesh(meshList[GEO_CUBE], false);
		modelStack.PopMatrix();
	}
	else if (cPlayer->GetWeapon()->GetWeaponRange() < 50)
	{
		modelStack.PushMatrix();
		modelStack.Translate(cPlayer->GetPosition().x, cPlayer->GetPosition().y + 5, cPlayer->GetPosition().z);
		modelStack.Scale(12.5, 1, 12.5);
		RenderMesh(meshList[GEO_CIRCLE], false);
		modelStack.PopMatrix();
	}
}

void SceneBasic::RenderNPC()
{

	for (int i = 0; i < vNPCSpawned.size(); ++i)
	{
		if (vNPCSpawned[i]->GetState() != CNPC::STATE::DEAD)
		{
			modelStack.PushMatrix();
			modelStack.Translate(vNPCSpawned[i]->GetPosition().x, vNPCSpawned[i]->GetPosition().y, vNPCSpawned[i]->GetPosition().z);
			modelStack.Rotate(-90, 0, 1, 0);
			modelStack.Rotate(Math::RadianToDegree(atan2f(vNPCSpawned[i]->GetDirection().x, vNPCSpawned[i]->GetDirection().z)), 0, 1, 0);
			modelStack.Scale(4.5, 4.5, 4.5);
			RenderMesh(meshList[GEO_NPC], false);
			modelStack.PopMatrix();
		}
	}
}

void SceneBasic::RenderPickUps()
{
	for (int pu = 0; pu < vPickUps.size(); ++pu)
	{
		if (vPickUps[pu] == nullptr)
			continue;
		if (vPickUps[pu]->GetPickUpType() == CPickUp::PICKUP_TYPE::VIRUS_LAUNCHER)
		{
			modelStack.PushMatrix();
			modelStack.Translate(vPickUps[pu]->GetPosition().x, vPickUps[pu]->GetPosition().y, vPickUps[pu]->GetPosition().z);
			modelStack.Rotate(Math::RadianToDegree(atan2f(cCamera.position.x - vPickUps[pu]->GetPosition().x, cCamera.position.z - vPickUps[pu]->GetPosition().z)), 0, 1, 0);
			modelStack.Scale(2, 2, 2);
			RenderMesh(meshList[GEO_VIRUS_LAUNCHER], true);
			modelStack.PopMatrix();
		}
		else if (vPickUps[pu]->GetPickUpType() == CPickUp::PICKUP_TYPE::AIR_FRESHENER)
		{
			modelStack.PushMatrix();
			modelStack.Translate(vPickUps[pu]->GetPosition().x, vPickUps[pu]->GetPosition().y, vPickUps[pu]->GetPosition().z);
			modelStack.Rotate(Math::RadianToDegree(atan2f(cCamera.position.x - vPickUps[pu]->GetPosition().x, cCamera.position.z - vPickUps[pu]->GetPosition().z)), 0, 1, 0);
			modelStack.Scale(1, 1, 1);
			RenderMesh(meshList[GEO_AIR_FRESHENER], true);
			modelStack.PopMatrix();
		}
		else if (vPickUps[pu]->GetPickUpType() == CPickUp::PICKUP_TYPE::MUTATION_PTS)
		{
			modelStack.PushMatrix();
			modelStack.Translate(vPickUps[pu]->GetPosition().x, vPickUps[pu]->GetPosition().y, vPickUps[pu]->GetPosition().z);
			modelStack.Rotate(Math::RadianToDegree(atan2f(cCamera.position.x - vPickUps[pu]->GetPosition().x, cCamera.position.z - vPickUps[pu]->GetPosition().z)), 0, 1, 0);
			modelStack.Scale(20, 20, 20);
			RenderMesh(meshList[GEO_MUTATION_POINTS], false);
			modelStack.PopMatrix();
		}
	}
}

void SceneBasic::ChangeFliter(FILLTERSTATE eFilterNew)
{
	eFilter = eFilterNew;
	switch (eFilter)
	{
	case NORMAL: 
		bgreymode = false;
		binversemode = false;
		break;
	case GREYSCALE:
		bgreymode = true;
		binversemode = false;
		break;
	case INVERSE:
		bgreymode = false;
		binversemode = true;
		break;
	case INVERSE_GREYSCALE: 
		bgreymode = true;
		binversemode = true;
		break;
	default:
		eFilter = NORMAL;
		bgreymode = false;
		binversemode = false;
		break;
	}
	glUniform1i(m_parameters[U_GREYMODE], bgreymode);
	glUniform1i(m_parameters[U_GLITCHMODE], binversemode);
}

bool SceneBasic::SaveGameValues()
{
	ifstream infile("Save_Values.txt");
	
	if (infile.good())
	{
		ofstream SaveValues;
		SaveValues.open("Save_Values.txt");

		SaveValues << iDayNumber << endl;								//Day No.
		if (cVaccine.GetVacineState())									//State
			SaveValues << to_string(1) << endl;
		else
			SaveValues << to_string(0) << endl;
		SaveValues << to_string(cVaccine.GetAwarness()) << endl;		//Awareness
		SaveValues << to_string(cVaccine.GetProgress()) << endl;		//Progress

		//Weapon Info
		cPlayer->SetCurrentWeapon(1);
		if (cPlayer->GetWeapon() != nullptr)
		{
			SaveValues << to_string(cPlayer->GetWeapon()->GetUses()) << endl;
		}
		else
			SaveValues << "0" << endl;
		cPlayer->SetCurrentWeapon(2);
		if (cPlayer->GetWeapon() != nullptr)
		{
			SaveValues << to_string(cPlayer->GetWeapon()->GetUses()) << endl;
		}
		else
			SaveValues << "0" << endl;
		cPlayer->SetCurrentWeapon(0);

		//Upgrades Info
		SaveValues << to_string(cUpgrade->GetCoughRangeLevel()) << endl;
		SaveValues << to_string(cUpgrade->GetVirusStrengthLevel()) << endl;
		SaveValues << to_string(cUpgrade->GetVirusLethalityLevel()) << endl;
		SaveValues << to_string(cUpgrade->GetLengthOfDayLevel()) << endl;
		SaveValues << to_string(cUpgrade->GetMovementSpeedLevel()) << endl;
		SaveValues << to_string(cUpgrade->GetMutateVirusLevel()) << endl;

		//Virus Info
		SaveValues << cVirus->GetName() << endl;
		SaveValues << to_string(cVirus->GetPoints()) << endl;

		//Game Info
		SaveValues << to_string(fScore) << endl;

		SaveValues.close();
	}
	else
	{
		ofstream  SaveValues("Save_Values.txt");

		if (SaveValues.is_open())
		{
			SaveValues << iDayNumber << endl;								//Day No.
			if (cVaccine.GetVacineState())									//State
				SaveValues << to_string(1) << endl;
			else
				SaveValues << to_string(0) << endl;
			SaveValues << to_string(cVaccine.GetAwarness()) << endl;		//Awareness
			SaveValues << to_string(cVaccine.GetProgress()) << endl;		//Progress

			//Weapon Info
			cPlayer->SetCurrentWeapon(1);
			if (cPlayer->GetWeapon() != nullptr)
			{
				SaveValues << to_string(cPlayer->GetWeapon()->GetUses()) << endl;
			}
			else
				SaveValues << "0" << endl;
			cPlayer->SetCurrentWeapon(2);
			if (cPlayer->GetWeapon() != nullptr)
			{
				SaveValues << to_string(cPlayer->GetWeapon()->GetUses()) << endl;
			}
			else
				SaveValues << "0" << endl;
			cPlayer->SetCurrentWeapon(0);

			//Upgrades Info
			SaveValues << to_string(cUpgrade->GetCoughRangeLevel()) << endl;
			SaveValues << to_string(cUpgrade->GetVirusStrengthLevel()) << endl;
			SaveValues << to_string(cUpgrade->GetVirusLethalityLevel()) << endl;
			SaveValues << to_string(cUpgrade->GetLengthOfDayLevel()) << endl;
			SaveValues << to_string(cUpgrade->GetMovementSpeedLevel()) << endl;
			SaveValues << to_string(cUpgrade->GetMutateVirusLevel()) << endl;

			//Virus Info
			SaveValues << cVirus->GetName() <<endl;
			SaveValues << to_string(cVirus->GetPoints()) << endl;

			//Game Info
			SaveValues << to_string(fScore) << endl;

			SaveValues.close();
		}
	}
	infile.close();
	return true;
}

bool SceneBasic::SaveGameEntities()
{
	ifstream infile("Save_Entities.txt");

	if (infile.good())
	{
		ofstream SaveEntities;
		SaveEntities.open("Save_Entities.txt");

		//Pos
		SaveEntities << cPlayer->GetPosition().x << "," << cPlayer->GetPosition().y << "," << cPlayer->GetPosition().z << ",";
		//Dir
		SaveEntities << cPlayer->GetDirection().x << "," << cPlayer->GetDirection().y << "," << cPlayer->GetDirection().z << "," << endl;

		//NPCs Info
		int size = vNPCS.size();
		for (int i = 0; i < size; i++)
		{
			//Pos
			SaveEntities << vNPCS[i]->GetPosition().x << "," << vNPCS[i]->GetPosition().y << "," << vNPCS[i]->GetPosition().z << ",";
			//Dir
			SaveEntities << vNPCS[i]->GetDirection().x << "," << vNPCS[i]->GetDirection().y << "," << vNPCS[i]->GetDirection().z << ",";
			//Speed
			SaveEntities << vNPCS[i]->GetSpeed() << ",";						
			
			//State (Health = 0, Infected, Dead, Vaccinated)
			if (vNPCS[i]->GetState() == CNPC::HEALTHY)
				SaveEntities << "0" << ",";
			else if (vNPCS[i]->GetState() == CNPC::INFECTED)
				SaveEntities << "1" << ",";
			else if (vNPCS[i]->GetState() == CNPC::DEAD)
				SaveEntities << "2" << ",";
			else if (vNPCS[i]->GetState() == CNPC::VACCINATED)
				SaveEntities << "3" << ",";
			
			SaveEntities << vNPCS[i]->GetHealth() << ",";
			SaveEntities << vNPCS[i]->GetDaysLeft() << "," << endl;
		}
		SaveEntities.close();
	}
	else
	{
		ofstream  SaveEntities("Save_Entities.txt");

		if (SaveEntities.is_open())
		{
			//Pos
			SaveEntities << cPlayer->GetPosition().x << "," << cPlayer->GetPosition().y << "," << cPlayer->GetPosition().z << ",";
			//Dir
			SaveEntities << cPlayer->GetDirection().x << "," << cPlayer->GetDirection().y << "," << cPlayer->GetDirection().z << ",";

			cPlayer->SetCurrentWeapon(1);
			if (cPlayer->GetWeapon() != nullptr)
			{
				SaveEntities << to_string(cPlayer->GetWeapon()->GetUses()) << ",";
			}
			else
				SaveEntities << "0" << ",";
			cPlayer->SetCurrentWeapon(2);
			if (cPlayer->GetWeapon() != nullptr)
			{
				SaveEntities << to_string(cPlayer->GetWeapon()->GetUses()) << ",";
			}
			else
				SaveEntities << "0," << endl;
			cPlayer->SetCurrentWeapon(0);
			
			//NPCs Info
			int size = vNPCS.size();
			for (int i = 0; i < size; i++)
			{
				//Pos
				SaveEntities << vNPCS[i]->GetPosition().x << "," << vNPCS[i]->GetPosition().y << "," << vNPCS[i]->GetPosition().z << ",";
				//Dir
				SaveEntities << vNPCS[i]->GetDirection().x << "," << vNPCS[i]->GetDirection().y << "," << vNPCS[i]->GetDirection().z << ",";
				//Speed
				SaveEntities << vNPCS[i]->GetSpeed() << ",";

				//State (Health = 0, Infected, Dead, Vaccinated)
				if (vNPCS[i]->GetState() == CNPC::HEALTHY)
					SaveEntities << "0" << ",";
				else if (vNPCS[i]->GetState() == CNPC::INFECTED)
					SaveEntities << "1" << ",";
				else if (vNPCS[i]->GetState() == CNPC::DEAD)
					SaveEntities << "2" << ",";
				else if (vNPCS[i]->GetState() == CNPC::VACCINATED)
					SaveEntities << "3" << ",";

				SaveEntities << vNPCS[i]->GetHealth() << ",";
				SaveEntities << vNPCS[i]->GetDaysLeft() << "," << endl;
			}
			SaveEntities.close();
		}
	}
	infile.close();
	return true;
}

//no shadows
void SceneBasic::RenderPassMain()
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
		if (lights[1].type == Light::LIGHT_DIRECTIONAL)
		{
			Vector3 lightDir(lights[1].position.x, lights[1].position.y, lights[1].position.z);
			Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
			glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
		}
		else if (lights[1].type == Light::LIGHT_SPOT)
		{
			Position lightPosition_cameraspace = viewStack.Top() * lights[1].position;
			glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
			Vector3 spotDirection_cameraspace = viewStack.Top() * lights[1].spotDirection;
			glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
		}
		else
		{
			Position lightPosition_cameraspace = viewStack.Top() * lights[1].position;
			glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
		}
		
		switch (eGameState)
		{
		case MAINMENU:
			RenderMainMenu();
			break;
		case GAME:
			RenderGame();
			break;
		}

		glUniform1i(m_parameters[U_FOG_ENABLED], 0);
		if (cTimeControl->isDay())
		{

			//On screen text
			float Textoffset = 43;
			std::ostringstream ss;
			ss.str("");

			//On screen text

			ss.str("");

			//Display FPS
			ss.precision(3);
			ss << "FPS: " << fps;
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 0), 3, 0, Textoffset);
			Textoffset += 3;
		}

		

		RenderUIs(projectionStack, viewStack, modelStack);
		if (fogenabled == true)
		{
			glUniform1i(m_parameters[U_FOG_ENABLED], 1);
		}

}




void SceneBasic::RenderWorld()
{

}


void SceneBasic::InitUIs()
{
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");

	// Sunrise
	Mesh* cSunRiseMesh = MeshBuilder::GenerateSpriteAnimation("cSunRiseMesh", 17, 5);
	cSunRiseMesh->textureArray[0] = LoadTexture("Image//UI/sunrise.png");
	SpriteAnimation* cSunRiseAnim = dynamic_cast<SpriteAnimation*>(cSunRiseMesh);
	cSunRiseAnim->AddAnimation("Default", 0, 81);
	cSunRiseAnim->PlayAnimation("Default", 0, 5.f);
	cSunRise.SetBackgroundMesh(cSunRiseAnim);
	cSunRise.SetBackgroundAnimationEnabled(true);
	cSunRise.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE]);
	cSunRise.SetLength(800);
	cSunRise.SetHeight(600);
	cSunRise.SetPosition(400, 300);
	cSunRise.SetActive(false);

	cSunRiseText.SetBackgroundMesh(nullptr);
	cSunRiseText.SetTextMesh(meshList[GEO_TEXT]);
	cSunRiseText.SetTextColor(Color(10, 10, 0));
	cSunRiseText.SetText("Day 0");
	cSunRiseText.SetTextSize(8);
	cSunRiseText.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cSunRiseText.SetLength(800);
	cSunRiseText.SetHeight(200);
	cSunRiseText.SetPosition(400, 300);
	cSunRiseText.SetFollowMouse(false);
	cSunRiseText.SetCenter(true);
	cSunRiseText.SetActive(false);
	cSunRiseText.SetRenderOrder(0.01);

	Mesh* cSunRiseButtonMesh = MeshBuilder::GenerateQuad("cSunRiseButtonMesh", Color(1, 1, 1), 1.f);
	cSunRiseButtonMesh->textureArray[0] = LoadTexture("Image//UI/buttontemplate.tga");
	cSunRiseButton.SetBackgroundMesh(cSunRiseButtonMesh);
	cSunRiseButton.SetTextMesh(meshList[GEO_TEXT]);
	cSunRiseButton.SetTextColor(Color(10, 10, 10));
	cSunRiseButton.SetText("Start");
	cSunRiseButton.SetTextSize(3.5);
	cSunRiseButton.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cSunRiseButton.SetLength(250);
	cSunRiseButton.SetHeight(75);
	cSunRiseButton.SetPosition(400, 60);
	cSunRiseButton.SetActive(false);
	cSunRiseButton.SetCooldownTime(0.5);
	cSunRiseButton.SetRenderOrder(0.02);

	// Sunset
	Mesh* cSunSetMesh = MeshBuilder::GenerateSpriteAnimation("cSunSetMesh", 9, 5);
	cSunSetMesh->textureArray[0] = LoadTexture("Image//UI/sunset.png");
	SpriteAnimation* cSunSetAnim = dynamic_cast<SpriteAnimation*>(cSunSetMesh);
	cSunSetAnim->AddAnimation("Default", 0, 41);
	cSunSetAnim->PlayAnimation("Default", 0, 3.f);
	cSunSet.SetBackgroundMesh(cSunSetAnim);
	cSunSet.SetBackgroundAnimationEnabled(true);
	cSunSet.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE]);
	cSunSet.SetLength(800);
	cSunSet.SetHeight(600);
	cSunSet.SetPosition(400, 300);
	cSunSet.SetActive(false);

	cSunSetText.SetBackgroundMesh(nullptr);
	cSunSetText.SetTextMesh(meshList[GEO_TEXT]);
	cSunSetText.SetTextColor(Color(10, 10, 0));
	cSunSetText.SetText("Night 0");
	cSunSetText.SetTextSize(8);
	cSunSetText.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cSunSetText.SetLength(800);
	cSunSetText.SetHeight(200);
	cSunSetText.SetPosition(400, 250);
	cSunSetText.SetFollowMouse(false);
	cSunSetText.SetCenter(true);
	cSunSetText.SetActive(false);
	cSunSetText.SetRenderOrder(0.01);

	cSunSetButton.SetBackgroundMesh(cSunRiseButtonMesh);
	cSunSetButton.SetTextMesh(meshList[GEO_TEXT]);
	cSunSetButton.SetTextColor(Color(10, 10, 10));
	cSunSetButton.SetText("Start");
	cSunSetButton.SetTextSize(3.5);
	cSunSetButton.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cSunSetButton.SetLength(250);
	cSunSetButton.SetHeight(75);
	cSunSetButton.SetPosition(400, 60);
	cSunSetButton.SetActive(false);
	cSunSetButton.SetCooldownTime(0.2);
	cSunSetButton.SetRenderOrder(0.02);

	// Upgrades 
	Mesh* cUpgradesMesh = MeshBuilder::GenerateQuad("cUpgradesMesh", Color(1, 1, 1), 1.f);
	cUpgradesMesh->textureArray[0] = LoadTexture("Image//UI//introductionframe.png");
	cUpgradesFrame.SetBackgroundMesh(cUpgradesMesh);
	cUpgradesFrame.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE]);
	cUpgradesFrame.SetLength(800);
	cUpgradesFrame.SetHeight(600);
	cUpgradesFrame.SetPosition(400, 300);
	cUpgradesFrame.SetActive(false);

	cUpgradesTitle.SetBackgroundMesh(nullptr);
	cUpgradesTitle.SetTextMesh(meshList[GEO_TEXT]);
	cUpgradesTitle.SetTextColor(Color(10, 0, 0));
	cUpgradesTitle.SetText("Upgrades");
	cUpgradesTitle.SetTextSize(8);
	cUpgradesTitle.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cUpgradesTitle.SetLength(250);
	cUpgradesTitle.SetHeight(60);
	cUpgradesTitle.SetPosition(400, 540);
	cUpgradesTitle.SetActive(false);
	cUpgradesTitle.SetCenter(true);
	cUpgradesTitle.SetRenderOrder(0.02);

	cUpgradesPoints.SetBackgroundMesh(cUpgradesMesh);
	cUpgradesPoints.SetTextMesh(meshList[GEO_TEXT]);
	cUpgradesPoints.SetTextColor(Color(10, 0, 0));
	cUpgradesPoints.SetText("Mutation Points");
	cUpgradesPoints.SetTextSize(2.5);
	cUpgradesPoints.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cUpgradesPoints.SetLength(220);
	cUpgradesPoints.SetHeight(90);
	cUpgradesPoints.SetPosition(135, 70);
	cUpgradesPoints.SetActive(false);
	cUpgradesPoints.SetRenderOrder(0.02);

	cUpgradesMouseLabel.SetBackgroundMesh(cUpgradesMesh);
	cUpgradesMouseLabel.SetTextMesh(meshList[GEO_TEXT]);
	cUpgradesMouseLabel.SetTextColor(Color(10, 10, 10));
	cUpgradesMouseLabel.SetText("\n I am RED??!!\n Da Ba Dee\n Da Ba Da");
	cUpgradesMouseLabel.SetTextSize(2);
	cUpgradesMouseLabel.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cUpgradesMouseLabel.SetLength(400);
	cUpgradesMouseLabel.SetHeight(300);
	cUpgradesMouseLabel.SetPosition(400, 300);
	cUpgradesMouseLabel.SetActive(false);
	cUpgradesMouseLabel.SetFollowMouse(true);
	cUpgradesMouseLabel.SetRenderOrder(0.1);

	cUpgradesFrame2.SetBackgroundMesh(cUpgradesMesh);
	cUpgradesFrame2.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE]);
	cUpgradesFrame2.SetLength(700);
	cUpgradesFrame2.SetHeight(320);
	cUpgradesFrame2.SetPosition(400, 300);
	cUpgradesFrame2.SetRenderOrder(0.01);
	cUpgradesFrame2.SetActive(false);

	cUpgradesCoughRangeTitle.SetBackgroundMesh(nullptr);
	cUpgradesCoughRangeTitle.SetTextMesh(meshList[GEO_TEXT]);
	cUpgradesCoughRangeTitle.SetTextColor(Color(10, 10, 10));
	cUpgradesCoughRangeTitle.SetText("Cough Range (Lvl 10)");
	cUpgradesCoughRangeTitle.SetTextSize(2.75);
	cUpgradesCoughRangeTitle.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cUpgradesCoughRangeTitle.SetLength(680);
	cUpgradesCoughRangeTitle.SetHeight(40);
	cUpgradesCoughRangeTitle.SetCenter(true);
	cUpgradesCoughRangeTitle.SetPosition(400, 430);
	cUpgradesCoughRangeTitle.SetRenderOrder(0.03);
	cUpgradesCoughRangeTitle.SetActive(false);

	cUpgradesCoughRangeButton.SetBackgroundMesh(cSunRiseButtonMesh);
	cUpgradesCoughRangeButton.SetTextMesh(meshList[GEO_TEXT]);
	cUpgradesCoughRangeButton.SetTextColor(Color(10, 10, 10));
	cUpgradesCoughRangeButton.SetText("UPGRADE");
	cUpgradesCoughRangeButton.SetTextSize(2.5);
	cUpgradesCoughRangeButton.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cUpgradesCoughRangeButton.SetLength(200);
	cUpgradesCoughRangeButton.SetHeight(50);
	cUpgradesCoughRangeButton.SetPosition(400, 380);
	cUpgradesCoughRangeButton.SetCooldownTime(0.2);
	cUpgradesCoughRangeButton.SetRenderOrder(0.03);
	cUpgradesCoughRangeButton.SetActive(false);

	cUpgradesVirusStrengthTitle.SetBackgroundMesh(nullptr);
	cUpgradesVirusStrengthTitle.SetTextMesh(meshList[GEO_TEXT]);
	cUpgradesVirusStrengthTitle.SetTextColor(Color(10, 10, 10));
	cUpgradesVirusStrengthTitle.SetText("Virus Strength (Lvl 10)");
	cUpgradesVirusStrengthTitle.SetTextSize(2.75);
	cUpgradesVirusStrengthTitle.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cUpgradesVirusStrengthTitle.SetLength(680);
	cUpgradesVirusStrengthTitle.SetHeight(40);
	cUpgradesVirusStrengthTitle.SetCenter(true);
	cUpgradesVirusStrengthTitle.SetPosition(400, 330);
	cUpgradesVirusStrengthTitle.SetRenderOrder(0.03);
	cUpgradesVirusStrengthTitle.SetActive(false);

	cUpgradesVirusStrengthButton.SetBackgroundMesh(cSunRiseButtonMesh);
	cUpgradesVirusStrengthButton.SetTextMesh(meshList[GEO_TEXT]);
	cUpgradesVirusStrengthButton.SetTextColor(Color(10, 10, 10));
	cUpgradesVirusStrengthButton.SetText("UPGRADE");
	cUpgradesVirusStrengthButton.SetTextSize(2.5);
	cUpgradesVirusStrengthButton.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cUpgradesVirusStrengthButton.SetLength(200);
	cUpgradesVirusStrengthButton.SetHeight(50);
	cUpgradesVirusStrengthButton.SetPosition(400, 280);
	cUpgradesVirusStrengthButton.SetCooldownTime(0.2);
	cUpgradesVirusStrengthButton.SetRenderOrder(0.03);
	cUpgradesVirusStrengthButton.SetActive(false);

	cUpgradesVirusLethalityTitle.SetBackgroundMesh(nullptr);
	cUpgradesVirusLethalityTitle.SetTextMesh(meshList[GEO_TEXT]);
	cUpgradesVirusLethalityTitle.SetTextColor(Color(10, 10, 10));
	cUpgradesVirusLethalityTitle.SetText("Virus Lethality (Lvl 10)");
	cUpgradesVirusLethalityTitle.SetTextSize(2.75);
	cUpgradesVirusLethalityTitle.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cUpgradesVirusLethalityTitle.SetLength(680);
	cUpgradesVirusLethalityTitle.SetHeight(40);
	cUpgradesVirusLethalityTitle.SetCenter(true);
	cUpgradesVirusLethalityTitle.SetPosition(400, 230);
	cUpgradesVirusLethalityTitle.SetRenderOrder(0.03);
	cUpgradesVirusLethalityTitle.SetActive(false);

	cUpgradesVirusLethalityButton.SetBackgroundMesh(cSunRiseButtonMesh);
	cUpgradesVirusLethalityButton.SetTextMesh(meshList[GEO_TEXT]);
	cUpgradesVirusLethalityButton.SetTextColor(Color(10, 10, 10));
	cUpgradesVirusLethalityButton.SetText("UPGRADE");
	cUpgradesVirusLethalityButton.SetTextSize(2.5);
	cUpgradesVirusLethalityButton.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cUpgradesVirusLethalityButton.SetLength(200);
	cUpgradesVirusLethalityButton.SetHeight(50);
	cUpgradesVirusLethalityButton.SetPosition(400, 180);
	cUpgradesVirusLethalityButton.SetCooldownTime(0.2);
	cUpgradesVirusLethalityButton.SetRenderOrder(0.03);
	cUpgradesVirusLethalityButton.SetActive(false);


	cUpgradesDayLengthTitle.SetBackgroundMesh(nullptr);
	cUpgradesDayLengthTitle.SetTextMesh(meshList[GEO_TEXT]);
	cUpgradesDayLengthTitle.SetTextColor(Color(10, 10, 10));
	cUpgradesDayLengthTitle.SetText("Day Length (Lvl 10)");
	cUpgradesDayLengthTitle.SetTextSize(2.75);
	cUpgradesDayLengthTitle.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cUpgradesDayLengthTitle.SetLength(680);
	cUpgradesDayLengthTitle.SetHeight(40);
	cUpgradesDayLengthTitle.SetCenter(true);
	cUpgradesDayLengthTitle.SetPosition(400, 430);
	cUpgradesDayLengthTitle.SetRenderOrder(0.03);
	cUpgradesDayLengthTitle.SetActive(false);

	cUpgradesDayLengthButton.SetBackgroundMesh(cSunRiseButtonMesh);
	cUpgradesDayLengthButton.SetTextMesh(meshList[GEO_TEXT]);
	cUpgradesDayLengthButton.SetTextColor(Color(10, 10, 10));
	cUpgradesDayLengthButton.SetText("UPGRADE");
	cUpgradesDayLengthButton.SetTextSize(2.5);
	cUpgradesDayLengthButton.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cUpgradesDayLengthButton.SetLength(200);
	cUpgradesDayLengthButton.SetHeight(50);
	cUpgradesDayLengthButton.SetPosition(400, 380);
	cUpgradesDayLengthButton.SetCooldownTime(0.2);
	cUpgradesDayLengthButton.SetRenderOrder(0.03);
	cUpgradesDayLengthButton.SetActive(false);

	cUpgradesPlayerSpeedTitle.SetBackgroundMesh(nullptr);
	cUpgradesPlayerSpeedTitle.SetTextMesh(meshList[GEO_TEXT]);
	cUpgradesPlayerSpeedTitle.SetTextColor(Color(10, 10, 10));
	cUpgradesPlayerSpeedTitle.SetText("Player Speed (Lvl 10)");
	cUpgradesPlayerSpeedTitle.SetTextSize(2.75);
	cUpgradesPlayerSpeedTitle.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cUpgradesPlayerSpeedTitle.SetLength(680);
	cUpgradesPlayerSpeedTitle.SetHeight(40);
	cUpgradesPlayerSpeedTitle.SetCenter(true);
	cUpgradesPlayerSpeedTitle.SetPosition(400, 330);
	cUpgradesPlayerSpeedTitle.SetRenderOrder(0.03);
	cUpgradesPlayerSpeedTitle.SetActive(false);

	cUpgradesPlayerSpeedButton.SetBackgroundMesh(cSunRiseButtonMesh);
	cUpgradesPlayerSpeedButton.SetTextMesh(meshList[GEO_TEXT]);
	cUpgradesPlayerSpeedButton.SetTextColor(Color(10, 10, 10));
	cUpgradesPlayerSpeedButton.SetText("UPGRADE");
	cUpgradesPlayerSpeedButton.SetTextSize(2.5);
	cUpgradesPlayerSpeedButton.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cUpgradesPlayerSpeedButton.SetLength(200);
	cUpgradesPlayerSpeedButton.SetHeight(50);
	cUpgradesPlayerSpeedButton.SetPosition(400, 280);
	cUpgradesPlayerSpeedButton.SetCooldownTime(0.2);
	cUpgradesPlayerSpeedButton.SetRenderOrder(0.03);
	cUpgradesPlayerSpeedButton.SetActive(false);

	cUpgradesMutateVirusTitle.SetBackgroundMesh(nullptr);
	cUpgradesMutateVirusTitle.SetTextMesh(meshList[GEO_TEXT]);
	cUpgradesMutateVirusTitle.SetTextColor(Color(10, 10, 10));;
	cUpgradesMutateVirusTitle.SetText("Mutate Virus");
	cUpgradesMutateVirusTitle.SetTextSize(2.75);
	cUpgradesMutateVirusTitle.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cUpgradesMutateVirusTitle.SetLength(680);
	cUpgradesMutateVirusTitle.SetHeight(40);
	cUpgradesMutateVirusTitle.SetCenter(true);
	cUpgradesMutateVirusTitle.SetPosition(400, 230);
	cUpgradesMutateVirusTitle.SetRenderOrder(0.03);
	cUpgradesMutateVirusTitle.SetActive(false);

	cUpgradesMutateVirusButton.SetBackgroundMesh(cSunRiseButtonMesh);
	cUpgradesMutateVirusButton.SetTextMesh(meshList[GEO_TEXT]);
	cUpgradesMutateVirusButton.SetTextColor(Color(10, 10, 10));
	cUpgradesMutateVirusButton.SetText("UPGRADE");
	cUpgradesMutateVirusButton.SetTextSize(2.5);
	cUpgradesMutateVirusButton.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cUpgradesMutateVirusButton.SetLength(200);
	cUpgradesMutateVirusButton.SetHeight(50);
	cUpgradesMutateVirusButton.SetPosition(400, 180);
	cUpgradesMutateVirusButton.SetCooldownTime(0.2);
	cUpgradesMutateVirusButton.SetRenderOrder(0.03);
	cUpgradesMutateVirusButton.SetActive(false);

	cUpgradesVirusLauncherTitle.SetBackgroundMesh(nullptr);
	cUpgradesVirusLauncherTitle.SetTextMesh(meshList[GEO_TEXT]);
	cUpgradesVirusLauncherTitle.SetTextColor(Color(10, 10, 10));
	cUpgradesVirusLauncherTitle.SetText("Virus Launcher");
	cUpgradesVirusLauncherTitle.SetTextSize(2.75);
	cUpgradesVirusLauncherTitle.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cUpgradesVirusLauncherTitle.SetLength(680);
	cUpgradesVirusLauncherTitle.SetHeight(40);
	cUpgradesVirusLauncherTitle.SetCenter(true);
	cUpgradesVirusLauncherTitle.SetPosition(400, 430);
	cUpgradesVirusLauncherTitle.SetRenderOrder(0.03);
	cUpgradesVirusLauncherTitle.SetActive(false);

	cUpgradesVirusLauncherAmmoText.SetBackgroundMesh(nullptr);
	cUpgradesVirusLauncherAmmoText.SetTextMesh(meshList[GEO_TEXT]);
	cUpgradesVirusLauncherAmmoText.SetTextColor(Color(10, 10, 10));
	cUpgradesVirusLauncherAmmoText.SetText("0/0");
	cUpgradesVirusLauncherAmmoText.SetTextSize(2.75);
	cUpgradesVirusLauncherAmmoText.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cUpgradesVirusLauncherAmmoText.SetLength(200);
	cUpgradesVirusLauncherAmmoText.SetHeight(40);
	cUpgradesVirusLauncherAmmoText.SetCenter(true);
	cUpgradesVirusLauncherAmmoText.SetPosition(400, 380);
	cUpgradesVirusLauncherAmmoText.SetRenderOrder(0.03);
	cUpgradesVirusLauncherAmmoText.SetActive(false);

	cUpgradesVirusLauncherButton.SetBackgroundMesh(cSunRiseButtonMesh);
	cUpgradesVirusLauncherButton.SetTextMesh(meshList[GEO_TEXT]);
	cUpgradesVirusLauncherButton.SetTextColor(Color(10, 10, 10));
	cUpgradesVirusLauncherButton.SetText("BUY 30 AMMO");
	cUpgradesVirusLauncherButton.SetTextSize(2.5);
	cUpgradesVirusLauncherButton.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cUpgradesVirusLauncherButton.SetLength(300);
	cUpgradesVirusLauncherButton.SetHeight(50);
	cUpgradesVirusLauncherButton.SetPosition(400, 330);
	cUpgradesVirusLauncherButton.SetCooldownTime(0.2);
	cUpgradesVirusLauncherButton.SetRenderOrder(0.03);
	cUpgradesVirusLauncherButton.SetActive(false);

	cUpgradesAirFreshenerTitle.SetBackgroundMesh(nullptr);
	cUpgradesAirFreshenerTitle.SetTextMesh(meshList[GEO_TEXT]);
	cUpgradesAirFreshenerTitle.SetTextColor(Color(10, 10, 10));
	cUpgradesAirFreshenerTitle.SetText("Infected Air Freshener");
	cUpgradesAirFreshenerTitle.SetTextSize(2.75);
	cUpgradesAirFreshenerTitle.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cUpgradesAirFreshenerTitle.SetLength(680);
	cUpgradesAirFreshenerTitle.SetHeight(40);
	cUpgradesAirFreshenerTitle.SetCenter(true);
	cUpgradesAirFreshenerTitle.SetPosition(400, 280);
	cUpgradesAirFreshenerTitle.SetRenderOrder(0.03);
	cUpgradesAirFreshenerTitle.SetActive(false);

	cUpgradesAirFreshenerAmmoText.SetBackgroundMesh(nullptr);
	cUpgradesAirFreshenerAmmoText.SetTextMesh(meshList[GEO_TEXT]);
	cUpgradesAirFreshenerAmmoText.SetTextColor(Color(10, 10, 10));
	cUpgradesAirFreshenerAmmoText.SetText("0/0");
	cUpgradesAirFreshenerAmmoText.SetTextSize(2.75);
	cUpgradesAirFreshenerAmmoText.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cUpgradesAirFreshenerAmmoText.SetLength(120);
	cUpgradesAirFreshenerAmmoText.SetHeight(40);
	cUpgradesAirFreshenerAmmoText.SetCenter(true);
	cUpgradesAirFreshenerAmmoText.SetPosition(400, 230);
	cUpgradesAirFreshenerAmmoText.SetRenderOrder(0.03);
	cUpgradesAirFreshenerAmmoText.SetActive(false);

	cUpgradesAirFreshenerButton.SetBackgroundMesh(cSunRiseButtonMesh);
	cUpgradesAirFreshenerButton.SetTextMesh(meshList[GEO_TEXT]);
	cUpgradesAirFreshenerButton.SetTextColor(Color(10, 10, 10));
	cUpgradesAirFreshenerButton.SetText("BUY 5 AMMO");
	cUpgradesAirFreshenerButton.SetTextSize(2.5);
	cUpgradesAirFreshenerButton.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cUpgradesAirFreshenerButton.SetLength(300);
	cUpgradesAirFreshenerButton.SetHeight(50);
	cUpgradesAirFreshenerButton.SetPosition(400, 180);
	cUpgradesAirFreshenerButton.SetCooldownTime(0.2);
	cUpgradesAirFreshenerButton.SetRenderOrder(0.03);
	cUpgradesAirFreshenerButton.SetActive(false);

	Mesh* cUpgradesFrame2ButtonMesh = MeshBuilder::GenerateQuad("cUpgradesFrame2ButtonMesh", Color(1, 1, 1), 1.f);
	cUpgradesFrame2ButtonMesh->textureArray[0] = LoadTexture("Image//UI/frametemplate.png");
	cUpgradesFrame2Button1.SetBackgroundMesh(cUpgradesFrame2ButtonMesh);
	cUpgradesFrame2Button1.SetTextMesh(meshList[GEO_TEXT]);
	cUpgradesFrame2Button1.SetTextColor(Color(10, 10, 10));
	cUpgradesFrame2Button1.SetText("1");
	cUpgradesFrame2Button1.SetTextSize(3);
	cUpgradesFrame2Button1.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cUpgradesFrame2Button1.SetLength(35);
	cUpgradesFrame2Button1.SetHeight(35);
	cUpgradesFrame2Button1.SetPosition(600, 115);
	cUpgradesFrame2Button1.SetActive(false);
	cUpgradesFrame2Button1.SetCooldownTime(0.2);
	cUpgradesFrame2Button1.SetEnlarge(true);
	cUpgradesFrame2Button1.SetRenderOrder(0.04);

	cUpgradesFrame2Button2.SetBackgroundMesh(cUpgradesFrame2ButtonMesh);
	cUpgradesFrame2Button2.SetTextMesh(meshList[GEO_TEXT]);
	cUpgradesFrame2Button2.SetTextColor(Color(10, 10, 10));
	cUpgradesFrame2Button2.SetText("2");
	cUpgradesFrame2Button2.SetTextSize(3);
	cUpgradesFrame2Button2.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cUpgradesFrame2Button2.SetLength(35);
	cUpgradesFrame2Button2.SetHeight(35);
	cUpgradesFrame2Button2.SetPosition(650, 115);
	cUpgradesFrame2Button2.SetActive(false);
	cUpgradesFrame2Button2.SetCooldownTime(0.2);
	cUpgradesFrame2Button2.SetRenderOrder(0.04);

	cUpgradesFrame2Button3.SetBackgroundMesh(cUpgradesFrame2ButtonMesh);
	cUpgradesFrame2Button3.SetTextMesh(meshList[GEO_TEXT]);
	cUpgradesFrame2Button3.SetTextColor(Color(10, 10, 10));
	cUpgradesFrame2Button3.SetText("3");
	cUpgradesFrame2Button3.SetTextSize(3);
	cUpgradesFrame2Button3.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cUpgradesFrame2Button3.SetLength(35);
	cUpgradesFrame2Button3.SetHeight(35);
	cUpgradesFrame2Button3.SetPosition(700, 115);
	cUpgradesFrame2Button3.SetActive(false);
	cUpgradesFrame2Button3.SetCooldownTime(0.2);
	cUpgradesFrame2Button3.SetRenderOrder(0.04);

	cUpgradesNextDayButton.SetBackgroundMesh(cSunRiseButtonMesh);
	cUpgradesNextDayButton.SetTextMesh(meshList[GEO_TEXT]);
	cUpgradesNextDayButton.SetTextColor(Color(10, 10, 10));
	cUpgradesNextDayButton.SetText("Next Day");
	cUpgradesNextDayButton.SetTextSize(3);
	cUpgradesNextDayButton.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cUpgradesNextDayButton.SetLength(250);
	cUpgradesNextDayButton.SetHeight(75);
	cUpgradesNextDayButton.SetPosition(400, 60);
	cUpgradesNextDayButton.SetActive(false);
	cUpgradesNextDayButton.SetCooldownTime(0.5);
	cUpgradesNextDayButton.SetRenderOrder(0.02);

	// Gameplay UIs
	settingsIcon.SetBackgroundMesh(nullptr); // invisible
	Mesh* settingsIconAnim = MeshBuilder::GenerateSpriteAnimation("settingsIconAnimation", 2, 2);
	settingsIconAnim->textureArray[0] = LoadTexture("Image//UI//settings.png");
	SpriteAnimation* saSettingsIconAnim = dynamic_cast<SpriteAnimation*>(settingsIconAnim);
	saSettingsIconAnim->AddAnimation("Default", 0, 4);
	saSettingsIconAnim->PlayAnimation("Default", -1, 0.25f);
	settingsIcon.SetImageMesh(settingsIconAnim);
	settingsIcon.SetAnimationEnabled(true);
	settingsIcon.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE]);
	settingsIcon.SetLength(50);
	settingsIcon.SetHeight(50);
	settingsIcon.SetPosition(30, 30);

	Mesh* settingsBackgroundMesh = MeshBuilder::GenerateSpriteAnimation("settingsBackground", 31, 5);
	settingsBackgroundMesh->textureArray[0] = LoadTexture("Image//UI/settings_background.png");
	SpriteAnimation* settingsBackgroundAnim = dynamic_cast<SpriteAnimation*>(settingsBackgroundMesh);
	settingsBackgroundAnim->AddAnimation("Default", 0, 151);
	settingsBackgroundAnim->AddAnimation("Default", 151, 0);
	settingsBackgroundAnim->PlayAnimation("Default", -1, 20.f);
	settingsBackground.SetBackgroundMesh(settingsBackgroundMesh);
	settingsBackground.SetBackgroundAnimationEnabled(true);
	settingsBackground.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE]);
	settingsBackground.SetLength(800);
	settingsBackground.SetHeight(600);
	settingsBackground.SetPosition(400, 300);
	settingsBackground.SetActive(false);
	settingsBackground.SetRenderOrder(0);

	Mesh* settingsDecorMesh = MeshBuilder::GenerateSpriteAnimation("settingsDecor", 12, 5);
	settingsDecorMesh->textureArray[0] = LoadTexture("Image//UI/virus_spritesheet.png");
	SpriteAnimation* settingsDecorAnim = dynamic_cast<SpriteAnimation*>(settingsDecorMesh);
	settingsDecorAnim->AddAnimation("Default", 0, 56);
	settingsDecorAnim->PlayAnimation("Default", -1, 1.5f);
	settingsDecor.SetBackgroundMesh(settingsDecorAnim);
	settingsDecor.SetBackgroundAnimationEnabled(true);
	settingsDecor.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE]);
	settingsDecor.SetLength(300);
	settingsDecor.SetHeight(300);
	settingsDecor.SetPosition(175, 350);
	settingsDecor.SetActive(false);
	settingsDecor.SetRenderOrder(0.01);


	settingsTitle.SetBackgroundMesh(nullptr);
	settingsTitle.SetTextMesh(meshList[GEO_TEXT]);
	settingsTitle.SetTextColor(Color(1, 1, 1));
	settingsTitle.SetText("Settings");
	settingsTitle.SetTextSize(8);
	settingsTitle.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	settingsTitle.SetLength(800);
	settingsTitle.SetHeight(200);
	settingsTitle.SetPosition(420, 500);
	settingsTitle.SetCooldownTime(0.1);
	settingsTitle.SetFollowMouse(false);
	settingsTitle.SetActive(false);
	settingsTitle.SetRenderOrder(0.01);

	Mesh* settingsCloseButtonMesh = MeshBuilder::GenerateQuad("settingsCloseButton", Color(1, 1, 1), 1.f);
	settingsCloseButtonMesh->textureArray[0] = LoadTexture("Image//UI/closebutton.png");
	settingsCloseButton.SetBackgroundMesh(nullptr); // nothing
	settingsCloseButton.SetImageMesh(settingsCloseButtonMesh);
	settingsCloseButton.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE]);
	settingsCloseButton.SetLength(100);
	settingsCloseButton.SetHeight(100);
	settingsCloseButton.SetPosition(710, 535);
	settingsCloseButton.SetActive(false);
	settingsCloseButton.SetCooldownTime(0.2);
	settingsCloseButton.SetRenderOrder(0.02);

	settingsCheatTitle.SetBackgroundMesh(nullptr);
	settingsCheatTitle.SetTextMesh(meshList[GEO_TEXT]);
	settingsCheatTitle.SetTextColor(Color(1, 1, 1));
	settingsCheatTitle.SetText("Cheat Code:");
	settingsCheatTitle.SetTextSize(4);
	settingsCheatTitle.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	settingsCheatTitle.SetLength(400);
	settingsCheatTitle.SetHeight(50);
	settingsCheatTitle.SetPosition(550, 450);
	settingsCheatTitle.SetCooldownTime(0.1);
	settingsCheatTitle.SetFollowMouse(false);
	settingsCheatTitle.SetActive(false);
	settingsCheatTitle.SetRenderOrder(0.02);

	Mesh* settingsCheatCodeTextBoxMesh = MeshBuilder::GenerateQuad("Testing", Color(1, 1, 1), 1.f);
	settingsCheatCodeTextBox.SetBackgroundMesh(settingsCheatCodeTextBoxMesh);
	settingsCheatCodeTextBox.SetTextMesh(meshList[GEO_TEXT]);
	settingsCheatCodeTextBox.SetTextColor(Color(0, 0, 1));
	settingsCheatCodeTextBox.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	settingsCheatCodeTextBox.SetLength(400);
	settingsCheatCodeTextBox.SetHeight(50);;
	settingsCheatCodeTextBox.SetPosition(565, 400);
	settingsCheatCodeTextBox.SetActive(false);
	settingsCheatCodeTextBox.SetRenderOrder(0.02);

	Mesh* settingsGoMainMenuMesh = MeshBuilder::GenerateQuad("settingsGoMainMenuMesh", Color(1, 0, 1), 1.f);
	settingsGoMainMenuMesh->textureArray[0] = LoadTexture("Image//UI/buttontemplate.tga");
	settingsGoMainMenu.SetBackgroundMesh(settingsGoMainMenuMesh);
	settingsGoMainMenu.SetTextMesh(meshList[GEO_TEXT]);
	settingsGoMainMenu.SetTextColor(Color(0, 0, 1));
	settingsGoMainMenu.SetText("Main Menu");
	settingsGoMainMenu.SetTextSize(3);
	settingsGoMainMenu.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	settingsGoMainMenu.SetLength(280);
	settingsGoMainMenu.SetHeight(100);
	settingsGoMainMenu.SetPosition(565, 275);
	settingsGoMainMenu.SetActive(false);
	settingsGoMainMenu.SetRenderOrder(0.02);

	Mesh* settingsSkipDayMesh = MeshBuilder::GenerateQuad("settingsSkipDayMesh", Color(1, 0, 1), 1.f);
	settingsSkipDayMesh->textureArray[0] = LoadTexture("Image//UI/buttontemplate.tga");
	settingsSkipDay.SetBackgroundMesh(settingsSkipDayMesh);
	settingsSkipDay.SetTextMesh(meshList[GEO_TEXT]);
	settingsSkipDay.SetTextColor(Color(0, 0, 1));
	settingsSkipDay.SetText("Skip Day");
	settingsSkipDay.SetTextSize(3);
	settingsSkipDay.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	settingsSkipDay.SetLength(280);
	settingsSkipDay.SetHeight(100);
	settingsSkipDay.SetPosition(565, 100);
	settingsSkipDay.SetActive(false);
	settingsSkipDay.SetRenderOrder(0.02);

	Mesh* settingsControlsMesh = MeshBuilder::GenerateQuad("settingsControlsMesh", Color(1, 0, 1), 1.f);

	settingsControlsMesh->textureArray[0] = LoadTexture("Image//UI/buttontemplate.tga");

	settingsControls.SetBackgroundMesh(settingsControlsMesh);
	settingsControls.SetTextMesh(meshList[GEO_TEXT]);
	settingsControls.SetTextColor(Color(0, 0, 1));
	settingsControls.SetText("Controls");
	settingsControls.SetTextSize(3);
	settingsControls.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	settingsControls.SetLength(280);
	settingsControls.SetHeight(100);
	settingsControls.SetPosition(200, 100);
	settingsControls.SetActive(false);
	settingsControls.SetRenderOrder(0.02);

	// Controls Frame
	Mesh* cFrame4Mesh = MeshBuilder::GenerateQuad("cFrame4Mesh", Color(1, 1, 1), 1.f);
	cFrame4Mesh->textureArray[0] = LoadTexture("Image//UI/introductionframe.png");
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

	cControlCloseButton.SetBackgroundMesh(settingsControlsMesh);
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

	cCompass.AttachCamera(camera);
	cCompass.SetLength(150);
	cCompass.SetHeight(100);
	cCompass.SetPosition(100, 525);
	cCompass.SetFollowMouse(false);
	cCompass.Init();

	Mesh* cNewsFlashMesh = MeshBuilder::GenerateQuad("Testing", Color(1, 0, 0), 1.f);
	cNewsFlashMesh->textureArray[0] = LoadTexture("Image//UI/newsflash.png");
	cNewsFlash.SetBackgroundMesh(cNewsFlashMesh);
	cNewsFlash.SetTextMesh(meshList[GEO_TEXT]);
	cNewsFlash.SetTextColor(Color(1, 0, 0));
	cNewsFlash.SetLength(350);
	cNewsFlash.SetHeight(40);
	cNewsFlash.SetPosition(400, 550);
	cNewsFlash.SetCooldownTime(0.2);
	cNewsFlash.SetText("Mr Andrew is the best supervisor!");
	cNewsFlash.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);

	cClock.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE]);
	cClock.SetLength(150);
	cClock.SetHeight(150);
	cClock.SetPosition(700, 500);
	cClock.SetCooldownTime(0.2);
	cClock.SetFollowMouse(false);
	cClock.SetClockTime(12, 30);
	cClock.SetRenderOrder(0.02);
	cClock.Init();

	cClockTime.SetTextMesh(meshList[GEO_TEXT]);
	cClockTime.SetCenter(true);
	cClockTime.SetLength(150);
	cClockTime.SetHeight(30);
	cClockTime.SetPosition(700, 400);
	cClockTime.SetRenderOrder(0.05);
	cClockTime.SetText("00:00");
	cClockTime.SetTextColor(Color(10, 10, 10));
	cClockTime.SetTextSize(3);
	cClockTime.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);


	Mesh* statsIconBackGroundMesh = MeshBuilder::GenerateQuad("statsIconBackgroundMesh", Color(1, 1, 1), 1.f);
	statsIconBackGroundMesh->textureArray[0] = LoadTexture("Image//UI/textbutton.png");
	statsIcon.SetBackgroundMesh(statsIconBackGroundMesh); // invisible
	Mesh* statsIconMesh = MeshBuilder::GenerateSpriteAnimation("statsIconMesh", 7, 5);
	statsIconMesh->textureArray[0] = LoadTexture("Image//UI/stats.png");
	SpriteAnimation* statsIconAnim = dynamic_cast<SpriteAnimation*>(statsIconMesh);
	statsIconAnim->AddAnimation("Default", 0, 31);
	statsIconAnim->PlayAnimation("Default", -1, 1.f);
	statsIcon.SetImageMesh(statsIconAnim);
	statsIcon.SetAnimationEnabled(true);
	statsIcon.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE]);
	statsIcon.SetLength(40);
	statsIcon.SetHeight(40);
	statsIcon.SetPosition(770, 30);

	Mesh* statsBackgroundMesh = MeshBuilder::GenerateSpriteAnimation("statsBackground", 40, 5);
	statsBackgroundMesh->textureArray[0] = LoadTexture("Image//UI/stats_background.png");
	SpriteAnimation* statsBackgroundAnim = dynamic_cast<SpriteAnimation*>(statsBackgroundMesh);
	statsBackgroundAnim->AddAnimation("Default", 0, 195); // hehe
	statsBackgroundAnim->PlayAnimation("Default", -1, 10.f);
	statsBackground.SetBackgroundMesh(statsBackgroundMesh);
	statsBackground.SetBackgroundAnimationEnabled(true);
	statsBackground.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE]);
	statsBackground.SetLength(1000);
	statsBackground.SetHeight(600);
	statsBackground.SetPosition(400, 300);
	statsBackground.SetActive(false);
	statsBackground.SetRenderOrder(0);

	Mesh* statsDecorMesh = MeshBuilder::GenerateSpriteAnimation("statsDecor", 7, 5);
	statsDecorMesh->textureArray[0] = LoadTexture("Image//UI/stats_decor.png");
	SpriteAnimation* statsDecorAnim = dynamic_cast<SpriteAnimation*>(statsDecorMesh);
	statsDecorAnim->AddAnimation("Default", 0, 31);
	statsDecorAnim->PlayAnimation("Default", -1, 3.f);
	statsDecor.SetBackgroundMesh(statsDecorMesh);
	statsDecor.SetBackgroundAnimationEnabled(true);
	statsDecor.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE]);
	statsDecor.SetLength(300);
	statsDecor.SetHeight(300);
	statsDecor.SetPosition(150, 250);
	statsDecor.SetActive(false);
	statsDecor.SetRenderOrder(0.01);

	Mesh* statsFrameMesh = MeshBuilder::GenerateQuad("statsFrameMesh", Color(1, 1, 1), 1.f);
	statsFrameMesh->textureArray[0] = LoadTexture("Image//UI/stats_frame.png");
	statsFrame.SetBackgroundMesh(statsFrameMesh);
	statsFrame.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE]);
	statsFrame.SetLength(500);
	statsFrame.SetHeight(500);
	statsFrame.SetPosition(550, 250);
	statsFrame.SetActive(false);
	statsFrame.SetRenderOrder(0.02);

	statsVirusTitle.SetBackgroundMesh(nullptr);
	statsVirusTitle.SetTextMesh(meshList[GEO_TEXT]);
	statsVirusTitle.SetTextColor(Color(1, 1, 1));
	statsVirusTitle.SetText("Virus Name");
	statsVirusTitle.SetTextSize(5);
	statsVirusTitle.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	statsVirusTitle.SetLength(800);
	statsVirusTitle.SetHeight(200);
	statsVirusTitle.SetPosition(420, 475);
	statsVirusTitle.SetCooldownTime(0.1);
	statsVirusTitle.SetFollowMouse(false);
	statsVirusTitle.SetActive(false);
	statsVirusTitle.SetRenderOrder(0.01);

	Mesh* statsCloseButtonMesh = MeshBuilder::GenerateQuad("statsCloseButton", Color(1, 1, 1), 1.f);
	statsCloseButtonMesh->textureArray[0] = LoadTexture("Image//UI/closebutton.png");
	statsCloseButton.SetBackgroundMesh(nullptr); // nothing
	statsCloseButton.SetImageMesh(statsCloseButtonMesh);
	statsCloseButton.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE]);
	statsCloseButton.SetLength(75);
	statsCloseButton.SetHeight(75);
	statsCloseButton.SetPosition(750, 550);
	statsCloseButton.SetActive(false);
	statsCloseButton.SetRenderOrder(0.02);

	statsTotalAlive.SetBackgroundMesh(nullptr);
	statsTotalAlive.SetTextMesh(meshList[GEO_TEXT]);
	statsTotalAlive.SetTextColor(Color(0, 0.5, 0));
	statsTotalAlive.SetText("ERROR");
	statsTotalAlive.SetTextSize(2);
	statsTotalAlive.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	statsTotalAlive.SetLength(450);
	statsTotalAlive.SetHeight(60);
	statsTotalAlive.SetPosition(550, 425);
	statsTotalAlive.SetCooldownTime(0.1);
	statsTotalAlive.SetFollowMouse(false);
	statsTotalAlive.SetActive(false);
	statsTotalAlive.SetRenderOrder(0.03);

	statsTotalInfected.SetBackgroundMesh(nullptr);
	statsTotalInfected.SetTextMesh(meshList[GEO_TEXT]);
	statsTotalInfected.SetTextColor(Color(10, 0, 0));
	statsTotalInfected.SetText("ERROR");
	statsTotalInfected.SetTextSize(2);
	statsTotalInfected.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	statsTotalInfected.SetLength(450);
	statsTotalInfected.SetHeight(60);
	statsTotalInfected.SetPosition(550, 365);
	statsTotalInfected.SetCooldownTime(0.1);
	statsTotalInfected.SetFollowMouse(false);
	statsTotalInfected.SetActive(false);
	statsTotalInfected.SetRenderOrder(0.03);

	statsInfectedSpawned.SetBackgroundMesh(nullptr);
	statsInfectedSpawned.SetTextMesh(meshList[GEO_TEXT]);
	statsInfectedSpawned.SetTextColor(Color(10, 0, 0));
	statsInfectedSpawned.SetText("ERROR");
	statsInfectedSpawned.SetTextSize(2);
	statsInfectedSpawned.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	statsInfectedSpawned.SetLength(450);
	statsInfectedSpawned.SetHeight(60);
	statsInfectedSpawned.SetPosition(550, 305);
	statsInfectedSpawned.SetCooldownTime(0.1);
	statsInfectedSpawned.SetFollowMouse(false);
	statsInfectedSpawned.SetActive(false);
	statsInfectedSpawned.SetRenderOrder(0.03);

	statsInfectionProgressTitle.SetBackgroundMesh(nullptr);
	statsInfectionProgressTitle.SetTextMesh(meshList[GEO_TEXT]);
	statsInfectionProgressTitle.SetTextColor(Color(0, 0, 0));
	statsInfectionProgressTitle.SetText("Infection Progress:");
	statsInfectionProgressTitle.SetTextSize(2);
	statsInfectionProgressTitle.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	statsInfectionProgressTitle.SetLength(450);
	statsInfectionProgressTitle.SetHeight(60);
	statsInfectionProgressTitle.SetPosition(550, 245);
	statsInfectionProgressTitle.SetCooldownTime(0.1);
	statsInfectionProgressTitle.SetFollowMouse(false);
	statsInfectionProgressTitle.SetActive(false);
	statsInfectionProgressTitle.SetRenderOrder(0.03);

	statsInfectionProgress.SetBackgroundMesh(MeshBuilder::GenerateQuad("statsInfectionProgressBackground", Color(1, 0, 0), 1.f));
	statsInfectionProgress.SetImageMesh(MeshBuilder::GenerateQuad("statsInfectionProgress", Color(0, 1, 0), 1.f));
	statsInfectionProgress.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE]);
	statsInfectionProgress.SetLength(400);
	statsInfectionProgress.SetHeight(30);
	statsInfectionProgress.SetPosition(530, 225);
	statsInfectionProgress.SetActive(false);
	statsInfectionProgress.SetFollowMouse(false);
	statsInfectionProgress.SetRenderOrder(0.04);

	statsVaccineTitle.SetBackgroundMesh(nullptr);
	statsVaccineTitle.SetTextMesh(meshList[GEO_TEXT]);
	statsVaccineTitle.SetTextColor(Color(0, 0, 0));
	statsVaccineTitle.SetText("Vaccine Progress:");
	statsVaccineTitle.SetTextSize(2);
	statsVaccineTitle.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	statsVaccineTitle.SetLength(450);
	statsVaccineTitle.SetHeight(60);
	statsVaccineTitle.SetPosition(550, 165);
	statsVaccineTitle.SetCooldownTime(0.1);
	statsVaccineTitle.SetFollowMouse(false);
	statsVaccineTitle.SetActive(false);
	statsVaccineTitle.SetRenderOrder(0.03);

	statsVaccineProgress.SetBackgroundMesh(MeshBuilder::GenerateQuad("statsVaccineProgressBackground", Color(1, 0, 0), 1.f));
	statsVaccineProgress.SetImageMesh(MeshBuilder::GenerateQuad("statsVaccineProgress", Color(0, 1, 0), 1.f));
	statsVaccineProgress.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE]);
	statsVaccineProgress.SetLength(400);
	statsVaccineProgress.SetHeight(30);
	statsVaccineProgress.SetPosition(530, 145);
	statsVaccineProgress.SetActive(false);
	statsVaccineProgress.SetFollowMouse(false);
	statsVaccineProgress.SetRenderOrder(0.04);

	statsScore.SetBackgroundMesh(nullptr);
	statsScore.SetTextMesh(meshList[GEO_TEXT]);
	statsScore.SetTextColor(Color(0, 0, 0));
	statsScore.SetText("Score:");
	statsScore.SetTextSize(2);
	statsScore.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	statsScore.SetLength(450);
	statsScore.SetHeight(60);
	statsScore.SetPosition(550, 95);
	statsScore.SetCooldownTime(0.1);
	statsScore.SetFollowMouse(false);
	statsScore.SetActive(false);
	statsScore.SetRenderOrder(0.03);

	statsMutationPoints.SetBackgroundMesh(nullptr);
	statsMutationPoints.SetTextMesh(meshList[GEO_TEXT]);
	statsMutationPoints.SetTextColor(Color(0, 0, 0));
	statsMutationPoints.SetText("Mutation Points:");
	statsMutationPoints.SetTextSize(2);
	statsMutationPoints.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	statsMutationPoints.SetLength(450);
	statsMutationPoints.SetHeight(60);
	statsMutationPoints.SetPosition(550, 55);
	statsMutationPoints.SetCooldownTime(0.1);
	statsMutationPoints.SetFollowMouse(false);
	statsMutationPoints.SetActive(false);
	statsMutationPoints.SetRenderOrder(0.03);


	Mesh* inventoryFrame = MeshBuilder::GenerateQuad("inventoryFrame", Color(1, 0, 0), 1.f);
	inventoryFrame->textureArray[0] = LoadTexture("Image//UI/inventoryframe.png");

	Mesh* inventorySlot1Mesh = MeshBuilder::GenerateQuad("inventorySlot1Mesh", Color(1, 0, 0), 1.f);
	inventorySlot1Mesh->textureArray[0] = LoadTexture("Image//UI/inventoryslot1.png");
	cInventorySlot1.SetBackgroundMesh(inventoryFrame);
	cInventorySlot1.SetImageMesh(inventorySlot1Mesh);
	cInventorySlot1.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE]);
	cInventorySlot1.SetLength(100);
	cInventorySlot1.SetHeight(100);
	cInventorySlot1.SetPosition(200, 60);
	cInventorySlot1.SetFollowMouse(false);
	cInventorySlot1.SetRenderOrder(0.01);

	cInventorySlot1Label.SetBackgroundMesh(nullptr);
	cInventorySlot1Label.SetTextMesh(meshList[GEO_TEXT]);
	cInventorySlot1Label.SetTextColor(Color(10, 10, 10));
	cInventorySlot1Label.SetText("[1]");
	cInventorySlot1Label.SetTextSize(2);
	cInventorySlot1Label.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cInventorySlot1Label.SetLength(220);
	cInventorySlot1Label.SetHeight(20);
	cInventorySlot1Label.SetPosition(200, 60);
	cInventorySlot1Label.SetFollowMouse(false);
	cInventorySlot1Label.SetRenderOrder(0.03);

	Mesh* inventorySlot2Mesh = MeshBuilder::GenerateQuad("inventorySlot2Mesh", Color(1, 0, 0), 1.f);
	inventorySlot2Mesh->textureArray[0] = LoadTexture("Image//UI/inventoryslot2.png");
	cInventorySlot2.SetBackgroundMesh(inventoryFrame);
	cInventorySlot2.SetImageMesh(inventorySlot2Mesh);
	cInventorySlot2.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE]);
	cInventorySlot2.SetLength(100);
	cInventorySlot2.SetHeight(100);
	cInventorySlot2.SetPosition(400, 60);
	cInventorySlot2.SetFollowMouse(false);
	cInventorySlot2.SetRenderOrder(0.01);

	cInventorySlot2Label.SetBackgroundMesh(nullptr);
	cInventorySlot2Label.SetTextMesh(meshList[GEO_TEXT]);
	cInventorySlot2Label.SetTextColor(Color(10, 10, 10));
	cInventorySlot2Label.SetText("[2]");
	cInventorySlot2Label.SetTextSize(2);
	cInventorySlot2Label.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cInventorySlot2Label.SetLength(220);
	cInventorySlot2Label.SetHeight(20);
	cInventorySlot2Label.SetPosition(400, 60);
	cInventorySlot2Label.SetFollowMouse(false);
	cInventorySlot2Label.SetRenderOrder(0.03);

	cInventorySlot2Ammo.SetBackgroundMesh(nullptr);
	cInventorySlot2Ammo.SetTextMesh(meshList[GEO_TEXT]);
	cInventorySlot2Ammo.SetTextColor(Color(10, 10, 10));
	cInventorySlot2Ammo.SetText("0/0");
	cInventorySlot2Ammo.SetCenter(true);
	cInventorySlot2Ammo.SetTextSize(1.5);
	cInventorySlot2Ammo.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cInventorySlot2Ammo.SetLength(110);
	cInventorySlot2Ammo.SetHeight(20);
	cInventorySlot2Ammo.SetPosition(400, 25);
	cInventorySlot2Ammo.SetFollowMouse(false);
	cInventorySlot2Ammo.SetRenderOrder(0.02);

	Mesh* inventorySlot3Mesh = MeshBuilder::GenerateQuad("inventorySlot3Mesh", Color(1, 0, 0), 1.f);
	inventorySlot3Mesh->textureArray[0] = LoadTexture("Image//UI/inventoryslot3.png");
	cInventorySlot3.SetBackgroundMesh(inventoryFrame);
	cInventorySlot3.SetImageMesh(inventorySlot3Mesh);
	cInventorySlot3.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE]);
	cInventorySlot3.SetLength(100);
	cInventorySlot3.SetHeight(100);
	cInventorySlot3.SetPosition(600, 60);
	cInventorySlot3.SetFollowMouse(false);
	cInventorySlot3.SetRenderOrder(0.01);

	cInventorySlot3Label.SetBackgroundMesh(nullptr);
	cInventorySlot3Label.SetTextMesh(meshList[GEO_TEXT]);
	cInventorySlot3Label.SetTextColor(Color(10, 10, 10));
	cInventorySlot3Label.SetText("[3]");
	cInventorySlot3Label.SetTextSize(2);
	cInventorySlot3Label.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cInventorySlot3Label.SetLength(220);
	cInventorySlot3Label.SetHeight(20);
	cInventorySlot3Label.SetPosition(600, 60);
	cInventorySlot3Label.SetFollowMouse(false);
	cInventorySlot3Label.SetRenderOrder(0.03);

	cInventorySlot3Ammo.SetBackgroundMesh(nullptr);
	cInventorySlot3Ammo.SetTextMesh(meshList[GEO_TEXT]);
	cInventorySlot3Ammo.SetTextColor(Color(10, 10, 10));
	cInventorySlot3Ammo.SetText("0");
	cInventorySlot3Ammo.SetCenter(true);
	cInventorySlot3Ammo.SetTextSize(1.5);
	cInventorySlot3Ammo.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cInventorySlot3Ammo.SetLength(110);
	cInventorySlot3Ammo.SetHeight(20);
	cInventorySlot3Ammo.SetPosition(600, 25);
	cInventorySlot3Ammo.SetFollowMouse(false);
	cInventorySlot3Ammo.SetRenderOrder(0.02);


	// Win UIs
	cWinFrame.SetBackgroundMesh(cFrame4Mesh);
	cWinFrame.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE]);
	cWinFrame.SetLength(800);
	cWinFrame.SetHeight(600);
	cWinFrame.SetPosition(400, 300);
	cWinFrame.SetRenderOrder(0);

	Mesh* cWinDecorMesh = MeshBuilder::GenerateSpriteAnimation("cWinDecor", 7, 5);
	cWinDecorMesh->textureArray[0] = LoadTexture("Image//UI/win_decor.png");
	SpriteAnimation* cWinDecorAnim = dynamic_cast<SpriteAnimation*>(cWinDecorMesh);
	cWinDecorAnim->AddAnimation("Default", 0, 31);
	cWinDecorAnim->PlayAnimation("Default", -1, 3.f);
	cWinDecor.SetBackgroundMesh(cWinDecorMesh);
	cWinDecor.SetBackgroundAnimationEnabled(true);
	cWinDecor.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE]);
	cWinDecor.SetLength(300);
	cWinDecor.SetHeight(300);
	cWinDecor.SetPosition(200, 300);
	cWinDecor.SetRenderOrder(0.01);


	cWinText.SetBackgroundMesh(nullptr);
	cWinText.SetTextMesh(meshList[GEO_TEXT]);
	cWinText.SetTextColor(Color(10, 10, 10));
	cWinText.SetText("Congratulations!\nYou beat the game\nin");
	cWinText.SetTextSize(2.5);
	cWinText.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cWinText.SetLength(380);
	cWinText.SetHeight(200);
	cWinText.SetCenter(true);
	cWinText.SetPosition(550, 350);
	cWinText.SetRenderOrder(0.03);

	cWinDays.SetBackgroundMesh(nullptr);
	cWinDays.SetTextMesh(meshList[GEO_TEXT]);
	cWinDays.SetTextColor(Color(10, 10, 10));
	cWinDays.SetText("- Days");
	cWinDays.SetTextSize(5);
	cWinDays.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cWinDays.SetLength(380);
	cWinDays.SetHeight(200);
	cWinDays.SetCenter(true);
	cWinDays.SetPosition(550, 250);
	cWinDays.SetRenderOrder(0.03);

	Mesh* cWinButtonMesh = MeshBuilder::GenerateQuad("cWinButtonMesh", Color(1, 0, 1), 1.f);
	cWinButtonMesh->textureArray[0] = LoadTexture("Image//UI/buttontemplate.tga");
	cWinButton.SetBackgroundMesh(cWinButtonMesh);
	cWinButton.SetTextMesh(meshList[GEO_TEXT]);
	cWinButton.SetTextColor(Color(10, 10, 10));
	cWinButton.SetText("Main Menu");
	cWinButton.SetTextSize(3);
	cWinButton.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cWinButton.SetLength(280);
	cWinButton.SetHeight(100);
	cWinButton.SetPosition(550, 100);
	cWinButton.SetRenderOrder(0.05);

	// Lose UIs
	cLoseFrame.SetBackgroundMesh(cFrame4Mesh);
	cLoseFrame.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE]);
	cLoseFrame.SetLength(800);
	cLoseFrame.SetHeight(600);
	cLoseFrame.SetPosition(400, 300);
	cLoseFrame.SetRenderOrder(0);

	Mesh* cLoseDecorMesh = MeshBuilder::GenerateSpriteAnimation("cLoseDecor", 12, 5);
	cLoseDecorMesh->textureArray[0] = LoadTexture("Image//UI/lose_decor.png");
	SpriteAnimation* cLoseDecorAnim = dynamic_cast<SpriteAnimation*>(cLoseDecorMesh);
	cLoseDecorAnim->AddAnimation("Default", 0, 60);
	cLoseDecorAnim->PlayAnimation("Default", -1, 3.f);
	cLoseDecor.SetBackgroundMesh(cLoseDecorMesh);
	cLoseDecor.SetBackgroundAnimationEnabled(true);
	cLoseDecor.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE]);
	cLoseDecor.SetLength(300);
	cLoseDecor.SetHeight(300);
	cLoseDecor.SetPosition(200, 300);
	cLoseDecor.SetRenderOrder(0.01);


	cLoseText.SetBackgroundMesh(nullptr);
	cLoseText.SetTextMesh(meshList[GEO_TEXT]);
	cLoseText.SetTextColor(Color(10, 10, 10));
	cLoseText.SetText("Your virus\nis dead!\nYour score is");
	cLoseText.SetTextSize(2.5);
	cLoseText.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cLoseText.SetLength(380);
	cLoseText.SetHeight(200);
	cLoseText.SetCenter(true);
	cLoseText.SetPosition(550, 350);
	cLoseText.SetRenderOrder(0.03);

	cLoseScore.SetBackgroundMesh(nullptr);
	cLoseScore.SetTextMesh(meshList[GEO_TEXT]);
	cLoseScore.SetTextColor(Color(10, 10, 10));
	cLoseScore.SetText("- Days");
	cLoseScore.SetTextSize(5);
	cLoseScore.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cLoseScore.SetLength(380);
	cLoseScore.SetHeight(200);
	cLoseScore.SetCenter(true);
	cLoseScore.SetPosition(550, 250);
	cLoseScore.SetRenderOrder(0.03);

	Mesh* cLoseButtonMesh = MeshBuilder::GenerateQuad("cLoseButtonMesh", Color(1, 0, 1), 1.f);
	cLoseButtonMesh->textureArray[0] = LoadTexture("Image//UI/buttontemplate.tga");
	cLoseButton.SetBackgroundMesh(cLoseButtonMesh);
	cLoseButton.SetTextMesh(meshList[GEO_TEXT]);
	cLoseButton.SetTextColor(Color(10, 10, 10));
	cLoseButton.SetText("Main Menu");
	cLoseButton.SetTextSize(3);
	cLoseButton.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_TEXT_ENABLED], m_parameters[U_TEXT_COLOR], m_parameters[U_LIGHTENABLED]);
	cLoseButton.SetLength(280);
	cLoseButton.SetHeight(100);
	cLoseButton.SetPosition(550, 100);
	cLoseButton.SetRenderOrder(0.05);
	
	//cLoading Bar
	Mesh* cLoadingBarBackground = MeshBuilder::GenerateQuad("cLoadingBarBackground", Color(1, 1, 1), 1.f);
	cLoadingBarBackground->textureArray[0] = LoadTexture("Image//UI/Virus_HPBarGreen.tga");
	Mesh* cLoadingBarFront = MeshBuilder::GenerateQuad("cLoadingBarFront", Color(1, 1, 1), 1.f);
	cLoadingBarFront->textureArray[0] = LoadTexture("Image//UI/Virus_HPGreen.tga");

	cLoadingBar.SetImageMesh(cLoadingBarFront);
	cLoadingBar.SetBackgroundMesh(cLoadingBarBackground);
	cLoadingBar.SetLength(750);
	cLoadingBar.SetHeight(100);
	cLoadingBar.SetPosition(400, 100);
	cLoadingBar.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE]);
	cLoadingBar.SetRenderOrder(0.2);
	cLoadingBar.SetActive(false);
	cLoadingBar.SetFollowMouse(false);

	cLoadScreen.SetBackgroundMesh(meshList[GEO_LOAD_QUAD]);
	cLoadScreen.SetLength(800);
	cLoadScreen.SetHeight(600);
	cLoadScreen.SetPosition(400, 300);
	cLoadScreen.AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE]);
	cLoadScreen.SetRenderOrder(0.1);
	cLoadScreen.SetActive(false);
}

void SceneBasic::InitcProgressBar3D(CProgressBar3D* cProgressBar3D)
{
	cProgressBar3D->AddBackgroundMesh(meshList[GEO_HPBAR_GREEN]);
	cProgressBar3D->AddBackgroundMesh(meshList[GEO_HPBAR_YELLOW]);
	cProgressBar3D->AddBackgroundMesh(meshList[GEO_HP_RED]);
	cProgressBar3D->AddImageMesh(meshList[GEO_HP_GREEN]);
	cProgressBar3D->AddImageMesh(meshList[GEO_HP_YELLOW]);
	cProgressBar3D->AddImageMesh(meshList[GEO_HP_RED]);
	cProgressBar3D->AttachParameters(m_parameters[U_MVP], m_parameters[U_COLOR_TEXTURE_ENABLED], m_parameters[U_COLOR_TEXTURE], m_parameters[U_MODELVIEW], m_parameters[U_MODELSTACK]);
	cProgressBar3D->SetLength(30);
	cProgressBar3D->SetHeight(10);
}

void SceneBasic::UpdateUIs(double dt)
{
	double dcurrenttime = GetTickCount64();
	if (eGameState == GAME)
	{
		if (cTimeControl->isDay())
		{
			// Gameplay UIs
			if (settingsIcon.isActive())
			{
				settingsIcon.Update(dt);
				if (settingsIcon.IsMouseHover())
				{

					settingsIcon.SetEnlarge(true);
					settingsIcon.StartAnimation();
					if (settingsIcon.isLeftMouseButtonPressed())
					{
						cTimeControl->SetTimePaused(true);

						if (bouncetime[3] <= GetTickCount64())
						{
							cSoundController->PlaySoundByID(5);
							bouncetime[3] = GetTickCount64() + 100;
						}

						// UIs to Disable
						settingsIcon.SetActive(false);
						statsIcon.SetActive(false);
						cClock.SetActive(false);
						cClockTime.SetActive(false);
						cCompass.SetActive(false);
						cNewsFlash.SetActive(false);
						cInventorySlot1.SetActive(false);
						cInventorySlot2.SetActive(false);
						cInventorySlot2Ammo.SetActive(false);
						cInventorySlot3.SetActive(false);
						cInventorySlot3Ammo.SetActive(false);
						cInventorySlot1Label.SetActive(false);
						cInventorySlot2Label.SetActive(false);
						cInventorySlot3Label.SetActive(false);

						// UIs to Enable
						settingsBackground.SetActive(true);
						settingsDecor.SetActive(true);
						settingsTitle.SetActive(true);
						settingsCloseButton.SetActive(true);
						settingsCheatTitle.SetActive(true);
						settingsCheatCodeTextBox.SetActive(true);
						settingsGoMainMenu.SetActive(true);
						settingsSkipDay.SetActive(true);
						settingsControls.SetActive(true);
					}
				}
				else
				{
					settingsIcon.SetEnlarge(false);
					settingsIcon.StopAnimation();
				}
			}
			if (settingsBackground.isActive())
			{
				settingsBackground.Update(dt);
			}
			if (settingsDecor.isActive() && settingsDecor.IsMouseHover())
			{
				settingsDecor.Update(dt);
			}
			if (settingsTitle.isActive())
			{
				settingsTitle.Update(dt);
			}
			if (settingsCloseButton.isActive())
			{
				if (settingsCloseButton.IsMouseHover())
				{
					settingsCloseButton.Update(dt);
					settingsCloseButton.SetEnlarge(true);
					if (settingsCloseButton.isLeftMouseButtonPressed() && settingsCloseButton.isCooldownOver())
					{
						cTimeControl->SetTimePaused(false);

						if (bouncetime[3] <= GetTickCount64())
						{
							cSoundController->PlaySoundByID(5);
							bouncetime[3] = GetTickCount64() + 100;
						}


						// UIs to enable
						settingsIcon.SetActive(true);
						statsIcon.SetActive(true);
						cClock.SetActive(true);
						cClockTime.SetActive(true);
						cCompass.SetActive(true);
						cNewsFlash.SetActive(true);
						cInventorySlot1.SetActive(true);
						cInventorySlot2.SetActive(true);
						cInventorySlot2Ammo.SetActive(true);
						cInventorySlot3.SetActive(true);
						cInventorySlot3Ammo.SetActive(true);
						cInventorySlot1Label.SetActive(true);
						cInventorySlot2Label.SetActive(true);
						cInventorySlot3Label.SetActive(true);

						// UIs to disable
						settingsBackground.SetActive(false);
						settingsDecor.SetActive(false);
						settingsTitle.SetActive(false);
						settingsCloseButton.SetActive(false);
						settingsCheatTitle.SetActive(false);
						settingsCheatCodeTextBox.SetActive(false);
						settingsGoMainMenu.SetActive(false);
						settingsSkipDay.SetActive(false);
						settingsControls.SetActive(false);
					}
				}
				else
				{
					settingsCloseButton.SetEnlarge(false);
				}
			}
			if (settingsCheatTitle.isActive())
			{
				settingsCheatTitle.Update(dt);
			}
			if (settingsCheatCodeTextBox.isActive())
			{
				settingsCheatCodeTextBox.Update(dt);
				if (CGUI::IsKeyPressed(VK_RETURN))
				{
					// Send text to somewhere first...
					if (settingsCheatCodeTextBox.GetText() == "MOARPOINTS")
					{
						cVirus->IncreasePoints(200);
						cSoundController->PlaySoundByID(6);
					}
					else if (settingsCheatCodeTextBox.GetText() == "INFPOINTS")
					{
						cVirus->IncreasePoints(999999999);
						cSoundController->PlaySoundByID(6);

					}
					else if (settingsCheatCodeTextBox.GetText() == "MAINMENU")
					{
						Application::SetSceneType(Application::INTRO);
						cSoundController->PlaySoundByID(6);

					}
					else if (settingsCheatCodeTextBox.GetText() == "WIN")
					{
						eGameState = WIN;
						cSoundController->PlaySoundByID(7);
					}
					else if (settingsCheatCodeTextBox.GetText() == "LOSS")
					{
						eGameState = LOSS;
						cSoundController->PlaySoundByID(8);


					}
					else if (settingsCheatCodeTextBox.GetText() == "NIOR")
					{
						ChangeFliter(FILLTERSTATE::GREYSCALE);
						cSoundController->PlaySoundByID(6);

					}
					else if (settingsCheatCodeTextBox.GetText() == "MOONSIDE")
					{
						ChangeFliter(FILLTERSTATE::INVERSE);
						cSoundController->PlaySoundByID(6);

					}
					else if (settingsCheatCodeTextBox.GetText() == "MOONNIOR")
					{
						ChangeFliter(FILLTERSTATE::INVERSE_GREYSCALE);
						cSoundController->PlaySoundByID(6);

					}
					else if (settingsCheatCodeTextBox.GetText() == "WIRES")
					{
						glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
						cSoundController->PlaySoundByID(6);
					}
					else if (settingsCheatCodeTextBox.GetText() == "NORMAL")
					{
						ChangeFliter(FILLTERSTATE::NORMAL);
						cSoundController->PlaySoundByID(6);
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					}

					settingsCheatCodeTextBox.ClearText();
					settingsCheatCodeTextBox.SetEnabled(false);
				}
				
			}
			if (settingsGoMainMenu.isActive())
			{
				settingsGoMainMenu.Update(dt);
				if (settingsGoMainMenu.IsMouseHover())
				{
					settingsGoMainMenu.SetEnlarge(true);
					if (settingsGoMainMenu.isLeftMouseButtonPressed())
					{
						Application::SetSceneType(Application::INTRO);
						cLoadingBar.SetActive(true);
						cLoadScreen.SetActive(true);
						if (bouncetime[3] <= GetTickCount64())
						{
							cSoundController->PlaySoundByID(5);
							bouncetime[3] = GetTickCount64() + 100;
						}

					}
				}
				else
				{
					settingsGoMainMenu.SetEnlarge(false);
				}
			}
			if (settingsSkipDay.isActive())
			{
				settingsSkipDay.Update(dt);
				if (settingsSkipDay.IsMouseHover())
				{
					settingsSkipDay.SetEnlarge(true);
					if (settingsSkipDay.isLeftMouseButtonPressed())
					{
						dTimeToNight = dcurrenttime;
						if (bouncetime[3] <= GetTickCount64())
						{
							cSoundController->PlaySoundByID(5);
							bouncetime[3] = GetTickCount64() + 100;
						}

					}
				}
				else
				{
					settingsSkipDay.SetEnlarge(false);
				}
			}
			if (settingsControls.isActive())
			{
				settingsControls.Update(dt);
				if (settingsControls.IsMouseHover())
				{
					settingsControls.SetEnlarge(true);
					if (settingsControls.isLeftMouseButtonPressed())
					{
						if (bouncetime[3] <= GetTickCount64())
						{
							cSoundController->PlaySoundByID(5);
							bouncetime[3] = GetTickCount64() + 100;
						}

						cControlCloseButton.SetActive(true);
						cControlFrame.SetActive(true);
						cControlText.SetActive(true);

						settingsBackground.SetActive(false);
						settingsDecor.SetActive(false);
						settingsTitle.SetActive(false);
						settingsCloseButton.SetActive(false);
						settingsCheatTitle.SetActive(false);
						settingsCheatCodeTextBox.SetActive(false);
						settingsGoMainMenu.SetActive(false);
						settingsSkipDay.SetActive(false);
						settingsControls.SetActive(false);
					}
				}
				else
				{
					settingsControls.SetEnlarge(false);
				}
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
							cSoundController->PlaySoundByID(5);
							bouncetime[3] = GetTickCount64() + 100;
						}

						cControlCloseButton.SetActive(false);
						cControlFrame.SetActive(false);
						cControlText.SetActive(false);

						settingsBackground.SetActive(true);
						settingsDecor.SetActive(true);
						settingsTitle.SetActive(true);
						settingsCloseButton.SetActive(true);
						settingsCheatTitle.SetActive(true);
						settingsCheatCodeTextBox.SetActive(true);
						settingsGoMainMenu.SetActive(true);
						settingsSkipDay.SetActive(true);
						settingsControls.SetActive(true);
						settingsCloseButton.ResetTimer();
					}
				}
				else
				{
					cControlCloseButton.SetEnlarge(false);
				}
			}


			if (cCompass.isActive())
			{
				if (devCam == true)
					cCompass.AttachCamera(camera);
				else cCompass.AttachCamera(cCamera);
				cCompass.Update(dt);
			}
			if (cNewsFlash.isActive())
			{
				cNewsFlash.Update(dt);
				if (cVaccine.GetProgress() > 75)
				{
					std::string news = "Vaccine nearing completion, 75% complete! ~Dr Andrew Lam";
					if (cNewsFlash.GetText() != news)
						cNewsFlash.SetText(news);
				}
				else if (cVaccine.GetProgress() > 50)
				{
					std::string news = "Vaccine 50% complete, with Dr Andrew Lam leading in this research!";
					if (cNewsFlash.GetText() != news)
						cNewsFlash.SetText(news);
				}
				else if (cVaccine.GetProgress() > 25)
				{
					std::string news = "Dr Andrew Lam has made a huge progress for the vaccine, the progress is now 25% complete.";
					if (cNewsFlash.GetText() != news)
						cNewsFlash.SetText(news);
				}
				else
				{
					std::string news = "A new virus has been discovered! Vaccine Researcher, Dr Andrew Lam has named it \"" + cVirus->GetName() + "\"!";
					if (cNewsFlash.GetText() != news)
						cNewsFlash.SetText(news);
				}
				if (cNewsFlash.IsMouseHover())
				{
					cNewsFlash.SetEnlarge(true);
				}
				else
				{
					cNewsFlash.SetEnlarge(false);
				}
			}
			if (cClock.isActive())
			{
				cClock.SetClockTime(cTimeControl->GetHour(), cTimeControl->GetMinute());
				cClock.Update(dt);
			}
			if (cClockTime.isActive())
			{
				double timeHour = cTimeControl->GetHour();
				double timeMinute = cTimeControl->GetMinute();
				if (timeHour >= 17)
				{
					cClockTime.SetTextColor(Color(10, 0, 0));
				}
				else
				{
					cClockTime.SetTextColor(Color(0, 10, 0));
				}
				std::string torender = "";
				if (timeHour < 10)
				{
					torender.append("0");
				}
				torender.append(std::to_string((int)timeHour));
				torender.append(":");
				if (timeMinute < 10)
				{
					torender.append("0");
				}
				torender.append(std::to_string((int)timeMinute));
				cClockTime.SetText(torender);
				cClockTime.Update(dt);
			}
			if (statsIcon.isActive())
			{
				statsIcon.Update(dt);
				if (statsIcon.IsMouseHover())
				{
					statsIcon.SetEnlarge(true);
					statsIcon.StartAnimation();
					if (statsIcon.isLeftMouseButtonPressed())
					{
						if (bouncetime[3] <= GetTickCount64())
						{
							cSoundController->PlaySoundByID(5);
							bouncetime[3] = GetTickCount64() + 100;
						}

						cTimeControl->SetTimePaused(true);

						// UI to disable
						settingsIcon.SetActive(false);
						statsIcon.SetActive(false);
						cClock.SetActive(false);
						cClockTime.SetActive(false);
						cCompass.SetActive(false);
						cNewsFlash.SetActive(false);
						cInventorySlot1.SetActive(false);
						cInventorySlot2.SetActive(false);
						cInventorySlot2Ammo.SetActive(false);
						cInventorySlot3.SetActive(false);
						cInventorySlot3Ammo.SetActive(false);
						cInventorySlot1Label.SetActive(false);
						cInventorySlot2Label.SetActive(false);
						cInventorySlot3Label.SetActive(false);

						// UI to enable
						statsBackground.SetActive(true);
						statsDecor.SetActive(true);
						statsFrame.SetActive(true);
						statsVirusTitle.SetActive(true);
						statsCloseButton.SetActive(true);
						statsTotalAlive.SetActive(true);
						statsTotalInfected.SetActive(true);
						statsInfectedSpawned.SetActive(true);
						statsInfectionProgressTitle.SetActive(true);
						statsInfectionProgress.SetActive(true);
						statsVaccineTitle.SetActive(true);
						statsVaccineProgress.SetActive(true);
						statsScore.SetActive(true);
						statsMutationPoints.SetActive(true);
					}

				}
				else
				{
					statsIcon.StopAnimation();
					statsIcon.SetEnlarge(false);
				}
			}
			if (statsBackground.isActive())
			{
				statsBackground.Update(dt);
			}
			if (statsDecor.isActive() && statsDecor.IsMouseHover())
			{
				statsDecor.Update(dt);
			}
			if (statsFrame.isActive())
			{
				statsFrame.Update(dt);
			}
			if (statsVirusTitle.isActive())
			{
				statsVirusTitle.SetText(cVirus->GetName());
				statsVirusTitle.Update(dt);
			}
			if (statsCloseButton.isActive())
			{
				statsCloseButton.Update(dt);
				if (statsCloseButton.IsMouseHover())
				{
					statsCloseButton.SetEnlarge(true);
					if (statsCloseButton.isLeftMouseButtonPressed())
					{
						if (bouncetime[3] <= GetTickCount64())
						{
							cSoundController->PlaySoundByID(5);
							bouncetime[3] = GetTickCount64() + 100;
						}

						cTimeControl->SetTimePaused(false);

						// UI to enable
						settingsIcon.SetActive(true);
						statsIcon.SetActive(true);
						cClock.SetActive(true);
						cClockTime.SetActive(true);
						cCompass.SetActive(true);
						cNewsFlash.SetActive(true);
						cInventorySlot1.SetActive(true);
						cInventorySlot2.SetActive(true);
						cInventorySlot2Ammo.SetActive(true);
						cInventorySlot3.SetActive(true);
						cInventorySlot3Ammo.SetActive(true);
						cInventorySlot1Label.SetActive(true);
						cInventorySlot2Label.SetActive(true);
						cInventorySlot3Label.SetActive(true);

						// UI to disable
						statsBackground.SetActive(false);
						statsDecor.SetActive(false);
						statsFrame.SetActive(false);
						statsVirusTitle.SetActive(false);
						statsCloseButton.SetActive(false);
						statsTotalAlive.SetActive(false);
						statsTotalInfected.SetActive(false);
						statsInfectedSpawned.SetActive(false);
						statsInfectionProgressTitle.SetActive(false);
						statsInfectionProgress.SetActive(false);
						statsVaccineTitle.SetActive(false);
						statsVaccineProgress.SetActive(false);
						statsScore.SetActive(false);
						statsMutationPoints.SetActive(false);
					}
				}
				else
				{
					statsCloseButton.SetEnlarge(false);
				}
			}
			if (statsTotalAlive.isActive())
			{
				statsTotalAlive.Update(dt);
				statsTotalAlive.SetText("Total People Alive:\n" + std::to_string(iTotalAlive));
			}
			if (statsTotalInfected.isActive())
			{
				statsTotalInfected.Update(dt);
				statsTotalInfected.SetText("Total People Infected:\n" + std::to_string(iTotalInfected));
			}
			if (statsInfectedSpawned.isActive())
			{
				statsInfectedSpawned.Update(dt);
				statsInfectedSpawned.SetText("Dead People:\n" + std::to_string(vNPCS.size() - iTotalAlive));
			}
			if (statsInfectionProgressTitle.isActive())
			{
				statsInfectionProgressTitle.Update(dt);
			}
			if (statsInfectionProgress.isActive())
			{
				statsInfectionProgressTitle.Update(dt);
				statsInfectionProgress.SetValue(vNPCS.size() - iTotalAlive);
				statsInfectionProgress.SetMaxValue(vNPCS.size());
			}
			if (statsVaccineTitle.isActive())
			{
				statsVaccineTitle.Update(dt);
			}
			if (statsVaccineProgress.isActive())
			{
				statsVaccineProgress.SetValue(cVaccine.GetProgress());
				statsVaccineProgress.SetMaxValue(100);
			}
			if (statsScore.isActive())
			{
				statsScore.SetText("Score: " + std::to_string(fScore));
			}
			if (statsMutationPoints.isActive())
			{
				statsMutationPoints.SetText("Mutation Points:\n" + std::to_string(cVirus->GetPoints()));
			}

			// Inventory
			if (cInventorySlot1.isActive())
			{
				cInventorySlot1.Update(dt);
			}
			if (cInventorySlot1Label.isActive())
			{
				cInventorySlot1Label.Update(dt);
			}
			if (cInventorySlot2.isActive())
			{
				cInventorySlot2.Update(dt);
			}
			if (cInventorySlot2Label.isActive())
			{
				cInventorySlot2Label.Update(dt);
			}
			if (cInventorySlot2Ammo.isActive())
			{
				cInventorySlot2Ammo.Update(dt);
				cInventorySlot2Ammo.SetText(std::to_string(cPlayer->AccessWeapon(1)->GetUses()));
			}
			if (cInventorySlot3.isActive())
			{
				cInventorySlot3.Update(dt);
			}
			if (cInventorySlot3Label.isActive())
			{
				cInventorySlot3Label.Update(dt);
			}
			if (cInventorySlot3Ammo.isActive())
			{
				cInventorySlot3Ammo.Update(dt);
				cInventorySlot3Ammo.SetText(std::to_string(cPlayer->AccessWeapon(2)->GetUses()));
			}
		}
		else // night
		{
			if (cSunRise.isActive())
			{
				cSunRise.Update(dt);
			}
			if (cSunRiseButton.isActive())
			{
				cSunRiseButton.Update(dt);
				if (cSunRiseButton.IsMouseHover())
				{
					cSunRiseButton.SetEnlarge(true);
					if (cSunRiseButton.isLeftMouseButtonPressed() && cSunRiseButton.isCooldownOver())
					{
						if (bouncetime[3] <= GetTickCount64())
						{
							cSoundController->PlaySoundByID(5);
							bouncetime[3] = GetTickCount64() + 100;
						}

						cSunRise.SetActive(false);
						cSunRiseText.SetActive(false);
						cSunRiseButton.SetActive(false);
							bouncetime[3] = dcurrenttime + 500;
							dTimeToNight = dcurrenttime + dDayPhaseTime;
							cTimeControl->Update(dTimeToNight, dcurrenttime, dDayPhaseTime);

							lights[0].color.Set(1, 1, 1);
							lights[0].power = 10.f;
							glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
							glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
					}
				}
				else
				{
					cSunRiseButton.SetEnlarge(false);
				}
			}
			if (cSunRiseText.isActive())
			{
				cSunRiseText.Update(dt);
				cSunRiseText.SetText("Day " + std::to_string(iDayNumber));
			}


			if (cUpgradesFrame.isActive())
			{
				cUpgradesFrame.Update(dt);
				cUpgradesMouseLabel.SetActive(false);
			}
			if (cUpgradesPoints.isActive())
			{
				cUpgradesPoints.Update(dt);
				cUpgradesPoints.SetText("Mutation\nPoints\n" + std::to_string(cVirus->GetPoints()));
			}
			if (cUpgradesCoughRangeTitle.isActive())
			{
				cUpgradesCoughRangeTitle.SetText("Cough Range (Lvl " + std::to_string(cUpgrade->GetCoughRangeLevel()) + ")");
				if (cUpgradesCoughRangeTitle.IsMouseHover())
				{
					cUpgradesMouseLabel.SetActive(true);

					std::string print = "\n Cough Range\n Level " + std::to_string(cUpgrade->GetCoughRangeLevel()) + "\n";
					if (cUpgrade->GetCoughRangeLevel() < 10)
					{
						double dMultiplier = 1;
						for (int i = 0; i < cUpgrade->GetCoughRangeLevel(); ++i)
						{
							dMultiplier *= cUpgrade->dCoughRangeMultiplier;
						}
						int before = cUpgrade->iInitialCoughRange * dMultiplier;
						int after = before * cUpgrade->dCoughRangeMultiplier;
						print += " RANGE: " + std::to_string(before) + "->" + std::to_string(after) + "\n";
						print += " Upgrade Cost: " + std::to_string(cUpgrade->GetCoughUpgradeCost()) + "\n";
						if (cUpgrade->CanAffordCoughRangeUpgrade())
							print += " CAN AFFORD\n";
						else
							print += " CANNOT AFFORD\n";
					}
					else
					{
						double dMultiplier = 1;
						for (int i = 0; i < cUpgrade->GetCoughRangeLevel(); ++i)
						{
							dMultiplier *= cUpgrade->dCoughRangeMultiplier;
						}
						int value = cUpgrade->iInitialCoughRange * dMultiplier;
						print += " RANGE: " + std::to_string(value) + "\n";
						print += " MAXED UPGRADE!\n";
					}
					print += "\n DESCRIPTION:\n";
					print += " Increases the range\n of Cough so that\n more NPCs can be\n infected at once.";
					cUpgradesMouseLabel.SetText(print);
				}
			}
			if (cUpgradesCoughRangeButton.isActive())
			{
				cUpgradesCoughRangeButton.Update(dt);
				if (cUpgradesCoughRangeButton.IsMouseHover() && cUpgrade->GetCoughRangeLevel() < 10)
				{
					cUpgradesCoughRangeButton.SetEnlarge(true);
					cUpgradesMouseLabel.SetActive(true);
					std::string print = "\n Cough Range\n Level " + std::to_string(cUpgrade->GetCoughRangeLevel()) + "\n";
					if (cUpgrade->GetCoughRangeLevel() < 10)
					{
						double dMultiplier = 1;
						for (int i = 0; i < cUpgrade->GetCoughRangeLevel(); ++i)
						{
							dMultiplier *= cUpgrade->dCoughRangeMultiplier;
						}
						int before = cUpgrade->iInitialCoughRange * dMultiplier;
						int after = before * cUpgrade->dCoughRangeMultiplier;
						print += " RANGE: " + std::to_string(before) + "->" + std::to_string(after) + "\n";
						print += " Upgrade Cost: " + std::to_string(cUpgrade->GetCoughUpgradeCost()) + "\n";
						if (cUpgrade->CanAffordCoughRangeUpgrade())
							print += " CAN AFFORD\n";
						else
							print += " CANNOT AFFORD\n";
					}
					else
					{
						double dMultiplier = 1;
						for (int i = 0; i < cUpgrade->GetCoughRangeLevel(); ++i)
						{
							dMultiplier *= cUpgrade->dCoughRangeMultiplier;
						}
						int value = cUpgrade->iInitialCoughRange * dMultiplier;
						print += " RANGE: " + std::to_string(value) + "\n";
						print += " MAXED UPGRADE!\n";
					}
					print += "\n DESCRIPTION:\n";
					print += " Increases the range\n of Cough so that\n more NPCs can be\n infected at once.";
					cUpgradesMouseLabel.SetText(print);

					if (cUpgradesCoughRangeButton.isLeftMouseButtonPressed() && cUpgradesCoughRangeButton.isCooldownOver())
					{
						if (bouncetime[3] <= GetTickCount64())
						{
							cSoundController->PlaySoundByID(5);
							bouncetime[3] = GetTickCount64() + 100;
						}

						cUpgrade->BuyCoughRangeUpgrade();
						cUpgradesCoughRangeButton.ResetTimer();
					}
				}
				else
				{
					cUpgradesCoughRangeButton.SetEnlarge(false);
				}
			}
			if (cUpgradesVirusStrengthTitle.isActive())
			{
				cUpgradesVirusStrengthTitle.SetText("Virus Strength (Lvl " + std::to_string(cUpgrade->GetVirusStrengthLevel()) + ")");
				if (cUpgradesVirusStrengthTitle.IsMouseHover())
				{
					cUpgradesMouseLabel.SetActive(true);

					std::string print = "\n Virus Strength\n Level " + std::to_string(cUpgrade->GetVirusStrengthLevel()) + "\n";
					if (cUpgrade->GetVirusStrengthLevel() < 10)
					{
						double dMultiplier = 1;
						for (int i = 0; i < cUpgrade->GetVirusStrengthLevel(); ++i)
						{
							dMultiplier *= cUpgrade->dVirusStrengthMultiplier;
						}
						int before = cUpgrade->fInitialVirusStrength * dMultiplier;
						int after = before * cUpgrade->dVirusStrengthMultiplier;
						print += " STRENGTH: " + std::to_string(before) + "->" + std::to_string(after) + "\n";
						print += " Upgrade Cost: " + std::to_string(cUpgrade->GetVirusStrengthCost()) + "\n";
						if (cUpgrade->CanAffordVirusStrengthUpgrade())
							print += " CAN AFFORD\n";
						else
							print += " CANNOT AFFORD\n";
					}
					else
					{
						double dMultiplier = 1;
						for (int i = 0; i < cUpgrade->GetVirusStrengthLevel(); ++i)
						{
							dMultiplier *= cUpgrade->dVirusStrengthMultiplier;
						}
						int value = cUpgrade->fInitialVirusStrength * dMultiplier;
						print += " STRENGTH: " + std::to_string(value) + "\n";
						print += " MAXED UPGRADE!\n";
					}
					print += "\n DESCRIPTION:\n";
					print += " Increases the\n damage of your\n weapons.";
					cUpgradesMouseLabel.SetText(print);
				}
			}
			if (cUpgradesVirusStrengthButton.isActive())
			{
				cUpgradesVirusStrengthButton.Update(dt);
				if (cUpgradesVirusStrengthButton.IsMouseHover() && cUpgrade->GetVirusStrengthLevel() < 10)
				{
					cUpgradesMouseLabel.SetActive(true);
					std::string print = "\n Virus Strength\n Level " + std::to_string(cUpgrade->GetVirusStrengthLevel()) + "\n";
					if (cUpgrade->GetVirusStrengthLevel() < 10)
					{
						double dMultiplier = 1;
						for (int i = 0; i < cUpgrade->GetVirusStrengthLevel(); ++i)
						{
							dMultiplier *= cUpgrade->dVirusStrengthMultiplier;
						}
						int before = cUpgrade->fInitialVirusStrength * dMultiplier;
						int after = before * cUpgrade->dVirusStrengthMultiplier;
						print += " STRENGTH: " + std::to_string(before) + "->" + std::to_string(after) + "\n";
						print += " Upgrade Cost: " + std::to_string(cUpgrade->GetVirusStrengthCost()) + "\n";
						if (cUpgrade->CanAffordVirusStrengthUpgrade())
							print += " CAN AFFORD\n";
						else
							print += " CANNOT AFFORD\n";
					}
					else
					{
						double dMultiplier = 1;
						for (int i = 0; i < cUpgrade->GetVirusStrengthLevel(); ++i)
						{
							dMultiplier *= cUpgrade->dVirusStrengthMultiplier;
						}
						int value = cUpgrade->fInitialVirusStrength * dMultiplier;
						print += " STRENGTH: " + std::to_string(value) + "\n";
						print += " MAXED UPGRADE!\n";
					}
					print += "\n DESCRIPTION:\n";
					print += " Increases the\n damage of your\n weapons.";
					cUpgradesMouseLabel.SetText(print);
					cUpgradesVirusStrengthButton.SetEnlarge(true);

					if (cUpgradesVirusStrengthButton.isLeftMouseButtonPressed() && cUpgradesVirusStrengthButton.isCooldownOver())
					{
						if (bouncetime[3] <= GetTickCount64())
						{
							cSoundController->PlaySoundByID(5);
							bouncetime[3] = GetTickCount64() + 100;
						}

						cUpgrade->BuyVirusStrengthUpgrade();
						cUpgradesVirusStrengthButton.ResetTimer();
					}
				}
				else
				{
					cUpgradesVirusStrengthButton.SetEnlarge(false);
				}
			}
			if (cUpgradesVirusLethalityTitle.isActive())
			{
				cUpgradesVirusLethalityTitle.SetText("Virus Lethality (Lvl " + std::to_string(cUpgrade->GetVirusLethalityLevel()) + ")");
				if (cUpgradesVirusLethalityTitle.IsMouseHover())
				{
					cUpgradesMouseLabel.SetActive(true);
					std::string print = "\n Virus Lethality\n Level " + std::to_string(cUpgrade->GetVirusLethalityLevel()) + "\n";
					if (cUpgrade->GetVirusLethalityLevel() < 10)
					{
						double dMultiplier = 1;
						for (int i = 0; i < cUpgrade->GetVirusLethalityLevel(); ++i)
						{
							dMultiplier *= cUpgrade->dVirusLethalityMultiplier;
						}
						int before = cUpgrade->fInitialVirusLethality * dMultiplier;
						int after = before * cUpgrade->dVirusLethalityMultiplier;
						print += " LETHALITY: " + std::to_string(before) + "->" + std::to_string(after) + "\n";
						print += " Upgrade Cost: " + std::to_string(cUpgrade->GetVirusLethalityCost()) + "\n";
						if (cUpgrade->CanAffordVirusLethalityUpgrade())
							print += " CAN AFFORD\n";
						else
							print += " CANNOT AFFORD\n";
					}
					else
					{
						double dMultiplier = 1;
						for (int i = 0; i < cUpgrade->GetVirusLethalityLevel(); ++i)
						{
							dMultiplier *= cUpgrade->dVirusLethalityMultiplier;
						}
						int value = cUpgrade->fInitialVirusLethality * dMultiplier;
						print += " LETHALITY: " + std::to_string(value) + "\n";
						print += " MAXED UPGRADE!\n";
					}
					print += "\n DESCRIPTION:\n";
					print += " Increases the\n lethality of your\n virus.";
					cUpgradesMouseLabel.SetText(print);
				}
			}
			if (cUpgradesVirusLethalityButton.isActive())
			{
				cUpgradesVirusLethalityButton.Update(dt);
				if (cUpgradesVirusLethalityButton.IsMouseHover() && cUpgrade->GetVirusLethalityLevel() < 10)
				{
					cUpgradesMouseLabel.SetActive(true);
					std::string print = "\n Virus Lethality\n Level " + std::to_string(cUpgrade->GetVirusLethalityLevel()) + "\n";
					if (cUpgrade->GetVirusLethalityLevel() < 10)
					{
						double dMultiplier = 1;
						for (int i = 0; i < cUpgrade->GetVirusLethalityLevel(); ++i)
						{
							dMultiplier *= cUpgrade->dVirusLethalityMultiplier;
						}
						int before = cUpgrade->fInitialVirusLethality * dMultiplier;
						int after = before * cUpgrade->dVirusLethalityMultiplier;
						print += " LETHALITY: " + std::to_string(before) + "->" + std::to_string(after) + "\n";
						print += " Upgrade Cost: " + std::to_string(cUpgrade->GetVirusLethalityCost()) + "\n";
						if (cUpgrade->CanAffordVirusLethalityUpgrade())
							print += " CAN AFFORD\n";
						else
							print += " CANNOT AFFORD\n";
					}
					else
					{
						double dMultiplier = 1;
						for (int i = 0; i < cUpgrade->GetVirusLethalityLevel(); ++i)
						{
							dMultiplier *= cUpgrade->dVirusLethalityMultiplier;
						}
						int value = cUpgrade->fInitialVirusLethality * dMultiplier;
						print += " LETHALITY: " + std::to_string(value) + "\n";
						print += " MAXED UPGRADE!\n";
					}
					print += "\n DESCRIPTION:\n";
					print += " Increases the\n lethality of your\n virus.";
					cUpgradesMouseLabel.SetText(print);
					cUpgradesVirusLethalityButton.SetEnlarge(true);

					if (cUpgradesVirusLethalityButton.isLeftMouseButtonPressed() && cUpgradesVirusLethalityButton.isCooldownOver())
					{
						if (bouncetime[3] <= GetTickCount64())
						{
							cSoundController->PlaySoundByID(5);
							bouncetime[3] = GetTickCount64() + 100;
						}

						cUpgrade->BuyVirusLethalityUpgrade();
						cUpgradesVirusLethalityButton.ResetTimer();
					}
				}
				else
				{
					cUpgradesVirusLethalityButton.SetEnlarge(false);
				}
			}
			if (cUpgradesDayLengthTitle.isActive())
			{
				cUpgradesDayLengthTitle.SetText("Day Length (Lvl " + std::to_string(cUpgrade->GetLengthOfDayLevel()) + ")");
				if (cUpgradesDayLengthTitle.IsMouseHover())
				{
					cUpgradesMouseLabel.SetActive(true);
					std::string print = "\n Length Of Day\n Level " + std::to_string(cUpgrade->GetLengthOfDayLevel()) + "\n";
					if (cUpgrade->GetLengthOfDayLevel() < 10)
					{
						double dMultiplier = 1;
						for (int i = 0; i < cUpgrade->GetLengthOfDayLevel(); ++i)
						{
							dMultiplier *= cUpgrade->dLengthOfDayMultiplier;
						}
						int before = cUpgrade->dInitialLengthOfDay * dMultiplier;
						int after = before * cUpgrade->dLengthOfDayMultiplier;
						print += " DURATION: " + std::to_string(before / 1000) + "s->" + std::to_string(after / 1000) + "s\n";
						print += " Upgrade Cost: " + std::to_string(cUpgrade->GetLengthOfDayCost()) + "\n";
						if (cUpgrade->CanAffordLengthOfDayUpgrade())
							print += " CAN AFFORD\n";
						else
							print += " CANNOT AFFORD\n";
					}
					else
					{
						double dMultiplier = 1;
						for (int i = 0; i < cUpgrade->GetLengthOfDayLevel(); ++i)
						{
							dMultiplier *= cUpgrade->dLengthOfDayMultiplier;
						}
						int value = cUpgrade->dInitialLengthOfDay * dMultiplier;
						print += " DURATION: " + std::to_string(value / 10) + "seconds\n";
						print += " MAXED UPGRADE!\n";
					}
					print += "\n DESCRIPTION:\n";
					print += " Increases the\n the length of\n one day.";
					cUpgradesMouseLabel.SetText(print);
				}
			}
			if (cUpgradesDayLengthButton.isActive())
			{
				cUpgradesDayLengthButton.Update(dt);
				if (cUpgradesDayLengthButton.IsMouseHover())
				{
					cUpgradesMouseLabel.SetActive(true);
					cUpgradesDayLengthButton.SetEnlarge(true);
					std::string print = "\n Length Of Day\n Level " + std::to_string(cUpgrade->GetLengthOfDayLevel()) + "\n";
					if (cUpgrade->GetLengthOfDayLevel() < 10)
					{
						double dMultiplier = 1;
						for (int i = 0; i < cUpgrade->GetLengthOfDayLevel(); ++i)
						{
							dMultiplier *= cUpgrade->dLengthOfDayMultiplier;
						}
						int before = cUpgrade->dInitialLengthOfDay * dMultiplier;
						int after = before * cUpgrade->dLengthOfDayMultiplier;
						print += " Duration: " + std::to_string(before / 1000) + "s->" + std::to_string(after / 1000) + "s\n";
						print += " Upgrade Cost: " + std::to_string(cUpgrade->GetLengthOfDayCost()) + "\n";
						if (cUpgrade->CanAffordLengthOfDayUpgrade())
							print += " CAN AFFORD\n";
						else
							print += " CANNOT AFFORD\n";
					}
					else
					{
						double dMultiplier = 1;
						for (int i = 0; i < cUpgrade->GetLengthOfDayLevel(); ++i)
						{
							dMultiplier *= cUpgrade->dLengthOfDayMultiplier;
						}
						int value = cUpgrade->dInitialLengthOfDay * dMultiplier;
						print += " DURATION: " + std::to_string(value / 1000) + "s\n";
						print += " MAXED UPGRADE!\n";
					}
					print += "\n DESCRIPTION:\n";
					print += " Increases the\n the length of\n one day.";
					cUpgradesMouseLabel.SetText(print);

					if (cUpgradesDayLengthButton.isLeftMouseButtonPressed() && cUpgradesDayLengthButton.isCooldownOver())
					{
						if (bouncetime[3] <= GetTickCount64())
						{
							cSoundController->PlaySoundByID(5);
							bouncetime[3] = GetTickCount64() + 100;
						}
						cUpgrade->BuyLengthOfDayUpgrade();
						cUpgradesDayLengthButton.ResetTimer();
					}
				}
				else
				{
					cUpgradesDayLengthButton.SetEnlarge(false);
				}
			}
			if (cUpgradesPlayerSpeedTitle.isActive())
			{
				cUpgradesPlayerSpeedTitle.SetText("Player Speed (Lvl " + std::to_string(cUpgrade->GetMovementSpeedLevel()) + ")");
				if (cUpgradesPlayerSpeedTitle.IsMouseHover())
				{
					cUpgradesMouseLabel.SetActive(true);
					std::string print = "\n Player Speed\n Level " + std::to_string(cUpgrade->GetMovementSpeedLevel()) + "\n";
					if (cUpgrade->GetMovementSpeedLevel() < 10)
					{
						double dMultiplier = 0;
						for (int i = 0; i < cUpgrade->GetMovementSpeedLevel(); ++i)
						{
							dMultiplier += cUpgrade->dMovementSpeedBonus;
						}
						int before = cUpgrade->iInitialMovementSpeed + dMultiplier;
						int after = before + cUpgrade->dMovementSpeedBonus;
						print += " SPEED: " + std::to_string(before) + "->" + std::to_string(after) + "\n";
						print += " Upgrade Cost: " + std::to_string(cUpgrade->GetMovementSpeedCost()) + "\n";
						if (cUpgrade->CanAffordMovementSpeedUpgrade())
							print += " CAN AFFORD\n";
						else
							print += " CANNOT AFFORD\n";
					}
					else
					{
						double dMultiplier = 1;
						for (int i = 0; i < cUpgrade->GetMovementSpeedLevel(); ++i)
						{
							dMultiplier += cUpgrade->dMovementSpeedBonus;
						}
						int value = cUpgrade->iInitialMovementSpeed + dMultiplier;
						print += " SPEED: " + std::to_string(value) + "\n";
						print += " MAXED UPGRADE!\n";
					}
					print += "\n DESCRIPTION:\n";
					print += " Increases the\n the length of\n one day.";
					cUpgradesMouseLabel.SetText(print);
				}
			}
			if (cUpgradesPlayerSpeedButton.isActive())
			{
				cUpgradesPlayerSpeedButton.Update(dt);
				if (cUpgradesPlayerSpeedButton.IsMouseHover())
				{
					cUpgradesMouseLabel.SetActive(true);
					std::string print = "\n Player Speed\n Level " + std::to_string(cUpgrade->GetMovementSpeedLevel()) + "\n";
					if (cUpgrade->GetMovementSpeedLevel() < 10)
					{
						double dMultiplier = 0;
						for (int i = 0; i < cUpgrade->GetMovementSpeedLevel(); ++i)
						{
							dMultiplier += cUpgrade->dMovementSpeedBonus;
						}
						int before = cUpgrade->iInitialMovementSpeed + dMultiplier;
						int after = before + cUpgrade->dMovementSpeedBonus;
						print += " SPEED: " + std::to_string(before) + "->" + std::to_string(after) + "\n";
						print += " Upgrade Cost: " + std::to_string(cUpgrade->GetMovementSpeedCost()) + "\n";
						if (cUpgrade->CanAffordMovementSpeedUpgrade())
							print += " CAN AFFORD\n";
						else
							print += " CANNOT AFFORD\n";
					}
					else
					{
						double dMultiplier = 1;
						for (int i = 0; i < cUpgrade->GetMovementSpeedLevel(); ++i)
						{
							dMultiplier += cUpgrade->dMovementSpeedBonus;
						}
						int value = cUpgrade->iInitialMovementSpeed + dMultiplier;
						print += " SPEED: " + std::to_string(value) + "\n";
						print += " MAXED UPGRADE!\n";
					}
					print += "\n DESCRIPTION:\n";
					print += " Increases the\n the length of\n one day.";
					cUpgradesMouseLabel.SetText(print);
					cUpgradesPlayerSpeedButton.SetEnlarge(true);

					if (cUpgradesPlayerSpeedButton.isLeftMouseButtonPressed() && cUpgradesPlayerSpeedButton.isCooldownOver())
					{
						if (bouncetime[3] <= GetTickCount64())
						{
							cSoundController->PlaySoundByID(5);
							bouncetime[3] = GetTickCount64() + 100;
						}

						cUpgrade->BuyMovementSpeedUpgrade();
						cUpgradesPlayerSpeedButton.ResetTimer();
					}
				}
				else
				{
					cUpgradesPlayerSpeedButton.SetEnlarge(false);
				}
			}
			if (cUpgradesMutateVirusTitle.isActive())
			{
				if (cUpgradesMutateVirusTitle.IsMouseHover())
				{
					cUpgradesMouseLabel.SetActive(true);
					std::string print = "\n Mutate Virus\n Level " + std::to_string(cUpgrade->GetMutateVirusLevel()) + "\n";
					if (cUpgrade->GetMutateVirusLevel() < 10)
					{
						print += " DESTROY\n " + std::to_string((int)(cVaccine.GetProgress() / 100 * cUpgrade->dDestroyVaccineProgressPercentage)) + "% of vaccine\n";
						print += " Upgrade Cost: " + std::to_string(cUpgrade->GetMutateVirusCost()) + "\n";
						if (cUpgrade->CanAffordMutateVirusUpgrade())
							print += " CAN AFFORD\n";
						else
							print += " CANNOT AFFORD\n";
					}
					else
					{
						print += " YOU REACHED THE\n VIRUS MUTATION\n LIMIT!";

					}
					print += "\n DESCRIPTION:\n";
					print += " Destroy 10% of the\n vaccine progress.";
					cUpgradesMouseLabel.SetText(print);
				}
			}
			if (cUpgradesMutateVirusButton.isActive())
			{
				cUpgradesMutateVirusButton.Update(dt);
				if (cUpgradesMutateVirusButton.IsMouseHover())
				{
					cUpgradesMouseLabel.SetActive(true);
					std::string print = "\n Mutate Virus\n Level " + std::to_string(cUpgrade->GetMutateVirusLevel()) + "\n";
					if (cUpgrade->GetMutateVirusLevel() < 10)
					{
						print += " DESTROY\n " + std::to_string((int)(cVaccine.GetProgress() / 100 * cUpgrade->dDestroyVaccineProgressPercentage)) + "% of vaccine\n";
						print += " Upgrade Cost: " + std::to_string(cUpgrade->GetMutateVirusCost()) + "\n";
						if (cUpgrade->CanAffordMutateVirusUpgrade())
							print += " CAN AFFORD\n";
						else
							print += " CANNOT AFFORD\n";
					}
					else
					{
						print += " YOU REACHED THE\n VIRUS MUTATION\n LIMIT!";

					}
					print += "\n DESCRIPTION:\n";
					print += " Destroys 10% of\n the vaccine progress.";
					cUpgradesMouseLabel.SetText(print);
					cUpgradesMutateVirusButton.SetEnlarge(true);
					if (cUpgradesMutateVirusButton.isLeftMouseButtonPressed() && cUpgradesMutateVirusButton.isCooldownOver())
					{
						if (bouncetime[3] <= GetTickCount64())
						{
							cSoundController->PlaySoundByID(5);
							bouncetime[3] = GetTickCount64() + 100;
						}
						cUpgrade->BuyMutateVirusUpgrade();
						cUpgradesMutateVirusButton.ResetTimer();
					}
				}
				else
				{
					cUpgradesMutateVirusButton.SetEnlarge(false);
				}
			}
			if (cUpgradesVirusLauncherTitle.isActive())
			{
				cUpgradesVirusLauncherTitle.Update(dt);
			}
			if (cUpgradesVirusLauncherAmmoText.isActive())
			{
				cUpgradesVirusLauncherAmmoText.Update(dt);
				cPlayer->SetCurrentWeapon(1);
				cUpgradesVirusLauncherAmmoText.SetText(std::to_string(cPlayer->GetWeapon()->GetUses()) + " (" + std::to_string(cUpgrade->iVirusLauncherAmmoCost) + " Points)");
			}
			if (cUpgradesVirusLauncherButton.isActive())
			{
				cUpgradesVirusLauncherButton.Update(dt);
				if (cUpgradesVirusLauncherButton.IsMouseHover())
				{
					if (cUpgradesVirusLauncherButton.isLeftMouseButtonPressed() && cUpgradesVirusLauncherButton.isCooldownOver() && cVirus->GetPoints() >= cUpgrade->GetVirusLauncherAmmoCost())
					{
						if (bouncetime[3] <= GetTickCount64())
						{
							cSoundController->PlaySoundByID(5);
							bouncetime[3] = GetTickCount64() + 100;
						}

						cUpgradesVirusLauncherButton.ResetTimer();
						cPlayer->SetCurrentWeapon(1);
						cPlayer->GetWeapon()->AddUses(30);
						cVirus->ReducePoints(cUpgrade->iVirusLauncherAmmoCost);
					}
					cUpgradesVirusLauncherButton.SetEnlarge(true);
				}
				else
				{
					cUpgradesVirusLauncherButton.SetEnlarge(false);
				}
			}
			if (cUpgradesAirFreshenerTitle.isActive())
			{
				cUpgradesAirFreshenerTitle.Update(dt);
			}
			if (cUpgradesAirFreshenerAmmoText.isActive())
			{
				cUpgradesAirFreshenerAmmoText.Update(dt);
				cPlayer->SetCurrentWeapon(2);
				cUpgradesAirFreshenerAmmoText.SetText(std::to_string(cPlayer->GetWeapon()->GetUses()) + " (" + std::to_string(cUpgrade->iInfectedAirFreshenerCost) + " Points)");
			}
			if (cUpgradesAirFreshenerButton.isActive())
			{
				cUpgradesAirFreshenerButton.Update(dt);
				if (cUpgradesAirFreshenerButton.IsMouseHover())
				{
					cUpgradesAirFreshenerButton.SetEnlarge(true);
					if (cUpgradesAirFreshenerButton.isLeftMouseButtonPressed() && cUpgradesAirFreshenerButton.isCooldownOver() && cVirus->GetPoints() >= cUpgrade->GetInfectedAirFreshenerAmmoCost())
					{
						if (bouncetime[3] <= GetTickCount64())
						{
							cSoundController->PlaySoundByID(5);
							bouncetime[3] = GetTickCount64() + 100;
						}

						cUpgradesAirFreshenerButton.ResetTimer();
						cPlayer->SetCurrentWeapon(2);
						cPlayer->GetWeapon()->AddUses(5);
						cVirus->ReducePoints(cUpgrade->iInfectedAirFreshenerCost);
					}
				}
				else
				{
					cUpgradesAirFreshenerButton.SetEnlarge(false);
				}
			}

			if (cUpgradesNextDayButton.isActive())
			{
				cUpgradesNextDayButton.Update(dt);
				if (cUpgradesNextDayButton.IsMouseHover())
				{
					cUpgradesNextDayButton.SetEnlarge(true);
					if (cUpgradesNextDayButton.isLeftMouseButtonPressed() && cUpgradesNextDayButton.isCooldownOver())
					{
						if (bouncetime[3] <= GetTickCount64())
						{
							cSoundController->PlaySoundByID(5);
							bouncetime[3] = GetTickCount64() + 100;
						}

						++iDayNumber;
						cUpgrade->UpdateValues();


						if (cVaccine.CheckCompleted() == true)
						{
							//LOSS CONDIDTION
							eGameState = LOSS;
							cSoundController->PlaySoundByID(8);

							//Enable loss ui here
							cLoseFrame.SetActive(true);
							cLoseDecor.SetActive(true);
							cLoseText.SetActive(true);
							cLoseScore.SetActive(true);
							cLoseButton.SetActive(true);
						}

						cPlayer->SetCurrentWeapon(2);
						cPlayer->GetWeapon()->SetWeaponDamage(cVirus->GetStrength());
						cPlayer->SetCurrentWeapon(1);
						cPlayer->GetWeapon()->SetWeaponDamage(cVirus->GetStrength() / 2);
						cPlayer->SetCurrentWeapon(0);
						cPlayer->GetWeapon()->SetWeaponDamage(cVirus->GetStrength());

						cUpgradesFrame.SetActive(false);
						cUpgradesTitle.SetActive(false);
						cUpgradesFrame2.SetActive(false);
						cUpgradesCoughRangeTitle.SetActive(false);
						cUpgradesCoughRangeButton.SetActive(false);
						cUpgradesVirusStrengthTitle.SetActive(false);
						cUpgradesVirusStrengthButton.SetActive(false);
						cUpgradesVirusLethalityTitle.SetActive(false);
						cUpgradesVirusLethalityButton.SetActive(false);
						cUpgradesDayLengthTitle.SetActive(false);
						cUpgradesDayLengthButton.SetActive(false);
						cUpgradesPlayerSpeedTitle.SetActive(false);
						cUpgradesPlayerSpeedButton.SetActive(false);
						cUpgradesMutateVirusTitle.SetActive(false);
						cUpgradesMutateVirusButton.SetActive(false);
						cUpgradesVirusLauncherTitle.SetActive(false);
						cUpgradesVirusLauncherAmmoText.SetActive(false);
						cUpgradesVirusLauncherButton.SetActive(false);
						cUpgradesAirFreshenerTitle.SetActive(false);
						cUpgradesAirFreshenerAmmoText.SetActive(false);
						cUpgradesAirFreshenerButton.SetActive(false);
						cUpgradesFrame2Button1.SetActive(false);
						cUpgradesFrame2Button2.SetActive(false);
						cUpgradesFrame2Button3.SetActive(false);
						cUpgradesNextDayButton.SetActive(false);
						cUpgradesPoints.SetActive(false);

						cSunRise.StopBackgroundAnimation();
						cSunRise.StartBackgroundAnimation();
						cSunRise.SetActive(true);
						cSunRiseText.SetActive(true);
						cSunRiseButton.SetActive(true);
						cSunRiseButton.ResetTimer();
					}
				}
				else
				{
					cUpgradesNextDayButton.SetEnlarge(false);
				}
			}
			if (cUpgradesFrame2Button1.isActive())
			{
				if (cUpgradesFrame2Button1.isLeftMouseButtonPressed())
				{
					if (bouncetime[3] <= GetTickCount64())
					{
						cSoundController->PlaySoundByID(5);
						bouncetime[3] = GetTickCount64() + 100;
					}

					cUpgradesFrame2Button1.SetEnlarge(true);
					cUpgradesFrame2Button2.SetEnlarge(false);
					cUpgradesFrame2Button3.SetEnlarge(false);

					// First Frame
					cUpgradesCoughRangeTitle.SetActive(true);
					cUpgradesCoughRangeButton.SetActive(true);
					cUpgradesVirusStrengthTitle.SetActive(true);
					cUpgradesVirusStrengthButton.SetActive(true);
					cUpgradesVirusLethalityTitle.SetActive(true);
					cUpgradesVirusLethalityButton.SetActive(true);

					// Second Frame
					cUpgradesDayLengthTitle.SetActive(false);
					cUpgradesDayLengthButton.SetActive(false);
					cUpgradesPlayerSpeedTitle.SetActive(false);
					cUpgradesPlayerSpeedButton.SetActive(false);
					cUpgradesMutateVirusTitle.SetActive(false);
					cUpgradesMutateVirusButton.SetActive(false);

					// Third Frame
					cUpgradesVirusLauncherTitle.SetActive(false);
					cUpgradesVirusLauncherAmmoText.SetActive(false);
					cUpgradesVirusLauncherButton.SetActive(false);
					cUpgradesAirFreshenerTitle.SetActive(false);
					cUpgradesAirFreshenerAmmoText.SetActive(false);
					cUpgradesAirFreshenerButton.SetActive(false);
				}
			}
			if (cUpgradesFrame2Button2.isActive())
			{
				if (cUpgradesFrame2Button2.isLeftMouseButtonPressed())
				{
					if (bouncetime[3] <= GetTickCount64())
					{
						cSoundController->PlaySoundByID(5);
						bouncetime[3] = GetTickCount64() + 100;
					}

					cUpgradesFrame2Button1.SetEnlarge(false);
					cUpgradesFrame2Button2.SetEnlarge(true);
					cUpgradesFrame2Button3.SetEnlarge(false);

					// First Frame
					cUpgradesCoughRangeTitle.SetActive(false);
					cUpgradesCoughRangeButton.SetActive(false);
					cUpgradesVirusStrengthTitle.SetActive(false);
					cUpgradesVirusStrengthButton.SetActive(false);
					cUpgradesVirusLethalityTitle.SetActive(false);
					cUpgradesVirusLethalityButton.SetActive(false);

					// Second Frame
					cUpgradesDayLengthTitle.SetActive(true);
					cUpgradesDayLengthButton.SetActive(true);
					cUpgradesPlayerSpeedTitle.SetActive(true);
					cUpgradesPlayerSpeedButton.SetActive(true);
					cUpgradesMutateVirusTitle.SetActive(true);
					cUpgradesMutateVirusButton.SetActive(true);

					// Third Frame
					cUpgradesVirusLauncherTitle.SetActive(false);
					cUpgradesVirusLauncherAmmoText.SetActive(false);
					cUpgradesVirusLauncherButton.SetActive(false);
					cUpgradesAirFreshenerTitle.SetActive(false);
					cUpgradesAirFreshenerAmmoText.SetActive(false);
					cUpgradesAirFreshenerButton.SetActive(false);
				}
			}
			if (cUpgradesFrame2Button3.isActive())
			{
				if (cUpgradesFrame2Button3.isLeftMouseButtonPressed())
				{
					if (bouncetime[3] <= GetTickCount64())
					{
						cSoundController->PlaySoundByID(5);
						bouncetime[3] = GetTickCount64() + 100;
					}

					cUpgradesFrame2Button1.SetEnlarge(false);
					cUpgradesFrame2Button2.SetEnlarge(false);
					cUpgradesFrame2Button3.SetEnlarge(true);

					// First Frame
					cUpgradesCoughRangeTitle.SetActive(false);
					cUpgradesCoughRangeButton.SetActive(false);
					cUpgradesVirusStrengthTitle.SetActive(false);
					cUpgradesVirusStrengthButton.SetActive(false);
					cUpgradesVirusLethalityTitle.SetActive(false);
					cUpgradesVirusLethalityButton.SetActive(false);

					// Second Frame
					cUpgradesDayLengthTitle.SetActive(false);
					cUpgradesDayLengthButton.SetActive(false);
					cUpgradesPlayerSpeedTitle.SetActive(false);
					cUpgradesPlayerSpeedButton.SetActive(false);
					cUpgradesMutateVirusTitle.SetActive(false);
					cUpgradesMutateVirusButton.SetActive(false);

					// Third Frame
					cUpgradesVirusLauncherTitle.SetActive(true);
					cUpgradesVirusLauncherAmmoText.SetActive(true);
					cUpgradesVirusLauncherButton.SetActive(true);
					cUpgradesAirFreshenerTitle.SetActive(true);
					cUpgradesAirFreshenerAmmoText.SetActive(true);
					cUpgradesAirFreshenerButton.SetActive(true);
				}
			}


			if (cSunSet.isActive())
			{
				cSunSet.Update(dt);
			}
			if (cSunSetButton.isActive())
			{
				cSunSetButton.Update(dt);
				if (cSunSetButton.IsMouseHover())
				{
					cSunSetButton.SetEnlarge(true);
					if (cSunSetButton.isLeftMouseButtonPressed() && cSunSetButton.isCooldownOver())
					{
						if (bouncetime[3] <= GetTickCount64())
						{
							cSoundController->PlaySoundByID(5);
							bouncetime[3] = GetTickCount64() + 100;
						}

						cInventorySlot1.SetEnlarge(true);
						cInventorySlot1Label.SetEnlarge(true);

						cInventorySlot2.SetEnlarge(false);
						cInventorySlot2Label.SetEnlarge(false);
						cInventorySlot2Ammo.SetEnlarge(false);

						cInventorySlot3.SetEnlarge(false);
						cInventorySlot3Label.SetEnlarge(false);
						cInventorySlot3Ammo.SetEnlarge(false);

						cSunSet.SetActive(false);
						cSunSetText.SetActive(false);
						cSunSetButton.SetActive(false);

						cUpgradesFrame.SetActive(true);
						cUpgradesTitle.SetActive(true);
						cUpgradesFrame2.SetActive(true);
						cUpgradesCoughRangeTitle.SetActive(true);
						cUpgradesCoughRangeButton.SetActive(true);
						cUpgradesVirusStrengthTitle.SetActive(true);
						cUpgradesVirusStrengthButton.SetActive(true);
						cUpgradesVirusLethalityTitle.SetActive(true);
						cUpgradesVirusLethalityButton.SetActive(true);
						cUpgradesFrame2Button1.SetActive(true);
						cUpgradesFrame2Button1.SetEnlarge(true);
						cUpgradesFrame2Button2.SetActive(true);
						cUpgradesFrame2Button2.SetEnlarge(false);
						cUpgradesFrame2Button3.SetActive(true);
						cUpgradesFrame2Button3.SetEnlarge(false);
						cUpgradesNextDayButton.SetActive(true);
						cUpgradesNextDayButton.ResetTimer();
						cUpgradesPoints.SetActive(true);
					}
				}
				else
				{
					cSunSetButton.SetEnlarge(false);
				}
			}
			if (cSunSetText.isActive())
			{
				cSunSetText.Update(dt);
				cSunSetText.SetText("Night " + std::to_string(iDayNumber));
			}
		}
	}
	else if (eGameState == WIN)
	{
		if (cWinFrame.isActive())
		{
			cWinFrame.Update(dt);
		}
		if (cWinDecor.isActive() && cWinDecor.IsMouseHover())
		{
			cWinDecor.Update(dt);
		}
		if (cWinText.isActive())
		{
			cWinText.Update(dt);
		}
		if (cWinDays.isActive())
		{
			cWinDays.Update(dt);
			cWinDays.SetText(std::to_string(iDayNumber) + " Days!");
		}
		if (cWinButton.isActive())
		{
			cWinButton.Update(dt);
			if (cWinButton.IsMouseHover())
			{

				// Send text to somewhere first...
				// Do Something()
				settingsCheatCodeTextBox.ClearText();
				settingsCheatCodeTextBox.SetEnabled(false);


				cWinButton.SetEnlarge(true);
				if (cWinButton.isLeftMouseButtonPressed())
				{
					if (bouncetime[3] <= GetTickCount64())
					{
						cSoundController->PlaySoundByID(5);
						bouncetime[3] = GetTickCount64() + 100;
					}

					Application::SetSceneType(Application::INTRO);
					cLoadingBar.SetActive(true);
					cLoadScreen.SetActive(true);
				}
			}
			else
			{
				cWinButton.SetEnlarge(false);

			}
		}
	}
	else if (eGameState == LOSS)
	{
		if (cLoseFrame.isActive())
		{
			cLoseFrame.Update(dt);
		}
		if (cLoseDecor.isActive() && cLoseDecor.IsMouseHover())
		{
			cLoseDecor.Update(dt);
		}
		if (cLoseText.isActive())
		{
			cLoseText.Update(dt);
		}
		if (cLoseScore.isActive())
		{
			cLoseScore.Update(dt);
			cLoseScore.SetText(std::to_string(iDayNumber));
		}
		if (cLoseButton.isActive())
		{
			cLoseButton.Update(dt);
			if (cLoseButton.IsMouseHover())
			{
				cLoseButton.SetEnlarge(true);
				if (cLoseButton.isLeftMouseButtonPressed())
				{
					if (bouncetime[3] <= GetTickCount64())
					{
						cSoundController->PlaySoundByID(5);
						bouncetime[3] = GetTickCount64() + 100;
					}

					Application::SetSceneType(Application::INTRO);
					cLoadingBar.SetActive(true);
					cLoadScreen.SetActive(true);
				}
			}
			else
			{
				cLoseButton.SetEnlarge(false);
			}
		}
	}
}

void SceneBasic::RenderUIs(MS& projectionStack, MS& viewStack, MS& modelStack)
{
	if (eGameState == GAME)
	{
		if (cTimeControl->isDay())
		{
			// Gameplay UIs
			settingsIcon.Render(projectionStack, viewStack, modelStack);
			settingsBackground.Render(projectionStack, viewStack, modelStack);
			settingsDecor.Render(projectionStack, viewStack, modelStack);
			settingsTitle.Render(projectionStack, viewStack, modelStack);
			settingsCloseButton.Render(projectionStack, viewStack, modelStack);
			settingsCheatTitle.Render(projectionStack, viewStack, modelStack);
			settingsCheatCodeTextBox.Render(projectionStack, viewStack, modelStack);
			settingsGoMainMenu.Render(projectionStack, viewStack, modelStack);
			settingsSkipDay.Render(projectionStack, viewStack, modelStack);
			settingsControls.Render(projectionStack, viewStack, modelStack);
			cControlFrame.Render(projectionStack, viewStack, modelStack);
			cControlCloseButton.Render(projectionStack, viewStack, modelStack);
			cControlText.Render(projectionStack, viewStack, modelStack);
			cCompass.Render(projectionStack, viewStack, modelStack);
			cNewsFlash.Render(projectionStack, viewStack, modelStack);
			cClock.Render(projectionStack, viewStack, modelStack);
			cClockTime.Render(projectionStack, viewStack, modelStack);
			statsIcon.Render(projectionStack, viewStack, modelStack);
			statsBackground.Render(projectionStack, viewStack, modelStack);
			statsDecor.Render(projectionStack, viewStack, modelStack);
			statsFrame.Render(projectionStack, viewStack, modelStack);
			statsVirusTitle.Render(projectionStack, viewStack, modelStack);
			statsCloseButton.Render(projectionStack, viewStack, modelStack);
			statsTotalAlive.Render(projectionStack, viewStack, modelStack);
			statsTotalInfected.Render(projectionStack, viewStack, modelStack);
			statsInfectedSpawned.Render(projectionStack, viewStack, modelStack);
			statsInfectionProgressTitle.Render(projectionStack, viewStack, modelStack);
			statsInfectionProgress.Render(projectionStack, viewStack, modelStack);
			statsVaccineTitle.Render(projectionStack, viewStack, modelStack);
			statsVaccineProgress.Render(projectionStack, viewStack, modelStack);
			statsScore.Render(projectionStack, viewStack, modelStack);
			statsMutationPoints.Render(projectionStack, viewStack, modelStack);
			cInventorySlot1.Render(projectionStack, viewStack, modelStack);
			cInventorySlot1Label.Render(projectionStack, viewStack, modelStack);
			cInventorySlot2.Render(projectionStack, viewStack, modelStack);
			cInventorySlot2Label.Render(projectionStack, viewStack, modelStack);
			cInventorySlot2Ammo.Render(projectionStack, viewStack, modelStack);
			cInventorySlot3.Render(projectionStack, viewStack, modelStack);
			cInventorySlot3Label.Render(projectionStack, viewStack, modelStack);
			cInventorySlot3Ammo.Render(projectionStack, viewStack, modelStack);
			

		}
		else
		{
			cSunRise.Render(projectionStack, viewStack, modelStack);
			cSunRiseText.Render(projectionStack, viewStack, modelStack);
			cSunRiseButton.Render(projectionStack, viewStack, modelStack);

			cUpgradesFrame.Render(projectionStack, viewStack, modelStack);
			cUpgradesTitle.Render(projectionStack, viewStack, modelStack);;
			cUpgradesFrame2.Render(projectionStack, viewStack, modelStack);
			cUpgradesCoughRangeTitle.Render(projectionStack, viewStack, modelStack);
			cUpgradesCoughRangeButton.Render(projectionStack, viewStack, modelStack);
			cUpgradesVirusStrengthTitle.Render(projectionStack, viewStack, modelStack);
			cUpgradesVirusStrengthButton.Render(projectionStack, viewStack, modelStack);
			cUpgradesVirusLethalityTitle.Render(projectionStack, viewStack, modelStack);
			cUpgradesVirusLethalityButton.Render(projectionStack, viewStack, modelStack);
			cUpgradesDayLengthTitle.Render(projectionStack, viewStack, modelStack);
			cUpgradesDayLengthButton.Render(projectionStack, viewStack, modelStack);
			cUpgradesPlayerSpeedTitle.Render(projectionStack, viewStack, modelStack);
			cUpgradesPlayerSpeedButton.Render(projectionStack, viewStack, modelStack);
			cUpgradesMutateVirusTitle.Render(projectionStack, viewStack, modelStack);
			cUpgradesMutateVirusButton.Render(projectionStack, viewStack, modelStack);
			cUpgradesVirusLauncherTitle.Render(projectionStack, viewStack, modelStack);
			cUpgradesVirusLauncherAmmoText.Render(projectionStack, viewStack, modelStack);
			cUpgradesVirusLauncherButton.Render(projectionStack, viewStack, modelStack);
			cUpgradesAirFreshenerTitle.Render(projectionStack, viewStack, modelStack);
			cUpgradesAirFreshenerAmmoText.Render(projectionStack, viewStack, modelStack);
			cUpgradesAirFreshenerButton.Render(projectionStack, viewStack, modelStack);
			cUpgradesFrame2Button1.Render(projectionStack, viewStack, modelStack);
			cUpgradesFrame2Button2.Render(projectionStack, viewStack, modelStack);
			cUpgradesFrame2Button3.Render(projectionStack, viewStack, modelStack);
			cUpgradesNextDayButton.Render(projectionStack, viewStack, modelStack);
			cUpgradesMouseLabel.Render(projectionStack, viewStack, modelStack);
			cUpgradesPoints.Render(projectionStack, viewStack, modelStack);

			cSunSet.Render(projectionStack, viewStack, modelStack);
			cSunSetText.Render(projectionStack, viewStack, modelStack);
			cSunSetButton.Render(projectionStack, viewStack, modelStack);
		}
	}
	else if (eGameState == WIN)
	{
		cWinFrame.Render(projectionStack, viewStack, modelStack);
		cWinDecor.Render(projectionStack, viewStack, modelStack);
		cWinText.Render(projectionStack, viewStack, modelStack);
		cWinDays.Render(projectionStack, viewStack, modelStack);
		cWinButton.Render(projectionStack, viewStack, modelStack);
	}
	else if (eGameState == LOSS)
	{
		cLoseFrame.Render(projectionStack, viewStack, modelStack);
		cLoseDecor.Render(projectionStack, viewStack, modelStack);
		cLoseText.Render(projectionStack, viewStack, modelStack);
		cLoseScore.Render(projectionStack, viewStack, modelStack);
		cLoseButton.Render(projectionStack, viewStack, modelStack);
	}

	cLoadScreen.Render(projectionStack, viewStack, modelStack);
	cLoadingBar.Render(projectionStack, viewStack, modelStack);

}


//barycentric coordinates attempt
float SceneBasic::GetTerrainHeight(float px, float py)
{
	if (px < 0)px = 0;
	if (py < 0)py = 0;
	int tilex = int(px); //conversion to int floors our value.
	int tiley = int(py); //float to int is a classic problem sse solves

	//Get values from our height data.
	float z1 = ReadHeightMap(m_heightMap, tilex, tiley);
	float z2 = ReadHeightMap(m_heightMap, tilex + 1, tiley);
	float z3 = ReadHeightMap(m_heightMap, tilex +1, tiley +1);
	float z4 = ReadHeightMap(m_heightMap, tilex, tiley +1);

	float flx = px - tilex;
	float fly = py - tiley;
	//at this point flx and fly should be in range of -1 to +1
	//which is excellent for barycentric math space
	//the only complication now is organizing for which
	//triangle in our current terrain tile we want to calculate

	float wyy3 = fly - 1.0f;
	float wxx3 = flx - 1.0f;
	float m[4] = { wxx3,-1.0f,-1.0f,0.0f };
	float upper[4] = { flx,0.0f,0.0f,-1.0f };

	if (fly >= flx) {
		m[0] = upper[0];
		m[1] = upper[1];
		m[2] = upper[2];
		m[3] = upper[3];
		z2 = z3;
		z3 = z4;
	}
	//now calculate the barycentric and interpolate.
	//all this should look very familiar as vector calculations.
	//b1=(y2-y3)*(wx-x3)+(x3-x2)*(wy-y3);
	//b2=(y3-y1)*(wx-x3)+(x1-x3)*(wy-y3);

	//0 - x
	//1 - y
	//2 - z
	//3 - w
	float b[4];
	b[0] = m[0] * m[2] + m[3] * wyy3;
	b[1] = m[0] + m[1] * wyy3;
	b[2] = 1.0f - b[0] - b[1];
	b[3] = (b[0] * z1) + (b[0] * z2) + (b[2] * z3);
	return b[3];
}