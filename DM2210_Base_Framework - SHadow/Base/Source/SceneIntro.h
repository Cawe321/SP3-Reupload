#ifndef SCENEINTRO_H
#define SCENEINTRO_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera3.h"
#include "CCamera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include <vector>
#include "SpriteAnimation.h"
#include "ParticleSystem.h"
#include "DepthFBO.h"
#include "WaterFBO.h"
#include "KeyboardController.h"
#include "SoundController.h"
#include <fstream>

#include "CGUI/CImageButton.h"
#include "CGUI/CTextBox.h"
#include "CGUI/CTextButton.h"
#include "CGUI/CTextLabel.h"
#include "CGUI/CImageLabel.h"
#include "CGUI/CNewsFlash.h"
#include "CGUI/CProgressBar.h"
#include "CGUI/CCompass.h"
#include "CGUI/CClock.h"

class SceneIntro : public Scene
{
public:
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELSTACK,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHTENABLED,
		U_NUMLIGHTS,
		U_LIGHT0_TYPE,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_COLOR_TEXTURE,
		U_COLOR_TEXTURE1,
		U_COLOR_TEXTURE2,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE_ENABLED1,
		U_COLOR_TEXTURE_ENABLED2,

		U_FOG_COLOR,
		U_FOG_START,
		U_FOG_END,
		U_FOG_DENSITY,
		U_FOG_TYPE,
		U_FOG_ENABLED,

		U_TEXT_ENABLED,
		U_TEXT_COLOR,

		U_WATER,
		U_WATERENABLED,
		U_WAVENUMBER,

		U_GREYMODE,
		U_GLITCHMODE,


		U_LIGHT_DEPTH_MVP_GPASS,
		U_LIGHT_DEPTH_MVP,
		U_SHADOW_MAP,
		U_SHADOW_COLOR_TEXTURE,
		U_SHADOW_COLOR_TEXTURE1,
		U_SHADOW_COLOR_TEXTURE2,
		U_SHADOW_COLOR_TEXTURE_ENABLED,
		U_SHADOW_COLOR_TEXTURE_ENABLED1,
		U_SHADOW_COLOR_TEXTURE_ENABLED2,

		U_TOTAL,
	};

	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_CROSSHAIR,
		GEO_LIGHTBALL,
		GEO_SPHERE,
		GEO_QUAD,
		GEO_CUBE,
		
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,
		GEO_GRASS_DARKGREEN,
		GEO_GRASS_LIGHTGREEN,
		GEO_OBJECT,
		GEO_TEXT,
		GEO_FOUNTAIN,
		GEO_VIRUS,
		GEO_ANIMATED_SPRITE,
		GEO_GROUND,
		GEO_SKYPLANE,
		GEO_LOAD_QUAD,
		NUM_GEOMETRY,
	};

	enum RENDER_PASS
	{
		RENDER_PASS_PRE,
		RENDER_PASS_MAIN,
	};


	SceneIntro();
	~SceneIntro();

	virtual bool Init();
	virtual bool InitSections(double& dValue, double dMaxValue, std::string& sRenderMessage);
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	virtual void DeleteInstances();

	virtual void RenderNextProgressBar(double dt, double& dValue, double dMaxValue, string sRenderMessage);

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshIn2D(Mesh* mesh, bool enableLight, float size = 1.0f, float x = 0.0f, float y = 0.0f);
	void RenderMesh(Mesh* mesh, bool enableLight);

	void RenderGround();
	void RenderSkybox();
	void RenderSkyPlane();
	void RenderParticles();
	void RenderVirusFountain();

	void UpdateParticles(double dt);
	ParticleObject* Part_P_One();
	void RenderParticles(ParticleObject* Part);

	void RenderPassGPass();
	void RenderPassMain();
	void RenderWorld();

	void InitUIs();
	void UpdateUIs(double dt);
	void RenderUIs(MS& projectionStack, MS& viewStack, MS& modelStack);
	void SwapToGameScene();

	bool CheckSaveFiles();

private:
	static const int MAX_PARTICLE = 7500;

	ParticleSystem* Particles;
	float m_speed;
	int m_objectCount; //Number of particle objects
	float m_worldWidth;
	float m_worldHeight;

	Vector3 m_gravity; //Gravity affecting particles


	float fogdensity; //Fogdensity amount
	double fogupdate; //Time until next fog thickeness increase

	float water; //The effect the sin wave foor water (Get it WAVEEEE - Sleepy Clement at 4am)
	bool waterEnabled; //Check if need to add wave effect
	float waterwavesnum;//Number of waves

	//0 is 
	//1 is 
	//3 is sound
	double bouncetime[4]; //Limit key presses


	bool greymode;
	bool glitchmode;

	std::vector<unsigned char> m_heightMap;

	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	Camera3 camera;
	CCamera cCamera;

	float rotateAngle;

	MS modelStack;
	MS viewStack;
	MS projectionStack;

	Light lights[1];

	bool bLightEnabled;

	bool fogenabled;

	float fps;

	Vector3 FPos, VPos;

	float fWindowHeight;
	float fWindowWidth;

	float moveVal;


	//Hander for Sprite Animation
	SpriteAnimation* sa;

	unsigned m_gPassShaderID;
	DepthFBO m_lightDepthFBO;
	WaterFBO m_lightWaterFBO;
	Mtx44 m_lightDepthProj;
	Mtx44 m_lightDepthView;
	RENDER_PASS m_renderPass;
	CKeyboardController* cKeyboardController;
	CSoundController* cSoundController;

	// UIs
	CGUI cLoadScreen;
	CProgressBar cLoadingBar;

	CGUI cIntroDisplay;
	bool bIntroDisplayRendered;
	double dAnimTimer;

	CTextButton cBackButton;

	CGUI cGameTitle;
	CGUI cGroupName;

	CTextButton cStartButton;
	CTextButton cControlsButton;
	CTextButton cExitButton;
	

	CGUI cFrame2;
	CTextLabel cLoadFileHeader;
	CTextLabel cLoadFileAlert;
	CTextButton cLoadFileYes;
	CTextButton cLoadFileNo;

	CGUI cFrame3;
	CTextLabel cVirusNameHeader;
	CTextBox cVirusNameTextBox;
	CTextButton cVirusNameConfirmButton;

	CGUI cFrame4; // Introduction Menu
	CTextLabel cFrame4Title;
	CGUI cFrame4Text;
	CTextButton cFrame4Button;


	CGUI cControlFrame;
	CTextButton cControlCloseButton;
	CTextLabel cControlText;


	double dGUIPositionXOffSet;
	double dGUIPositionXOffSetDestination;

	bool devCam;



};

#endif