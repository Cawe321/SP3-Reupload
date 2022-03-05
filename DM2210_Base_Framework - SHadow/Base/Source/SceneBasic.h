#ifndef SCENE_BASIC_H
#define SCENE_BASIC_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera3.h"
#include "CCamera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include <vector>
#include <String>

#include "SpriteAnimation.h"
#include "ParticleSystem.h"
#include "DepthFBO.h"
#include "WaterFBO.h"
#include "KeyboardController.h"
#include "SoundController.h"
#include "Entity.h"
#include "Player.h"
#include "NPC.h"
#include "Structure.h"
#include "PickUp.h"
#include "EntityManager.h"
#include "CUpgrade.h"

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
#include "CProgressBar3D.h"

#include "CTimeControl.h"

#include "CVaccine.h"
#include "CVirus.h"

class SceneBasic : public Scene
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
		U_LIGHT1_TYPE,
		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,

		// Week 4
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
		GEO_SPHERE2,
		GEO_SPHERE3,
		GEO_SPHERE4,
		GEO_SPHERE5,
		GEO_SPHERE6,
		GEO_SPHERE7,
		GEO_SPHERE8,
		GEO_SPHERE9,
		GEO_QUAD,
		GEO_CUBE,
		GEO_CIRCLE,
	
		
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
		//TSL
		GEO_SKYPLANE,
		GEO_SKYPLANENIGHT,
		GEO_SKYPLANEDAWNDUSK,
		GEO_TERRAIN,

		//My stuff
		
		GEO_SUN,


		GEO_POND,


		
		GEO_ANIMATED_SPRITE,

		GEO_LOGCABIN,


		GEO_ROAD,
		GEO_SCENE,
		

		GEO_QUAD_PART,

		GEO_LEAF,
		GEO_RUNE,

		GEO_LIGHT_DEPTH_QUAD,

		GEO_VIRUS_BUTTON,
		GEO_HPBAR_GREEN,
		GEO_HPBAR_YELLOW,

		GEO_HP_GREEN,
		GEO_HP_YELLOW,
		GEO_HP_RED,

		GEO_PALMTREE,
		GEO_BEACHCHAIR,

		GEO_VIRUS_PARTICLE,

		GEO_LOAD_QUAD,

		GEO_MUTATION_POINTS,
		GEO_VIRUS_LAUNCHER,
		GEO_AIR_FRESHENER,

		GEO_PLAYER,
		GEO_NPC,
		NUM_GEOMETRY,
	};

	enum RENDER_PASS
	{
		RENDER_PASS_PRE,
		RENDER_PASS_MAIN,
	};

	enum FILLTERSTATE
	{
		NORMAL,
		GREYSCALE,
		INVERSE,
		INVERSE_GREYSCALE
	};

public:


	SceneBasic();
	~SceneBasic();

	virtual bool Init();
	virtual bool InitSections(double& dValue, double dMaxValue, std::string& sRenderMessage);
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	virtual void RenderNextProgressBar(double dt, double& dValue, double dMaxValue, string sRenderMessage);
	virtual void DeleteInstances();

	void UpdateMainMenu(double dt);
	void UpdateGame(double dt);
	 
	void RenderMainMenu();
	void RenderGame();

	void StartGame(string sVirusName);


	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshIn2D(Mesh *mesh, bool enableLight, float size=1.0f, float x=0.0f, float y=0.0f);
	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderGround();
	//void RenderSkybox();
	void RenderSkyPlane();


	float GetTerrainHeight(float px, float py);
	//void RenderSkyPlane(Mesh* mesh, Color color, int slices, float PlanetRadius, float height, float hTile, float vTile); //TSL

	void RenderTerrain();

	void RenderImageOnScreen(Mesh* mesh, float sizex, float sizey, float x, float y);

	void RenderParticles();

	void UpdateParticles(double dt);
	ParticleObject* Part_P_One();
	void RenderParticles(ParticleObject* Part);

	void RenderPassGPass();
	void RenderPassMain();
	void RenderWorld();

	void RenderWater();

	void RenderCProgressBar3D(CProgressBar3D* cProgressBar3D);

	void RenderPalmTrees();

	void InitUIs();
	void InitcProgressBar3D(CProgressBar3D* cProgressBar);
	void UpdateUIs(double dt);
	void RenderUIs(MS& projectionStack, MS& viewStack, MS& modelStack);

	void RenderPlayer();
	void RenderWeaponHitBox();

	void RenderNPC();

	void RenderPickUps();

	void ChangeFliter(FILLTERSTATE eFilterNew);

	bool SaveGameValues();
	bool SaveGameEntities();

private:
	static const int MAX_PARTICLE = 7500;

	static const int MAX_NPC = 100;
	static const int MAX_SPAWN_NPC = 25;

	bool SceneBasic::CompareDist(const Vector3& Obj1, const Vector3& Obj2);

	enum GAMESTATE
	{
		MAINMENU,
		GAME,
		LOSS,
		WIN
	};


	//std::vector<ParticleObject*> m_PList; //Particle List
	ParticleSystem* cParticles;
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


	//Limit key presses
	//0 is TAB key
	//1 is switch camera
	//2 is everything else
	//3 is sound
	double bouncetime[4]; 
	
	bool bgreymode;
	bool binversemode;


	//Terrain
	std::vector<unsigned char> m_heightMap;

	FILLTERSTATE eFilter;


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

	Light lights[2];

	bool bLightEnabled;

	bool fogenabled;

	float fps;

	float fWindowHeight;
	float fWindowWidth;

	//**********************************************\\
	//Game Values :D

	//---Player---
	CPlayer* cPlayer;

	//---NPC---
	vector<CNPC*> vNPCS;
	vector<CNPC*> vNPCSpawned;
	int iTotalInfected; //Total NPCS infected
	int iInfectedSpawned; //Number of NPC that wes
	int iTotalAlive; //Total NPCS alive

	//---Pickups---
	vector<CPickUp*> vPickUps;

	//---Vaccine---
	CVaccine cVaccine;

	//---Virus---
	CVirus* cVirus;

	//---Upgrade---
	CUpgrade* cUpgrade;

	//---Misc Values---
	float fScore; //Score of player this game
	float fHighScore; //Highscore
	
	int iDayNumber; //Number of days this game has gone on
	int iDayLoad; //Which day the player resumes on
	double dTimeToNight; //Time to next night phase
	double dDayPhaseTime; //The length of time for each day phase
	
	
	double dInitalDayPhaseTime;
	//TIME eTimeState; //State of the world (DAY, EVENING, NIGHT)
	CTimeControl* cTimeControl;

	GAMESTATE eGameState; //State of the game (MAINMENU, GAME,LOSS)
	//**********************************************\\

	// Sunrise
	CGUI cSunRise;
	CTextLabel cSunRiseText;
	CTextButton cSunRiseButton;

	// Sunset
	CGUI cSunSet;
	CTextLabel cSunSetText;
	CTextButton cSunSetButton;
	bool bNightStarted;

	// Upgrades
	CGUI cUpgradesFrame;
	CTextLabel cUpgradesTitle;
	CTextLabel cUpgradesPoints;
	CGUI cUpgradesFrame2;
	CTextLabel cUpgradesMouseLabel;
	// Frame 2 1st Slide
	CTextLabel cUpgradesCoughRangeTitle;
	CTextButton cUpgradesCoughRangeButton;
	CTextLabel cUpgradesVirusStrengthTitle;
	CTextButton cUpgradesVirusStrengthButton;
	CTextLabel cUpgradesVirusLethalityTitle;
	CTextButton cUpgradesVirusLethalityButton;
	
	// Frame 2 2nd Slide
	CTextLabel cUpgradesDayLengthTitle;
	CTextButton cUpgradesDayLengthButton;
	CTextLabel cUpgradesPlayerSpeedTitle;
	CTextButton cUpgradesPlayerSpeedButton;
	CTextLabel cUpgradesMutateVirusTitle;
	CTextButton cUpgradesMutateVirusButton;

	// Frame 2 3rd Slide
	CTextLabel cUpgradesVirusLauncherTitle;
	CTextLabel cUpgradesVirusLauncherAmmoText;
	CTextButton cUpgradesVirusLauncherButton;
	CTextLabel cUpgradesAirFreshenerTitle;
	CTextLabel cUpgradesAirFreshenerAmmoText;
	CTextButton cUpgradesAirFreshenerButton;


	CTextButton cUpgradesFrame2Button1;
	CTextButton cUpgradesFrame2Button2;
	CTextButton cUpgradesFrame2Button3;


	CTextButton cUpgradesNextDayButton;

	// Gameplay UIs
	CImageButton settingsIcon;
	CGUI settingsBackground;
	CGUI settingsDecor;
	CTextLabel settingsTitle;
	CImageButton settingsCloseButton;
	CTextLabel settingsCheatTitle;
	CTextBox settingsCheatCodeTextBox;
	CTextButton settingsGoMainMenu;
	CTextButton settingsSkipDay;
	CTextButton settingsControls;

	


	CGUI cControlFrame;
	CTextButton cControlCloseButton;
	CTextLabel cControlText;

	CCompass cCompass;
	CNewsFlash cNewsFlash;
	CClock cClock;
	CTextLabel cClockTime;

	CImageButton statsIcon;
	CGUI statsBackground;
	CGUI statsDecor;
	CGUI statsFrame;
	CTextLabel statsVirusTitle;
	CImageButton statsCloseButton;
	CTextLabel statsTotalAlive;
	CTextLabel statsTotalInfected;
	CTextLabel statsInfectedSpawned;
	CTextLabel statsInfectionProgressTitle;
	CProgressBar statsInfectionProgress;
	CTextLabel statsVaccineTitle;
	CProgressBar statsVaccineProgress;
	CTextLabel statsScore;
	CTextLabel statsMutationPoints;

	CImageLabel cInventorySlot1;
	CTextLabel cInventorySlot1Label;
	CImageLabel cInventorySlot2;
	CTextLabel cInventorySlot2Label;
	CTextLabel cInventorySlot2Ammo;
	CImageLabel cInventorySlot3;
	CTextLabel cInventorySlot3Label;
	CTextLabel cInventorySlot3Ammo;

	CGUI cLoadScreen;
	CProgressBar cLoadingBar;

	//Hander for Sprite Animation
	SpriteAnimation* sa;
	SpriteAnimation* cVirusParticle;
	SpriteAnimation* cMutationPt;


	unsigned m_gPassShaderID;
	DepthFBO m_lightDepthFBO;
	WaterFBO m_lightWaterFBO;
	Mtx44 m_lightDepthProj;
	Mtx44 m_lightDepthView;
	RENDER_PASS m_renderPass;

	// Keyboard controller
	CKeyboardController* cKeyboardController;



	// Sound controller
	CSoundController* cSoundController;

	// Entity Manager
	CEntityManager* cEntityManager;

	// Win UI
	CGUI cWinFrame;
	CGUI cWinDecor;
	CTextLabel cWinText;
	CTextLabel cWinDays;
	CTextButton cWinButton;


	// Lose UI
	CGUI cLoseFrame;
	CGUI cLoseDecor;
	CTextLabel cLoseText;
	CTextLabel cLoseScore;
	CTextButton cLoseButton;



	bool devCam;

	//NPC Storage
};

#endif