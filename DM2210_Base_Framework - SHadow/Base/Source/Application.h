
#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"
#include "string"
struct GLFWwindow;

class Application
{
public:
	struct PASSVALUE
	{
		std::string virusName;
		bool load;
	};
	static PASSVALUE sPassValue;
	enum SCENETYPE
	{
		INTRO,
		GAME,
		EXIT,
	};
	static Application& GetInstance()
	{
		static Application app;
		return app;
	}
	void Init();
	void Run();
	void Exit();
	static bool IsKeyPressed(unsigned short key);
	static bool IsMousePressed(unsigned short key);

	static void GetCursorPos(double* xpos, double* ypos);
	static GLFWwindow* GetWindow();
	static int GetWindowWidth();
	static int GetWindowHeight();

	static bool GetMouseUpdate();
	//Declare variables to store the last and current mouse position
	static double mouse_last_x, mouse_last_y, mouse_current_x, mouse_current_y, mouse_diff_x, mouse_diff_y;
	static double camera_yaw, camera_pitch;


	static void SetSceneType(SCENETYPE eSceneType);
	static void SetIntroDisplayRendered(bool bIntroDisplayRendered);
	static bool GetIntroDisplayRendered();

	const static void SetMouseSeen(bool bSeen);

private:
	Application();
	~Application();

	// Declare the window width and height as constant integer
	const static int m_window_deadzone = 100;
	//Declare a window object
	StopWatch m_timer;

	static SCENETYPE eSceneType, eOldSceneType;
	static bool bIntroDisplayRendered;
	
};

#endif